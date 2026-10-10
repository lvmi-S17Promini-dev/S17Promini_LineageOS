# 执行重启时卡住：日志保存与内核检查点

2026-10-11。用户明确故障发生在执行重启的旧系统关机阶段，不是下一次启动阶段。cpufreq 已确认能够启动和调频，但重启可靠性尚未通过；目前没有故障时日志，不能直接归因于 cpufreq 或新接入的 connectivity 驱动。

## 当前 cache 日志为何不够

设备树中的 `rootdir/init.s17promini-bootlog.rc` 在 `sys.boot_completed=1` 停止 kmsg 和 logcat 服务。`tools/boot-kmsg-init.c` 的 service 模式最多运行 300 秒，并用 `O_TRUNC` 打开固定的 `/cache/s17-bootlog/kmsg.log`。独立 early wrapper 也有时限和固定文件名。

因此正常进入系统后的重启故障没有持续 cache 采集保证；下一次普通系统启动又会覆盖 kmsg。Android init 的 reboot 流程还会停止 adbd/logcat，所以 ADB 断开本身不等于卡点。不能靠让一个普通后台进程读 kmsg，就声称能抓到内核复位前的全部日志。

当前构建配置具备 PSTORE_RAM、PSTORE_CONSOLE、MAGIC_SYSRQ；PSTORE_FTRACE 和 PSTORE_PMSG 未开启。之前已从 recovery 读取到 console-ramoops，但也出现过旧记录，必须按版本、时间、boot ID marker 确认新鲜度。ramoops 是保留 RAM，彻底断电可能丢失；强制复位是否保留也取决于 bootloader/平台行为。

`console-ramoops` 不是多轮启动的永久归档：fs/pstore/ram_core.c 初始化时先将有效旧环形区复制到本轮内核的 `old_log`，ram.c 再清空持久区供本轮 console 写入。console 读取使用旧副本，不用当前 console 更新它。因此“故障系统 → recovery”这一次启动通常仍能读到故障系统日志，recovery 自身输出不会覆盖这轮暴露的旧副本；但再重启一次，旧副本随本轮内核消失，下一轮只能保存最近持久区内容。第一次进入 recovery 就应拉取到电脑，不要经过一次普通系统再来取。上述语义针对 console-ramoops，不能将不同类型 dmesg-ramoops 记录混作同一缓冲区。

## 第一轮：现有镜像也能采集

正常系统尚可操作时，在电脑工作目录运行：

```sh
cd /home/dengxh/S17Promini_LineageOS
adb root
python3 tools/capture_s17_reboot.py capture
```

脚本不会执行重启。看到 `Recording ... Reboot from the phone's menu now` 后，用手机菜单复现原来的重启操作。每次创建新的主机输出目录，最长采集 180 秒，可用 `--seconds` 调整，多个设备时使用 `--serial`。已是 root adbd 时不改变它；非 root 时尝试已有 su，失败则退出。

脚本先保存系统身份、挂载、当前 dmesg/logcat、CPU 状态和旧 pstore，并向 kmsg 写入带 boot ID 的 `[S17/REBOOT] capture armed` 标记。随后将 `dmesg -w` 和 logcat 流写在电脑上，每次 ADB 断开都保留已收到的输出。手机端 Toybox dmesg 支持 follow，并处理 kmsg 环形区读超出的 EPIPE。此流程不清空 logcat/kmsg/pstore，不写 cache 日志，不修改 reboot/调频策略。

卡住后，按已有恢复方法直接进入可用 recovery，先不要再启动普通系统，也不要 wipe。到 recovery ADB 后运行：

```sh
python3 tools/capture_s17_reboot.py recovery
```

脚本会在另一个全新主机目录保存 `pstore.tar`、`cache-logs.tar`、`recovery.log`、`recovery-dmesg.txt`、身份信息及 `capture.json`。cache/pstore 未挂载时尝试挂载现有文件系统，不格式化，不删除。每个命令错误/超时都保留 stderr 和已收到的部分输出；结尾会报告不完整项。`pstore.tar` 应与第一轮的 `pstore-before.tar` 对照，防止分析到旧记录。tar 保留原始内容，不在脚本中自动展开。

这两个 `reboot-capture-*`、`reboot-recovery-*` 目录一起用于分析。ADB 输出覆盖旧系统的前段；pstore 覆盖用户态已经停止之后的内核阶段。恢复时读出的 dmesg 是 recovery 本身的日志，不应当作上一轮系统日志。

## 第二轮：新增内核检查点

已在源码增加 `[S17/REBOOT]` 检查点，使用现有 `initcall_debug` 开关；不改配置版本、回调顺序、锁、重试预算、watchdog 或 SSPM 协议。

