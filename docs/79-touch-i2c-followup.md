# 原厂恢复环境触摸失败的静态复核

## 更正GPIO解读

pinctrl-paris.c:mtk_pctrl_show_one_pin使用 `%1d%1d%1d%1d%02d%1d%1d%1d%1d`，DRIVE两位。GPIO0 0000001100对应SMT1、IES1、pullen0、pullup0，原先“下拉开启/SMT关闭”判断错误，已更正文档并告知用户。GPIO82/83的1001001111为I2C0 SDA/SCL复用mode1、DIN1、SMT/IES开启和上拉开启；GPIO174输出高。单次状态本身未证明复位或总线配置有错。

运行时pin_profile仍可用于实验，但不再把pull-up作为依据已成立的修复；默认配置不改。

## 版本读取与恢复日志

原厂0x9f9b30构造两个标准i2c_msg，一字节写0x0c再两字节读；当前live_version同样执行两个消息。当前I2C_MTK的mt_i2c_should_combine把同地址写读合并为WRRD repeated-start，暂未发现客户端消息格式差异。

原厂恢复早期日志先0000、后ffff，发生在显示2.17秒重新复位之前。因此复位与升级重叠可能使升级进一步失败，但它不是最初版本读取异常的充分解释。当前报文全零/全FF及两次重复异常序列，不能只按“固件缺失”解释。

I2C0 DTS为400kHz、clock-div5、SCL83/SDA82，使用stock dtsi。没有凭经验降低频率、改GPIO复用或重写全局I2C完成判断。

## 控制器级定向trace

当前mt_i2c_irq会在IRQ状态0时打印日志并唤醒等待线程；mt_i2c_do_transfer末尾错误判断不要求TRANSAC_COMP存在，理论上可能有无完成位却报告成功的情况。但尚无当前触摸事务的irq_stat，不能认定实机就是此问题，也没有修改该返回行为。

新增I2C_MTK模块参数s17_touch_debug，0600、默认关闭。只在I2C0地址0x3b的事务输出限速trace：读写/WRRD类型、主/辅助长度、控制器返回值、捕获的IRQ状态、coherent DMA物理地址和最多32字节缓冲区。缓冲区是适配器完成后复制给客户端的来源；错误事务内容可能是旧数据，必须结合ret判断。不开启DMA预填充、额外寄存器访问或新的总线事务。

一键触摸采集脚本自动寻找该参数，临时开启，在实时版本和轮询期间抓取dmesg，结束恢复旧值。trace限速，所以不能把缺一条trace当成缺一条事务。它用于将“i2c_transfer成功”细分为控制器完成状态及返回数据，有助于判断该查总线完成还是芯片工作模式。

脚本语法、补丁空白检查通过，未运行内核编译。触摸故障尚未修复，等待用户下一次运行已准备好的诊断；当前不要求重进prebuilt recovery或执行flash升级。
