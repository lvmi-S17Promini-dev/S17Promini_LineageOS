# cache-log-2：系统服务已启动，GPU firmware 启动超时

三个原始文件归档 lineage-debug/cache-log-2。当次 kmsg /data 在 dm-2 上挂载成功（9.207879/9.313794 秒），zygote 10.688991 秒启动，surfaceflinger 11.016325 秒启动；logcat 当前 11:11 会话中 PID953 在 11:11:14.494 进入 SystemServer。上一轮 ext4 encrypted-casefold 修复已越过挂载卡点；尚未确认 boot_completed。

logcat 文件含 10:35 的旧成功绘制记录，不能用那些约 60 fps 的 HWC 日志判断当前 GPU 可工作。当前 SurfaceFlinger PID503，allocator PID470，system_server PID953。

## 当前首个图形错误

11.268 秒尝试 BVNC 专用 firmware 文件不存在，但驱动随后成功加载通用 rgx.fw。注册的 PVR 驱动为 1.11.5425693，BVNC22.87.104.18。

16.276224 秒首次 allocator 打开 GPU 时等待 FW started 的值 1，实际为 0，约五秒后超时。RGXPostPowerState 返回 PVRSRV_ERROR_TIMEOUT，设备 finalise/init 失败，被置为 bad state。随后 SurfaceFlinger/allocator/camerahalserver 的重复 DeviceOpen 报错是后果；SurfaceFlinger 初始化图形硬件未完成，客户端一直等待其注册。

还没有证据把根因定为 panel init、firmware 文件缺失、时序、电压或驱动 ABI；GPU 层的首个初始化失败必须先处理。

## 下一轮集中取证

只增加三个 GPU 文件中的诊断：

- gpufreq mt6765 的 MTCMOS enable 完成后，最多四次输出 mux 频率和 mux/async/mfg/core0 的 clk_is_enabled，以及 HW APM 状态；保留原 power/clock 调用。
- RGXPostPowerState 的启动超时记录 bFirmwareStarted 值；不修改等待预算或强行接受未启动。
- PVRSRVDeviceFinalise 首次 set-power-on 失败时，先释放 power lock，再调用驱动既有 PVRSRVDebugRequest(MAX)，获取已注册的设备调试信息；随后走原错误出口。RGX debug callback 自身会取 power lock，不能在 RGXPostPowerState 内直接调用 dump。此调用位于 allocations 尚未被清理的阶段。

未修改电压/频率/时钟策略、firmware/UMD 或显示初始化。静态调用链、锁关系和 git diff --check 已核对；用户负责编译，尚未完成编译或实机验证。该改动仅补充 GPU 取证，不是已修复 firmware 启动的结论。
