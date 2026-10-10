# cpufreq 与相关源码最近三个月审计

审计日期 2026-10-10，窗口 2026-07-10 至 2026-10-10。执行 git fetch origin 后核对 S17Promini-bringup 的 HEAD 祖先、远端 marlon-bringup 和其他已获取分支。远端 marlon HEAD 仍为 9fd0d5ca5f05。以 committer 时间筛选，另记录 author 时间，避免遗漏最近 cherry-pick 的旧补丁。HEAD 核对使用 --since-as-filter。命令结果保存在 `extracted/metadata/cpufreq-three-month-history.txt`。

## 直接调用链

以下路径窗口内没有提交修改：

- cpufreq_v1 的 main.c、hybrid.c 及 mach/mt6765 平台/OPP 表；
- sspm 目录，包括 ipi send-sync、mbox polling 和 mailbox 表；
- 通用 drivers/cpufreq；
- arch/arm64/lib/delay.c、drivers/clocksource/arm_arch_timer.c；
- PPM、EEM、FHCTL；MT6357 regulator 路径。

main.c、hybrid.c、sspm_ipi_mbox.c 最近一次提交为 220d8e1a728d，committer 日期 2022-08-01。sspm_mbox.c 最近一次为 f8c6e9c6c280，日期 2021-01-28。其他 OPPO 分支的历史不等于当前分支引入的改动。

## 确实存在的相关变化

| 日期（提交原时区） | 提交 | 变化与适用范围 |
| --- | --- | --- |
| 2026-08-13 | 194fdcb28ef5 | MCDI get_cluster_off_token 的参数补上 int 类型；MT6370 有编译兼容修改。 |
| 2026-08-14 | 94d218982dc3 | EAS+ 的 POD not ready 日志改成 once；不改变返回值/控制路径。 |
| 2026-08-23 | 6b0aa07e91d9 | scheduler Makefile 移除 CONFIG_SCHED_WALT 对应的 walt.o；cpupri 删除 OPPO UX 任务的 CPU mask 过滤/踢除逻辑。 |
| 2026-09-16 | e4fb2179a8a9 | 唯一直接 cpufreq_v1 目录提交：interface.c 默认 dvfs_power_mode 从零初始化改成 3。schedutil up/down 间隔改成 500/15000 us；fair migration cost 500000→50000；HPS 上线阈值 95→80、采样次数 4→1，下线阈值 85→65、采样次数 1→6。另改 GPU DVFS/限流，p50 defconfig 改 HZ=300。 |
| 2026-09-18 | 22845c7f98bd | PELT 衰减表/周期从 32 改为 16，影响负载统计和调频请求；另改 GPU/FPSGo。 |
| 2026-09-30 | c4025400b8ea | top-app 最小 boost=15、prefer_idle=1；另改 GPU/FPSGo/内存参数。 |
| 2026-10-02（committer） | 807038126655 等 | scheduler 相关 sysctl/PSI/BPF backport；locking 补丁删除 lockdep 未使用的 nested 参数，没有改 mailbox 锁或 arm64 锁算法。author 时间在 2019/2020，故必须按 committer 时间查。 |

这些提交均在当前 HEAD 的祖先中，属于继承的 marlon/p50 改动。p50 的 HZ=300 配置未进入我们的实际配置：r7 为 CONFIG_HZ_250=y / CONFIG_HZ=250。HPS_v3/MCDI 由 MT6765 平台 Makefile 编译。

## 与卡点的关系

cluster-on 参数构造/发送没有读取 dvfs_power_mode。该变量在接口文件用于显示/写入，API mt_cpufreq_get_ppb_state 返回它；全树搜索没有找到该 API 的调用者，仅有声明和定义。因此目前没有静态证据表明默认值 3 改变了这次 command 2 的参数。

schedutil 改动位于 sugov_init，发生在 cpufreq 驱动 policy init 返回之后；当前阻塞于驱动 policy init 内，不应直接归因于该 governor 的限速参数。HPS/PELT 等会影响整体调度时序，不能仅靠历史审计完全排除间接影响，但它们没有改 SSPM 的 ACK 协议。

九套 SRAM PV/分频表均在原厂 Image 中找到逐字节相同副本，详见 stock-dvfs-pv-table-comparison.json。这不证明固件或实际运行时选择完全一致。

## 本地未提交变化

当前 cpufreq main/platform/hybrid 和 SSPM 三个文件有我们 r2–r6 添加的日志、检查点、旁路 observer 状态写入。它们不属于上游历史，确实会影响时序；未修改命令值、锁顺序、重试次数和频率电压策略。r7 通过显式 S17_SKIP_CPUFREQ_INIT=true / initcall_blacklist 跳过 _mt_cpufreq_pdrv_init，是用户授权的临时验证，须与正常初始化对照。

结论：最近三个月有外围性能优化，并非只有显示和相机变化；但造成当前阻塞的 cpufreq→SSPM 直接实现近期没有提交改动。优先看 r7 能否越过初始化，不在无证据时批量回退所有性能补丁。此审计未改源码或制作新的 boot。
