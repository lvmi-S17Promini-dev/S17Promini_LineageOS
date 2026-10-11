# r14 电源采集：charger_thread 忙循环与持续唤醒锁

2026-10-11，分析 `oss-kernel-debug/power-debug/s17-power-before` 与 `s17-power-after`。当前 boot 为 `4.19.127-s17-bringup-r14-flash+`。本轮仅分析日志与源码，未修改内核、编译或刷写。

## 结论与证据

最优先的问题是充电管理线程异常，并非已有证据证明 cpufreq 驱动只支持一个 cluster。

| 采集项 | before | after |
| --- | --- | --- |
| policy0 CPUs | 0–3 | 0–3 |
| policy0 governor / min / max | schedutil / 900000 / 2301000 kHz | 相同 |
| L PLL 实际频率 | 2301000 kHz | 2301000 kHz |
| policy4 CPUs | 4–7 | 4–7 |
| policy4 governor / min / max | schedutil / 400000 / 1800000 kHz | 相同 |
| LL PLL 实际频率 | 399875 kHz | 399875 kHz |
| charger_thread CPU（top） | 100% | 100% |
| charger_thread 累计 CPU 时间（top） | 2:29.28 | 10:43.93 |
| suspend success / fail / attempts | 0 / 0 / 0 | 0 / 0 / 0 |

这两份是串行快照，不是整个息屏期间的连续频率轨迹。policy4 在 after 中 scaling_cur_freq=1226000、cpuinfo_cur_freq=501000、稍后的 PLL=399875，采样时间不同，不能据此宣称频率反馈错误。高频 2301 MHz 对应当前内核的 policy0/L；App 的“后四核”排序需要与实际 CPU 编号区分。

PPM Final limit 两个 cluster 都是 `(15)(0)(4)(4)`，频率索引从最低 15 到最高 0，未锁定最高频。USER_LIMIT 虽显示 `(0)(0)`，其 is_activated=0，不能作为正在锁频的证据。cpufreq_sched_disable=0。`Performance(Sports) mode` 的显示来自源码默认 `dvfs_power_mode=3`，不等于 governor=performance，也不能凭字符串判断锁频根因。

## 息屏期间的直接证据

after/logcat 显示 11:09:56 电源键息屏，11:15:09 插 USB 唤醒。after/dumpsys-batterystats 显示：

- 电池供电且息屏 5m13.349s，realtime 与 uptime 相同，即该期间没有整机 suspend 驻留。
- kernel `charger suspend wakelock` 持锁 5m13.354s，几乎覆盖整段电池供电息屏时间。
- 原生 PowerManagerService.WakeLocks、Display、SuspendLockout 在该电池统计中的持锁时间只有约 0.4 秒；因此不能把重接 USB 后的 AccuBattery 当前唤醒锁当成整段拔线息屏的阻塞来源。

wakeup_sources 的 charger 锁 active_count=1，last_change=11465ms；before 的 active_since/total_time=114287ms，after=609203ms，说明从开机约 11.465 秒开始持续持有，未因拔线释放。after suspend_control_internal 也显示该锁 Active、总时间 654268ms。

MCDI 有 CPU-off 次数，但 cluster-off 为 0，dpidle/SODI/SODI3 都为 0。整机 suspend 尝试次数为 0，应先处理持续持锁和 CPU 忙循环，暂不归因于某个 suspend 回调失败。

## 充电线程忙循环的具体机制

日志时间线（after/dmesg）：

1. 11.496 秒：找到 pd 算法，但找不到 primary charger；打印 charger_init fail。
2. 16.635 秒：同样失败，再次打印 charger_init fail。
3. 21.759 秒：再次找到 pd，打印 pd_hal_init_hardware / init hw fail；此后没有 get pe2、get pe、primary charger 或 charger_init fail 的正常线程日志。之后标记 charger_thread 的其他日志多是 IRQ/计时器在该线程上下文中发生，不能当成充电线程继续正常执行的证据。

对应源码：

- `drivers/power/supply/mtk_charger.c:1329` 的 charger_init_algo 在检查 primary_chg **之前**，调用 pd 初始化和 `register_chg_alg_notifier(alg, &info->chg_alg_nb)`。
- 注册返回后才检查 primary_chg；缺失时返回 false。
- charger_routine_thread 失败后每 5 秒重试整个初始化；没有撤销已注册的 notifier。
- `mtk_charger_algorithm_class.c:146` 使用 srcu_notifier_chain_register。
- `kernel/notifier.c:22` 的 notifier_chain_register 没有检测重复对象。第一次将 nb 插入链表；第二次相同优先级遍历到 nb->next 后，将 nb 再次接到自身，形成 `nb->next=nb`；第三次循环永远走不到 NULL，产生 100% CPU 忙循环。

日志恰好在第三次 PD 初始化之后停止、线程长期占满一核，与这个静态可确定的缺陷高度吻合。当前没有该线程实时 PC/调用栈，因此“实机正卡在 notifier_chain_register”仍是有强证据的推断。

`_wake_up_charger` 在开机约 11 秒获取 charger_wakelock；初始化卡住使线程无法到达主循环末尾 `__pm_relax`，于是持续持锁。单核忙循环也足以使 schedutil 将承载线程的 cluster 请求到高频；修复前不应把 2301 MHz 直接定性为调频硬件失效。

## 为什么没有 primary_chg

原厂 config `ref/内核config.gz.txt` 明确启用 `CONFIG_CHARGER_PSC5425E=y`，而 MT6357 pulse charger 未开启。原厂 DT 和当前 s17promini-stock.dtsi 都有 enabled `psc5425e@6a`，compatible=`mediatek,psc5425e`，子节点 chg_name=`primary_chg`。

当前内核树没有 PSC5425E 源文件，也没有对应 Kconfig/Makefile 项。日志始终找不到 primary charger，与缺失 PSC5425E 支持一致。DT 中另有 MT6357 pulse charger 节点，但原厂并未启用其驱动；不能为了消除缺设备日志就直接启用 MT6357 pulse charger 替代 PSC5425E。

## 修复顺序及验收

1. 修正充电管理初始化的重试：先验证必需 charger_device 可用，再初始化算法；notifier 必须只注册一次。若支持多个算法，应给各链表独立的 notifier_block；同一个 block 不能作为多个链表的独立成员。失败退出/等待期间还需正确释放本线程持有的唤醒锁，避免仅消除忙循环却仍不深睡。
2. 接入原厂实际 PSC5425E 充电驱动，核对寄存器、charger class 接口、DT 参数和芯片标识；不能用伪造 primary_chg 或强制忽略错误替代。
3. 冷启动新内核后检查 charger_thread 回到等待态、CPU 接近 0%，primary_chg 注册成功、charger_init fail 消失。现有 notifier 自环不是杀掉 App 能恢复的。
4. 拔线息屏复测，确认 charger 锁不覆盖整段等待，suspend attempts/success 增长，有实际 suspend 驻留。再看 policy0 是否能正常下降；仍异常才继续做 governor/SSPM 运行时追踪。

若先补一次调用栈验证，可在 root 下触发 SysRq 全 CPU 栈并保存 dmesg，检查 charger_thread 是否落在 notifier_chain_register/srcu_notifier_chain_register/charger_init_algo。这是诊断操作，不会修复既有链表。
