# r15：PSC5425E 与充电初始化修复

2026-10-11。用户要求移植 PSC5425E，提供两个 GitHub 样例，并指定使用 `/home/dengxh/ghidra_12.1.4_PUBLIC` 逆向原厂。本轮已实现驱动、修复充电管理重试、完成 boot 编译和离线验证。用户随后确认 CPU 占用下降和深度休眠恢复；充电、OTG 与各 cluster 调频仍需分别验收。代理未执行刷写。

## 来源与原厂核对

参考用户提供的 [flitsmeister hl7005.c](https://github.com/flitsmeister/android-os-kernel/blob/ed306e55b1ff9a96980edde7998fdff792b4b04e/drivers/power/supply/mediatek/charger/hl7005.c) 和 [Blackview psc5415a.c](https://github.com/visarloshaj/android_kernel_blackview_mt6765/blob/a2fe326ece668ed1aefdc93c18755b615e30b9c5/drivers/power/supply/mediatek/charger/psc5415a.c)，固定各自提交，不跟随默认分支。二者均有 PSC5425E 分支，但不是 S17 的直接等价驱动。参考原件保存在 `ref/psc5425e-flitsmeister/` 和 `ref/psc5415a-blackview/`。

芯片寄存器解释使用厂商 [PSC5425E Rev.06.5 手册](https://www.prisemi.com/pdf/1ldccdxp/PSC5425E.pdf)。手册的电流表依赖采样电阻与 REG51[0]。公开样例中分别有 1–3 A、0.8–2.6 A 等电流表，不能视为本板的实际电流。

`tools/reverse_psc5425e.py` 验证原厂 gzip kernel SHA256 `5851e16038244576d0a863d9f36b4ad602af68ea45477f5c6f44a9dfb0127286`，解压 Image、解析 kallsyms 并生成函数边界。使用用户指定 Ghidra analyzeHeadless，BinaryLoader、AARCH64:LE:64:v8A、基址 `0xffffff8008080000`、`-noanalysis`，复用 `tools/S17CameraDecompile.java` 对边界内函数定点反编译。日志及结果分别为 `extracted/metadata/psc5425e-ghidra.log` 和 `ghidra-psc5425e/`。

原厂真正的 charger_ops 回调证据：

| 功能 | 原厂实际回调使用的寄存器 |
| --- | --- |
| 启停充电 | REG1[2] CE，0 开启、1 关闭 |
| 充电电流 | REG4[6:4] |
| 输入电流 | REG1[7:6] |
| CV | REG2[7:2] |
| 充满 | REG0[5:4] = 2 |
| OTG | REG1[0] OPA_MODE |

原厂 current 回调的比较阈值为 656000、786000、1048000、1180000、1442000、1574000、1836000 uA，与手册 REG51[0]=0、50 mΩ 表吻合；最高档对应 1966000 uA。原厂 CV 回调为离散 4.1/4.2/4.35/4.4 V，编码 0/2/44/45；4.35 V 编码 36–44 等价。其 probe 使用 REG1=0xf8、REG2 code36、REG4=0x76、REG5=0x24。

文件中另一些 `psc5425e_set_ichg/vreg/iinlim` 辅助函数使用不同布局，与真实 charger_ops 和 PSC5425E 手册不一致，本移植不使用它们。原厂 probe 读取 REG0B 判断“非零存在”，也不能替代实际 IC_INFO（REG3）识别。

审核输出：`extracted/metadata/stock-psc5425e-audit.json`；`python3 tools/reverse_psc5425e.py` 已通过寄存器 mask 和阈值检查。

## 实现

内核 `drivers/power/supply/psc5425e.c`：

- 匹配现有 `mediatek,psc5425e`、I²C 地址 0x6a；读取 REG3，当前只接受样例明确区分的 E 型 ID=0xf0。未知 ID 在写入任何充电参数之前拒绝，不能把 PSC5425A revision6 套用 E 型参数。
- 保留现有 DT，读取 `psc5425e` 子节点的 chg_name/alias，注册真实 `primary_chg`。不启用 PMIC pulse charger 代替外部芯片。
- 采用每设备状态、SMBus byte-data 访问、互斥保护的 read/modify/write；读失败不继续写，错误返回调用方，REG4 写入时清除 RESET bit7。
- 实现充电启停、电流/输入电流/CV 读写、充满状态、寄存器诊断及 charger event，注册 `usb-otg-vbus` regulator。使用 charger device 的 device drvdata 存私有数据，避免与 manager 的 charger_dev_set_drvdata 冲突。
- REG51[0] 清为 0，以匹配原厂阈值。probe 先关闭充电，设输入 500 mA、CV 4.35 V、电流 code1、终止电流 code6、REG5=0x24，再交由 manager 设置并开启。没有设置 RESET，也没有取消硬件充电超时保护。
- 手册说明 32 秒 watchdog 只针对 OTG，任意 I²C 读写可以喂狗。只有启用 OTG 才启动 10 秒 alarm/work，执行期间临时持锁；关闭、remove、shutdown 均取消 alarm/work 并释放锁。没有常驻 RT 轮询线程，普通充电和拔线不启动 OTG alarm。

`drivers/power/supply/mtk_charger.c/.h`：先检查必需 primary/secondary charger，再初始化/注册算法；各算法使用独立 notifier_block，成功路径只运行一次。硬件仍不可用时，在每次 5 秒等待前释放 charger_wakelock，避免缺驱动时自环、100% CPU 和长时间持锁。

`mtk_basic_charger.c`：输入电流、充电电流或 CV 设置返回错误时，停止继续设置并尝试关闭充电，返回错误。避免 driver 已拒绝不支持的电压或发生 I²C 失败时，manager 继续开启充电。

新增 `CONFIG_CHARGER_PSC5425E`、Makefile 对象和测试配置开关；版本改为 `-s17-bringup-r15-charger`。

## 档位与限制

与原厂向上选档不同，本移植对可表示的电流/CV 请求向下选档。例如 1 A 选 786 mA，4.3 V 选 4.2 V；4.35 V 可以精确设置。电流低于最小正档时使用硬件最低档 656 mA，0 请求关闭充电；输入限流独立作用。没有增加一个并不存在的线性电流或 CV 档位。

输入档只有 150/500/800 mA 和 unlimited。有限请求均向下选档，2 A 请求仍为 800 mA；只有显式 U32_MAX 请求才选择 unlimited，getter 也以 U32_MAX 报告。普通 USB 500 mA 保持 500 mA。这与两个样例及原厂的“超过很小阈值就不限流”不同，可能使适配器充电较慢，后续可结合电源能力、板级电流和实机日志单独评估 unlimited。当前没有声明恢复原厂大电流充电能力。

没有实现外部 GPIO IRQ、线性 MIVR、PE 高压调节或 PSC 单独 power_supply 节点；manager 使用 polling，现有 PMIC 电源类型与电池上报继续工作。公开样例中部分 getter 错读 IINLIM、硬编码表和无效 watchdog bit 未沿用。OTG lifecycle 已实现并编译，硬件输出尚未验收。

## 验证和产物

- `tools/check_s17_psc5425e.py` 抽取实际 C 回调，在 ASan/UBSan 下用模拟 I²C 寄存器测试电流边界、CV 编码、输入 500/800 mA、2 A 不取消限流、0 请求关闭、充满状态、RESET 位、读/写失败返回和 drvdata 所有权；抽取实际 charger_init_algo 测试 primary/secondary 缺失、多次重试、多个算法独立 notifier、算法初始化失败。通过，日志 `extracted/metadata/psc5425e-host-tests.log`。没有用主机测试替代实机时序验收。
- 驱动及内核变更 checkpatch：0 errors / 0 warnings；内核和工程 diff 空白检查、Python 语法检查通过。
- `lunch lineage_S17Promini-userdebug`、`m -j8 bootimage` 通过；首次缺 power_supply 头文件的错误已修复。最后一次构建明确重新编译 mtk_basic_charger.o，最终产物包含最后加入的错误处理。
- `tools/verify_source_boot.py` 校验源码 kernel/DTB、原厂 ramdisk、加载地址、gzip、AVB hash、AXS/GPU 配置、overlay 后设备树、cpufreq 开启，通过。PSC5425E=y，未加入 cpufreq blacklist。没有运行 Android libufdt 独立检查。

交付：`artifacts/source-charger-r1/boot.img`，32 MiB，版本 `4.19.127-s17-bringup-r15-charger+`。SHA256：

```text
24ae1eac72a597901d1c7e7ec09d1a04e7f71ddcf1345155cfac94f5a1b70a06
```

标准编译输出：`/home/dengxh/android3/lineage/out/target/product/S17Promini/boot.img`。

最终构建/校验记录：`extracted/metadata/psc5425e-r15-build-final.log`、`psc5425e-r15-validation.json`。r14 备份仍为 `artifacts/source-flash-r1/boot.img`。只需测试 boot，无需重刷 system/vendor。

## 实机复测

先检查日志有 `PSC5425E ID=0xf0 registered primary_chg` 和 `Found primary charger`，确认不再重复 charger_init fail；看 charger_thread 在空闲时 CPU 接近 0%。未知 ID 或 I²C probe 错误应原样保留日志，不能跳过识别强行写寄存器。

检查 USB 插拔、充电电流/温度、充满/停止充电，再进行拔线息屏 3–5 分钟对照，使用现有采集脚本的新目录保存 before/after。验收 charger 锁不持续覆盖整个息屏期间、suspend attempts/success 增长及实际 suspend 驻留，并看 policy0 的 PLL 是否能下降。AccuBattery 在 USB 连接时的持锁仍应与内核锁分别判断。OTG 单独测试输出与关闭后的 alarm/唤醒锁释放。

离线检查与用户实机反馈分别记录；CPU 占用和深度休眠的验收状态见下节，其他测试项不据此推定通过。

## 实机反馈与提交

用户测试后反馈“可以了，cpu低了”，随后进一步确认“深度休眠也好了”。据此将 r15 的 CPU 占用改善和深度休眠恢复标记为用户确认实机通过。未提供新的 suspend 计数、驻留时间或频率日志，不记录具体数值，也不将各 cluster 调频、充电电流/充满和 OTG 标记为已验收。

内核提交按独立改动拆分，均带 Signed-off-by：

- `d997bb2c0f5e`：S17Promini: Fix charger initialization retries。
- `40d6cd858f18`：S17Promini: Add PSC5425E charger support。
