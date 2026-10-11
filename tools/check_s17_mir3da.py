#!/usr/bin/env python3
"""Audit stock registers and exercise actual MIR3DA math and owner callbacks."""
import hashlib
import re
import struct
import subprocess
import tempfile
import zlib
from pathlib import Path
from reverse_axs15260 import ROOT, STOCK_HASH
from check_s17_psc5425e import function

DRIVER = Path('/home/dengxh/android3/lineage/kernel/lvmi/mt6765/drivers/misc/mediatek/'
              'sensors-1.0/accelerometer/mir3da/mir3da.c')

SHIM = r'''
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned int uint;
typedef unsigned long ulong;
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define min_t(t,a,b) min((t)(a),(t)(b))
#define max_t(t,a,b) max((t)(a),(t)(b))
#define clamp_t(t,v,lo,hi) min_t(t,max_t(t,v,lo),hi)
#define div_u64(a,b) ((a)/(b))
static unsigned long int_sqrt(unsigned long n) {
    unsigned long x=0, bit=1UL<<30;
    while(bit>n)bit>>=2;
    while(bit){if(n>=x+bit){n-=x+bit;x=(x>>1)+bit;}else x>>=1;bit>>=2;}
    return x;
}
static s16 reference_history[3][30];
static bool reference_valid;
static int iRamffffff80099dac0c;
#define _mcount() ((void)0)
static int sensor_mir3da_temp_calibrate_detect_static(int x,int y,int z) {
    int v[3]={x,y,z}, spread=0;
    for(int a=0;a<3;a++){
        if(!reference_valid) for(int i=0;i<30;i++)reference_history[a][i]=v[a];
        else {
            int lo=reference_history[a][0],hi=lo;
            for(int i=1;i<30;i++){if(reference_history[a][i]<lo)lo=reference_history[a][i];
                if(reference_history[a][i]>hi)hi=reference_history[a][i];}
            spread+=hi-lo;
            memmove(reference_history[a],reference_history[a]+1,29*sizeof(s16));
            reference_history[a][29]=v[a];
        }
    }
    reference_valid=true;return spread<60;
}
'''

MATH_TEST = r'''
static uint32_t random_state=17;
static int sample(void){random_state=random_state*1664525U+1013904223U;return (random_state>>16)%4096-2048;}
int main(void){
    struct mir3da_calibration c={0};
    unsigned count=0;
    int near[3]={1024,0,0},far[3]={0,0,1024};
    for(int i=0;i<3;i++)assert(mir3da_proximity_sample(near,&count)==1);
    assert(mir3da_proximity_sample(near,&count)==0);
    assert(mir3da_proximity_sample(far,&count)==1 && count==0);
    for(int i=0;i<200000;i++){
        if(i%1000==0){memset(&c,0,sizeof(c));reference_valid=false;
            iRamffffff80099dac0c=0;}
        int v[3]={sample(),sample(),sample()},expected[3];
        if(i%300<70){v[0]=i%13-6;v[1]=i%7-3;v[2]=900+i%3;}
        if(i%300>=70&&i%300<140){v[0]=950+i%13;v[1]=i%7-3;v[2]=50;}
        if(i%300>=140&&i%300<210){v[0]=i%7-3;v[1]=-950-i%13;v[2]=-50;}
        memcpy(expected,v,sizeof(v));
        sensor_mir3da_temp_calibrate(&expected[0],&expected[1],&expected[2]);
        mir3da_temperature_compensate(&c,v);
        if(memcmp(v,expected,sizeof(v)) || c.z_offset!=iRamffffff80099dac0c){
            fprintf(stderr,"calibration mismatch %d: %d,%d,%d / %d,%d,%d offsets %d/%d\n",
                i,v[0],v[1],v[2],expected[0],expected[1],expected[2],c.z_offset,iRamffffff80099dac0c);
            return 1;
        }
        int raw[3]={sample(),sample(),sample()};
        int px=raw[0]*9807,py=raw[1]*9807,pz=raw[2]*9807;
        int ax=abs(px/1024),ay=abs(py/1024);
        bool candidate=ax>8500||ax>7500||(pz< -1023&&ay>7000)||
            (ax>5000&&ay>5000)||(((uint32_t)(py-0x4e2400)>>14)<125 &&
            (uint32_t)(pz+0x4e1fff)<0x4e1c00);
        count=0;
        for(int n=0;n<4;n++)assert(mir3da_proximity_sample(raw,&count)==(candidate&&n==3?0:1));
    }
    puts("200000 stock calibration comparisons and proximity sequences passed");
}
'''

