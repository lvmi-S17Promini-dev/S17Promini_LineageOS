# r3：卡点收窄到 SSPM cluster-on，r4 仅细化通信日志

2026-10-10。用户反馈 r3 多了日志但仍未进入系统。原始日志归档在 `lineage-debug/oss-kernel-r3/console-ramoops-0`。

## r3 已排除的调用

最后一个没有返回记录的 initcall 是 `_mt_cpufreq_pdrv_init`。其内部日志显示：

- CPU PLL 读取返回 1499500 kHz；CPU AEE 记录完成，索引 9。
- CPU 索引同步返回 0，policy 字段设置完成。
- CCI PLL 读取返回 599750 kHz；CCI AEE 记录完成，索引 9。
- CCI 索引同步返回 0。
- 1.207164 秒打印 `cluster on SSPM command begin`，没有对应 `done`。
- 其他 CPU 的 USB/电池 worker 仍输出到约 1.254 秒。

因此继续只检查 cluster-on 的调用链：

```text
_mt_cpufreq_init
  -> cpuhvfs_set_cluster_on_off
  -> dvfs_to_spm2_command(IPI_SET_CLUSTER_ON_OFF)
  -> sspm_ipi_send_sync(mid=CPU_DVFS, polling)
  -> mailbox send / ACK polling
```

现有日志证明这个调用边界未返回，尚未区分参数构造/AEE 回调、发送锁、邮箱 MMIO 或 ACK 等待。
不能跳过该命令或修改等待逻辑来假称问题已修复。

pstore 文件为 262132 字节，等于 256 KiB console 区域减去 12 字节头部，开头从约 0.600 秒开始，已发生环形覆盖。不是从开机 0 秒保存的完整文本，但锁内的最后检查点仍在。

## 原厂对照及等待预算更正

从原厂 gzip kernel 的 PREL32 导出符号表定位 `sspm_ipi_send_sync`：Image 偏移 `0x7b4db4`。
原厂 cpufreq policy init 和 cluster-on 也使用命令编号 2、同步轮询发送。
原厂发送函数反汇编同样出现 200 万次外层轮询重试。记录在 `extracted/metadata/stock-sspm-send-sync.txt`。

当前 `sspm_mbox_polling()` 虽有名为 retries 的参数，但函数体只读取/检查一次邮箱 IRQ 位，没有使用该参数循环。因此此前按“外层次数 × 内层 2000 等待预算”的解释不成立。
既然原厂也有同类外层重试，不能仅依据该常量认定它是新移植错误。此轮保持全部重试、超时、锁和握手逻辑。

## r4 取证范围

只增加 CPU-DVFS 发送路径的日志，不记录所有其他 IPI，也不打印每次轮询：

1. cluster-on 参数及 AEE 回调完成；
2. send-sync 的服务初始化状态、频道、模式和数据长度；
3. polling lock 的取得前后；
4. mailbox send 的 mbox/slot/IRQ bit 及返回值；
5. ACK 等待和第一次 mailbox poll 的进入/返回；
6. 外层轮询退出时的 ret、剩余次数和 IRQ ACK 状态。

这些标记用于确定阻塞点，保持原来的命令值、频道、频率/电压参数、锁和等待行为。版本标识变为 r4；DTB、ramdisk 与 r3 相同。

## 产物

`artifacts/source-boot-r4/boot-source-r4.img`，32 MiB。
SHA256：`f0a0898beb2921842a8584ec96c81a9f4e842dce89530956e21c66fd09bee4bd`。

Lineage bootimage 构建通过；源内核、DTB、原厂 ramdisk、加载地址、AVB 和 Android libufdt pinctrl 校验通过。r4 尚未实机测试。
验证报告：`extracted/metadata/source-boot-r4-validation.json`；r3 摘要：`source-boot-r3-pstore-review.json`。

下一次拉取 pstore 后，按最后一条 `[S17/IPI]` 和是否有对应返回来判断。此轮没有通过工具执行设备刷写。
