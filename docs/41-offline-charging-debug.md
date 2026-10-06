# 关机充电停留 orange state：静态分析与取证（2026-10-06）

用户现象：关机插电后停留 orange state，未自动重启，ADB 未出现。尚无本次 charger 启动日志，不能确认是真正死锁还是没有进程更新屏幕。

## 当前证据

- 原厂 system 的 `etc/init/kpoc_charger.rc` 在 `on charger` 中启动 `/system/bin/kpoc_charger`，服务属于 `class charger`。
- 当前 Lineage system 输出没有 `kpoc_charger` 和该 RC；有 `/system/bin/charger` 及图片资源，但扫描 system/vendor init RC，没有服务调用这个 charger 程序。已知充电 UI 启动链缺失，是首要嫌疑。vendor 的 `health-hal-2-1` 属于 `class hal charger`，仅凭此不能证明它接替了充电界面。
- `system/core/init/init.cpp` 根据 `ro.bootmode=charger` 选择 `charger` 事件而非 `late-init`。现有 `init.s17promini-rom.rc` 的 USB/ADB 启动在 `post-fs` / `load_persist_props_action`；charger 路径不会正常执行这些事件。adbd 又是 updatable 服务，受 APEX 激活约束。因此没 ADB 是预期可能性，不是内核未启动的证据。
- stock vendor 的 `init.mt6765.rc` 在 `on charger` 中同步执行 cache e2fsck，随后挂 cache/nvcfg、启动 fuelgauged/fuelgauged_nvram 和 light HAL。这些操作也可能阻塞或失败，需日志区分。
- 正常 boot 已验证使用 stock kernel/DTB/first-stage ramdisk。优先核对替换 system 后缺失的用户态链路；orange state 画面本身不能定位停在 LK、内核还是 init。

## 无 ADB 的优先取证方法

现有 `artifacts/boot-kmsg-debug.img` 的 PID1 wrapper 在 exec 原厂 init 前 fork kmsg collector，向 cache 写 `s17-early-kmsg.log`，无需 charger ADB。历史实测已取到日志（见 25/26 文档）；本次 charger 路径未验证。镜像 SHA256：`d749476602be41e49d7370d029df6d2fb7dae6c36ff62ccf94ccaf4447e270c6`。

1. 在正常可操作状态先准备该 debug boot、正常回退 boot，以及现有按键 bootloader/recovery 入口。进入 recovery，确认 cache 已挂载，先保存旧日志，再删除旧 `s17-early-kmsg.log`，避免误认历史记录。只删除这个诊断日志，不清 cache/data。
2. 在 bootloader fastboot 刷 debug boot。沿用当前 disabled vbmeta，不需要改 LK/vendor/system/recovery。

   ```powershell
   fastboot flash boot .\boot-kmsg-debug.img
   ```

3. 进入 recovery 后选择关机，断开 USB，确认关机，再插电复现。普通 `fastboot reboot` 产生的是正常重启，不能代替冷关机插电测试。记录警告后画面、等待时长、短按电源反应、拔线是否退出，以及主机 USB 是否出现设备。
4. 复现后通过已验证的按键入口进入 bootloader，直接 `fastboot reboot recovery`。避免先启动正常 Android：debug collector 每次运行会截断同一个日志文件。
5. 在 recovery 读取 cache。先检查挂载；未挂载时可从 recovery 菜单挂 cache，或使用下面命令。已经挂载则跳过 mount。

   ```powershell
   adb shell cat /proc/mounts
   adb shell mount -t ext4 -o ro /dev/block/by-name/cache /cache
   adb shell ls -l /cache/s17-early-kmsg.log
   adb pull /cache/s17-early-kmsg.log .\charger-early-kmsg.log
   adb pull /sys/fs/pstore .\charger-pstore
   ```

   cache 必须使用绝对路径访问。先前出现过挂载前进入 `/cache` 的 shell 仍看到旧目录的情况。pstore 仅作补充，需要与旧文件比较；本机之前出现过重启后内容不更新。recovery 的当前 dmesg 不能代替失败启动的日志。
6. 取完日志后回 bootloader 恢复 `boot-normal-r1.img`（此前已验证正常启动，SHA256 `f3da42d23bc3283e99614ad43e80e5f0f15c68a4dc849adcc3db1323055b07d4`），或当前安装包对应的正常 boot。

   ```powershell
   adb reboot bootloader
   fastboot flash boot .\boot-normal-r1.img
   fastboot reboot
   ```

采集器限制：仅约 30 秒；SELinux 加载后可能失去权限。它会额外挂载 cache，而 vendor charger 路径对同一分区执行 e2fsck，可能改变原问题表现；若日志表明 fsck 因已挂载退出，不应直接当作正常 boot 的根因。无文件也不能直接证明没有进内核。若这一后端不够，下一步应做独立 charger 日志服务或 UART，而非反复重刷同一镜像。

## 如何判读

- 出现 `processing action (charger)`：已经进入 init charger 分支，orange state 只是尚未被刷新。
- charger 分支正常完成，但只有 health/fuelgauge 服务而无 UI 服务：支持缺失充电 UI 链路；再做单独启动 Lineage charger 的对照镜像。
- 停在 `e2fsck`、wait/mount 或某个同步 exec：先定位阻塞命令、分区和退出状态。
- 服务 `Could not start`、`CANNOT LINK EXECUTABLE`、退出循环、`avc: denied`：分别检查可执行文件/依赖、崩溃和真实节点标签。
- 没有 charger 事件：检查日志中实际 `androidboot.mode`/bootmode、first-stage mount 和策略加载；再考虑 LK/KPOC 分支或内核问题。旧 LK 日志中的 charger 错误不是此次关机充电的证据。

## 修复方向（尚未实施）

优先验证为 `/system/bin/charger` 增加独立 `class charger` 服务的效果，核对 health/sysfs 权限、显示 backend、电源键和拔线行为。它不能仅凭动画出现就算完成 MTK KPOC 支持。

如果必须保留原厂 KPOC 特有行为，再评估恢复原厂 `kpoc_charger` 链路。原程序依赖 `libshowlogo.so`、`libhidltransport.so`、`libui.so` 等，并使用独立 SELinux 域；不能只复制可执行文件或把它改名覆盖 Lineage charger。当前尚未改生产 init、SELinux 或构建配置，也未执行设备刷写。
