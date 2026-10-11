# 后四核频率与息屏 suspend 排查

2026-10-11，检查 camera-debug/normalized 的 r12 日志及当前内核源码。尚无设备在线，未执行实机改动、编译或刷写。

## 已有证据

- dmesg-after 第 4314–4398 行：cluster 0/1 初始化 IPI 都返回 ret=0、ack=0；两个 policy 注册和 OPP/energy model 初始化成功。这只证明初始化，不证明运行时每个 cluster 都能降频。
- 第 9770/9771 行：vendor init.cgroup.rc 设置 policy0/4 governor=schedplus 均失败（EINVAL）。当前构建 KERNEL_OBJ/.config 默认 governor 为 schedutil，且没有 schedplus；因此这是兼容问题，但失败本身不能证明实际 governor 是 performance，也不能解释为什么仅一组核异常。必须读取实机 scaling_governor。
- 第 863 行的 MCDI `can not iomap` 是误导信息：mt6765/mtk_mcdi_plat.c:119 判断为 `if (*base)`，映射成功才打印失败。第 9390 行的 bootup check PASS 仅表示启动时间门槛通过，不证明进入 cluster-off 或系统 suspend。
- 日志没有 `PM: suspend entry/exit` 或系统息屏进入 suspend 的完整过程。该日志主要覆盖相机测试，不足以确定深睡失败阶段。
- logcat 第 11919 行充电应用（PID 2597）打印 `Acquire charge wakelock for 15 min`；GNSS HAL 多次 acquire/release；ISP 在相机测试期间也持锁。需要息屏时的 dumpsys power 和 wakeup_sources 才能确认阻塞者。AVC 为 permissive=1，不能将这些记录当作访问被实际阻断。
- MUSB 缺少 type_c_port0 的工作每约 0.5 秒重试。这会产生工作负载，但没有证据证明它持有阻止系统 suspend 的锁。

还有一个需要优先核实的编号差异：当前源码 cpu0–3 对应 MT_CPU_DVFS_L（PLL_L），cpu4–7 对应 MT_CPU_DVFS_LL（PLL_LL）；启动日志也显示 cpu4 读取 LL PLL=919500 kHz，而 cpu0 的 L PLL 后续读数=1878000 kHz。不能按监控软件排列直接认为后四核对应高频 L cluster。如果 policy4 的 OPP 上限小于所显示的 2300 多 MHz，需优先对照 policy、PLL、监控软件编号与原厂拓扑，不能盲目交换 cluster 编号。

## 第一轮采集

`tools/capture_s17_power_debug.sh` 是手机端 root 快照脚本，只读取调频、电源和日志接口，写入指定新目录；不修改 governor、PPM 或 suspend 开关。不存在的节点跳过，失败信息保留。若 debugfs 未挂载，wakeup_sources 等可能缺失；可用 root 挂载 debugfs 后补采。sysfs wakeup 统计也会采集。

在电脑执行（userdebug 可先 adb root；若只能 su，改用 adb shell su -c）：

```sh
adb root
adb push tools/capture_s17_power_debug.sh /data/local/tmp/
adb shell sh /data/local/tmp/capture_s17_power_debug.sh /data/local/tmp/s17-power-before
```

退出相机和频率监控应用，拔掉 USB 和充电线，手动息屏等待 3–5 分钟。期间不运行轮询脚本，不持续抓 logcat。手动唤醒后再接 USB，执行：

```sh
adb shell sh /data/local/tmp/capture_s17_power_debug.sh /data/local/tmp/s17-power-after
adb pull /data/local/tmp/s17-power-before oss-kernel-debug/power-debug/before
adb pull /data/local/tmp/s17-power-after oss-kernel-debug/power-debug/after
```

唤醒和重接 USB 会改变当前唤醒锁状态，因此需同时看累积时间差、suspend 成功/失败计数和 dmesg，而不能只看 after 的 active 状态。如没有深睡，另做一次 USB 连接下的息屏快照可以找持续持锁者，但不能用连接 USB 的测试代替拔线测试。

## 调频判读与可恢复实验

先比较两个 policy 的 related_cpus、governor、min/max、当前频率，再对照 `/proc/cpufreq/MT_CPU_DVFS_L/cpufreq_freq`、LL 对应节点（源码直接读取 PLL）和 oppidx。

- governor=performance：先切为可用的 schedutil，再比较实际频率。
- min=max 或 min 接近最大：查 PPM dump_policy_list 的 Final limit 与 activated policy（SYS_BOOST、USER_LIMIT、HARD_USER_LIMIT 等）。OPP 索引需按 oppidx 表换算；索引 0 通常是最高频。不能直接关闭全部 PPM，避免丢失热限制。
- governor=schedutil、min/max 正常而频率高：结合 CPU 使用率、schedtune boost、PPM 约束判断；检查 cpufreq_sched_disable 的实际值。
- sysfs 频率下降、PLL 仍高：优先核对反馈、cluster/PLL 映射及 SSPM 执行路径；所有读数正常而 App 显示高，则先解决 App 编号/读数问题。

在 root 手机 shell 中，选 **实际异常的 policy** 做短时 powersave 对照；不要凭 App 编号固定选 policy4。先保存快照。以下以 policy4 为例，先确认 available_governors 包含 powersave：

```sh
p=/sys/devices/system/cpu/cpufreq/policy4
cat "$p/scaling_available_governors"
old_gov=$(cat "$p/scaling_governor")
echo powersave > "$p/scaling_governor"
sleep 3
cat "$p/scaling_governor" "$p/scaling_min_freq" "$p/scaling_max_freq" "$p/scaling_cur_freq"
cat /proc/cpufreq/MT_CPU_DVFS_L/cpufreq_freq /proc/cpufreq/MT_CPU_DVFS_LL/cpufreq_freq
echo "$old_gov" > "$p/scaling_governor"
```

能降频支持运行时降频路径可用，应继续查 governor/负载/限制；不能降频仍需检查 min 限制、固件自治调频及实际 governor，不能直接认定 SRAM 表坏了。若 shell 中断，恢复保存的 governor 或重启。暂不往 cpufreq_freq/volt 调试节点写值，它们会进入固定频率/电压路径。

## suspend 判读

1. 完全没有 suspend entry：查 dumpsys power 的 PARTIAL_WAKE_LOCK、Suspend Blockers，suspend_control 的统计及 kernel wakeup_sources；优先检验充电应用、GNSS、USB、ISP 是否持续持锁。
2. 有 entry 但失败：查 suspend_stats 的 failed_*、last_failed_dev/errno 和 dmesg 中失败设备，定位 prepare/suspend/noirq 阶段。
3. suspend 成功但马上退出：查 last_resume_reason、SPM wake reason，比较 wakeup/IRQ 计数，定位反复唤醒源。
4. 系统 suspend 有驻留但 App 深睡为零：核对监控口径。cpuidle 的 WFI/CPU-off/cluster-off 与整机 suspend 是不同统计；单看当前频率或 MCDI PASS 不能判断整机深睡。

下一步根据这轮实机采集再决定是否修正 vendor governor 配置、PPM 限制来源、SSPM 映射或某个设备的 suspend 回调。
