# 原厂 MIR3DA 与 GSENSOR_PROXIMITY 逆向

2026-10-11。按用户要求使用 `/home/dengxh/ghidra_12.1.4_PUBLIC`，离线分析原厂内核；未刷机、未修改内核驱动或 vendor。

## 结论

原厂接近功能由 MIR3DA 加速度驱动实现。`mir3da_probe()` 直接向 ALS/PS 框架注册 PS 回调，`mir3da_get_data()` 在读取、校准、坐标映射和单位换算后根据姿态生成 0/1，再调用 `ps_report_interrupt_data(value)`。这条路径没有读取独立红外接近芯片。

`tpd_enable_ps()` 名字和日志带有 tpd/gsl，但反编译和汇编证明它只切换全局开关并操作 wakeup source，没有触摸 I²C 操作。因此不能把接近功能接到 AXS15260 的触摸命令上。

## 输入与可复现证据

- 原厂输入 `extracted/boot/kernel` 的 gzip SHA256：`5851e16038244576d0a863d9f36b4ad602af68ea45477f5c6f44a9dfb0127286`。
- Image VA 基址 `0xffffff8008080000`；使用已验证的原厂绝对 kallsyms 解析器；所有下面的函数地址均为 Image 偏移。
- `tools/reverse_s17_sensors.py` 验证 hash，生成 92 个函数的边界、符号 JSON 和关键函数汇编。
- Ghidra 项目复用 `/tmp/s17-camera-ghidra/S17Camera` 的原厂 `Image`。使用 `-noanalysis` 定点反编译，不依赖全内核自动分析。
- `tools/S17SensorSignatures.java` 显式设置已知的 `ps_report_interrupt_data(int value)` 和 `_mcount(void)` 原型，防止调用参数在反编译中丢失。
- 反编译输出 `extracted/metadata/ghidra-sensors/*.c`；汇编证据 `extracted/metadata/stock-sensor-proximity-disassembly.txt`。

复现边界与汇编：

```bash
python3 tools/reverse_s17_sensors.py
```

已有上述 Ghidra 项目时复现反编译：

```bash
/home/dengxh/ghidra_12.1.4_PUBLIC/support/analyzeHeadless \
  /tmp/s17-camera-ghidra S17Camera -process Image -noanalysis \
  -scriptPath /home/dengxh/S17Promini_LineageOS/tools \
  -preScript S17SensorSignatures.java \
  -postScript S17CameraDecompile.java \
  /home/dengxh/S17Promini_LineageOS/extracted/metadata/stock-sensor-bounds.tsv \
  /home/dengxh/S17Promini_LineageOS/extracted/metadata/ghidra-sensors
```

第一次在新项目运行时，应先运行一次不带 `-preScript` 的定点反编译以建立函数，再运行上面的命令。新项目导入方式与 103/107 号文档相同：验证后的 raw Image、BinaryLoader、AARCH64:LE:64:v8A、上述基址。项目不进入 Git。

## 注册与开关

| 函数 | Image 偏移 | 行为 |
| --- | --- | --- |
| mir3da_init | 0x153ddc8 | 向加速度框架调用 acc_driver_add |
| mir3da_local_init | 0x630cac | 注册 I²C 驱动并检查 probe 成功标志 |
| mir3da_probe | 0x630d80 | 同时注册加速度与 PS 回调 |
| ps_enable_nodata | 0x631384 | 调用 tpd_enable_ps；操作一个 0x48 字节对象偏移 0x30 的 atomic64 bit mask，mask=4 |
| ps_get_data | 0x631430 | 返回 (~near_flag)&1，status=2 |
| mir3da_get_data | 0x6316d8 | 读取加速度并生成 PS 事件 |
| tpd_enable_ps | 0x631b98 | 切换开关并持有/释放 wakeup source |

原厂 probe 首先将 I²C 地址强制设为 0x26，通过寄存器 0x01 检查返回值 0x13；多次重试仍不匹配时改为 0x27，再继续核心初始化。设备树原始 `gsensor@26` 在 I²C1 下。

probe 读取设备树的加速度属性后，**把 direction 强制改成 4**，再调用 `hwmsen_get_convert()`。因此后续移植不能仅根据 DT 的 direction=6 推断算法坐标系，必须保留并核对原厂转换和校准处理。

PS 的 open_report_data、set_delay、batch、flush 都只返回 0。PS data path 的 vender_div=100；注册的控制结构可见直接报告/批处理相关字节置零。它没有独立 PS 芯片初始化。

`tpd_enable_ps(1)` 调用 `__pm_stay_awake()`，设置开关；关闭时清开关并调用 `__pm_relax()`。wakeup source 注册名是 `gsensor ps wakelock`。原厂开启该功能可能影响深度休眠，后续移植需实测其与应用启停的关系。

