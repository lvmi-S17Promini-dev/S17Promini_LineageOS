# LK 多按键 fastboot r2 与启动副本标记（2026-10-06）

## 实机反馈

用户在取得 r2 和测试步骤后反馈“可以了”，确认本轮开机按键 fastboot 入口成功。r2 作为当前已获用户实机成功反馈的候选保留。用户未分别报告哪个实体键成功、是否三个键全部通过、是否观察到 S17-LK-r2 标记，不能将这些细项自动记为已验证；也尚未验证刷入 Lineage 后或 boot/recovery 损坏时的表现。

下文保留制作和测试前的边界记录。

用户两次 `fastboot flash lk lk-volup-fastboot-r1.img` 均完成 Sending/Writing OKAY；实机冷启动按音量上、音量下和快门键仍进入 Orange State 后启动 Android，未进入 fastboot。r1 的静态/模拟通过不能替代这一实机失败报告。尚不清楚是实际加载 lk2、按键号覆盖不够、初始化时序还是其他问题。

r1 已删除原来的空转等待，只检测一次 PMIC 键号 17；因此失败不能直接归因于“r1 的 50 ms 窗口太短”。r2 按用户要求同时增加按键覆盖和检测时长，并提供加载副本的可见标记。

## 产物与预期操作

`artifacts/lk-keys-fastboot-r2.img`，1048576 bytes。
SHA256：`2a664154dafa634e85210d9827b9384de8dfde837f7d761757fc71ee6b10c6e9`。
重新签名完成，44 组机器码模拟检查通过，尚未实机验收。本轮没有连接或刷写设备。

预期任意一个键即可：完全关机，提前按住音量上、音量下或快门键中的一个，再按电源；屏幕点亮后可松开电源，继续按住选择的键。三个键都对应 bootloader fastboot，没有 recovery 菜单。正常路径无按键时增加约 3 秒等待，再继续原厂启动。

先用不按键启动确认实际加载：保留 `Orange State` 和原厂设备已解锁警告，只把其倒计时行改为 `S17-LK-r2: boot in 5 seconds`。看到 `S17-LK-r2` 才能证明本次启动加载了带该标记的镜像；该标记自身不证明按键逻辑成功。启动成功进入 fastboot 时可能不会出现 Orange State，不以缺少警告认定失败。

若不按键仍显示原来的 `Your device will boot in 5 seconds`、没有版本标记，优先核对文件、读回分区和 preloader 实际选择的 lk/lk2。不能由此单独断言一定加载 lk2；也不据此直接覆盖 lk2。

## 键号与检测窗口

读取原厂 dtbo 的 `mediatek,kpd-hw-init-map`，不是修改 DT：

| 路径 | LK 硬件键号 | 原厂证据 |
| --- | --- | --- |
| keypad 音量下 | 0 | map[0] = 114 / KEY_VOLUMEDOWN |
| keypad 音量上 | 1 | map[1] = 115 / KEY_VOLUMEUP |
| PMIC home/reset | 17 | kpd-hw-rstkey = 17；原厂解锁确认分支使用该号 |
| keypad 快门 | 19 | map[19] = 212 / KEY_CAMERA |

Linux KEY_CAMERA=212 是映射结果，不能直接传给 LK 的 mtk_detect_key。本机 recovery 的 mtk-kpd 节点声明 KEY_CAMERA；实体快门是否在 LK 阶段确实上报该硬件键号仍待实测。

依次检测 0/1/17/19，命中即写 g_boot_mode=99 并返回。未命中则调用原厂 watchdog restart（0x48044280，写 0x1971 到 0x10007008），等待 10 ms，检查 get_timer 的 elapsed 是否达到 3000 ms，继续下一轮。另设最多 300 轮，避免计时器不增长导致永久等待。实际无按键耗时包括初始化和检测调用成本，不是精确 3.000 秒。

选择位于 META/dump/已有 fastboot 重启处理之后，RTC/RGU/misc recovery 和充电判断之前；保持诊断启动原有优先级。电源键本身不触发救援分支。

## 修改范围与校验

r1 的 24-byte 空转块不够容纳完整轮询，因此 r2 在原函数自身空间重新实现 boot_mode_select，仍使用相同 Thumb 入口 0x480037fc、栈帧和调用约定。原函数与 literal pool 覆盖文件偏移 0x39fc..0x3b93，共 408 bytes；新函数及 literal pool 为 272 bytes，其余填 Thumb NOP，不延长 payload，不挪动 DTB或占用 BSS。

