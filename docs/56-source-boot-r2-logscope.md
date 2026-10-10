# r2 日志复核：先修日志覆盖，r3 限定为取证

2026-10-10。用户反馈 r2 与 r1 一样停住，日志在 `oss-kernel-debug/s17-pstore-r2/console-ramoops-0`，已原样归档到 `lineage-debug/oss-kernel-r2/console-ramoops-0`。

## 已确认

- 内核确为 `4.19.127-s17-bringup-r2+ #3`。
- 触摸 pinctrl 错误消失，出现 `existing firmware version 0000`、`input: axs_ts` 和 `basic reports ready: addr=0x3b irq=16`。这只证明探测/注册路径通过；版本 0000 和真实报点仍待验证。
- CPU0 的 efuse 临时设备释放、CPU level 获取、频率表设置均完成。
- 最后一个 DVFS 检查点是 `policy cpufreq lock acquired`，时间 1.201712 秒。
- 此后其他 CPU 的 USB/电池 worker 仍打印到 1.246350 秒。不能把它描述为所有 CPU 在同一时刻停机。
- 锁内包含 CPU/CCI 的索引同步、PLL 读数、AEE 记录和 SSPM cluster-on 命令；r2 未在每个调用之间打印，尚不能定位具体函数。

## 先核对 marlon 和日志，而不是扩大适配改动

在同一源码树/编译器下生成 marlon 配置，对比当前 S17 r2 配置，CPU_FREQ、MTK_CPU_FREQ、SSPM、AEE、频率跳变、schedutil 和 ENERGY_MODEL 的关键开关一致。比较记录：`extracted/metadata/source-boot-r2-dvfs-config-comparison.json`。

平台头文件会从 SSPM 支持派生 HYBRID_CPU_DVFS，所以不能仅凭 `.config` 没有显式 HYBRID_CPU_DVFS 就判断路径不同。
原厂内核对应 policy init 的反汇编也包含持锁索引同步和 cluster-on 调用；持锁本身不能视为错误。

`initcall_debug` 确实在 r2 命令行里，但 `init/main.c` 的开始/完成消息使用 KERN_DEBUG=7。
配置的 console_loglevel 默认也是 7；`kernel/printk/printk.c` 会过滤 level >= console_loglevel 的消息。
pstore 的 console 后端受这层过滤影响。这解释了为何找不到 `calling ...` / `initcall ... returned ...`；并不证明 INFO 级 DVFS 检查点之后已经执行完成。

## r3 的范围

用户要求收窄排查。r3 仅改日志与版本标识：

- 命令行添加 `loglevel=8 ignore_loglevel`，继续使用 initcall_debug。
- `_sync_opp_tbl_idx` 添加 PLL 读取和 AEE 记录的前后消息。
- policy init 添加 CPU 索引完成、CCI 索引前后、SSPM cluster-on 前后消息。
- 频率/电压表、锁、governor、watchdog、SSPM 超时及其他驱动策略保持原样。

SSPM 轮询代码曾作为候选线索检查，但现有日志未证明执行到该调用，因此不修改其重试/超时逻辑。

## 产物

`artifacts/source-boot-r3/boot-source-r3.img`，32 MiB。
SHA256：`8f528093491260b57e41d4cc7f1a1b21bc7ec46d8308dd5cbc6df5bbfa2212bd`。

Lineage bootimage 构建成功；内核/DTB/原厂 ramdisk/加载地址/AVB 校验通过，Android libufdt 合并后的触摸 pinctrl 引用也通过。DTB table 和 ramdisk 哈希与 r2 相同。
报告：`extracted/metadata/source-boot-r3-validation.json`。

下一次只观察 r3 的日志：若输出已进入 Android init，应调整对停点的判断；若仍在锁内，按最后一个前后检查点定位调用。r3 尚未实机测试，未通过工具执行设备刷写。
