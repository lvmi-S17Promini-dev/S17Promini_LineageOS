# r8：已进入 Android init，policy load 后采集器失权

用户提供 oss-kernel-debug/s17-early-kmsg.log，已归档 lineage-debug/oss-kernel-r8/s17-early-kmsg.log。collector 为 r8，kernel 为 r7-nocpufreq，命令行 blacklist 有效；日志明确显示 _mt_cpufreq_pdrv_init blacklisted。

## 已确认阶段

1. disp_late initcall 在 1.227515 秒进入并正常返回；LCM probe 找到 AXS15260，is_lcm_inited=1 仍是 LK handoff，不能证明冷初始化/显示完整工作。
2. 内核初始化返回，/init.stock 在 1.307179 秒打印 init first stage started。
3. system/vendor 逻辑分区创建并成功挂载，switch-root 成功。
4. SELinux policy 编译完成，6.416913 秒打印 Loading SELinux policy，此后文件停止更新。

system_ext/product 的不存在告警发生在它们被合入 system 的既有布局；这里没有据此改分区。充电/SD/传感器错误未作为本次根因扩展排查。

## 为什么这不是已证实的屏幕卡点

已观测到 Android init，但文件在 policy load 边界停止，未看到后续 SurfaceFlinger/bootanimation 的执行。采集器是 fork 出的早期 kernel 域进程；平台 kernel.te 允许早期部分访问，却没有持续读取 kmsg 和写 cache 的权限。collector read/write 错误会结束循环，导致不能区分采集失权和实际 init 失败。仍需运行时验证，而不能把最后一条日志直接当成阻塞栈。

## r9 只修取证覆盖范围

保留 DTB 和 cpufreq blacklist，测试 fragment 改为 r9-cachelog，并启用 CONFIG_SECURITY_SELINUX_DEVELOP=y。r7 配置关闭了该选项，不能接受 enforcing=0 或用户态 setenforce；因此必须重新编译内核，不能只重新打包 r7。

早期采集时长从 30 秒扩至 120 秒，tag 改为 r9。临时加入 enforcing=0 与 androidboot.selinux=permissive，使采集器可以跨越 policy load 继续读取/写入。正常 init 不指定 permissive 时仍要求 enforcing；没有改设备基础 defconfig。make-debug-boot.py 在 --permissive 模式检查输入 kernel 的嵌入配置，若 DEV 未启用直接拒绝制作镜像。

system/core/init/selinux.cpp 的 StatusFromProperty/IsEnforcing 和 Android.bp 的 userdebug ALLOW_PERMISSIVE_SELINUX=1 已核对。是否实际生效必须从新设备日志确认；仅 cmdline 静态检查不能代替它。此 boot 仅供诊断，尚未修复未知的后续启动失败。

无需刷 system。回 recovery 挂载 cache，拉 /cache/s17-early-kmsg.log 和 /tmp/recovery.log。新日志首行必须包含 collector r9，以排除旧文件。读取 recovery 的 reason 可辅助定位 FBE/vold 或目录加密策略失败，不能据 wipe 提示就格式化 data。

用户接管编译。脚本语法和 git diff --check 通过，新的 r9 内核/最终 boot 尚未构建验证，未实机测试。之前仅重打包 r7 的候选因 DEV 未启用已删除，不能提供为有效 r9 镜像。
