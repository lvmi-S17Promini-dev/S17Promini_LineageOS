# r5：循环前约 100 次可推进，r6 从其他 CPU 采样

用户返回的 r5 日志归档在 `lineage-debug/oss-kernel-r5/console-ramoops-0`。版本为 r5；262132 字节，仍是被覆盖的 console 环形区。

cluster-on 命令的前两次 mailbox 读取返回 -1，`udelay(1)` 均返回。remaining=1999900 时读取仍返回 -1，延时也返回，时间 1.212936 秒。没有 remaining=1900000 检查点，也没有 ACK polling ended。其他 CPU 的日志持续到 1.238657 秒。

因此 r5 不能证明第一次延时卡住；也不能证明后面两百万次轮询实际全部执行。仍需区分后续 mailbox 锁/MMIO、延时内部阻塞及循环只是推进很慢。

原厂 `__delay` / `__const_udelay` / `__udelay` 的 PREL32 导出定位和反汇编在 `extracted/metadata/stock-arm64-delay.txt`。原厂同样读取架构计数器、可用时使用 WFE；没有据此修改全局延时实现。

## r6 观测

仅为首次 CPU-DVFS command 2、state 1 启动一次旁路 delayed work，在 polling lock 内确定当前 CPU，目标为另一颗在线 CPU。约 20 ms、100 ms、1.1 s、2.1 s 采样最多四次。工作线程只读取普通内存里的诊断状态和 ACK 原子值，不获取 mailbox/cpufreq 锁、不访问 mailbox MMIO、不发送命令。

循环在原来的步骤前后用 WRITE_ONCE 更新阶段和剩余次数。阶段 1=mailbox 进入；2=mailbox 返回及 ACK 检查；3=udelay 内部；4=udelay 返回；5=循环结束。

mailbox 内部阶段 1=等待锁；2=取得锁、进入 IRQ MMIO 读取；3=读取完成；4=ACK 清除写入完成；5=无 ACK、解锁返回。另记录最近 IRQ 值及 lock0 状态。

这些读数不是原子一致的多字段快照，必须按多次样本判断。没有样本不等于工作线程已经执行，也不能单凭此认定整机所有 CPU 停止。诊断写入和输出会影响时序，但不修改协议、重试次数、延时参数、锁顺序或频率电压策略。

当前源码九套 SRAM 频率/电压/分频表的全部 48×4 个 word，在原厂 Image 中均找到逐字节相同副本，包括当前 level 3 对应的 c65Tbl。比较结果在 `extracted/metadata/stock-dvfs-pv-table-comparison.json`。这不能证明运行时表选择/写入或固件行为完全一致，但没有发现需要替换静态表的证据。

r6 已编译并通过 boot 校验，未实机测试。用户随后要求先屏蔽 cpufreq，优先测试 r7；无需先刷 r6。
