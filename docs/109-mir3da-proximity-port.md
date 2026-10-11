# MIR3DA 与接近功能 r16/r17

2026-10-11。实现基于 108 号原厂 Ghidra 分析，内核源码位于 `~/android3/lineage/kernel/lvmi/mt6765/drivers/misc/mediatek/sensors-1.0/accelerometer/mir3da/`。

## 实现

- NSA_NTO 身份检查、原厂九项寄存器初始化、软复位、芯片/package/variant 判断、13 位数据读取、direction=4、原厂温度补偿及软件校准。没有把温度补偿当作 thermal 温区修复。
- MTK 加速度控制/data/factory 路径；普通加速度消费者、factory 和接近分别管理硬件启停，最后一个消费者关闭后芯片进入 standby。
- 接近沿用原厂姿态条件和连续四次判断；独立 delayed work 确保只开启接近时也采样。5–50 ms 周期，没有 batch FIFO；只报告首次值和状态变化，flush 使用 MTK flush 事件。
- 接近开启时在原始 DT 客户端设备上持有 wakeup source，关闭后同步取消采样并释放。它恢复原厂保持唤醒的行为，息屏时是否有应用长期保持接近开启需要实机观察。
- 仅在 CONFIG_GSENSOR_PROXIMITY 开启时允许 ALSPS 框架先创建 context，之后在 MIR3DA probe 注册 PS。驱动 built-in，禁用用户态 bind/unbind，避免没有 unregister API 的框架留下悬空回调。
- CONFIG_MTK_MIR3DA、CONFIG_GSENSOR_PROXIMITY 在 S17 boot_test fragment 开启；温区驱动未改。

## r16 实机失败与 r17 修复

r16 镜像 `artifacts/source-sensors-r1/boot.img` 编译和离线校验通过，但用户反馈 DevCheck 没有数据。

`oss-kernel-debug/sensor-r16-debug/normalized/dmesg-before.txt` 的 5144–5150 行显示：

```text
i2c i2c-1: addr:0x26,ACK error
s17-mir3da 1-0026: hardware initialization failed: -121
s17-mir3da: probe of 1-0026 failed with error -121
<ACCELEROMETER> acc real driver init fail
```

因此 `/sys/bus/i2c/devices/1-0026/driver` 缺失，后续 HAL batch 失败是下游结果。r16 错误在于遗漏了原厂 probe 的 0x27 地址回退；原厂 DT 描述的 0x26 不能作为芯片实际响应地址已确认的证据。

r17 在 0x26 对 REG01 重试六次；仍未获得 ID=0x13 时，用 `i2c_new_dummy()` 保留同一 adapter 的 0x27 地址，检查 REG01=0x13 后才采用这个客户端读取/写入。不会修改原始 DT client 的 addr，也不会访问已被其他 I²C 客户端占用的 0x27。两地址均不识别时拒绝初始化，日志记录实际返回值；所有 probe 失败路径释放保留地址。

唤醒锁和 PM 生命周期继续由原始 1-0026 设备管理，数据传输客户端可能是 1-0027；不能因为 1-0027 的 driver 是 dummy 而判断 S17 驱动失败。正常绑定后 1-0026/driver 应指向 s17-mir3da，日志会报告实际传输地址与芯片信息。

## 验证

`python3 tools/check_s17_mir3da.py`：

- 对照已验证原厂 Image 的九项初始化寄存器。
- ASan/UBSan 下将实际补偿 helper 与 Ghidra 原厂补偿函数比较 20 万组样本，核对接近四次判定和立即远离。
- 抽取实际驱动回调测试 0x26 NACK 后成功使用 0x27、地址被占用、错误 ID 不写寄存器、原始客户端地址保持、唤醒锁设备归属、variant 分支、短读失败。
- 测试接近独立使用、不同消费者交叉启停、最后关闭后 standby、休眠恢复及 I²C 失败。

这些测试使用模拟总线，不代表硬件验收；后续 r17/r18 的实际响应和验收结果见下节。

## r17 实机结果与 r18

用户复测 r17 仍无传感器数据。`sensor-r17-debug/normalized/dmesg.txt` 5261 行已经确认 `MIR3DA found at alternate address 0x27`：REG01 实际读到 0x13，地址回退已实机成功。随后约 22 ms，hardware initialization 返回 -19，仍未绑定驱动。

这段日志说明失败已经推进到后续硬件初始化；但 r17 没有记录 REG14/REGC0/REGC1/REG8F 的值，无法仅凭 -19 区分身份字段、C0 下界或 revision 上界判断。

再次对照原厂 parse_chip_info 汇编：原厂先将 package/variant 初始化为 2/2；revision>4 时跳过 jump table，保留 variant=2，返回最后一次 I²C 读取的成功状态 0。r16/r17 错把这个分支改成 -ENODEV。r18 恢复原厂默认行为，并明确记录各身份寄存器值、reset/init 寄存器错误及实际 revision。仍保留原厂 C0>=0x40 的检查和 I²C 错误返回，没有跳过芯片识别。

主机新增 revision=7/package=2、revision=7/package=3 成功采用默认 variant=2，以及 C0<0x40 仍拒绝的测试，全部通过。随后 r18 日志确认实际 revision=7，证明 r17 的上界拒绝正是本机初始化阻塞原因。

r18 产物 `artifacts/source-sensors-r3/boot.img`，release `4.19.127-s17-bringup-r18-sensors+`；构建、校验和 host tests 记录位于 `extracted/metadata/sensors-r18-*`。

## r18 日志复核

`sensor-r18-debug/normalized/dmesg.txt` 确认 REG01=0x13、REG14=0、REGC0=0x59、REGC1=0xe0、REG8F=0x02，因此 package=4、revision=7。恢复原厂默认 variant=2 后，约 1.538 秒加速度与接近注册成功，acc_init 返回 0。由此确认 r17 的 revision 上界拒绝确实是本机初始化阻塞原因。

约 24.369 秒出现 `ps_report_interrupt_data:value=1`。当前 PS worker 只有成功读取六字节加速度、完成姿态计算后才会走到这条报告路径，因此这也证明至少一次实际数据读取和接近事件上报成功；尚未证明 Android/DevCheck 收到事件或近/远切换已验收。

约 9.538 秒 `acc set cali err -22` 对应当前 set_cali callback 要求 12 字节而 HAL 写入长度不匹配。框架 acccali_store 会打印错误后返回原写入 count，故不能仅凭此日志断定 HAL 或采样已经失败。需后续核对 HAL 校准格式，再调整接口。

## 用户验收

用户随后反馈“devcheck测试ok”，确认 r18 的加速度和接近功能在 DevCheck 实机测试通过。r18 保留为当前传感器基线；没有将温区、所有 factory 校准接口或接近启用期间的休眠行为标记为已验收。

r18 boot SHA256：`ae171d8df311f6f41dbba488e6718b9b3a04bbff79dfbfcc5a38c26ef0c4ba3c`。

内核提交：`2ed8b8fd16d4`（`S17Promini: Add MIR3DA acceleration and posture proximity`），包含驱动、ALSPS 生命周期适配和 r18 配置。

构建：`lunch lineage_S17Promini-userdebug`、`m -j8 bootimage`。镜像校验使用 `tools/verify_source_boot.py`，核对构建 kernel/DTB、原厂 ramdisk、加载地址、gzip、AVB、AXS/GPU 配置和 CPU 调频。产物及 SHA256 位于 `artifacts/source-sensors-r2/`，版本 `4.19.127-s17-bringup-r17-sensors+`。