STATE_SHIM = r'''
struct device { int wakes; };
struct i2c_adapter { int unused; };
struct i2c_client { struct device dev; u8 regs[256]; int addr; struct i2c_adapter *adapter; };
struct mutex { int unused; };
struct delayed_work { int queued; };
static struct {
    struct i2c_client *client;
    struct i2c_client *alternate;
    struct device *dev;
    struct mutex lock,control_lock;
    struct delayed_work ps_work;
    struct hwmsen_convert { int sign[3],map[3]; } convert;
    struct mir3da_calibration calibration;
    int cali[3];
    unsigned near_count,acc_period_ms,factory_period_ms,ps_period_ms;
    int ps_value,last_report;
    u8 chip_info,package,variant;
    bool ready,identified,powered,suspended,acc_enabled,factory_enabled,ps_enabled;
} sensor;
static int read_error,write_error,writes,cancellations;
static bool primary_nack,alternate_busy;
static int alternate_id=0x13,primary_reads,alternate_reads,unregistered;
static struct i2c_client alternate_client;
static void mutex_lock(struct mutex *m){(void)m;}
static void mutex_unlock(struct mutex *m){(void)m;}
static void msleep(int n){(void)n;}
static void usleep_range(int a,int b){(void)a;(void)b;}
#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))
#define dev_info(...) ((void)0)
#define dev_err(...) ((void)0)
static struct i2c_client *i2c_new_dummy(struct i2c_adapter *a,int addr){
    if(alternate_busy)return NULL;
    memset(&alternate_client,0,sizeof(alternate_client));
    alternate_client.addr=addr;alternate_client.adapter=a;
    alternate_client.regs[1]=alternate_id;
    alternate_client.regs[0xc0]=0x4a;alternate_client.regs[0x8c]=0xc0;
    return &alternate_client;
}
static void i2c_unregister_device(struct i2c_client *c){assert(c==&alternate_client);unregistered++;}
static int i2c_smbus_read_byte_data(struct i2c_client *c,u8 r){
    if(c->addr==0x26)primary_reads++;else if(c->addr==0x27)alternate_reads++;
    if(primary_nack&&c->addr==0x26)return -EREMOTEIO;
    return read_error?read_error:c->regs[r];
}
static int i2c_smbus_write_byte_data(struct i2c_client *c,u8 r,u8 v){writes++;if(write_error)return write_error;c->regs[r]=v;return 0;}
static int block_result=6;
static int i2c_smbus_read_i2c_block_data(struct i2c_client *c,u8 r,int n,u8 *out){
    if(read_error)return read_error;
    memcpy(out,c->regs+r,n);return block_result;
}
static void pm_stay_awake(struct device *d){d->wakes=1;}
static void pm_relax(struct device *d){d->wakes=0;}
#define system_wq NULL
static void mod_delayed_work(void *w,struct delayed_work *d,int n){(void)w;(void)n;d->queued=1;}
static void cancel_delayed_work_sync(struct delayed_work *d){cancellations++;d->queued=0;}
'''

