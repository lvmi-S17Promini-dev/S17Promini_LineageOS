# S17 GPIO 闪光灯修复与日志采集

2026-10-11。用户反馈手电筒／闪光灯不亮。现有 `oss-kernel-debug/camera-debug/normalized/` 是 r12 日志，已能定位闪光灯驱动未注册；r13 已确认相机恢复，但没有新的闪光灯实机日志。

## 根因和原厂对照

`dmesg-after.txt` 第 2466..2468 行：模板驱动查找 `xxx_high/xxx_low` 失败，probe 返回 -14。第 10031 行起 HAL ioctl 报 `Find no flashlight device`。logcat 在 10:17:21 显示 `getPartId/hasFlashHw` ioctl 为 Invalid argument，但随后 torch 状态仍切换 AVAILABLE_ON，故 UI 的开灯状态不能证明 GPIO 已执行。

原厂 overlay 与当前 `s17promini-stock.dtsi` 一致：GPIO164（pinmux 0xa400），状态 `dummy_gpio_low/high`，两个注册通道为 `(type,ct,part)=(0,0,0)/(0,1,0)`。当前驱动除错误状态名外，enable/disable/set_level/init/uninit 全是未实现模板，都会尝试拉低。

通过已有原厂 Image 的 kallsyms 和 Ghidra 对照：`dummy_ioctl` 位于 0x8c4124，SET_ONOFF=1 选择高，其余选择低；SET_DUTY 选择低。`dummy_set_driver`（0x8c4338）首次获取／最终释放均选择低；`dummy_strobe_store`（0x8c42dc）执行低→高→等待→低。probe 查询的两个字符串实际是 `dummy_gpio_high/low`。反编译使用已有 `S17CameraDecompile.java` 和 `/tmp/s17-camera-ghidra/S17Camera` 项目，边界在 `extracted/metadata/stock-flash-bounds.tsv`，输出在 `ghidra-flash/`。

## 改动和验证

修正 `drivers/misc/mediatek/flashlight/flashlights-dummy-gpio.c`：匹配实际 pinctrl 状态，按原厂恢复高开低关和 SET_DUTY 复位；返回 GPIO 选择错误，保留 probe 的原始 errno。probe 先关灯，并打印注册通道数；开关打印 channel/on/timeout/ret。

新请求先取消旧 timer 并同步清理旧 work，避免旧超时熄灭新开的手电筒；timeout=0 常亮也取消前次 timer。操作锁串行化 ioctl／引用释放／strobe，GPIO 锁保护 work 回调。最终释放和 remove 取消 timer/work 并关灯；多个引用中途释放保留当前超时。strobe 不改写全局 timeout。

`tools/check_s17_flashlight.py` 抽取实际驱动函数，mock 内核 GPIO／定时器／锁，ASan/UBSan 和编译警告检查通过。覆盖常亮、1500ms 超时、旧 work 与新请求、GPIO 失败返回、负超时、多个引用、重复释放、strobe 后 timeout 保留。模拟测试不替代硬件测试。checkpatch 0 errors / 0 warnings，diff 空白检查通过。

完整 `lunch lineage_S17Promini-userdebug`、`m -j8 bootimage` 成功；版本 `4.19.127-s17-bringup-r14-flash+`。输出：

- `artifacts/source-flash-r1/boot.img`
- 标准路径 `~/android3/lineage/out/target/product/S17Promini/boot.img`
- SHA256 `58de7f3564b93d59d09634566ffba78daf2556fe742da22f46b2ef9db8f33f69`

`tools/verify_source_boot.py` 通过源码 kernel/DTB、原厂 first-stage ramdisk、32MiB 分区大小与 AVB hash 检查，cpufreq 仍开启。原 r13 在 `artifacts/source-camera-r2/boot.img` 保留。构建日志 `flash-r14-build.log`，验证报告 `flash-r14-validation.json`，主机测试 `flash-r14-host-tests.log`。构建阶段未操作手机；随后用户确认闪光灯已修复，r14 标记实机通过。

## 抓取日志

下面在电脑的 Windows **cmd.exe** 或 Bash 执行；Windows PowerShell 可先输入 `cmd`，避免旧版 PowerShell 重定向生成 UTF-16。在工作目录下运行：

```sh
mkdir oss-kernel-debug/flash-debug-r14
adb root
adb wait-for-device
adb shell id
adb shell uname -a > oss-kernel-debug/flash-debug-r14/kernel-version.txt
adb shell dmesg > oss-kernel-debug/flash-debug-r14/dmesg-before.txt
adb shell cat /sys/class/flashlight_core/flashlight/flashlight_capability > oss-kernel-debug/flash-debug-r14/capability-before.txt
adb logcat -b all -v threadtime > oss-kernel-debug/flash-debug-r14/logcat.txt
```

最后一条持续运行，保留这个终端；另开终端（相同工作目录）运行：

```sh
adb shell dmesg -w > oss-kernel-debug/flash-debug-r14/dmesg-live.txt
```

若设备 dmesg 不支持 `-w`，用前后快照即可。若 `adb root` 不支持且 `id` 不是 uid=0，已有 su 的设备可将 dmesg 命令替换为 `adb shell su -c "dmesg"` 或 `adb shell su -c "dmesg -w"`。

复现顺序：退出相机→手电筒开关三次→开灯保持约 10 秒确认常亮→关灯→打开主摄并选择“闪光灯始终开启”拍照→退出相机后再次开关手电筒。记下各步时间、实际是否亮／是否能熄灭。

完成后两个持续采集终端分别 Ctrl+C，再执行：

```sh
adb shell dmesg > oss-kernel-debug/flash-debug-r14/dmesg-after.txt
adb shell dumpsys media.camera > oss-kernel-debug/flash-debug-r14/media-camera.txt
adb shell cat /sys/class/flashlight_core/flashlight/flashlight_capability > oss-kernel-debug/flash-debug-r14/capability-after.txt
```

新日志应包含 `Registered GPIO flashlight, channels=2`，开关应出现 `on=1/0 ... ret=0`。若这些均正常但灯不亮，再检查实际 GPIO 电平和供电；不要根据 AVAILABLE_ON 单独判断恢复。请保留完整原始日志，不仅提供过滤结果。

## 实机验收

2026-10-11，用户确认“已经修复”，要求提交修改，无需继续分析日志。闪光灯修复标记实机通过；未提供逐项测试清单，不将拍照各模式和所有超时场景单独列为已验收。

内核修复提交：`56cfed2c53f9`（S17Promini: Restore stock GPIO flashlight control）。
