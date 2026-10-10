# AXS 触摸运行时诊断

目标是在一次更新 boot 后完成多种定位，不把诊断功能当作触摸已经修复。完整内核编译由用户执行。

设备节点 `/sys/bus/i2c/devices/0-003b/`：

| 节点 | 用途 |
| --- | --- |
| diagnostics | IRQ/休眠状态、边沿类型、选中的GPIO配置、IRQ/手动/轮询次数、读错/解析错/全零/全FF/有接触/空帧计数、最近报文 |
| live_version | 使用原厂0x0c查询实时版本，同时读取供电启用和电压；执行I2C寄存器选择和读取，不擦写固件 |
| supply_state | 不执行I2C事务，只读取regulator的启用状态和电压；负值保留为错误码 |
| reset_touch | root写20..200：复用GPIO174所有者，执行高1ms/低10ms/高后等待指定毫秒的复位；暂停时拒绝，不擦写固件 |
| firmware_pram / firmware_status | 显式load执行经校验原厂固件的PRAM下载，状态节点读阶段/包数/结果/版本；默认不加载，详见docs/82-manual-pram-ghidra.md |
| firmware_probe / flash_status | root写1执行原厂Flash接口的读取探测；不执行擦写，输出身份尾字节、状态、内容CRC及差异数，详见docs/83-touch-log-4-pram-result.md |
| firmware_repair | root显式repair-stock-0007：只补差异为FF的页，原厂初始化、编程、完整校验及版本检查，默认不执行，详见docs/87-guarded-ff-firmware-repair.md |
| flash_dump / flash_backup / flash_regions | 完整读取缓存、实际写前备份、每4KB统计，避免只靠CRC猜测缺失位置 |
| report_history | 最近16次读取的时间(ns，ktime monotonic)、来源、I2C/解析结果及32字节数据，按时间顺序排列；失败读取raw零填充，用read结果区分真正全零帧 |
| debug_reports | 写1临时开启限速dmesg原始报文日志，写0关闭，默认关闭 |
| poll_once | 写1读取单帧并走实际输入上报路径 |
| debug_poll_ms | 写20..1000开启对应毫秒间隔的轮询，写0停止；每次开启最多30秒，屏幕/系统暂停时跳过读取但期限继续计时 |
| irq_trigger | 写falling/rising/both测试边沿；默认保持DTS下降沿，不提供可能持续触发的低电平IRQ模式 |
| pin_profile | 写pull-up/no-pull/boot-default选择DTS配置；两种debug配置都开启输入和SMT，默认不主动切换 |

I/M/P分别表示IRQ、手动单帧和后台轮询。统计在驱动装载后累计；全零计入合法空帧，全FF计入解析失败，有接触计数代表走过解析并产生pressed状态，不代表Android应用确认收到。

GPIO配置使用私有DTS组，不通过放开整个mt_gpio接口的权限完成。boot-default恢复DTS状态，并不保证还原此前其他组件对GPIO寄存器做的未知覆盖；pin_profile是驱动选择记录，不是硬件寄存器的实时读回，需配合mt_gpio判断。运行时IRQ和配置切换会暂时mask IRQ，完成后恢复原禁用深度；这些实验可能改变输入行为，仅供定位。

所有报文读取由report_lock串行。后台轮询和手动读取持state_lock防止与暂停切换交错；cancel_delayed_work_sync在state_lock外执行，避免等待worker时死锁。暂停时先disable_irq等待IRQ线程结束，再锁report_lock释放已有接触。删除sysfs入口等待活动操作退出，再停止worker；devm按顺序先停止worker、再释放IRQ/input；shutdown同样停止worker并暂停IRQ。

只启用记录的16帧环形缓存，不默认开启周期读取、修改边沿或引脚状态。后续已补probe复位、手动PRAM和受控FF修复，均无自动固件写入；FF修复会显式编程并设置原厂SPI状态寄存器，不包含擦除。见docs/80、82、87。DEBUG_FS和GENERIC_IRQ_DEBUGFS已在实际使用的boot_test配置开启。

一键采集 tools/capture_s17_touch_debug.sh：只临时开启debug_reports和20ms轮询，保存运行前/每秒/运行后的状态、历史报文、GPIO和中断计数，以及实时版本、regulator摘要、dmesg和logcat。结束后关闭轮询、恢复原日志开关，拒绝覆盖已有输出目录及打断已有轮询。GPIO/边沿实验单独手动执行，脚本不会自动切换。若手机自动熄屏会跳过读取，应先延长screen_off_timeout；脚本不改系统超时设置。

用法：push脚本到/data/local/tmp，root shell运行 `sh /data/local/tmp/capture_s17_touch_debug.sh`，执行期间持续触摸，随后pull `/data/local/tmp/s17-touch-debug`。采集耗时会受诊断节点timeout影响，不保证严格15秒。
