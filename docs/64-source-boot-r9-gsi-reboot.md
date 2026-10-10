# r9 完整日志：GSI rc 主动请求 bootloader 重启

原始日志归档 lineage-debug/oss-kernel-r9/s17-early-kmsg.log，collector r9、kernel r9-cachelog，AVC 日志明确 permissive=1，证明取证已越过 SELinux policy load。

7.596357 秒 early-init 执行 init.vndk-nodef.rc；7.597052 秒启动 /system/bin/reboot bootloader；7.618858 秒 init 收到 sys.powerctl=reboot,bootloader；8.624997 秒进入 shutdown/reboot 流程。未观测到 SurfaceFlinger 启动。不是仅凭最后日志推测卡点，而是明确的主动重启请求。

GSI nodef 脚本来自继承的 gsi_release.mk，init 会自动扫描安装目录的每个 rc，所以即便 ro.vndk.version 已为 31、init.gsi.rc 展开的 import 是 init.vndk-31.rc，也会独立执行 nodef 的无条件 early-init 动作。branch 中没有生成版本化 init.vndk rc 的模块。

init/service.cpp 在 enforcing 时拒绝 reboot 的错误 label/no-transition，permissive 时仅报警后继续，解释此次行为变化。因此只能确认 r9 的这一处阻断，不能说它就是 r7 enforcing 时停第二屏的根因。跳过 cpufreq 造成 PPM 缺 DVFS table 的告警仍在，未当作新的直接根因。

DT 将已有 init.s17promini-rom.rc 从 COPY_FILES 转为 ETC 预编译模块，通过 LOCAL_OVERRIDES_MODULES 覆盖 init.gsi.rc/init.vndk-nodef.rc。保留 GSI 的其他兼容设置和 VNDK/APEX 配置，未修改显示驱动。详细说明在 DT docs/49-gsi-init-reboot-guard.md。

host_init_verifier、git diff --check 通过，build/core 的 ETC override 解析路径已核对。完整构建/镜像中旧 rc 清理及设备启动验证交给用户；当前尚未构建验证此修复。先 m installclean 清安装产物，再 m -j8 systemimage，输出不得残留上述两个 GSI rc。须刷 system 才能改变安装在 system 的 init 脚本，单换 boot 无效。
