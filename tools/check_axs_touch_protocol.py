#!/usr/bin/env python3
"""Compile the actual kernel report parser and check recovered wire examples."""
import argparse
from pathlib import Path
import subprocess
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--header", type=Path,
        default=Path.home() / "android3/lineage/kernel/lvmi/mt6765/drivers/input/touchscreen/axs15260_s17.h",
    )
    args = parser.parse_args()
    source = r'''
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "axs15260_s17.h"

int main(void)
{
    unsigned char buf[AXS_REPORT_SIZE];
    struct axs_frame frame;
    unsigned int i;

    /* Contact at the inclusive stock axis maxima, default pressure/area. */
    memset(buf, 0xff, sizeof(buf));
    buf[0] = 0; buf[1] = 1;
    memcpy(buf + 2, (unsigned char[]){0x01,0x80,0x03,0x56,0,0}, 6);
    assert(axs_decode_report(buf, sizeof(buf), &frame) == 0);
    assert(frame.count == 1 && frame.points[0].x == 384);
    assert(frame.points[0].y == 854 && frame.points[0].event == 0);
    assert(frame.points[0].id == 0 && frame.points[0].pressure == 63);
    assert(frame.points[0].area == 9);

    /* Five records exercise six-byte stride, contact/up flags and IDs. */
    memset(buf, 0, sizeof(buf)); buf[1] = 5;
    for (i = 0; i < 5; i++) {
        unsigned char *p = buf + 2 + 6 * i;
        p[0] = (i % 3) << 6; p[1] = 40 + i;
        p[2] = i << 4; p[3] = 80 + i;
        p[4] = 10 + i; p[5] = (1 + i) << 4;
    }
    assert(axs_decode_report(buf, sizeof(buf), &frame) == 0);
    assert(frame.count == 5);
    for (i = 0; i < 5; i++) {
        assert(frame.points[i].id == i && frame.points[i].x == 40 + i);
        assert(frame.points[i].y == 80 + i && frame.points[i].event == i % 3);
        assert(frame.points[i].pressure == 10 + i && frame.points[i].area == 1 + i);
    }

    /* Unused/sentinel IDs terminate parsing as in stock, not an overrun. */
    buf[2 + 2 * 6 + 2] = 0xf0;
    assert(axs_decode_report(buf, sizeof(buf), &frame) == 0 && frame.count == 2);
    buf[4] = 0x50;
    assert(axs_decode_report(buf, sizeof(buf), &frame) == 0 && frame.count == 0);

    /* A real empty report releases contacts; special headers are ignored. */
    buf[1] = 0;
    assert(axs_decode_report(buf, sizeof(buf), &frame) == 0 && frame.count == 0);
    buf[0] = 0xff;
    assert(axs_decode_report(buf, sizeof(buf), &frame) == -EAGAIN);
    buf[0] = 0; buf[1] = 0x81;
    assert(axs_decode_report(buf, sizeof(buf), &frame) == -EAGAIN);
    buf[1] = 6;
    assert(axs_decode_report(buf, sizeof(buf), &frame) == -EINVAL);
    assert(axs_decode_report(buf, sizeof(buf)-1, &frame) == -EINVAL);
    puts("PASS: maxima, five contacts, flags, pressure/area, sentinel, empty, special and malformed frames");
    return 0;
}
'''
    with tempfile.TemporaryDirectory(prefix="s17-axs-report-") as folder:
        root = Path(folder)
        (root / "linux").mkdir()
        (root / "linux/types.h").write_text("#include <stdint.h>\ntypedef uint8_t u8; typedef uint16_t u16;\n")
        (root / "linux/errno.h").write_text("#define EINVAL 22\n#define EAGAIN 11\n")
        (root / "axs15260_s17.h").write_bytes(args.header.read_bytes())
        (root / "check.c").write_text(source)
        executable = root / "check"
        subprocess.run(["cc", "-Wall", "-Wextra", "-Werror", "-I", str(root), str(root / "check.c"), "-o", str(executable)], check=True)
        subprocess.run([str(executable)], check=True)


if __name__ == "__main__":
    main()
