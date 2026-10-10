# reboot-debug-2：确认卡在 deferred-probe 工作完成等待

2026-10-11。本次分析只使用 boot ID b276f6fa-9c9f-4927-a601-9a235984acd8 的 mode=reboot session（目录后缀 79-549173004-2435）。目录中混有上一轮 a28f6d4d 的旧日志和本轮 --check，不混用。

## 确定的卡点

task-snapshots.log 的 10.200、30.091、60.169 秒三个样本完全一致：PID 1 的 syscall 为 142（reboot），调用栈为 kernel_restart → kernel_restart_prepare → device_shutdown → wait_for_device_probe → flush_work → __flush_work，wchan 为 __flush_work。

因此上一轮优先怀疑的 system_transition_mutex 不成立：这次已取得该锁并进入 kernel_restart，阻塞点是 wait_for_device_probe 的第一步，即 flush_work(&deferred_probe_work)，尚未走到 probe_count 等待、cpufreq_suspend 或逐个设备 shutdown 回调。

本轮实际 initcall_debug 为 N，虽 cmdline 包含该参数，运行值已经变了。源码 drivers/misc/mediatek/mtprof/bootprof.c:240 在开机结束处理里设置 initcall_debug=0。这解释了此前缺少阶段日志；不能再仅依据缺检查点判断执行边界。

SysRq 工作队列转储显示 events 队列有 deferred_probe_work_func in-flight 和 flush barrier，支持 PID 1 确实等待它。但完整任务转储没有提供一个能对应具体驱动 probe 的有效栈，不能把表中的 worker 标识与同号用户服务混作同一任务，也不能据此直接宣称具体驱动或 worker 内存损坏。

## 新增针对性定位

drivers/base/dd.c 增加 /sys/kernel/debug/s17_deferred_probe，保存当前 deferred work 的 active、PID、复制的设备名/驱动名和回调结果。该节点使用 mutex_trylock，锁忙时返回说明，不在日志服务中无限等待 deferred_probe_mutex。设备名在移出 active list 后、释放该 mutex 前保存；驱动名在对应 really_probe 调用前更新，结果初始为 -EINPROGRESS，返回后更新真实值。

只有当前 deferred worker 的 probe 回调输出 [S17/PROBE] begin/returned，不依赖会被 bootprof 关闭的 initcall_debug。不修改 probe 返回值、重试顺序、flush、锁或关机等待策略。诊断只记录复制字符串/PID，不从 debugfs 解引用可能变化的 worker task 指针。

现有 reboot-kmsg 采集器预先打开这个新节点，沿用 10/30/60 秒的 task-snapshots.log 采样。私有 namespace 移除 debugfs 后仍通过 FD 读取。旧 snapshot 的 PID 1 栈和 SysRq 转储继续保留。

编译由用户执行，本轮未运行内核/Android 构建、未刷写、未提交。C 语法、rc 验证、内核补丁 checkpatch/空白检查通过；新诊断节点的实际读数和驱动回调仍需实机验证。

## 下一轮

更新 boot 和 system 后，按原 --check → --arm → ctl.start → svc power reboot 流程，等待至少 70 秒再走 fastboot/recovery 拉 /cache/s17-reboot 到 reboot-debug-3。新增节点和 [S17/PROBE] 会显示具体设备/驱动；据此修复该 probe 的无限等待或其他实际问题。不要直接跳过 wait_for_device_probe：这会使关机与仍在执行的 probe 并发，尚不是安全修复。
