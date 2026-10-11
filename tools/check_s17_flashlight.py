#!/usr/bin/env python3
"""Exercise actual S17 GPIO driver functions with mocked kernel primitives."""
import subprocess
import tempfile
from pathlib import Path

from reverse_imx278 import source_body

KERNEL = Path('/home/dengxh/android3/lineage/kernel/lvmi/mt6765')
source = (KERNEL / 'drivers/misc/mediatek/flashlight/flashlights-dummy-gpio.c').read_text()
functions = [
    ('int', 'dummy_pinctrl_set', 'int pin, int state'),
    ('int', 'dummy_enable', 'void'),
    ('int', 'dummy_disable', 'void'),
    ('int', 'dummy_set_level', 'int level'),
    ('int', 'dummy_init', 'void'),
    ('int', 'dummy_uninit', 'void'),
    ('void', 'dummy_work_disable', 'struct work_struct *data'),
    ('void', 'dummy_cancel_timeout', 'void'),
    ('int', 'dummy_ioctl', 'unsigned int cmd, unsigned long arg'),
    ('int', 'dummy_set_driver', 'int set'),
    ('ssize_t', 'dummy_strobe_store', 'struct flashlight_arg arg'),
]
prefix = r'''
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#define DUMMY_PINCTRL_PIN_GPIO 0
#define DUMMY_PINCTRL_PINSTATE_LOW 0
#define DUMMY_PINCTRL_PINSTATE_HIGH 1
#define FLASH_IOC_SET_TIME_OUT_TIME_MS 1
#define FLASH_IOC_SET_DUTY 2
#define FLASH_IOC_SET_ONOFF 3
#define HRTIMER_MODE_REL 0
#define IS_ERR_OR_NULL(p) (!(p) || (uintptr_t)(p) >= (uintptr_t)-4095)
#define pr_debug(...) ((void)0)
#define pr_info(...) ((void)0)
struct pinctrl { int unused; } chip;
struct pinctrl_state { int high; } high = {1}, low = {0};
struct work_struct { int unused; } dummy_work;
struct flashlight_dev_arg { int channel, arg; };
struct flashlight_arg { int level, dur; };
static struct pinctrl *dummy_pinctrl = &chip;
static struct pinctrl_state *dummy_gpio_high = &high, *dummy_gpio_low = &low;
static int dummy_mutex, dummy_op_mutex, dummy_timer, use_count;
static unsigned int dummy_timeout_ms = 100;
static int gpio, pin_error, pending_work, timer_active, sleeps;
static int64_t timer_ms;
static void dummy_work_disable(struct work_struct *data);
static void mutex_lock(int *m) { assert(!*m); *m = 1; }
static void mutex_unlock(int *m) { assert(*m); *m = 0; }
static int pinctrl_select_state(struct pinctrl *p, struct pinctrl_state *s)
{ assert(p == &chip); if (pin_error) return pin_error; gpio = s->high; return 0; }
static int64_t ms_to_ktime(unsigned int ms) { return ms; }
static void hrtimer_start(int *t, int64_t ms, int mode)
{ (void)t; (void)mode; timer_active = 1; timer_ms = ms; }
static void hrtimer_cancel(int *t) { (void)t; timer_active = 0; }
static void cancel_work_sync(struct work_struct *w)
{
    /* A worker already running may finish during the cancellation. */
    if (pending_work) { pending_work = 0; dummy_work_disable(w); }
}
static void msleep(int ms) { assert(gpio == 1); sleeps += ms; }
'''
tests = r'''
static int call(unsigned int cmd, int value)
{
    struct flashlight_dev_arg arg = {0, value};
    return dummy_ioctl(cmd, (unsigned long)&arg);
}
int main(void)
{
    assert(dummy_set_driver(1) == 0 && use_count == 1 && gpio == 0);
    assert(call(FLASH_IOC_SET_DUTY, 25) == 0 && gpio == 0);
    assert(call(FLASH_IOC_SET_TIME_OUT_TIME_MS, 0) == 0);
    assert(call(FLASH_IOC_SET_ONOFF, 1) == 0 && gpio == 1 && !timer_active);
    assert(call(FLASH_IOC_SET_ONOFF, 0) == 0 && gpio == 0);
    assert(call(FLASH_IOC_SET_TIME_OUT_TIME_MS, 1500) == 0);
    assert(call(FLASH_IOC_SET_ONOFF, 1) == 0 && timer_active && timer_ms == 1500);
    dummy_work_disable(&dummy_work);
    assert(gpio == 0);
    pending_work = 1;
    assert(call(FLASH_IOC_SET_TIME_OUT_TIME_MS, 0) == 0);
    assert(call(FLASH_IOC_SET_ONOFF, 1) == 0 && gpio == 1 && !timer_active);
    assert(!pending_work); /* Old timeout cannot turn off this new torch. */
    assert(call(FLASH_IOC_SET_TIME_OUT_TIME_MS, -1) == -EINVAL);
    assert(dummy_timeout_ms == 0);
    assert(call(999, 0) == -ENOTTY);
    assert(call(FLASH_IOC_SET_DUTY, 6) == 0 && gpio == 0);
    pin_error = -EIO;
    assert(call(FLASH_IOC_SET_ONOFF, 1) == -EIO && !timer_active);
    pin_error = 0;
    assert(call(FLASH_IOC_SET_TIME_OUT_TIME_MS, 100) == 0);
    assert(call(FLASH_IOC_SET_ONOFF, 1) == 0);
    assert(dummy_set_driver(1) == 0 && use_count == 2);
    assert(dummy_set_driver(0) == 0 && use_count == 1 && timer_active && gpio == 1);
    pending_work = 1;
    assert(dummy_set_driver(0) == 0 && use_count == 0 && !timer_active && gpio == 0);
    assert(!pending_work);
    assert(dummy_set_driver(0) == 0 && use_count == 0);
    pin_error = -EIO;
    assert(dummy_set_driver(1) == -EIO && use_count == 0);
    pin_error = 0;
    unsigned int timeout_before = dummy_timeout_ms;
    assert(dummy_strobe_store((struct flashlight_arg){25, 30}) == 0);
    assert(sleeps == 30 && gpio == 0 && use_count == 0);
    assert(dummy_timeout_ms == timeout_before);
    assert(dummy_pinctrl_set(1, 0) == -EINVAL);
    dummy_gpio_high = (void *)(intptr_t)-ENOENT;
    assert(dummy_enable() == -ENODEV);
    dummy_pinctrl = NULL;
    assert(dummy_disable() == -ENODEV);
    assert(!dummy_mutex && !dummy_op_mutex);
    puts("PASS: torch, timed flash, stale work, shared users, strobe, GPIO errors");
}
'''
body = ''.join(f'static {ret} {name}({args}) {{\n{source_body(source, name)}\n}}\n'
               for ret, name, args in functions)
with tempfile.TemporaryDirectory(prefix='s17-flash-test-') as directory:
    test = Path(directory) / 'test.c'
    binary = Path(directory) / 'test'
    test.write_text(prefix + body + tests)
    subprocess.run(['cc', '-Wall', '-Wextra', '-Werror', '-Wno-unused-parameter',
                    '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
                    '-g', str(test), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
