# prebuilt recovery 也无触摸：历史记录与当前判断

用户提供线索：Lineage recovery 使用 prebuilt 内核时也不能触摸。历史 docs/08-recovery-log-review.md、docs/09-touch-input-review.md 已记录此现象，并说明当时没有修改 prebuilt kernel。原始 recovery-debug/recovery-touch-events.txt 只有设备枚举，没有坐标或接触事件；设备为 axs_ts 和 mtk-tpd。它不是正常工作的触摸基线，不能要求当前源码仅复制其驱动就保证触摸工作。

## 原厂驱动历史日志

recovery-debug/normalized/recovery-r4-early-dmesg.txt：

* 1.101..1.114秒：axs_reset_proc，通过tpd_gpio_output执行复位；日志中的pin=0是该封装参数，不能直接当成实际GPIO0。原厂pinmux/LK分析已表明它选择GPIO174触摸复位。
* 1.142746秒：probe读取firmware version=0000。
* 1.143184秒：异步升级线程再次读取版本ffff，host版本7，决定升级。
* 1.143..1.172秒：flash初始化、擦除结束，开始sector写入。
* 2.118秒：recovery调用显示suspend_power。
* 2.142358秒：tpd_suspend明确记录tp in upgrade state, can't suspend。
* 2.148秒：显示resume_power；2.170..2.182秒又调用tpd_gpio_output执行触摸复位高低高，此时sector写入仍在进行。

recovery-debug/normalized/recovery-dmesg.txt（另一份采集，不将其逐行拼成同一会话）：

* 大量flash write waiting；sector写入一直到16.559秒。
* 16.676秒：read firmware is not same with writed firmware；flash_read_check失败。
* 16.814秒：upgrade fw app(times:1) fail，work结束。

## 含义与限制

这表明0000/ffff异常读取在原厂驱动运行的恢复环境也存在，不是新报文解码器才制造的现象；原厂已有完整升级逻辑，仍不能完成验证。正常自动流程从内核内嵌固件取数据，不支持简单归因于遗漏vendor固件文件。

显示复位与异步flash升级重叠是具体的可疑时序，可能中断升级。但早期日志已出现0000/ffff，升级之前也有通信/工作状态异常，不能将所有故障直接归到2秒的复位。只有readback校验失败，也不能宣称芯片flash必然损坏或必然需要重刷固件。

旧记录加强了对I2C访问可靠性、复位/供电协调和芯片当前工作模式的检查优先级。当前源码基本驱动没有flash擦写，保留此边界；不因这条线索启用自动flash升级。不要求用户立即重试prebuilt recovery，尤其不把历史已失败的自动升级当成恢复方案。

已新增的实时版本/供电查询、原始帧历史、GPIO配置实验和限时轮询仍可用于下一次测试。最终定位须区分：已确认的软件驱动差异、当前芯片状态、以及历史recovery升级失败对其状态的可能影响。
