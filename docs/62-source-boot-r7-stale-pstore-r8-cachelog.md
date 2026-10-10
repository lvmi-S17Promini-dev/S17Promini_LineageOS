# r7 旧 pstore 与 r8 cache 取证

r7 用户反馈第二屏不再自动重启，但手动回 recovery 出现启动失败/是否 wipe 提示。s17-pstore-r7 的 console 文件 SHA256 fe3c55796ace549bbebf06a5ef9a48992d39b513cce71fb3195e1c90ad1b53e4，与 r5 完全相同，不能用于定位 r7 阶段。

检查 DT 历史后，早期采集器是 2b9814e 的 boot PID1 wrapper，仅在独立 debug boot 激活；正常 boot 不包含它。当前 /cache/recovery 是 recovery 自身日志，last_kmsg 源于当次 recovery 的 klogctl，不能当成上一轮普通启动日志。wipe 提示对应 --prompt_and_wipe_data，可以由 Android init 的 FBE/vold 操作失败或目录加密策略失败等触发；必须读取 /tmp/recovery.log 的 reason 才能判断，不能只据提示认定 data 损坏。

用户要求恢复采集器和 serial console。r8 使用 r7 kernel/DTB 和 cpufreq blacklist，仅包装 ramdisk 启用 early-kmsg，同时更新 system rc，去掉 stop console 并增加 post-fs 的 kmsg/logcat 服务。详细设计在 DT docs/47-source-boot-cache-log.md。没有实际刷写设备。
