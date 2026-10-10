# ADB 停止后的关机取证：私有 cache 采集服务

后续实机采集已证实能够越过 adbd 停止和 init 的 cache 卸载。本轮 v2 增加关机延时 PID 1 栈/系统调用采样及 SysRq 转储，见 [97](97-reboot-debug-1-syscall-boundary.md)。

2026-10-11。用户确认 ADB 断开前日志只到 init 卸载阶段，要求重点保存断开后的内核关机日志，也允许调查 pstore 修复。本轮实现独立关机采集服务，编译由用户执行；没有修改或刷写 LK，没有写 expdb，没有启动 Android/内核构建，没有提交。

## pstore 的保存/恢复链路

`oss-kernel-debug/pstore-reboot/console-ramoops-0` 与旧 r5 文件完全相同，详见 docs/94；不能用它判断当前重启卡点。

用用户提供的 Ghidra 12.1.4 analyzeHeadless，按 BinaryLoader/ARM:LE:32:v7、Thumb、基址 0x48000000 导入原厂 LK payload，禁用全镜像分析，仅分析 kedump 和 restore 的函数。脚本为 `tools/S17PstoreDecompile.java`，输出保存在 `extracted/metadata/ghidra-lk-pstore`，项目在 `/tmp/s17-pstore-ghidra/S17Pstore`。函数名由分析者赋予，不是原始符号。

ARM 指令与反编译交叉核对：

- `kedump_entry`（VA 0x4802174c）在已有处理标志或 fault 检查未触发新 dump 的回退路径，调用 VA 0x480205f4。
- VA 0x480205f4 从 expdb 读取档案头，检查 magic 0xaee0dead，并查找 `SYS_RAMCONSOLE_RAW`、`SYS_PSTORE_RAW` 和 CRC 信息。
- pstore 读入临时缓冲区并校验 CRC。restore 判定 VA 0x48023e14 根据 ram-console 的状态/标识比较作出决定；返回 true 时，VA 0x48020970–0x4802098a 将归档 ram-console 和 pstore 拷贝回目标区域。
- 该 memcpy 前没有针对当前 Linux console 的新鲜度/唯一 boot ID 比较。因此 LK 确实具有恢复历史 pstore 的路径，而 Linux 持续写 console 并不自动保证在本机强制复位路径中保留下来。

上述证明恢复路径存在，尚未取得 LK 本轮运行日志，不能把本次实际分支、DDR 保留状态或 expdb 更新时间记为已确定。修复此链路涉及 bootloader/复位时 DRAM 保留，不能仅靠开启 CONFIG_PSTORE 或清除 sysfs 旧文件解决。旧 pstore 中看不到本次日志的原因仍需平台实测，不把本轮采集服务称为 pstore 已修复。

## 服务设计与边界

设备树增加：

- `tools/reboot-kmsg.c`：新的 `s17-rebootlog` 静态可执行程序，避免依赖 APEX 动态库。
- Android.bp 和 userdebug/eng PRODUCT_PACKAGES 接线。
- 原 `init.s17promini-bootlog.rc` 中新增 `s17-reboot-kmsg`：disabled、oneshot、`shutdown critical`、root、现有 su 域。旧启动采集器仍维持原行为。

Android init 会保留 shutdown critical 服务，且在开始关机时启动尚未运行的此类服务。但超时后的 sysrq-i/KillAllProcesses 仍可能杀死它。二进制要求显式 --arm 留下与本轮 boot ID 一致的标记，然后等待 sys.powerctl 的普通 reboot 请求；未 armed、下一轮启动、poweroff/纯 userspace reboot 均不持有私有 cache，以免干扰日常重启和关机 fsck。可手动提前启动服务进入等待状态。

读取 `/dev/kmsg` 前，程序执行 `unshare(CLONE_NEWNS)`，将克隆挂载树递归设为 private；只保留本轮根挂载与 `/cache`，在自己的空间中按子挂载优先顺序 detach 其他挂载。原始 init 的挂载树不受这些操作影响。打开 proc、dev 的必要 FD 和身份信息后便不再依赖其路径。验证私有空间中没有剩余 data/APEX 等挂载；失败即退出，不退回共享挂载空间继续采集。

