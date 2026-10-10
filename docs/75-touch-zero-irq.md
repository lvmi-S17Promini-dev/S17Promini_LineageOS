# touch-log-1：零中断与受控报文读取

axs_ts 注册为 event2，触摸前后 IRQ16、mt-eint hwirq0 的所有 CPU 计数均为零。getevent 只有电源键事件。Android 输入设备存在，不能证明芯片在正常报告。

运行时 mt_gpio 的格式为 MODE DIR DOUT DIN DRIVE SMT IES PULL_EN PULL_SEL，但DRIVE占两位。GPIO0的0000001100须拆为0/0/0/0/00/1/1/0/0：GPIO输入、DIN0、SMT1、IES1、上下拉关闭。早先误按所有字段一位解读为下拉开启，已依据pinctrl-paris.c中%02d格式纠正。GPIO174的0111000100为GPIO输出高，触摸复位已释放。单次DIN0不能区分长期拉低和某一瞬间的有效中断；须结合读报文和中断状态。

原厂IRQ注册 Image0x9f8ea0 flags0x2002，即ONESHOT|TRIGGER_FALLING，与当前DT下降沿一致，不直接改成低电平触发。原厂reset流程也经过GPIO174高低高。更正格式后，GPIO0配置与DTS bias-disable一致，没有证据证明这里被异常覆盖。

新增axs15260_s17驱动 diagnostics只读入口：IRQ软件暂停状态、IRQ核心disabled位、IRQ/手动读取计数、probe时固件版本读取结果、最近报文读取/解析返回值及32字节原始报文。固件版本是probe快照，不是新的运行时读取。

poll_once为root写入口，只接受1，读取一个32字节报文并执行现有事件上报路径。IRQ和手动读取通过report_lock串行，屏幕/系统暂停时拒绝手动读取。它可能消费一个待处理报文，也可能清除中断线，因此是主动诊断，不是只读状态查询；它不写固件或控制寄存器。不会自动启动永久轮询。需在按住/滑动时多次读取，并在另一终端观察getevent。

已通过原有报文解析验证和git diff --check，未运行内核编译，运行时效果尚待验证。当前是诊断补丁，不宣称触摸已修复。
