#!/usr/bin/env python3
"""Stock-table audit and host tests of the S17 front driver's actual functions."""
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import tempfile
import zlib

from reverse_axs15260 import ROOT, STOCK_HASH

KERNEL = Path('/home/dengxh/android3/lineage/kernel/lvmi/mt6765')
DRIVER = KERNEL / ('drivers/misc/mediatek/imgsensor/src/mt6765/camera_project/'
                   'r65v3_pmz_w1a/gc5025sub_mipi_raw/gc5025sub_mipi_Sensor.c')


def function(source, name):
    match = re.search(r'static [^;{}]*\b' + name + r'\([^)]*\)\s*\{', source)
    assert match, name
    end, depth = match.end(), 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[match.start():end]


def main():
    compressed = (ROOT / 'extracted/boot/kernel').read_bytes()
    assert hashlib.sha256(compressed).hexdigest() == STOCK_HASH
    image = zlib.decompress(compressed, 31)
    source = DRIVER.read_text()
    ghidra = (ROOT / 'extracted/metadata/ghidra-gc5025sub/gc5025sub_open.c').read_text()
    start = ghidra.index('  uStack_138 = 0x80fe;')
    end = ghidra.index('  if (sRamffffff8009adc4c8', start)
    stock = [[int(m[1], 0) & 255, int(m[1], 0) >> 8] for m in re.finditer(
        r'uStack_138 = (0x[0-9a-f]+|[0-9]+);\s*camera_iWriteRegI2C\(&uStack_138,2,0x6e\);',
        ghidra[start:end])]
    actual = [[int(a, 0), int(v, 0)] for a, v in re.findall(
        r'write_cmos_sensor\((0x[0-9a-f]+),\s*(0x[0-9a-f]+)\)', function(source, 'sensor_init'))]
    assert len(stock) == 107 and actual == stock
    for field, value in [('pclk', 72000000), ('linelength', 2400), ('framelength', 988)]:
        assert len(re.findall(r'\.' + field + r'\s*=\s*' + str(value) + r'\b', source)) == 5
    assert '.sensor_output_dataformat = SENSOR_OUTPUT_FORMAT_RAW_R' in source
    crop = [list(struct.unpack_from('<16H', image, 0x10A4D2A + i * 32)) for i in range(5)]
    source_crop = source.split('imgsensor_winsize_info[5] =')[1].split('};')[0]
    assert [[int(n) for n in row.split(',') if n.strip()] for row in
            re.findall(r'\{([^{}]+)\}', source_crop)] == crop

    prelude = r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint8_t kal_uint8;
typedef uint16_t kal_uint16;
typedef uint32_t kal_uint32;
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define DD_PARAM_QTY 200
#define WINDOW_WIDTH 0xa30
#define WINDOW_HEIGHT 0x79c
#define REG_ROM_START 0x62
#define LOG_INF(...) ((void)0)
#define pr_warn(...) ((void)0)
#define mdelay(n) ((void)(n))
#define min_t(t,a,b) ((t)(a) < (t)(b) ? (t)(a) : (t)(b))
#define max_t(t,a,b) ((t)(a) > (t)(b) ? (t)(a) : (t)(b))
#define spin_lock_irqsave(lock,flags) ((void)(lock), (flags)=0)
#define spin_unlock_irqrestore(lock,flags) ((void)(lock), (void)(flags))
static int imgsensor_drv_lock;
static struct { uint32_t shutter, frame_length, min_frame_length; } imgsensor;
static uint8_t otp[2][128], regs[256];
static unsigned int cursor, automatic, write_count;
static uint16_t writes[4096][2];
static void write_cmos_sensor(uint32_t addr, uint32_t data) {
    assert(write_count < ARRAY_SIZE(writes));
    writes[write_count][0]=addr; writes[write_count++][1]=data & 255;
    regs[addr]=data;
    if (addr == 0xf3 && data == 0x20) {
        cursor=(((regs[0xd4] & 3) << 8) | regs[0xd5]) >> 3;
        automatic=0;
    }
    if (addr == 0xf3 && data == 0x88) automatic=1;
}
static uint16_t read_cmos_sensor(uint32_t addr) {
    if (addr == 0xd7) {
        unsigned int page=(regs[0xd4] >> 2) & 1;
        assert(cursor < 128);
        uint8_t value=otp[page][cursor];
        if (automatic) cursor++;
        return value;
    }
    return regs[addr];
}
enum otp_page { otp_page0, otp_page1 };
enum otp_state { otp_close, otp_open };
'''
    declaration = re.search(r'struct otp_gc5025 \{.*?\};', source, re.S).group()
    functions = ['gc5025_read_otp', 'gc5025_read_otp_group', 'gc5025_select_page_otp',
                 'gc5025_gcore_read_otp_info', 'gc5025_gcore_update_dd',
                 'gc5025_gcore_update_chipversion', 'set_shutter', 'set_gain',
                 'preview_setting', 'capture_setting', 'normal_video_setting',
                 'hs_video_setting', 'slim_video_setting']
    tests = r'''
