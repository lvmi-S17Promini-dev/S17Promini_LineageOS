# cache-log-8/s17-gpu-submit：新进程复现，首次提交仍未保留

gpu-before 时间为 73.608 秒、BootAnimation PID 846；gpu-after 为 85.676 秒、PID 2035。重启已成功。logcat 在 16:10:58..16:11:03 记录 PID 2035 的新 fence 35 重试从 6 到 11，QE/V3/P3 三个 pvrsrvkm sync point 均未完成。因此不是只有旧进程保留了卡住的资源。

fw-after 中 KCCB 命令统计：CLEANUP(0x2abc006a) 50 条，POW(0x2abc006b) 51 条，没有 KICK；fw-before 没有可解码命令。不能据此认定新任务没有提交到固件：最终环形缓冲只保留尾部日志，早期事件可能被高频 MTS/POW 输出覆盖。

脚本的 --restart-bootanim 模式新增后台连续快照：重启前启动，最多 100 次，每次 firmware_trace 读取限制两秒，间隔 0.1 秒；每份附带 /proc/uptime。正常快速读取约十余秒，若节点读取慢会延长，不能假定每次严格相隔 100ms。最终等待采样结束后恢复日志设置，异常退出会停止采样子进程。数据在 fw-samples 子目录，用于检查首次新任务的 KCCB、上下文及调度；默认模式不增加连续采样。

此修改是采集时序修正，不是 GPU 修复。已通过 shell 语法检查；手机端捕获效果尚待验证。
