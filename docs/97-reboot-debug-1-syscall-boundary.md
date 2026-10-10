# reboot-debug-1：卸载完成后、kernel_restart 之前停住

2026-10-11。用户提供 `oss-kernel-debug/reboot-debug-1`，并要求继续定位。

## 本次日志有效

目录包含 mode=check 和 mode=reboot 两次服务运行，共用 boot ID `a28f6d4d-c60b-45ee-b089-0cf8bc508bee`。分析对象是 `s17-reboot/a28f6d4d-c60b-45ee-b089-0cf8bc508bee-134-77024315-2460/kmsg-0.log`，SHA256 `12175bcf6876e8f59990bc43636ced2c32ff70652fed226874b17750a44833bc`。

身份为 r11-connectivity #18，编译时间 2026-10-11 01:28:56，与当前主机 Image.gz 内的 Linux identity 一致。cmdline 有 initcall_debug，无 cpufreq blacklist；主机镜像确认包含之前增加的 kernel_restart/device_shutdown 等 S17 检查点字符串。

采集记录从 134.066518 秒到 194.810958 秒，共 1044 条，kmsg 序号 10818–11861 完全连续，没有 overrun/序号跳跃。此前的 pstore 旧记录与本次无关。

## 关机已走到哪里

- 137.244735 秒 adbd 进程组被清理；之后采集服务仍持续写入。
- init 完成 vold 停止、24 ms 的 sync、APEX 卸载、data/nvcfg/nvdata/protect/cache/metadata 卸载。
- 137.654690 秒 init 的 `/cache` 卸载成功，之后日志仍保存；私有 cache mount 的设计在此次实机成功覆盖了这个阶段。
- `/data/user/0` 的一次 Invalid argument 不阻止流程，`/data` 本身成功卸载，最终 status 为 0。
- 137.765551 秒第二次 sync 完成；137.865811 秒输出 `powerctl_shutdown_time_ms:3872:0`。
- 137.865857 秒 PID 1 输出 `Reboot ending, jumping to kernel`。
- 后续至 194.810958 秒仍有 USB workqueue、温度、电池、hang_detect 和 watchdog ping。不是整机所有 CPU 已停止，也不是采集器恰好在 ADB 断开时退出。

没有出现 `kernel_restart entry` 或后续 notifier/device_shutdown/syscore 检查点；本轮 PVR_K、heap RETRY 的出现次数均为 0。因此当前主卡点不能继续解释为 init 卸载 cache/data，亦不能依据上一轮 PVR 刷屏就认定这轮卡在 GPU 清理。GPU callback 修复是否完全通过长期稳定性测试仍未确认。

## 新的定位边界

init 的上述消息位于 `RebootSystem()` 开头，后面还包括 IsRebootCapable 与 reboot syscall。源码的 sys_reboot 在进入 kernel_restart 前还会检查 capability/参数/PID namespace、等待 system_transition_mutex，并在 RESTART2 时复制用户参数。

优先怀疑 system_transition_mutex 的等待，但当前没有 syscall 前后日志或 PID 1 栈，所以不能把“锁已被某个 PM 线程永久持有”记成已确定事实。仍需排除 runtime initcall_debug 被改为 false、用户态能力检查、其他 syscall 前置步骤和参数复制异常。主机镜像身份/检查点字符串匹配提高了证据可信度，但没有替代对实机参数值和任务栈的读取。

静态核查：此锁与 suspend、lock_system_sleep、软件休眠路径共享。实际配置没有 CONFIG_HIBERNATION 和 CONFIG_PM_AUTOSLEEP；捕获日志中的 system_suspend 服务在 134.141 秒已经退出。检查到的 suspend/PM 设置接口都有对应释放路径；MTK DLPT 的 trylock 路径只在连续低电压计数达到阈值时直接 kernel_restart，本轮电池日志为 SOC 100、VBAT 约 4.388 V，没有触发低电压路径的证据。尚未找到能直接解释本次缺失检查点的静态锁泄漏，不绕过转换锁强制复位。

## 下一轮源码取证

`kernel/reboot.c` 新增 initcall_debug 下的 syscall entry（PID/cmd）、转换锁等待/取得，以及 RESTART2 参数复制前后检查点。保留原锁、验证、复制和重启顺序。

采集器升级 v2，在 detach proc/sys 前预先打开 `/proc/1/stack`、`/proc/1/wchan`、`/proc/1/syscall` 和 `/sys/module/kernel/parameters/initcall_debug`。关机采集开始后的 10、30、60 秒读取这些 FD，将结果同步写到当前 session 的 `task-snapshots.log`。节点不可读或不可 seek 时记录错误，不把缺数据当成正常栈。

首次延时采样还通过事先打开的 `/proc/sysrq-trigger` 写入 `w` 和 `t`，转储阻塞任务及任务栈到 kmsg；不会写入 c/b/o，不触发 panic 或复位。该操作只有已 armed 且普通 reboot 采集超过 10 秒时执行。正常迅速完成的重启不会到达这一步。`--check` 保存一次 snapshot 验证 FD 读取，但不会触发 SysRq。

内核原先阶段检查点继续保留。v2 仍只在本轮 --arm 后采集，文件目录、轮换与用户手工拉取方式不变。

## 验证与下一步

主机提取实际 snapshot helper，验证同一 FD 的重复 seek/read、内容变化、debug 参数、缺失节点和不可 seek FD 的错误保存，ASan/UBSan 通过。C11 -Wall/-Wextra/-Werror 语法、host_init_verifier、内核补丁 checkpatch 和 diff 空白检查通过。没有运行 Android/内核编译，没有刷写或提交。

用户重新编译安装 boot 和 system 后，沿用 docs/95 的 --check、--arm、ctl.start、svc power reboot 流程。回 recovery 拉取整个 `/cache/s17-reboot` 到新的 `oss-kernel-debug/reboot-debug-2`。重点查看 v2 的 task-snapshots.log、syscall/转换锁检查点，以及 SysRq 中等待锁的 PID 1 和其他相关任务栈。需要这组结果才能决定修复 PM/同步等待、参数访问还是其他前置步骤。
