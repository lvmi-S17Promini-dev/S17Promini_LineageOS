# reboot-debug-3：USB extcon 延迟探测失败留下悬空连接

2026-10-11。本轮日志使用 boot ID 1bc76e75-0416-41ff-9766-c408e63529da，目录中其他 boot ID 的历史记录不作为本次证据。

后续用户确认修复后重启正常，反馈“可以，修复了”。本机重启卡点已通过实机验证解除；未收到具体测试次数、USB 拔插或充电识别的结果，不将这些额外场景记为通过。

已提交：内核 extcon 修复 `2ff88b8c971a`，MM2 回调修复 `ca5639023d03`，重启/deferred-probe 诊断 `df79432ba560`；设备树关机采集器 `817cd56`。此前各阶段文档中的“未提交”描述为对应分析阶段的历史状态。

## 已定位的设备

mode=check 的 31 秒 session 已显示 `active=1 pid=110 device=extcon_usb driver=mtk-extcon-usb last_result=-115`。mode=reboot 的 64 秒 session 在 10、30、60 秒三个样本中仍是同样的状态。-115 是诊断代码在调用回调前记录的 -EINPROGRESS，不是驱动已经返回的错误码。

PID 1 三次栈仍为 kernel_restart → device_shutdown → wait_for_device_probe → flush_work。说明 extcon 的 deferred-probe 工作在正常系统阶段已经没有完成，直到重启仍阻挡 flush；不是进入关机后才开始出现的 cpufreq/设备 shutdown 等待。

## 确认的源码错误

`drivers/misc/mediatek/extcon/extcon-mtk-usb.c` 的 probe 先 devm 分配 extcon，再通过 `device_connection_add(&extcon->dev_conn)` 把其中的连接对象挂入全局 devcon_list，然后调用 usb_role_switch_get。

`drivers/usb/roles/class.c` 在匹配到连接、但 role-switch provider 尚不存在时返回 -EPROBE_DEFER。原 extcon probe 直接返回错误，没有 device_connection_remove。驱动核心随后释放 devm 对象，而全局链表继续引用已经释放的 extcon 内存。以后重试 add/find 都可能遍历损坏的链表。这与 extcon probe 不返回、关机 flush 无限等待相符。

原代码的 endpoint 名字使用普通 kasprintf，也未释放；DT node 和 of_find_device_by_node 返回的设备引用未释放；成功取得 role switch 后，如果 vbus 获取失败，该引用也未释放。这些均属于同一 probe 失败回滚路径的缺失。

已确认生命周期错误及具体卡住设备，但没有在运行的损坏链表上直接逐节点取证，仍需实机验证是否完全消除重启卡点。不据此声称 Type-C/OTG 所有功能正常，也不强制将缺失 provider 的 defer 改成成功。

## 修复

- connection_add 后立即注册 devm_add_action_or_reset，确保 probe 返回 defer/失败时先摘链再释放对象；注册动作失败时立即摘链。
- endpoint 使用 devm_kasprintf，检查分配失败。
- 解析 phandle 后释放 DT node；复制 provider 设备名称后释放设备引用。
- 成功取得 role switch 后注册 managed put 动作，后续错误自动释放引用。
- 删除原先手工摘链的 remove 回调，由 managed action 负责，避免正常移除时重复摘链。

不跳过 wait_for_device_probe，不改变 role-switch/充电/VBUS/Type-C 策略或原返回错误。该修复只覆盖连接和 role-switch 等 probe 生命周期；没有在本轮重写整个驱动的 notifier/workqueue 移除管理。

## 验证

从源码抽取实际 probe 和新增清理回调，模拟 devres 的后进先出释放、全局连接表及 provider 引用。连续 64 次 role-switch defer 均在释放前摘链，且没有残留分配、role/node/provider 引用；覆盖 vbus defer、两个 managed-action 注册失败、endpoint 分配失败，以及成功 probe 的 devres 回收。

用 HEAD 中原 probe 运行同一失败测试，回退释放后连接表仍非空，复现悬空连接。修复版本的 ASan/UBSan 和 -Wall/-Wextra/-Werror 检查通过（共享 mock 后端的未使用函数单独关闭 unused-function）。补丁 checkpatch 和 diff 空白检查通过。

代理未运行内核/Android 编译或刷写；用户完成构建和实机重启验证后授权提交。

## 实机测试

这轮仅需用户 `m -j8 bootimage` 并更新 boot；reboot-debug-3 已有可读取该节点的 system 采集器，不需要再次更新 system。更新后是全新启动，旧内核中已经损坏的链表不能通过热更新自动修好。

正常系统可先读 `adb shell cat /sys/kernel/debug/s17_deferred_probe`，不应持续卡在 extcon 同一未返回回调。然后沿用 --check/--arm/ctl.start 采集服务，先测 svc power reboot，再测 adb reboot，多次确认偶发问题。若仍卡住，保存整个 /cache/s17-reboot 到 reboot-debug-4，核对新的设备/驱动与 PID 1 栈。USB/ADB 连接、拔插和充电识别也需确认没有回归。
