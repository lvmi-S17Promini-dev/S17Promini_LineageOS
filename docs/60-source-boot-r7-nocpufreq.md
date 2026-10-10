# r7：临时屏蔽 MediaTek cpufreq 初始化

用户明确要求先屏蔽 cpufreq，避免继续多轮刷机细化日志。采用内核现有 `initcall_blacklist=_mt_cpufreq_pdrv_init`，跳过该驱动初始化及其 policy register / cluster-on 路径。CONFIG_KALLSYMS=y，init/main.c 的 blacklist 支持可用。

保留 CONFIG_MTK_CPU_FREQ 和相关符号，避免依赖模块因删除整个对象出现链接问题。这是禁用驱动初始化的旁路验证，不等于完成所有 cpufreq 客户端的适配。其他模块仍可能访问未注册的 cpufreq policy，须从后续实机日志判断。动态调频不可用；不手工指定电压/频率，不屏蔽看门狗。

设备树 BoardConfigRom.mk 新增源内核模式下的 `S17_SKIP_CPUFREQ_INIT=true` 开关，默认关闭。构建 r7 时显式开启。cmdline 保留 initcall_debug、loglevel=8、ignore_loglevel，记录后续全部 initcall 的进入/返回；r6 的旁路诊断仍在源码内，但本次目标调用被跳过，不会启动该 observer。

版本 `4.19.127-s17-bringup-r7-nocpufreq`。在 lineage 环境使用绝对 OUT_DIR=/home/dengxh/android3/lineage/out-s17-sourceboot、S17_SOURCE_KERNEL_BOOT=true、S17_SKIP_CPUFREQ_INIT=true 构建 bootimage。

此镜像优先于未测试的 r6。实机结果用于判断：是否越过 cpufreq 卡点、是否执行 /init 并出现动画，以及是否出现后续 panic/其他 initcall 阻塞。pstore 放到 s17-pstore-r7。

已检查 System.map 中存在精确符号 `_mt_cpufreq_pdrv_init`，与 blacklist 参数名称一致。该平台原代码也有 CPU_DVFS_NOT_READY 编译期开关提前返回路径；本次使用现有 initcall blacklist 达成运行时旁路，保持默认构建可恢复。

## 产物验证

`artifacts/source-boot-r7/boot-source-r7-nocpufreq.img`，32 MiB，SHA256 `5abc409e78c316f5a28e44ce2b0fb49511f4bd24d020dd98356a2f31f6821d28`。Lineage bootimage 构建通过。已校验源内核/DTB、原厂 ramdisk、加载地址、AVB、Android libufdt pinctrl，以及镜像中的 blacklist 参数与 CONFIG_KALLSYMS。DTB/ramdisk 与 r5 一致；尚待实机测试。
