# S17Promini 源码内核移植初步评估

2026-10-09，检查本地 `~/android3/lineage/kernel/lvmi/mt6765`，HEAD `9fd0d5ca5f05`。这是源码与原厂材料的静态评估，尚未编译或实机验证。

结论：值得以此树作为移植起点，但不能据同为 MT6765 推断只换屏幕即可。先解决显示及 GPU 用户态兼容，再逐项确认板级外设；相机后置处理。

## 已确认差异

| 项目 | S17Promini 原厂 | marlon 配置 |
| --- | --- | --- |
| 平台 | mt6765 | mt6765 |
| 工程 | r65v3_pmz_w1a | v956 |
| 屏幕 | axs15260_dsi_6580_hd，384×854 | ILI9881H/ICNL9911 等，720×1520 |
| GPU DDK | rgx doma m1.11ED5425693 | rgx doma m1.13ED5776728 |
| 相机编译列表 | gc5025sub_mipi_raw、imx278_mipi_raw | imx486、s5k2t7sp、ov5670、gc8034、gc5025sub、gc5035 |
| 触摸 | 原厂配置启用 MTK 框架，具体驱动仍需确认 | ILITEK_V2 |

证据：`extracted/metadata/kernel.config` 和内核 `arch/arm64/configs/marlon_defconfig`。树中已有 gc5025sub 驱动；按文件名搜索未找到 axs15260 和 imx278，不能视为已经支持。GPU 版本不同，需要进一步检查源码目录和原厂 EGL/gralloc/PVR 库的接口配套。

## 显示已有可靠线索

`lineage-debug/normalized/lk-boot-reassembled.log:1424` 明确记录实际选中的 axs15260，优先级高于静态 DTBO 中残留的 nt35695B 名称。

- DSI：SYNC_EVENT_VDO_MODE，3 lanes。
- 分辨率：384×854。
- 垂直时序：sync 2、back porch 20、front porch 170。
- 水平时序：sync 20、back porch 180、front porch 180。
- PLL_CLOCK：210；其余时钟选项保留日志供后续逐项核对。

逆向重点因此是原厂 LCM 初始化命令、复位/偏压供电时序、suspend/resume 和 ESD 检查，不必先猜分辨率及基本时序。可从 `extracted/boot/kernel` 提取原厂代码并恢复符号/数据引用，以 `stock_imgs` 中 LK 作交叉核对。日志参数来自 LK，还需核对 kernel 的 get_params 实现。

## marlon 改动范围

提交历史表明，除屏幕和相机，作者还处理了触摸、GPU、fuelgauged、传感器、OPPO 内存/热管理定制等。代表提交：

- `94c7f62ae64d`：LCM 结构及 DSI 工具接口对齐原厂 ABI。
- `f44ad82e9bbe`：移除 OPPO 定制并调整传感器校准。
- `90e87ad42f01`：fuelgauged 协议及 video mode splash。
- `6adf3c388453`：ILITEK 手势唤醒和 IRQ 同步。
- `6f9dc57cfc7c`：PowerVR DDK 和 display idle manager。

这些说明已有大量可复用的平台修复，也说明设备专用行为不能直接全盘继承。作者设备“完美”的反馈尚未在我们的设备上验证。

## 后续实施顺序

1. 比较原厂配置、DTB/DTBO 与 marlon，确认 GPU 配套、PMIC、充电、触摸及保留内存。静态 overlay 的相机候选列表不等于实际安装型号。
2. 恢复 axs15260 的 get_params、初始化表、供电和休眠唤醒回调，生成独立 S17 配置/板级文件。
3. 构建可回退的候选 boot，先验证串口/ADB/日志、亮屏、息屏唤醒及背光。当前已验证原厂内核 ROM 基线继续保留。
4. 确认触摸、GPU 合成、充电、声音、无线等，再处理相机。编译成功与启动成功都不能单独证明外设兼容。

本轮只新增评估文档，未更换当前内核或刷写设备。
