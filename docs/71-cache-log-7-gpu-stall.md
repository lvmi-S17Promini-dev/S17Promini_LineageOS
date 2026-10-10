# cache-log-7：GPU 渲染队列停滞与下一步 trace

本轮只分析和准备运行时采集，不编译内核，也不修改功耗、频率或同步状态。

## 证据及边界

`screencap` PID 3210 主线程在 ppoll，Binder 线程等待输入。仅凭这个内核栈不能确定主线程正在等待哪个 fd，不能把它当成已验证的具体 fence 栈。

SurfaceFlinger 主线程在 MessageQueue::waitMessage；RegionSampling 线程在 sync_wait → Fence::waitForever，另一个 Binder 线程在 RegionSamplingThread::binderDied 获取 mutex。采样线程等待渲染完成会连带阻塞其资源清理，但不能由此证明整个 SurfaceFlinger 主线程死锁。

logcat 中 BootAnimation 的 fence 35 未完成，连续重试已到 619 秒。dmesg 约 635 秒的 watchdog dump 显示多个 TA/3D CCCB 不推进，包括 BootAnimation 和应用 RenderThread。KCCB 已执行 2887 条，FW IRQ count 与 LISR sampled count 均为 2789；固件控制通路并非完全停止，中断也非完全丢失。

`gpu-debug.txt` 与 cache-log-6 的 SHA256 完全相同（7e9b9365ae867b8aed5d6b46f9b8ba87c998a3ba7f181d3e956476610806c39a），不是一个新的独立实时快照。以本轮 dmesg 和线程栈为当前证据。

HWR 的 T0 polling unset(reg 0x3820, mask 4) 是约 15.421 秒记录的历史事件；635 秒复现 dump 中反复打印相同记录，不能视作每个时刻都新发生同样异常。源码定义它为 SLC_STATUS0.FLUSH_INVAL_PENDING；0x3c8 为 SIDEKICK_IDLE。

## 原厂逆向对照

原厂 Image 基址 0xffffff8008080000。通过字符串 Disable forced SLC coherency 的 ADRP+ADD 引用找到 GPU 初始化代码，Image 偏移 0x56bbf8。

原厂 0x56b9f8..0x56ba54 对 SLC_CTRL_MISC(0x3800) 读取、保留 bit1、设置 0x200000、检查两个 erratum 条件，以及 line size 小于 1024 时设置 bit0，均对应当前 m1.13 的 __RGXInitSLC。它同样使用严格同地址写入排序条件的分支形式。这只验证该片段逻辑，没有验证所有运行时 feature 表和最终寄存器值。

原厂 HWAPM 写入片段 0x53a56c..0x53a6a8 与当前 mt6765/mtk_mfgsys.c 的 12 次写入值、顺序一致：0x504..0x528 的 10 个配置值及 0x24 的 0x9004001b、0x8004001b。目前没有依据修改这些常量或直接关闭硬件电源管理。

## 运行时固件日志

当前 dump 为 Debug log type: none，trace buffer not yet allocated。驱动实际提供 apphint/0/FirmwareLogType、EnableLogGroup 写接口，支持运行时申请 trace buffer 并通知固件更新，无须重新刷 boot。

EnableLogGroup 接受逗号分隔的名称，不接受十六进制 mask；FirmwareLogType 接受 trace。AppHint 写入异步排队，脚本保留设置前、设置后及恢复后的读回值，不能仅凭 write 返回成功宣称生效。

采集脚本 tools/capture_s17_gpu_trace.sh 自动寻找 procfs/debugfs 的 pvr 根目录，启用 main,mts,bif,pm,rtd,spm,pow,hwr 组、亮屏并等待 10 秒，读取 firmware_trace 和 debug_dump 后恢复原组过滤器。若旧模式没有可打印值，仅恢复过滤器；旧过滤器为 none 时日志关闭，trace mode bit 可能保留，重启后回到默认值。

脚本不关闭 APM，不强制 signal fence，不重启 SurfaceFlinger。读取单个诊断节点以 timeout 限制用户态等待。源码与 shell 语法已检查；实际手机节点是否存在、写入是否成功及固件 trace 解码是否匹配，须由运行结果确认。
