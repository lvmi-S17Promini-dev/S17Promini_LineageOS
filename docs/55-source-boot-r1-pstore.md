# r1 实机 pstore 与 r2 取证 boot

2026-10-10，用户反馈 r1 停在第二屏、没有启动动画，约一分钟后重启。日志为 `oss-kernel-debug/s17-pstore/console-ramoops-0`，原样归档到 `lineage-debug/oss-kernel-r1/console-ramoops-0`。

## 日志确认的事实

- 第 2 行是 `4.19.127-s17-bringup-r1+ #2`，编译时间与 r1 一致；这是新内核日志。
- 日志从 0 秒开始，共 1916 行、162973 字节，最后时间为 **1.204558 秒**。
- eMMC 已发现分区，PVR 1.11.5425693 已注册，显示框架识别到 AXS15260；`is_lcm_inited=1` 表示继续使用 LK 初始化的面板，不能据此证明新驱动冷初始化成功。
- 没有 `Run /init`、释放 init 内存或 Android init 的记录；可见的启动停点在内核 late-init CPU 调频阶段。
- 最后几行是 CPU cluster 初始化、两条 regulator consumer link 和 CPU level 输出。源代码对应 `_mt_cpufreq_pdrv_probe` / `_mt_cpufreq_init` 及 efuse 读取助手。具体卡在哪个调用尚未证明。
- watchdog 在约 1.06 秒启用，timeout=31 sec。卡住后 watchdog 重启是候选解释，但日志没有最终 reset/panic 记录，不能据此断言重启原因。
- 0.396 秒的 GIC WARNING 后仍继续启动到 1.204 秒；它不是日志里的终止点。`androidboot.bootreason=lk_crash` 是本轮启动命令行值，不是本次失败末尾的诊断。
- 触摸在第 1236 行报 `pinctrl-0 index 0 invalid phandle`，probe=-22，输入设备未注册。
- clkbuf DTS、充电/TCPC 和 mrdump control block 也有失败信息，留待后续；它们之后日志仍继续推进。

## 触摸引用错误已经复现

r1 从原厂 DTB + DTBO 合并树导出设备描述，但 LK 还会应用原厂 DTBO。
Linux `fdtoverlay` 与 Android `libufdt` 对合并节点 phandle 的处理不同。

使用本地 AOSP `system/libufdt` 的测试程序重现 r1：

| 属性 | 合并前 | Android libufdt 再叠加原厂 DTBO 后 |
| --- | --- | --- |
| 新触摸节点 pinctrl-0 | 0x105 | 0x105 |
| stock eint@0 的 phandle | 0x105 | 0x164 |

节点仍在，但引用失效，与实机错误一致。r1 的静态校验只检查新触摸节点保留及普通 `fdtoverlay` 行为，没有验证 Android 合并后的 pinctrl 引用，这是漏检。

r2 新建独立 `s17-axs-eint` pinctrl group，沿用原厂 GPIO0 的配置，并让新触摸节点引用它。原厂 overlay 不会覆盖这个私有节点。已用原始树、Linux fdtoverlay、Android libufdt 三种结果核对，引用均有效。

## r2 已生成

文件：`artifacts/source-boot-r2/boot-source-r2.img`，32 MiB。
SHA256：`b39355bf0fbdd7cb1d4ed1d64ba0220094a78e8f013d5f5477aed12ea496b92d`。

- 修复触摸 pinctrl 引用。
- 内核版本 `4.19.127-s17-bringup-r2+`。
- 开启 `initcall_debug`。
- `[S17/DVFS]` 检查点覆盖 CPU efuse 临时设备释放、平台 probe、cpufreq 注册、policy init、频率表、锁、OPP 和 energy model 注册。
- 电压/频率表、watchdog 设置及 ramdisk 未更改；这版用于进一步定位 CPU 调频停点，没有声称已经修复卡住原因。

Lineage `m -j8 bootimage` 通过；内核/DTB/ramdisk/地址/AVB 校验通过，另加 Android libufdt pinctrl 引用检查通过。r2 尚未实机测试。
报告：`extracted/metadata/source-boot-r2-validation.json`。r1 证据摘要：`source-boot-r1-pstore-review.json`。

再次测试只更新 boot。若仍停住，回现有 recovery 拉取整个 pstore 目录；末尾的 `calling ...` 和 `[S17/DVFS]` 最后一个检查点用于确定卡住调用。
回退仍使用 `artifacts/source-boot-r1/boot-stock-rollback.img`。此轮没有通过工具刷写设备。
