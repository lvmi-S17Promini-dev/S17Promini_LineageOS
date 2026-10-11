#!/usr/bin/env python3
"""Run the actual PSC5425E callbacks against a mock I2C register bank.

Also exercise charging-manager initialization with delayed/missing hardware.
No device access. Requires a C compiler with ASan/UBSan support.
"""
import argparse
from pathlib import Path
import re
import subprocess
import tempfile


def function(source, name):
    match = re.search(r"(?:static )?(?:int|bool|unsigned int|struct psc5425e \*)\s*"
                      + re.escape(name) + r"\([^;{}]*\)\s*\{", source)
    if not match:
        raise ValueError(f"Missing function: {name}")
    depth, pos = 1, match.end()
    while depth:
        depth += (source[pos] == "{") - (source[pos] == "}")
        pos += 1
    return source[match.start():pos]


DRIVER_SHIM = r'''
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
typedef uint8_t u8;
typedef uint32_t u32;
#define U32_MAX UINT32_MAX
#define BIT(n) (1U << (n))
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
struct device { void *data; };
struct charger_device { struct device dev; void *driver_data; };
struct i2c_client { u8 regs[256]; };
struct mutex { int unused; };
struct psc5425e {
    struct i2c_client *client;
    struct mutex lock;
    bool otg_enabled;
};
static void mutex_lock(struct mutex *m) { (void)m; }
static void mutex_unlock(struct mutex *m) { (void)m; }
static void *dev_get_drvdata(struct device *d) { return d->data; }
static int read_error, write_error, writes;
static int i2c_smbus_read_byte_data(struct i2c_client *c, u8 reg) {
    return read_error ? read_error : c->regs[reg];
}
static int i2c_smbus_write_byte_data(struct i2c_client *c, u8 reg, u8 val) {
    writes++;
    if (write_error) return write_error;
    c->regs[reg] = val;
    return 0;
}
'''

DRIVER_TEST = r'''
int main(void) {
    struct i2c_client client = {0};
    struct psc5425e info = { .client = &client };
    struct charger_device chg = { .dev.data = &info,
                                 .driver_data = (void *)0x1234 };
    u32 value;
    bool state;
    const u32 expected[] = {656000,786000,1048000,1180000,
                            1442000,1574000,1836000,1966000};
    assert(psc_info(&chg) == &info);
    for (unsigned i = 0; i < 8; i++) {
        client.regs[PSC_CURRENT] = 0x86;
        assert(!psc_set_current(&chg, expected[i]));
        assert(client.regs[PSC_CURRENT] == (i << 4 | 6));
        assert(!psc_get_current(&chg, &value) && value == expected[i]);
        if (i) {
            assert(!psc_set_current(&chg, expected[i] - 1));
            assert((client.regs[PSC_CURRENT] >> 4) == i - 1);
        }
    }
    assert(!psc_set_current(&chg, UINT32_MAX));
    assert(client.regs[PSC_CURRENT] == 0x76);
    assert(!psc_set_current(&chg, 0));
    assert(client.regs[PSC_CONTROL] & PSC_DISABLE);
    assert(!psc_set_input(&chg, 500000));
    assert((client.regs[PSC_CONTROL] >> 6) == 1);
    assert(!psc_get_input(&chg, &value) && value == 500000);
    assert(!psc_set_input(&chg, 800000));
    assert((client.regs[PSC_CONTROL] >> 6) == 2);
    assert(!psc_set_input(&chg, 2000000));
    assert((client.regs[PSC_CONTROL] >> 6) == 2);
    assert(!psc_set_input(&chg, UINT32_MAX));
    assert(!psc_get_input(&chg, &value) && value == UINT32_MAX);
    assert(!psc_set_cv(&chg, 4350000));
    assert(client.regs[PSC_VOLTAGE] == 0x90);
    assert(!psc_get_cv(&chg, &value) && value == 4350000);
    assert(!psc_set_cv(&chg, 4349999));
    assert(client.regs[PSC_VOLTAGE] == 0x08);
    assert(psc_set_cv(&chg, 4000000) == -ERANGE);
    assert(!psc_set_cv(&chg, 4400000));
    assert(client.regs[PSC_VOLTAGE] == 0xb4);
    client.regs[PSC_STATUS] = 0x20;
    assert(!psc_done(&chg, &state) && state);
    client.regs[PSC_STATUS] = 0x10;
    assert(!psc_done(&chg, &state) && !state);
    assert(!psc_enable(&chg, true));
    assert(!psc_is_enabled(&chg, &state) && state);
    info.otg_enabled = true;
    assert(psc_enable(&chg, true) == -EBUSY);
    info.otg_enabled = false;
    writes = 0; read_error = -EIO;
    assert(psc_set_current(&chg, 1000000) == -EIO && !writes);
    assert(psc_set_input(&chg, 500000) == -EIO && !writes);
    assert(psc_set_cv(&chg, 4350000) == -EIO && !writes);
    value = 0xdeadbeef;
    assert(psc_get_current(&chg, &value) == -EIO && value == 0xdeadbeef);
    read_error = 0; write_error = -EIO;
    assert(psc_set_current(&chg, 1000000) == -EIO);
    assert(psc_enable(&chg, true) == -EIO);
    puts("PSC5425E register boundaries, getters, RESET and I2C errors: PASS");
}
'''

