# AXS15260 原厂驱动逆向与接入

2026-10-09，内核分支 `S17Promini-bringup`，在配置基线 `c17173daebf2` 上继续。

已恢复并接入 `drivers/misc/mediatek/lcm/axs15260_dsi_6580_hd/axs15260_dsi_6580_hd.c`。LCM 子目录（包括新驱动、注册列表及公共实现）和更新后的 DTB 已编译通过；未完整编译内核、生成 boot 或实机刷入。

## 关键发现

- 原厂内核实际为 4.19.191。解压 Image 的虚拟基址是 `0xffffff8008080000`；唯一屏幕名指向 `0x176e8b0` 的 LCM_DRIVER，可从回调指针定位实际执行代码。
- 实际初始化表仅 8 项，位于 Image `0x176e9f8`，包括 BB 解锁、两条 B2、11/00、120 ms、29/00、10 ms、结束。与 LK 的初始化表全部 576 字节一致。
- 原厂 kernel 的休眠表为 28/00、120 ms、10/00、30 ms、结束；LK 的等待不同，按 kernel 恢复。
- 偏压采用 GPIO 150 和 I2C5 地址 0x3e；寄存器 0/1 均写 0x12，初始化前启用、休眠后关闭。原厂 power 三个独立回调为空，不能照搬 marlon 的 regulator 电源回调。
- `axs_reset_proc(20)` 是触摸复位协同，不是普通延时。它脉冲 GPIO 174；原厂 kernel、LK 以及 stock pinctrl 相互印证。新驱动暂时通过新增 DT `touch-reset-gpios` 管理这条线，后续移植 AXS 触摸驱动时需要移交/统一所有权和状态处理。
- 屏幕 ID：读 DA/DB/DC 三字节后比较 0x005260。
- 显示参数为 384×854、3 lanes、SYNC_EVENT_VDO_MODE、RGB888、PLL 210，垂直 2/20/170、水平 20/180/180；与 LK 日志一致。
- 修正当前头文件 RGB888 枚举为原厂的 3，补 RGB666_LOOSE=2；当前参数结构较原厂短，但全部实际赋值字段均落在现有结构内且偏移一致。

## 验证及产物

- `tools/reverse_axs15260.py`：校验原厂 kernel SHA256，提取表、反汇编辅助函数，重放 get_params 的直线赋值；默认输出至忽略的 `extracted/kernel-reverse/`。
- `extracted/metadata/axs15260-stock.json`、`axs15260-disassembly.txt`：恢复数据及原厂执行代码证据。
- `axs15260-lk-disassembly.txt`、`axs15260-lk-table-validation.json`：LK 交叉核对。
- `axs15260-data-validation.json`：恢复 C 编译后的初始化表 576 字节、休眠表 360 字节、显示参数 1248 字节与原厂对应内容完全一致。
- `axs15260-dtb-validation.json`：保留原厂所有节点和其余属性；在此前机型/屏幕名变化基础上仅增加 GPIO174 的描述，指向真正的 GPIO 控制器 `pinctrl@1000b000`。
- 新驱动 checkpatch：0 errors、0 warnings。局部编译方法详见内核 `Documentation/S17Promini-axs15260.md`，必须构建 LCM 父目录以继承 include 路径。

当前仅完成显示驱动重建及静态/局部编译验证；触摸输入和固件加载尚未恢复，当前复位桥接不等同于完整触摸驱动。后续仍需补齐其他导致完整构建失败的驱动配置，才能制作候选 boot 验证实际亮屏、交接首帧、背光和休眠唤醒。
