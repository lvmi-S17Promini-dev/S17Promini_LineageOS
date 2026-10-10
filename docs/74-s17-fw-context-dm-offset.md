# 首次提交 trace：确认并修正固件上下文 eDM 偏移

cache-log-8/s17-gpu-submit-r2/fw-samples/002.trace.txt 捕获 KICK(0x2abc0065) 和 COMBINED_TA_3D_KICK(0x2abc0075)，更新新上下文的 CCB write offset。TA 的 UFO 检查成功，但 cCCB Woff update 日志里 TA、3D、TQ 上下文 DM 均为 0。提交没有完全丢失；任务标识读取错误是更具体的证据。

原厂 Image 0x543378 为 str w27,[x25,#0x6c]，位于 common-context 创建和 CCB 地址初始化之间。vendor rgx.fw 的 microMIPS C00068E0 为 lw a2,0x6c(s2)，C00068E4..C00068F2 使用 SFID 0x7003100e 输出 cCCB Woff update = %d, DM = %d, FWCtx = ...，其中 a2 是 DM 参数。内核写入和固件读取都为 0x6c，独立于本次日志文本解码。

当前已编译 rgxfwutils.o 的 DWARF 显示 RGXFWIF_FWCOMMONCONTEXT 大小 160，eDM offset104(0x68)，ui64WaitSignalAddress offset112(0x70)。0x6c 是对齐空洞，零初始化后固件读为 GP(0)，与 trace 对应。

新增 CONFIG_MTK_RGX_S17_FW_CONTEXT，默认关闭，仅 S17 defconfig 开启。m1.13ED5776728 的 common-context 在 eDM 前增加一个零初始化 vendor reserved word，eDM 移到0x6c，消耗已有对齐空洞；结构体大小0xa0、后续字段从0x70开始不变。不猜测该 vendor word 的语义，不改 KICK 结构、频率、电源管理或 checkpoint 值。

构造函数添加 BUILD_BUG_ON 检查三个关键大小/偏移。独立主机布局测试提取实际结构体正文、使用与当前 DWARF 一致的基本类型和8字节对齐，分别验证开启和关闭配置：通用eDM0x68、S17 eDM0x6c，大小0xa0及wait/list/queue/robustness/deadline偏移保持不变，均通过 -Wall -Wextra -Werror 编译。该测试不代替完整内核编译；按用户要求未启动内核/Lineage编译。修改的源码通过 git diff --check。

刷机验证首先观察新的 TA/3D/TQ 是否读成 DM2/3/3，以及 CCCB read offset、完成 fence 是否推进。这个确定的 ABI 错误已修正，但其他 vendor ABI 差异仍可能存在，尚不能宣称设备黑屏完全修复。