| 文件 | 可辨认的阶段 |
| --- | --- |
| kernel/reboot.c | 进入 kernel_restart、reboot notifier、禁用 usermodehelper、device_shutdown、迁移到复位 CPU、syscore |
| kernel/notifier.c | 仅 reboot_notifier_list 的各回调 begin/done 与返回值 |
| drivers/base/core.c | 等待 probe、cpufreq_suspend、每个设备的锁、runtime PM barrier、shutdown 回调完成 |
| drivers/base/syscore.c | 现有回调进入日志后的对应返回日志 |
| arch/arm64/kernel/process.c | machine_restart、smp_send_stop 前后、进入硬件复位回调 |

编译由用户执行，本轮不启动编译，不提交：正常 ROM lunch 后执行 `m -j8 bootimage`，更新 boot，再重复以上采集。现有 ROM cmdline 已包含 initcall_debug。旧内核仍可用第一轮工具采集，但不会出现这些新增检查点。单刷 boot 不会改变 system 的原 cache 采集器；本轮没有修改该采集器。

## 如何读结果

- 主机日志停在 init 的服务退出、vold、sync、APEX/分区卸载，且新 pstore 没有 kernel_restart entry：优先查用户态关机。没有 entry 也可能是日志丢失，不能单凭缺行定论。
- notifier begin 没有对应 done：结合回调符号定位通知处理函数。
- cpufreq_suspend begin 没有 done：查通用 cpufreq suspend 和 governor 停止。
- 设备 shutdown locks/runtime barrier 或原有 shutdown 日志后没有完成：查该设备、驱动和 PM 等待。
- 停在 CPU migration/syscore：查热插拔锁、调度迁移或指定 syscore 回调。
- smp_send_stop begin 没有 done：查其他 CPU 停机/IPI。
- reset handler begin 后没有新启动：查 ARM 平台复位/watchdog/固件入口；复位函数成功时本来就不会返回。

正常 ARM64 reboot 的 `smp_send_stop` 不等于逐个执行 CPU hotplug offline，因此不能预先认定一定卡在 cpufreq 的 cluster-off。现有 S17/IPI 日志若出现 cluster state=0 且没有返回，再沿那个具体调用分析。

## 本轮验证

主机 mock ADB 验证 root/su、serial 选择、断开后输出保留、recovery tar、已有输出目录拒绝覆盖，以及不会发出 reboot/删除/清日志命令。Python 语法与内核补丁 checkpatch/空白检查通过。未编译内核、未连接手机、未取得实机重启日志。诊断 printk 会影响时序，应结合完整记录分析。

## pstore-reboot 实机返回：仍为旧记录

用户经强制重启进入 bootloader fastboot，再执行 `fastboot reboot recovery`，手动拉取 `oss-kernel-debug/pstore-reboot/console-ramoops-0`。大小 262132 字节，SHA256 `fe3c55796ace549bbebf06a5ef9a48992d39b513cce71fb3195e1c90ad1b53e4`，与 r5 原始日志、r5 归档和 r7 返回记录逐字节相同。末尾仍为 r5 首次 cluster-on 轮询，最终时间 1.238657 秒；不能用于本次重启定位，也不能据此认为当前 cpufreq 初始化再次失败。

源码的 `pstore_console_write → ramoops_pstore_write(PSTORE_TYPE_CONSOLE) → persistent_ram_write` 持续写 console，没有要求 panic/启动失败。因此“只在启动失败后读到新 pstore”是本机保存/恢复链路的现象，不能解释为普通 console 设计只在失败时写入。fs/pstore 的 Linux 旧缓冲区语义也不足以保证本机 LK 路径保留最近日志。

对原厂 `stock_imgs/lk.bin` 的 strings 检查发现 `kedump: found content in expdb`、`SYS_PSTORE_RAW`、`kedump: read %s from offset 0x%x size 0x%x`、`kedump: temp pstore crc fail`、`ram_console_should_restore`，以及 DDR reserve/PMIC reset/boot_reason 相关消息。这支持 LK 存在读取崩溃归档、恢复 pstore 的路径；可能解释为何多轮启动仍读到同一份 r5 档案。但字符串不能证明本次实际分支或恢复条件，尚未反汇编该调用链，不把 LK 恢复旧档案记为已确定根因。

后续先保留当前正常系统的 `/proc/cmdline`、`/proc/consoles`、完整 dmesg，核对 pstore console 注册和实际物理区；重启前向 kmsg 写入唯一标记，验证复位后是否能读到该标记。用户选择手动采集，无需使用 Python 工具。关机前段可以在电脑分别运行 `adb shell dmesg -w > reboot-kmsg.txt` 和 `adb logcat -b all -v threadtime > reboot-logcat.txt`，ADB 停止后保留已收到的内容。内核关机后段仍需验证可用的 pstore/平台 dump 链路，不能承诺这两个 ADB 流覆盖该阶段。
