# prebuilt-touch-1：正常LineageOS触摸与完整自动固件流程

原始文件oss-kernel-debug/prebuilt-touch-1，经BOM判断将UTF16读取后分析，未改原件。摘要extracted/metadata/prebuilt-touch-1-review.json。

启动0.937秒probe版本0000，自动线程读取ffff、host7，决定升级。实际走axs_Y15205_upgrade → flash_init → flash_erase → flash_write，197包在2.714秒完成，随后读回校验与复位，4.233秒报告upgrade success,new fw version07。flash write waiting是流程里的就绪等待打印，此会话最终成功，不能只因ERROR前缀判断失败。

getevent为axs_ts event3，mtk-tpd event4仅枚举；实际85个SYN_REPORT、9次BTN_TOUCH DOWN及9次UP，包含真实X/Y坐标及默认pressure63/area9。中断快照axs_ts hwirq0累计63次。当前源码IRQ0、输入范围、报文结构与这一基线不矛盾，但未实现它实际使用的完整Flash流程。

这一会话是用户确认的正常LineageOS+prebuilt基线，不与旧recovery会话混淆。原厂每次“初始化成功前先0000/ffff”并不等于无通信；当前靠复位或独立PRAM不能替代该完整过程。

下一步先完成此前基线对照的第二阶段：回到已有诊断source boot，观察触摸/实时版本；若未恢复，再用现有--probe读取CRC和差异数。这不要求新内核编译。它用于判断prebuilt完整处理后，源码是否可直接使用同一状态，或启动/复位后仍必须完整执行固件流程。没有这个后续结果，尚不能认定存储损坏是唯一根因，或直接要求新增C7自动擦写。

另修正Ghidra脚本中的已确认参数类型：raw_read/write为byte pointer+uint，write_read四参数，flash/pram writer为pointer+uint。先前自动推断的锁定空参数原型导致部分反编译调用省略参数，不能把这种输出当成原厂没有传包头。重新生成C并继续与汇编交叉核对，未修改内核。
