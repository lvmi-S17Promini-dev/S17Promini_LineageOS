# 重启偶发卡死：修正 MM2 预约对象的释放回调

2026-10-11。用户先报告 adb reboot 可以成功、svc power reboot 卡住，随后补充 adb reboot 也卡住一回。因此不能根据单次成功排除内核关机阶段，也不能把故障限定为 PowerManager/ShutdownThread。

## 返回日志的结论与限制

用户贴出的 dmesg 中，145.152–146.088 秒 `pvr_defer_free` 约每 100 ms 重试一批连接，DevmemIntHeapDestroy 报剩余引用 2、154、70，ConnectionDataDestroy 的 PVRSRVFreeHandleBase:2 返回 RETRY。

146.018 秒 init 的 SIGTERM 等待结束，只有 audio HAL 未退出；随后 SIGKILL 成功清理它，接着执行 vdc abort_fuse、vdc shutdown，并在 146.139 秒开始杀 vold。PVR 报错期间 init 仍在前进；日志末尾邻近停止 adbd/logd 的正常阶段，不能把 ADB 最后一行当成最终卡死调用。Binder ioctl oneway spam detection 返回 EINVAL 和 permissive AVC 也不足以证明阻塞原因。

源码路径确认 GPU 堆的引用必须回到 1 才能销毁；映射和预约分别持有 heap 引用。连接清理由后台线程重试，所以该问题既可能产生持续资源泄漏，也可能影响后续关机，但尚未取得 ADB 断开后的本次日志或阻塞栈，不能宣称已确定重启卡死的唯一根因。

## 已确认的生命周期错误

当前 S17 使用 `drivers/gpu/mediatek/gpu_rgx/m1.13ED5776728`，此前 docs/70 的 vendor 扩展 MM2 bridge 创建了新版预约对象，包含 hMapping2Lock、psMapping2 和 uiMapping2Refs。

`generated/rogue/mm_bridge/server_mm_bridge.c` 的 `PVRSRVBridgeDevmemIntReserveRange2` 却为该 handle 注册了旧 `DevmemIntUnreserveRange` 回调。

旧释放函数只释放 MMU 预约、递减预约持有的 heap 引用，并释放预约对象；不会解映射 psMapping2、递减映射持有的 heap/PMR 引用、销毁新锁或检查借用计数。进程退出时，即使应用未显式 unmap，handle 清理也会调用这个错误的回调，留下映射引用，并可能使仍在借用预约的对象访问已经释放的预约。

新版 `DevmemIntUnreserveRange2` 已包含正确清理：借用未结束或解映射失败时返回 RETRY/真实错误、保留对象；可释放时先解除映射、释放映射引用，再销毁锁并调用旧版预约释放。这些 helper 在原有直接调用测试中通过，但此前测试没有经过实际 bridge 注册的回调，遗漏了接线错误。

## 修改

只将 ReserveRange2 的 handle release callback 从 `DevmemIntUnreserveRange` 改为 `DevmemIntUnreserveRange2`。保留旧 ReserveRange 的回调、opaque handle 类型、ABI 报文、GPU 电源策略和清理顺序；不强制归零引用计数，不把 RETRY 伪装为成功。

此修复覆盖 handle 正常释放及进程退出的自动清理。它不会修复当前已运行内核中早先泄漏的对象，测试需使用更新后的 boot 重新启动。

## 验证

从当前源码抽取实际 ReserveRange2 bridge 完整函数及 MM2 helper，模拟 handle/旧 MMU/PMR 后端，调用 bridge 实际注册的函数指针。验证成功创建→映射→借用时 RETRY→解映射失败时保留→释放成功后 PMR/映射/锁均回收、heap 引用回到 1；同时覆盖 handle 分配、lookup、锁创建和映射失败回滚。ASan/UBSan、-Wall/-Wextra/-Werror（原 bridge 未使用的 dispatch 参数单独关闭 unused-parameter）通过。

将抽取函数的回调还原旧值后，同一测试在带映射预约的旧释放路径触发断言，复现此前回调遗漏；没有通过直接调用新版 helper 来绕过注册错误。内核补丁 checkpatch/空白检查通过。

未运行内核或 Android 编译，未刷写手机，未提交。单独验证 GPU 修复只需用户重新编译、安装 boot；验证 ADB 停止后的卡点则需配套安装 docs/95 的 system 采集服务。观察重启期间 PVR heap 引用 RETRY 是否消失或减少，并保存 `/cache/s17-reboot` 的完整本轮目录，定位最后未返回的 `[S17/REBOOT]` 阶段。adb reboot 与 svc power reboot 都应多次验证，不能用一次成功验收偶发问题。