MANAGER_SHIM = r'''
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define MAX_ALG_NO 10
#define DUAL_CHARGERS_IN_SERIES 1
#define chr_err(...) ((void)0)
struct notifier_block { void *notifier_call; };
struct chg_alg_device { int config; };
struct charger_device { int dummy; };
struct mtk_charger {
    int config;
    struct charger_device *chg1_dev, *chg2_dev;
    struct { void *do_event; } algo;
    struct notifier_block chg1_nb, chg_alg_nb[MAX_ALG_NO];
    struct chg_alg_device *alg[MAX_ALG_NO];
};
static struct charger_device primary, secondary;
static struct chg_alg_device algorithms[4];
static struct notifier_block *registered[4];
static bool primary_present, secondary_present;
static int init_calls, registrations, init_error;
static struct charger_device *get_charger_by_name(const char *name) {
    return !strcmp(name, "primary_chg") ?
        (primary_present ? &primary : NULL) :
        (secondary_present ? &secondary : NULL);
}
static struct chg_alg_device *get_chg_alg_by_name(const char *name) {
    const char *names[] = {"pe4", "pd", "pe2", "pe"};
    for (int i = 0; i < 4; i++) if (!strcmp(name, names[i])) return &algorithms[i];
    assert(0); return NULL;
}
static int chg_alg_init_algo(struct chg_alg_device *alg) {
    (void)alg; init_calls++; return init_error;
}
static int register_chg_alg_notifier(struct chg_alg_device *alg, struct notifier_block *nb) {
    int i = alg - algorithms;
    assert(!registered[i]);
    for (int j = 0; j < 4; j++) assert(registered[j] != nb);
    registered[i] = nb; registrations++; return 0;
}
static int register_charger_device_notifier(struct charger_device *c, struct notifier_block *n) {
    (void)c; (void)n; return 0;
}
static void charger_dev_set_drvdata(struct charger_device *c, void *p) {
    (void)c; (void)p;
}
'''

MANAGER_TEST = r'''
int main(void) {
    struct mtk_charger info = { .config = DUAL_CHARGERS_IN_SERIES };
    for (int i = 0; i < 3; i++) assert(!charger_init_algo(&info));
    assert(!init_calls && !registrations);
    primary_present = true;
    for (int i = 0; i < 3; i++) assert(!charger_init_algo(&info));
    assert(!init_calls && !registrations);
    secondary_present = true;
    assert(charger_init_algo(&info));
    assert(init_calls == 4 && registrations == 4);
    for (int i = 0; i < 4; i++) assert(info.alg[i] == &algorithms[i]);
    memset(registered, 0, sizeof(registered));
    memset(&info, 0, sizeof(info));
    init_calls = registrations = 0; init_error = -5;
    assert(charger_init_algo(&info));
    assert(init_calls == 4 && !registrations);
    for (int i = 0; i < 4; i++) assert(!info.alg[i]);
    puts("Charging manager missing hardware, retries and independent notifiers: PASS");
}
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--kernel-source", type=Path,
                        default=Path.home() / "android3/lineage/kernel/lvmi/mt6765")
    args = parser.parse_args()
    supply = args.kernel_source / "drivers/power/supply"
    source = (supply / "psc5425e.c").read_text()
    defines = "\n".join(re.findall(r"^#define PSC_.*$", source, re.M))
    tables = "\n".join(re.findall(r"static const u(?:32|8) psc_\w+\[\] = \{.*?\};",
                                 source, re.S))
    callbacks = "\n".join(function(source, name) for name in (
        "psc_info", "psc_floor", "psc_update", "psc_enable", "psc_is_enabled",
        "psc_set_current", "psc_get_current", "psc_set_input", "psc_get_input",
        "psc_set_cv", "psc_get_cv", "psc_done"))
    manager = (supply / "mtk_charger.c").read_text()
    units = [DRIVER_SHIM + defines + "\n" + tables + "\n" + callbacks + DRIVER_TEST,
             MANAGER_SHIM + function(manager, "charger_init_algo") + MANAGER_TEST]
    with tempfile.TemporaryDirectory(prefix="s17-psc-test-") as tmp:
        for i, unit in enumerate(units):
            c = Path(tmp) / f"test{i}.c"
            exe = c.with_suffix("")
            c.write_text(unit)
            subprocess.run(["cc", "-std=c11", "-g", "-O1", "-Wall", "-Wextra",
                            "-Wno-sign-compare", "-fsanitize=address,undefined",
                            "-fno-omit-frame-pointer", "-no-pie", str(c), "-o", str(exe)],
                           check=True)
            subprocess.run([str(exe)], check=True, timeout=10)


if __name__ == "__main__":
    main()