STATE_TEST = r'''
int main(void){
    struct i2c_client client={0};sensor.client=&client;sensor.ready=true;
    sensor.dev=&client.dev;
    sensor.acc_period_ms=20;sensor.factory_period_ms=20;sensor.ps_period_ms=50;
    client.addr=0x26;
    primary_nack=true;
    assert(!mir3da_select_address(&client));
    assert(primary_reads==6&&alternate_reads==1&&writes==0);
    assert(sensor.client==&alternate_client&&client.addr==0x26);
    assert(!mir3da_hardware_init());
    assert(sensor.client->regs[0x8c]==0xc0); /* stock trim edit is only for 0x26 */
    assert(!mir3da_ps_enable(1));assert(client.dev.wakes && !alternate_client.dev.wakes);
    assert(!mir3da_ps_enable(0));assert(!client.dev.wakes);
    i2c_unregister_device(sensor.alternate);sensor.alternate=NULL;sensor.client=&client;
    alternate_busy=true;assert(mir3da_select_address(&client)==-EBUSY);
    assert(sensor.client==&client&&!sensor.alternate);
    alternate_busy=false;alternate_id=0x55;
    assert(mir3da_select_address(&client)==-ENODEV);
    assert(sensor.client==&client&&!sensor.alternate&&unregistered==2);
    primary_nack=false;alternate_id=0x13;
    writes=0;
    sensor.identified=false;
    assert(mir3da_hardware_init()==-ENODEV && writes==0);
    client.regs[1]=0x13;client.regs[0x14]=1;
    assert(mir3da_hardware_init()==-ENODEV && writes==0);
    client.regs[0x14]=0;client.regs[0xc0]=0x4a;
    assert(!mir3da_hardware_init());assert(sensor.package==2&&sensor.variant==2);
    assert(client.regs[0x00]==0x24&&client.regs[0x11]==0xbe);
    assert(client.regs[0x0f]==1&&client.regs[0x7f]==0xbd);
    client.regs[0xc1]=0x40;assert(!mir3da_hardware_init());assert(sensor.variant==4);
    client.regs[0xc1]=0x80;assert(!mir3da_hardware_init());assert(sensor.variant==2);
    client.regs[0xc1]=0xc0;assert(!mir3da_hardware_init());assert(sensor.variant==6);
    client.regs[0xc1]=0;client.regs[0x8f]=1;
    assert(!mir3da_hardware_init());assert(sensor.variant==6);
    client.regs[0xc1]=0xc0;client.regs[0x8f]=1;
    assert(!mir3da_hardware_init());assert(sensor.variant==2); /* revision 7: stock default */
    client.regs[0xc0]=0x50;client.regs[0xc1]=0xe0;
    assert(!mir3da_hardware_init());assert(sensor.package==3&&sensor.variant==2);
    client.regs[0xc0]=0x30;
    assert(mir3da_hardware_init()==-ENODEV); /* retain the real stock C0 check */
    client.regs[0xc0]=0x4a;
    for(int a=0;a<3;a++){sensor.convert.map[a]=a;sensor.convert.sign[a]=a==1?1:-1;}
    sensor.powered=true;sensor.chip_info=0x4b;
    int raw[3],v[3]={123,-456,1024};
    for(int a=0;a<3;a++){uint16_t val=(uint16_t)(v[a]*8);client.regs[2+a*2]=val&255;client.regs[3+a*2]=val>>8;}
    assert(!mir3da_read(raw,false));assert(raw[0]==-123&&raw[1]==-456&&raw[2]==0);
    block_result=5;assert(mir3da_read(raw,false)==-EIO);block_result=6;
    sensor.powered=false;
    client.regs[0x11]=0xbe;
    assert(!mir3da_ps_enable(1));
    assert(sensor.powered&&sensor.ps_enabled&&sensor.ps_work.queued&&client.dev.wakes);
    assert(client.regs[0x11]==0x3e);
    assert(!mir3da_acc_enable(1));assert(!mir3da_acc_delay(5000000));
    assert((client.regs[0x10]&15)==8);
    assert(!mir3da_acc_enable(0));assert(sensor.powered&&(client.regs[0x10]&15)==6);
    assert(!mir3da_factory_enable(true,10));assert((client.regs[0x10]&15)==7);
    assert(!mir3da_ps_enable(0));assert(sensor.powered&&!client.dev.wakes&&!sensor.ps_work.queued);
    assert(!mir3da_factory_enable(false,10));assert(!sensor.powered&&client.regs[0x11]==0xbe);
    assert(!mir3da_acc_enable(1));assert(!mir3da_ps_enable(1));
    assert(!mir3da_ps_enable(0));assert(sensor.powered);
    assert(!mir3da_suspend(&client.dev));assert(!sensor.powered&&sensor.acc_enabled);
    assert(!mir3da_resume(&client.dev));assert(sensor.powered);
    assert(!mir3da_acc_enable(0));
    read_error=-EIO;int before=writes;assert(mir3da_ps_enable(1)==-EIO);
    assert(writes==before&&!sensor.ps_enabled&&!sensor.powered&&!client.dev.wakes);
    read_error=0;write_error=-EIO;assert(mir3da_acc_enable(1)==-EIO);
    assert(!sensor.acc_enabled&&!sensor.powered);
    write_error=0;assert(!mir3da_ps_enable(1));
    assert(mir3da_suspend(&client.dev)==-EBUSY);
    assert(!mir3da_ps_enable(0));assert(!sensor.powered);
    assert(mir3da_acc_batch(0,-1,0)==-EINVAL);
    assert(mir3da_ps_batch(0,0,0)==-EINVAL);
    assert(!mir3da_acc_batch(0,10000000,0));assert(!mir3da_ps_batch(0,1000000000,0));
    assert(sensor.ps_period_ms==50);
    sensor.ready=false;assert(mir3da_ps_enable(1)==-ENODEV);
    puts("actual hardware/owner callbacks: alternate address, busy/wrong ID, wakeup owner, variant map, raw reads, mixed consumers, suspend and I2C errors passed");
}
'''