这样 init 可卸载自己空间的 cache，而日志进程通过另一个 cache mount 继续持有文件系统。不能简单在 init 的同一个 mount 上长期打开输出文件，否则可能制造 EBUSY，触发卸载超时/kill-all，反而扰动故障。该设计需要实机验证 namespace 权限、挂载释放和关机行为，主机 mock 不等于通过手机卸载测试。

日志保存到 `/cache/s17-reboot/<boot-id>-<monotonic-time>-<pid>/`。不同启动和服务运行创建独立目录，不覆盖此前测试。每个目录最多四个 4 MiB 的 `kmsg-0.log` 至 `kmsg-3.log`，只轮换本次运行；按文件头 `part=N` 排序可还原最近日志。目录及文件头含 mode/boot ID/内核版本/cmdline，`ready` 表明本次隔离和文件建立已完成。

这会延长诊断用 cache 文件系统的挂载生命周期，不能作为普通干净卸载路径的替代。归档之后可由用户清理旧目录；没有自动删除历史记录的策略，反复测试需留意 cache 剩余空间。armed 标记虽然留在 cache，但下一轮 boot ID 不匹配，不会自动开启下一轮采集。

每次写入使用 O_DSYNC，文件和目录元数据在建立/轮换时 fsync；不等待下一轮启动再落盘。只追踪启动采集以后的新 kmsg，避免同步重写大量旧 boot 消息。EPIPE 记录 overrun 并继续；短写/EINTR 正确重试。没有将日志写入裸分区或清除历史目录。

覆盖边界：可以在 adbd/logcat 停止、init 卸载自己的 cache 后继续读取与落盘，但不能保证存储驱动停机后还能写入；CPU 停止调度、关中断或 I/O 已卡住时用户态也无法继续运行。内核已有 docs/94 的前后检查点，会增加可捕获的定位信息；如果停在采集器也无法运行的阶段，仍需验证平台 dump 或串口通道，不能声称该服务能覆盖硬件复位前的所有指令。

## 用户编译与测试

沿用正常 ROM lunch：

```sh
m -j8 bootimage systemimage
```

需要安装新 system 才有二进制和 rc；单刷 boot 不会安装服务。新 boot 包含此前增加的 `[S17/REBOOT]` 阶段日志。此次没有因服务修改 vendor，也没有改动其他未提交的 connectivity/recovery 配置。

正常系统中用 root ADB，先检查私有 mount 设置：

```sh
adb root
adb shell /system/bin/s17-rebootlog --check
```

成功会输出 `Private cache setup passed: /cache/s17-reboot/...`，留下 mode=check 的头文件并立即退出。这不是正在采集；检查失败时先保存终端错误，不继续假设服务可用。

然后启动服务进入等待：

```sh
adb shell /system/bin/s17-rebootlog --arm
adb shell setprop ctl.start s17-reboot-kmsg
adb shell getprop init.svc.s17-reboot-kmsg
```

应为 `running`。从手机菜单复现普通重启；sys.powerctl 请求到达后，服务自动开始隔离和写入 mode=reboot 的日志，不需要 ADB 继续连接。没有固定 300 秒时限，也不被旧 rc 的 boot_completed 停止事件停止。

卡住后走用户实际可用路径：强制重启到 bootloader fastboot，再 `fastboot reboot recovery`。进入 recovery 挂载 cache 后立即取证：

```sh
adb shell mount /cache
adb pull /cache/s17-reboot oss-kernel-debug/reboot-cache-1
```

cache 已挂载时不用再次 mount。将整个目录提供回来，保留四个日志、ready 和不同 session 目录。无需 Python；下一次普通系统启动不会覆盖这些独立目录。此次方法不依赖旧 pstore 内容，但正常存储写入失败、强制复位导致文件系统错误等仍可能影响日志完整性。

## 离线验证

主机 C11 -Wall/-Wextra/-Werror 语法通过，host_init_verifier 接受新增服务。ASan/UBSan mock 测试验证 unshare→private→detach 的顺序、unshare/private 失败时不执行 detach、cache 不可用时退出、子挂载先于父挂载、路径转义只解码一次、剩余挂载拒绝、重启原因过滤、短写/EINTR 及四文件轮换。所有 diff 空白检查通过。

这些是主机源码/模拟检查，尚未进行 Android/Bionic 静态链接、手机 namespace/SELinux 权限、cache 卸载与实际重启测试；Android 编译和实机验证仍由用户执行。