保留原厂 dump 检测、META 检测、环境中的 reboot_meta_flag 处理、RTC/RGU fastboot 标志处理、无按键时 recovery/充电处理。原 factory-forbidden 分支只打印日志，没有实际 factory 按键逻辑，重写时省略该无功能日志；其余部分日志也省略，功能路径通过差分模拟核对。

除此之外仅修改 Orange State 倒计时文本，以及 LK cert2 payload SHA256 和 RSA-PSS 签名。LK header、cert1、根公钥、镜像公钥、内嵌 DTB及其 cert1/cert2 均逐字节不变，外部 dtbo/boot/recovery/system 不改。

使用与原厂匹配的 MTK 测试 image 私钥，保留原 cert1 信任链；原厂与修改后 cert2 均验证，修改后签名另用 OpenSSL 复验 `Verified OK`。没有修改启动验证策略或 preloader。

44 组检查覆盖每个键保持按下、延迟 2.5 秒按下、超时后才按下、快门短按、无按键、电源键、无关按键、双音量键、watchdog MMIO 写入、300 轮上限、计时器回绕、RTC/RGU/misc recovery 覆盖、充电、META 环境标志清除与设置失败、dump/reset 和已有 fastboot。每组核对模式、正常路径平台调用顺序、栈和 r4..r11；环境标志写入的名称和值与原厂一致。真实 key detector 执行，mock 按键配置初始化和 PMIC 状态，按键矩阵用 active-low MMIO 模拟；仍不证明实机硬件初始化、按键事件、preloader 接受或实际启动副本。

原厂机器码直接分支扫描未发现外部跳入被替换函数内部的入口；整镜像间接跳转未穷举。报告见 `artifacts/lk-keys-fastboot-r2.json` 和 `extracted/metadata/lk-key-entry-r2-emulation.json`，反汇编见 `extracted/metadata/lk-key-entry-r2-disassembly.txt`。

## 当前实机测试步骤

沿用此前已成功的 BROM/mtkclient 原厂 LK 回写条件，保留 `stock_imgs/lk.bin` 与原厂 lk2。首轮仍只写 lk；这次需要版本标记取证，不以刷写 OKAY 或出现 Orange State 判断新代码已运行。

将 r2 文件复制到 Windows 当前目录：

```powershell
adb reboot bootloader
fastboot flash lk .\lk-keys-fastboot-r2.img
fastboot reboot
```

首次不按键启动，记录倒计时行有无 `S17-LK-r2`。有标记再分别冷启动测试音量上、音量下、快门，按住单个键；进入 fastboot 后用电脑 `fastboot devices` 确认。双音量插 USB 仍是原有 BROM 接入，不把它和这个测试混用。

有标记但三个键都不生效：下一步核实真实按键事件与 LK 初始化路径，不继续盲目扩大窗口。无标记：先查加载副本及实际读回内容，不直接刷 lk2。发生异常则经 BROM 只回写本轮改动的 lk，命令见 [r1 回退说明](20-lk-key-entry-r1.md)。

## 重建

```bash
lk_toolchain=/home/dengxh/android3/lineage/prebuilts/clang/host/linux-x86/clang-r416183b1/bin
mkdir -p extracted/lk-key-entry-r2
"$lk_toolchain/clang" --target=armv7a-none-eabi -c tools/lk_key_entry_r2.S -o extracted/lk-key-entry-r2/key-entry.o
"$lk_toolchain/ld.lld" -T tools/lk_key_entry_r2.ld extracted/lk-key-entry-r2/key-entry.o -o extracted/lk-key-entry-r2/key-entry.elf
"$lk_toolchain/llvm-objcopy" --dump-section .boot_mode_r2=extracted/lk-key-entry-r2/boot-mode.bin extracted/lk-key-entry-r2/key-entry.elf
/home/dengxh/mtkclient/venv/bin/python tools/patch_lk_key_entry.py --revision r2 --output artifacts/lk-keys-fastboot-r2-rebuild.img
/home/dengxh/mtkclient/venv/bin/python tools/verify_lk_key_entry_r2.py --image artifacts/lk-keys-fastboot-r2-rebuild.img --report extracted/metadata/lk-key-entry-r2-rebuild-emulation.json
```

签名使用随机 PSS salt，重建整包 SHA256 可以不同。最后一个命令验证重建文件，不带参数时验证正式 r2 文件。