## 姿态算法

下面是基于 Ghidra 并逐条核对 AArch64 比较/分支后整理的等价判断，**不是已经接入内核的实现**。

先按原厂完成校准、方向映射及特殊 Z 校准修正。设映射后整数样本为 rx/ry/rz，`px=rx*9807`、`py=ry*9807`、`pz=rz*9807`，再用 C 的向零截断除法得到 `x=px/1024`、`y=py/1024`、`z=pz/1024`。判断中既有除法后的 x/y，也有除法前的 py/pz，不能统一替换成同一单位。

仅当 PS 开关为 1 时执行：

```c
int ax = abs(x), ay = abs(y);
bool candidate =
    ax > 8500 || ax > 7500 ||
    (pz < -1023 && ay > 7000) ||
    (ax > 5000 && ay > 5000) ||
    (((uint32_t)(py - 0x4e2400) >> 14) < 125 &&
     (uint32_t)(pz + 0x4e1fff) < 0x4e1c00);

if (candidate) {
    consecutive++;
    if (consecutive >= 4)
        near_flag = 1;
} else {
    consecutive = 0;
    near_flag = 0;
}
ps_report_interrupt_data((~near_flag) & 1);
```

其中 ax>8500 已被 ax>7500 覆盖，仍列出以反映原厂分支。最后一项保留原厂 32 位无符号运算与移位，避免把 Ghidra 的范围优化误写成不同边界。返回 0 对应 Android 接近状态，1 对应远离状态；需要后续实机验证姿态与产品行为。

算法没有显示基于距离、红外强度或接近 ADC 的判断。所谓“连续 4 次”是四次成功加速度读取，**不是固定毫秒数**。原厂没有在 tpd_enable_ps/ps_enable_nodata 中启动独立采样；已经看到加速度框架 acc_work_func 会调用注册的 get_data。仅开 PROXIMITY 时由哪一层维持加速度轮询仍需完整跟踪 HAL/调用者，不能假定注册 PS 回调就会持续产生事件。

## 当前 OSS 移植的关键差异

原厂开启 CONFIG_MTK_MIR3DA 和 CONFIG_GSENSOR_PROXIMITY；当前源码只开启 CUSTOM_KERNEL_ACCELEROMETER/ALSPS 框架，缺少具体驱动。camera-debug 的两类 initcall -19 与此吻合。

还存在框架生命周期差异：原厂 `alsps_init()` 没有 ALSPS 实驱动时返回 -19，但保留已分配的全局 context，随后 MIR3DA probe 仍能注册 PS。当前源码 `alsps_probe()` 在 real driver init 失败时释放 context 并设 NULL。**不能原样照抄原厂 MIR3DA probe，再向当前空 context 注册 PS**。移植时必须先设计 ALSPS context 的创建和 PS 注册顺序，或采用符合现有 ALSPS 框架的注册方式。

本轮没有改这些行为，没有编译/刷写新 boot。

## 新 sensor-debug 的解读

原始三份文件是 UTF-16；保留原文件，在 `oss-kernel-debug/sensor-debug/normalized/` 写 UTF-8 副本。

sensorservice 列出 ACCELEROMETER、ORIENTATION、LIGHT、PROXIMITY，但 Recent Sensor events 和 Active sensors 为空、0 active connections。因此只能证明 HAL 枚举了四项，不能证明它们能够采样。

thermalservice 的 CPU/GPU/NPU、SKIN、电池、PA 有当前读数，Thermal HAL ready。thermal-zones 中：

- tzimgs0..7 均为 -127000：当前相机温区源码用此表示未上电、无对应相机或不支持温度等无效状态；tzimgs0 是其余相机温区的最大值汇总，不是额外硬件传感器。仅凭这组闲置状态读数不能判断八路硬件损坏。
- mtktscharger=-127000：源码取不到 charger power_supply 时会返回此默认值；当前节点与充电驱动能力需要单独核对，不能认定必须启用第三路板载热敏电阻。
- mtk-master-charger=-12700、mtk-slave-charger=0：该快照不足以判断有效温度，需要核对充电器 TEMP 属性及单位。
- mtktswmt=0：legacy WMT 温区实际存在。此前另一套 DT 温区注册失败的 -19 **并不等于 legacy 温区缺失**；0 的原因仍需查看查询条件。

现有证据不需要切回 prebuilt 内核。接近功能后续重点是恢复 MIR3DA 与原厂姿态逻辑、解决 ALSPS context 生命周期，并确认只启用接近时仍有加速度采样。