def remove_debug_blocks(text):
    while True:
        m = re.search(r'  if \(\(bRamffffff80097db2e8[^\n]*\) \{', text)
        if not m:
            return text
        pos, depth = m.end(), 1
        while depth:
            depth += (text[pos] == '{') - (text[pos] == '}')
            pos += 1
        text = text[:m.start()] + text[pos:]


def compile_run(text):
    with tempfile.TemporaryDirectory(prefix='s17-mir3da-test-') as tmp:
        source, binary = Path(tmp) / 'test.c', Path(tmp) / 'test'
        source.write_text(text)
        subprocess.run(['gcc', '-std=gnu11', '-O1', '-g', '-fsanitize=address,undefined',
                        '-fno-omit-frame-pointer', str(source), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)


def main():
    compressed = (ROOT / 'extracted/boot/kernel').read_bytes()
    assert hashlib.sha256(compressed).hexdigest() == STOCK_HASH
    image = zlib.decompress(compressed, 31)
    source = DRIVER.read_text()
    array = source.split('mir3da_init_regs[] = {')[1].split('};')[0]
    actual = [tuple(int(v, 16) for v in m) for m in re.findall(
        r'\{ (0x[0-9a-f]+), (0x[0-9a-f]+), (0x[0-9a-f]+) \}', array)]
    expected = [struct.unpack_from('<hBB', image, 0x175b2f8 + off)
                for off in range(0x1a, 0x3e, 4)]
    assert actual == expected, (actual, expected)
    print('Stock NSA_NTO initialization register audit passed', flush=True)
    oracle = remove_debug_blocks((ROOT / 'extracted/metadata/ghidra-sensors/'
                                  'sensor_mir3da_temp_calibrate.c').read_text())
    helper = (DRIVER.parent / 'mir3da_math.h').read_text()
    compile_run(SHIM + oracle + helper + MATH_TEST)
    names = ['mir3da_update', 'mir3da_period', 'mir3da_power', 'mir3da_acc_enable',
             'mir3da_acc_delay', 'mir3da_acc_batch', 'mir3da_ps_enable',
             'mir3da_ps_delay', 'mir3da_ps_batch', 'mir3da_factory_enable',
             'mir3da_suspend', 'mir3da_resume', 'mir3da_select_address',
             'mir3da_hardware_init', 'mir3da_read']
    regs = source[source.index('struct mir3da_reg {'):source.index('static int mir3da_update')]
    compile_run(SHIM + helper + STATE_SHIM + regs +
                '\n'.join(function(source, n) for n in names) + STATE_TEST)


if __name__ == '__main__':
    main()