static void reset(void) {
    memset(otp,0,sizeof(otp)); memset(regs,0,sizeof(regs));
    memset(&gc5025_otp_info,0,sizeof(gc5025_otp_info));
    cursor=automatic=write_count=0;
}
static void defects(unsigned int count) {
    unsigned int i, sum=count;
    otp[0][0]=1; otp[0][1]=count;
    for (i=0;i<count;i++) {
        uint8_t record[4]={100,0x80,12,0x13}; /* x=100, y=200, four pixels */
        unsigned int j;
        for (j=0;j<4;j++) {
            unsigned int index=i*4+j;
            if (index<124) otp[0][3+index]=record[j];
            else otp[1][0x29+index-124]=record[j];
            sum+=record[j];
        }
    }
    otp[1][0x75]=sum%255+1;
}
int main(void) {
    unsigned int i;
    reset(); gc5025_gcore_read_otp_info();
    assert(gc5025_otp_info.dd_flag==0 && gc5025_otp_info.dd_cnt==0);
    reset(); defects(0); gc5025_gcore_read_otp_info(); gc5025_gcore_update_dd();
    assert(gc5025_otp_info.dd_cnt==0);
    reset(); defects(1); otp[0][127]=0x10;
    otp[1][0x62]=0x1c; otp[1][0x63]=0x1f;
    gc5025_gcore_read_otp_info();
    assert(gc5025_otp_info.dd_cnt==4 && gc5025_otp_info.reg_num==1);
    write_count=0; gc5025_gcore_update_dd();
    assert(write_count==27);
    for (i=0;i<4;i++) {
        assert(gc5025_otp_info.dd_param_x[i]==2508);
        assert(gc5025_otp_info.dd_param_y[i]==1746+i);
        assert(writes[5+i*5][0]==0xaa && writes[5+i*5][1]==i);
        assert(writes[9+i*5][0]==0xac && writes[9+i*5][1]==2);
    }
    gc5025_gcore_update_chipversion(); assert(regs[0x1c]==0x1f);
    reset(); defects(50); gc5025_gcore_read_otp_info();
    assert(gc5025_otp_info.dd_cnt==200); gc5025_gcore_update_dd();
    reset(); otp[0][0]=1; otp[0][1]=51; gc5025_gcore_read_otp_info();
    assert(gc5025_otp_info.dd_flag==2 && gc5025_otp_info.dd_cnt==0);
    reset(); imgsensor.min_frame_length=988;
    set_shutter(0); assert(regs[3]==0 && regs[4]==2);
    set_shutter(65535); assert(regs[3]==0x3f && regs[4]==0xfe);
    assert(imgsensor.frame_length==0x3fff);
    set_gain(0); assert(regs[0xb6]==0 && regs[0xb1]==1 && regs[0xb2]==0);
    set_gain(92); assert(regs[0xb6]==1 && regs[0xb1]==1 && regs[0xb2]==0);
    write_count=0; preview_setting(); capture_setting(300);
    normal_video_setting(300); hs_video_setting(); slim_video_setting();
    assert(write_count==0);
    puts("PASS: OTP empty/0/4/200 pixels, invalid count, SRAM order, register patch, shutter, gain, mode stability");
}
'''
    code = prelude + declaration + '\nstatic struct otp_gc5025 gc5025_otp_info;\n'
    code += '\n'.join(function(source, name) for name in functions) + tests
    with tempfile.TemporaryDirectory(prefix='s17-gc5025-test-') as folder:
        path = Path(folder)
        (path / 'test.c').write_text(code)
        subprocess.run(['cc', '-std=gnu11', '-Wall', '-Wextra', '-Werror',
                        '-Wno-unused-parameter', '-Wno-unused-variable',
                        '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
                        str(path / 'test.c'), '-o', str(path / 'test')], check=True)
        subprocess.run([str(path / 'test')], check=True)
    report = dict(stock_kernel_sha256=STOCK_HASH, source_sha256=hashlib.sha256(DRIVER.read_bytes()).hexdigest(),
                  init=stock, crop=crop, timing=dict(pclk=72000000, line=2400, frame=988),
                  source=str(DRIVER), checks='PASS: stock init/crop; host ASan/UBSan OTP/exposure/gain/mode checks')
    (ROOT / 'extracted/metadata/stock-gc5025sub-audit.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
