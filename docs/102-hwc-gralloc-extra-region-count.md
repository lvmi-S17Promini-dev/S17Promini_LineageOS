# HWC 每帧 private handle 报错：GED 元数据区域数量不匹配

2026-10-11。用户提供 09:48 的 hwcomposer 日志，要求排查并修复。已有 wifi-debug-2/normalized/wifi-logcat.txt 在 09:37 就包含同类报错，两类错误合计 8402 行。这是用户空间 logcat 的显示合成错误；与已验收的 Wi-Fi DMA MPU region 修复分别处理。

## 直接证据

对当前 vendor 的 64 位 hwcomposer.mt6765.so、libgralloc_extra.so 和 libged.so 进行 AArch64 反汇编：

- HWC 的每帧 private handle 查询函数位于 0x83920，必调 query 117；按 feature 条件还调用 114 和 120。将查询返回值 OR 后，非零输出 getPrivateHandleInfoModifyPerFrame。setupPrivateHandle 将该失败转换为 -22。
- libgralloc_extra.so 的 ge_alloc 在 0x3064 使用 mov w0,#18，区域大小表位于 0x737c，共 18 项。
- query 117 的分支表指向 0x2298，读取 region 13、offset 0、1 个 uint32；query 120 指向 0x22c8，读取 region 16、offset 0、30 个 uint32。相应区域原厂大小分别为 4 和 120 字节。
- ged_ge_get 的失败在 gralloc_extra 中转换为 -6，即日志的 fffffffa；并非仅凭 -22 推断整个 native handle 格式错误。

源码 drivers/gpu/mediatek/ged/src/ged_ge.h 定义 GE_ALLOC_STRUCT_NUM=13。ged_main.c 分配并复制 GE_ALLOC 输入时只复制 13 项大小，ged_ge.c 又强制把 region_num 设置为该宏。valid_parameters 因 region_id >= entry->region_num 拒绝 region 13/16。原厂请求的后五个区域因此根本没有建立。

用户提供 Ghidra 12.1.4 路径，本轮上述函数及只读表的直接反汇编已足以确认参数，未启动 Ghidra 分析。

## 修复

将 GE_ALLOC_STRUCT_NUM 改为 18，并注释本机 vendor 的配套要求。该宏同时调整输入大小表复制及对象区域数量，无需修改 vendor/system，不修改 HWC 日志级别或将失败伪装为成功。

此常量是本机配套 vendor 的固定 ABI，不能据此宣称适配所有 MT6765 vendor。新对象需要用更新内核重新启动后建立。

## 验证与实机边界

核对原厂 ge_alloc opcode、分支表及 region 13/16 参数，导出完整 18 项大小表到 extracted/metadata/stock-gralloc-region-check.json，含库 SHA256。

从实际 ged_ge.c 抽取 valid_parameters 编译主机测试，使用原厂大小表。旧 13 区域配置复现 query 117/120 被拒；18 区域配置两项通过，原 query 114 仍通过。全部 18 个区域的恰好边界通过、超过边界拒绝，以及非法区域、负 offset/size 拒绝。ASan/UBSan、编译告警检查通过；原函数已有有符号/无符号比较单独关闭 sign-compare。内核 diff --check 和 checkpatch 通过。

本轮代理未进行完整内核编译、刷机或提交。复测方法为在已配置的 Lineage 构建环境执行 m -j8 bootimage，安装新 boot 并重启，无需更新 system/vendor。

## 实机验收

2026-10-11 用户反馈“修复ok”，本次 HWC private handle 刷日志修复标记实机通过。未提供修复后的完整日志或分项测试结果，不将桌面滚动、视频、熄屏/唤醒分别记为已验收。

用户随后要求提交，内核修复提交为 `237a4e69f539`（S17Promini: Match GED metadata regions to vendor gralloc）。
