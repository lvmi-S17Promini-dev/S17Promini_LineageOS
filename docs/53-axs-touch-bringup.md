# AXS 触摸基础接入

2026-10-09：显示驱动先提交为内核 `b11b89b4bdad`，对应逆向记录 `0d019fc`，随后在 `S17Promini-bringup` 继续触摸工作。

触摸基础实现提交：`60faf9d34423`。

## 本轮完成

- 新增 `drivers/input/touchscreen/axs15260_s17.c` 和报点解析头文件；启用 `CONFIG_TOUCHSCREEN_MTK_AXS`。
- DTS 将 I2C0 的 BSP `touch@38` 替换为实际地址 `axs15260@3b`，恢复 GPIO0 下降沿 IRQ、原厂输入 pinctrl 和 vldo28 触摸供电。
- 读取现有固件版本、接入 32 字节原始 I2C 报点、最多 5 个触点的 type-A 输入上报，设备名仍为 `axs_ts`。
- 屏幕/系统休眠时暂停 IRQ、清除已有按下状态；GPIO174 复位仍由显示驱动管理，没有新增第二个 GPIO 所有者。
- 固件升级和手势唤醒尚未接入，当前驱动不执行 flash/升级命令。

## 原厂逆向依据

原厂 probe 在 Image `0x9f88c4` 比较地址 0x3b，随后覆盖 DTS 的其他地址；仅按原厂静态 DTS 的 0x38 配置会访问错误地址。

IRQ 解析入口 `0x9f92a4` 读取 32 字节，头部两字节后为 6 字节/点，最多 5 点；X/Y 各 12 位，触点 ID 在 Y 高字节上半部，event 在 X 高字节最高两位。0/2 表示按下/接触，pressure/area 为零时使用 63/9。

固件版本通过 0x0c 的一字节命令加两字节读取得到。已经提取原厂内嵌固件 50192 字节，按原厂版本偏移算法得到版本 0007；它保存在忽略的 `extracted/kernel-reverse/axs-touch` 下，仅供后续分析，不上传设备。

当前恢复的是基本输入路径。注册节点并不能证明固件已就绪或 IRQ 能收到触摸数据；在实机取证后再判断是否必须移植原厂升级逻辑。

## 验证

- 触摸目标文件及新 DTB 编译通过。
- defconfig 归一化后只增加触摸开关。
- driver/header checkpatch 0 errors、0 warnings。
- 实际 C 解析器通过坐标边界、5 点步长、事件/ID、pressure/area、无效 ID、空包、特殊头和异常长度测试；这些是合成数据，不是实机触摸测试。
- FDT 核对确认地址 0x3b、IRQ `<0 2>`、中断父控制器 0x1f、触摸供电 0x8c、pinctrl 0x105，原 0x38 节点已移除。标准 touchscreen size 属性为 385/855，用于复现原厂包含上界的轴最大值 384/854。

证据：`extracted/metadata/axs-touch-stock.json`、`axs-touch-disassembly.txt`、`axs-touch-dtb-validation.json`。复现工具：`tools/reverse_axs_touch.py` 和 `tools/check_axs_touch_protocol.py`。内核详细说明：`Documentation/S17Promini-axs-touch.md`。

此轮未生成完整源码内核 boot，也未刷机。新实现比原厂更严格地拒绝点数大于 5 的包，并在空包或 I2C 失败时主动清除旧按下状态；不能称为原厂触摸驱动逐指令完整还原。下一步结合完整内核候选，核对 probe 版本日志、IRQ 计数、真实坐标以及息屏/唤醒，再决定固件和复位协同的后续处理。
