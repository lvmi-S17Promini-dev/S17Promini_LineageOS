# 正常Lineage/prebuilt基线与源码probe复位差异

用户澄清：完整原厂系统和LineageOS+prebuilt kernel触摸正常；Lineage recovery+prebuilt kernel触摸不正常，只能按键操作。当前源码kernel在LineageOS中无触摸。不能混淆两种prebuilt启动环境，也不能据恢复环境升级失败推定芯片flash损坏。

touch-log-3轮询771次，read_errors0、contact0、decode_errors756、zero14、ff98。脚本实时版本ffff，用户采集前0000；供电开启2.8V。捕获的I2C事务均ret0、irq0x1，没有支持“虚假完成”的证据；限速trace的结论仅覆盖记录到的事务。

原厂probe Image0x9f8a78..0x9f8a94先将触摸reset输出高、等待100ms，再axs_reset_proc(20)。Image0x9f85cc..0x9f8610执行高1ms、低10ms、高后等待20ms；0x9f8b2c才读版本。当前基础驱动启用vtouch后直接查版本，缺少独立probe复位。

LCM中的touch reset不等价：primary_display.c在is_lcm_inited非零时跳过disp_lcm_init。此前熄屏亮屏后的LCM复位也没有恢复触摸，故缺probe复位是确定漏项，尚不能认定唯一根因。原厂还包含100ms电源稳定等待、升级及其他扩展，与基础驱动不完全相同。

新增共享s17_axs15260_reset_touch(settle_ms)，复用LCM GPIO174描述符及bias_lock，允许20..200ms。LCM仍传20。touch probe在启用vtouch后先等待100ms、读版本前调用20ms复位；GPIO所有者未就绪返回EPROBE_DEFER，不重复申请GPIO。TOUCHSCREEN_MTK_AXS依赖MTK_LCM和匹配CUSTOM_KERNEL_LCM，S17现有defconfig满足，确保共享符号提供者存在。

新增root sysfs reset_touch入口，写20..200执行该序列。state_lock防止轮询/暂停交错，disable_irq等待IRQ线程，report_lock防止并发报文读取；释放接触、复位后恢复IRQ禁用深度。diagnostics记录reset_requests、last_reset_result和last_reset_ns。此入口不执行固件下载或flash擦写。

验证：提取实际共享函数正文，主机mock检查非法参数、无GPIO时无写入/锁释放、高低高及1/10/20ms时序、120ms等待选择；通过-Wall -Wextra -Werror。报文解析、touch checkpatch、diff空白检查通过。未运行内核编译或手机验证。

下一版先确认reset_requests至少1、last_reset_result0，再运行live_version和采集。若仍无触摸，可在亮屏时写reset_touch=120，再读版本、采集报文，以验证重置与额外等待；不必为延时试验再次刷boot。
