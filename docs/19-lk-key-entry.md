# 开机实体按键进入 fastboot：原厂 LK 静态分析（2026-10-06）

后续已制作并重新签名 r1 候选，静态与 16 组 ARM 模拟检查通过，尚未实机验证；见 [r1 补丁与验收说明](20-lk-key-entry-r1.md)。本文保留制作前的分析边界。

用户当前优先事项：在 Lineage 无法启动时仍能通过开机按键进入 fastboot，暂不改 DT。`mtkclient-to-fastboot/FILES` 内两个文件已实测不能使用，不再作为已有效方案。

## 本机证据

分析原厂 `stock_imgs/lk.bin`，SHA256：
`a745cfc9f513aba14bf461f90253e0824a778172621868f53b4d4ac01b6d7864`。
`lk2.bin` 与它哈希相同。MTK 头长 0x200，payload 链接基址 0x48000000；ARM 入口与 Thumb 函数混用。完整相关反汇编见 `extracted/metadata/lk-key-entry-analysis.txt`。

- `0x48002eac` 调用 `0x480037fc`，后者结合 RTC/RGU、misc recovery 命令、充电状态选择启动模式，与公开 MT6765 LK 的 `boot_mode_select` 结构对应。
- `0x480038f0` 读取计时器，打印 `eng build`，然后进入 `0x480038fe..0x48003906` 等待循环。循环只调用计时器并比较 0x31，**没有按键检测调用**，结束后进入充电模式判断。
- `0x48004aec` 是可识别的按键检测函数：初始化按键配置，按键号匹配电源/PMIC home key 时调用 PMIC 检测，否则读取 0x10010004 起的 keypad 寄存器。
- 静态扫描到该函数的直接调用在 `0x48026390/0x4802639a/0x48026562/0x4802656a/0x4804aed6`，未见启动模式选择函数调用它。其他调用者与确认界面相关；未完成整个 LK 所有间接调用路径审计。
- `0x48003852` 比较启动模式 0x63（fastboot），非 fastboot 才继续后续流程；`0x480038d2`、`0x4800391a` 的 RTC/RGU fastboot 分支均写 0x63。这为补按键分支提供现成模式选择依据。

结论：本机正常启动模式选择中的按键判断缺失，是当前最有证据的原因；之前仅凭 `boot menu key` 等字符串无法确定这一点。不能由此保证不存在其他入口或特殊复位条件。

## FILES 两个脚本的边界

`redirect_to.py` 连接电脑端 mtkclient 后调用修改版 `Preloader`，不会安装永久开机按键功能。
`direct_fastboot` 将 watchdog + 0x24 低四位写成 3 后触发复位；`bybutton` 调用 `direct_factory`，低四位写成 0xC。固定数值不是跨机型的“开启按键”协议。
本机 LK 确实有 RGU 模式 3 的 fastboot 检查，因此不能断言脚本失败就是模式号错误。连接权限、寄存器访问是否成功、复位时标志保留、preloader 是否改写均未由实机日志确定。脚本还将某些 USB 异常直接打印为成功，输出本身不能作为进入 fastboot 的验收。

## 推荐方案：补 LK 的开机按键判断

在 LK 正常启动路径、加载 Android boot 镜像之前，调用现有按键检测函数；检测到选定实体键持续按下时，设置本机 fastboot 模式 0x63 并返回。可增加有限采样窗口以避免偶发触发。具体实体键与 LK 原始按键号需要进一步核实，不能直接拿 Linux 的 KEY_VOLUMEUP=115 等事件码当 LK 参数。

对于救援用途，设计上应使按键检查先于 misc recovery 命令处理，避免损坏的 recovery 加上残留重启命令抢占入口；插入位置必须同时检查早期 META、dump、复位分支的优先级和按键硬件初始化条件。当前已定位普通启动中的缺失代码，尚未完成这个补丁的控制流、安全校验与实机验证。

收益：bootloader fastboot 在 Android 内核/Lineage system 启动前工作，不依赖 recovery 内核启动；符合用户首选。
需要改的是 LK 分区。只刷 system 不会给原厂 LK 增加缺失的按键检查。LK 自身异常、启动认证失败或更早的 preloader 问题不由这个方案覆盖。已解锁不等于修改后的 LK 一定通过早期认证，需先检查镜像签名/校验和启动验证链。

后续工作顺序：确认键号 → 设计可审阅的离线补丁及反汇编校验 → 确认 LK 恢复写入链路及备份覆盖 → 再安排实机验证。按键冷启动进入 fastboot、正常不按键启动、system 启动失败时仍可进入、recovery 重启标志存在时能否抢占，需分别验收。本轮只分析，没有生成可刷 LK、连接设备或刷分区。

## 次选与兜底

- boot/init 早期检测按键后重启 bootloader：可绕过 system 挂载/框架故障，但依赖内核和早期 init 能运行。内核或 boot 镜像坏时无效，不能承担首选救援入口。
- BROM/mtkclient：本机历史上用双音量键插 USB 接入并回写过 recovery，是独立于 Android 的底层恢复路径；不是用户要求的单机按键 fastboot。保留已成功的环境，不用失败的 FILES 脚本代替。
- 不推荐常驻 misc 的 boot-fastboot/recovery 命令：没有按键选择，可能反复进入该模式；不能解决缺失的开机按键逻辑。

参考实现：[公开 MT6765 LK boot_mode.c](https://github.com/svoboda18/lk/blob/main/platform/mt6765/boot_mode.c)。该源码展示按键检测所在层次与优先级，但不是本机源码；本机判断以反汇编为依据。
