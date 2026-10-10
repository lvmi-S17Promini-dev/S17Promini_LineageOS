# r4：第一次 mailbox 轮询之后停止前进

r4 实机仍未启动。原始日志归档在 `lineage-debug/oss-kernel-r4/console-ramoops-0`，262132 字节，console 环形区已覆盖早期日志。

同一 CPU-DVFS 通道在 0.649 秒和 1.205 秒完成两次 ACK 事务。1.206 秒的 cluster-on 命令编号 2，cluster 0，state 1；AEE 回调完成，polling lock 取得，mbox 0 / slot 3 / IRQ bit 1 发送返回 0。第一次 mailbox polling 返回 -1，之后没有外层轮询结束记录。

有限次数的 ACK 循环本应返回。因此尚不能把根因直接定为固件不回 ACK：也可能停在后续 mailbox 锁/MMIO、ACK 检查或 `udelay(1)` 内。接收线程后续使用 cpufreq mutex，但当前没有证据证明它和 ACK 构成死锁。

原厂反汇编再次确认相同的 command 2、mid 1、polling 1、4 个发送 word、1 个 ACK word。原厂 mailbox polling 同样只读一次 IRQ，不使用传入的 retries 参数。摘录保存在 `extracted/metadata/stock-sspm-cluster-and-poll.txt`。未发现足以直接修改协议的差异。

r5 在前两次、约第 100 次、第 100000 次、第 1000000 次及最后一次轮询记录 mailbox 进入/返回及 delay 进入/返回。只对 CPU-DVFS 频道输出，最多六组；不修改重试次数、延时、锁、握手或电压频率参数。用于区分循环无法推进与能推进但没有 ACK。版本改为 `4.19.127-s17-bringup-r5`。

## 产物

`artifacts/source-boot-r5/boot-source-r5.img`，32 MiB，SHA256 `8ef6a38090138982b1000695b2c88aa10d2b9ec7f51ca97aabc1845c36f910f0`。Lineage bootimage 编译及源内核/DTB、原厂 ramdisk、AVB、Android libufdt 验证通过，DTB/ramdisk/cmdline 与 r4 相同，尚待实机测试。

下一份 pstore 保存为 `s17-pstore-r5`。若停在 delay enter，优先查 arm64 delay 的 timer 计数；若停在 mailbox poll enter，优先查 mailbox 锁/MMIO；若循环进度和结束均出现，再按返回值分析 ACK。
