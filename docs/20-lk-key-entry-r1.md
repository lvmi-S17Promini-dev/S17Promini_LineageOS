# LK 音量上键 fastboot 补丁 r1（2026-10-06）

后续实机反馈：只刷 lk 两次均 Writing OKAY，但按键冷启动仍进 Android，r1 按键入口未通过验收。新候选与副本取证方法见 [r2](21-lk-key-entry-r2.md)；下文保留 r1 制作时的记录。

候选产物：`artifacts/lk-volup-fastboot-r1.img`，大小 1048576 bytes。
SHA256：`866ab08f07075bb56d413f6d08373f8ff5a1d3541f8a8cf63ba61e115963d9d2`。
原厂镜像未改；本轮未连接设备或刷写。

## 目标操作与行为

刷入并通过实机验收后，完全关机，先按住音量上键，再按电源开机，持续按住音量上直到进入 bootloader fastboot。不要同时按双音量键；双音量插 USB 是原来的 BROM 恢复路径。

本补丁在 LK 启动模式选择中检测一次按键，需提前按住；没有增加长按计时或启动菜单。检测到按键后将 g_boot_mode 设为 0x63，然后直接返回，不加载 Android 内核，也不启动 recovery/fastbootd。

插入点优先于 RTC/RGU recovery、misc recovery 命令和充电启动判断；保留原有更早的 META、dump 和 fastboot 重启路径。它不是无条件抢占所有启动分支。普通路径不按键时继续原厂处理，仅移除了原来的约 50 ms 空转等待和 `eng build` 日志。

键号证据：原厂 LK 解锁界面把 `mtk_detect_key(0x11)` 命中后的路径接到 `Start unlock flow`，对应它显示的 Volume UP 确认；另一个 factory key 接取消。原厂 dtbo 的 `mediatek,kpd-hw-rstkey=0x11` 使该键走 PMIC home/reset 检测。dtbo keypad map 中还存在其他音量键编码；不能用 Linux input event 编码直接替代。最终实体键对应关系须实机确认。

## 补丁范围

源码：`tools/lk_key_entry.S`、`tools/lk_key_entry.ld`；生成器：`tools/patch_lk_key_entry.py`。

- 文件偏移 0x3a6a / VA 0x4800386a：原来调用 PMIC just-reset 检测的 BL 改成 B.W 到按键入口，4 bytes。
- 文件偏移 0x3af0..0x3b07 / VA 0x480038f0..0x48003907：用原空转等待块容纳跳板，24 bytes。正常 misc 处理后的落入路径直接跳过跳板，避免重复检测和回跳循环。
- 按键未命中：跳板重放原 PMIC just-reset 调用，再回到 0x4800386e。
- 按键命中：写模式 0x63，然后用原函数保存的栈帧返回。现成 key detector 会自行初始化按键配置。
- cert2 的 payload SHA256 与 256-byte RSA-PSS 签名更新。header、cert1、所有公钥、LK 内嵌 DTB 及其两份证书、分区填充保持原样；dtbo/boot/recovery/system 不改。

代码覆盖范围共 28 bytes；加上哈希和签名，本次实际有 313 个 byte 的值改变。没有向 payload 尾部插入代码，不占用 LK 的 BSS 或 DTB 空间。

## 签名与来源

本机原厂 cert1/cert2 与公开 MTK 测试密钥匹配。原厂 cert1 的 RSA-PSS 签名、cert2 的 RSA-PSS 签名、header SHA256 和 16-byte 对齐后的 payload SHA256 均先验证通过，才执行修改。

保留原厂 cert1，仅重签变更的 LK cert2；RSA-PSS/SHA256，MGF1-SHA256，salt 32 bytes。新签名已用 cryptography 与独立 OpenSSL 复验通过。签名包含随机 salt，重建得到的整包 SHA256 可以不同，代码和哈希字段应一致。

密钥参考仓库：[mtk-lk-tools](https://github.com/TheGammaSqueeze/mtk-lk-tools)，本地 `ref/mtk-lk-tools`，commit `1eff1b6051fd0312a43faa73f3e955a254de622d`。仅使用与原厂证书匹配的测试 image 私钥，不改信任根、启动验证策略、解锁状态或 preloader。原始证书解析不能直接用通用 X.509 parser；其 MTK 特定布局使用 DER TLV 定位 TBS、hash 与 signature。

静态签名正确仍不等于本机 preloader 已实测接受新 LK。本机 preloader 尚无备份，不能完成实际加载策略和硬件启动条件审计。

## 已完成检查

`artifacts/lk-volup-fastboot-r1.json` 保存补丁范围、公钥指纹、签名/哈希检查、整包 SHA256。
`extracted/metadata/lk-key-entry-emulation.json` 保存 16 组完整 ARM/Thumb 函数执行结果：

- 不按键正常启动；按键选择 fastboot。
- misc/RTC/RGU recovery 标志分别存在时，不按键仍进 recovery，按键优先 fastboot。
- 充电启动时，不按键保留充电路径，按键选择 fastboot。
- factory forbidden 标志不会屏蔽救援键。
- 原有 RTC/RGU/初始 fastboot 路径和 META 路径保留。
- 每组比较原厂和补丁机器码的执行结果，检查栈及 r4..r11 保存；平台 mock 会破坏 caller-saved 寄存器，以检查跳板的调用约定。
- 执行原厂 key detector，mock 按键配置初始化和 PMIC 状态；其余外围函数 mock。不能代替真实按键硬件、按键初始化时序、USB fastboot 枚举或 preloader 验证。
- Capstone 与 LLVM Thumb 反汇编核对分支目标。扫描未发现原厂其他代码直接跳入复用块；间接跳转未作整镜像穷举证明。

## 重建

在工作目录执行；已存在的输出会被生成器拒绝覆盖，可用 `--output` 指定新文件。需要 cryptography、capstone、unicorn；本机 mtkclient venv 已提供这些依赖。

```bash
lk_toolchain=/home/dengxh/android3/lineage/prebuilts/clang/host/linux-x86/clang-r416183b1/bin
mkdir -p extracted/lk-key-entry
"$lk_toolchain/clang" --target=armv7a-none-eabi -c tools/lk_key_entry.S -o extracted/lk-key-entry/key-entry.o
"$lk_toolchain/ld.lld" -T tools/lk_key_entry.ld extracted/lk-key-entry/key-entry.o -o extracted/lk-key-entry/key-entry.elf
"$lk_toolchain/llvm-objcopy" --dump-section .key_hook=extracted/lk-key-entry/hook.bin --dump-section .key_slot=extracted/lk-key-entry/slot.bin extracted/lk-key-entry/key-entry.elf
/home/dengxh/mtkclient/venv/bin/python tools/patch_lk_key_entry.py --output artifacts/lk-volup-fastboot-rebuild.img
/home/dengxh/mtkclient/venv/bin/python tools/verify_lk_key_entry.py --image artifacts/lk-volup-fastboot-rebuild.img --report extracted/metadata/lk-key-entry-rebuild-emulation.json
```

最后一个命令验证重建文件；不带参数时验证默认 r1 产物。本轮验证的就是上面的固定 SHA256 产物。

## 实机测试边界与回退

这是已签名、通过静态和模拟检查的候选镜像，尚未通过冷启动实测。LK 出错会使 bootloader fastboot 也无法进入；恢复依赖此前 BROM/mtkclient 链路，不能靠刷坏后的 adb reboot 回退。

准备测试时，先在已成功的 BROM/mtkclient 环境重新读取 `lk`、`lk2` 到新文件并与原厂备份核对，确认当前可独立接入且工具有相应写入条件。此前回写 recovery 成功不等于本轮已验证 LK 写入恢复。首轮只改 `lk`，保留 `lk2` 原厂副本；本机 preloader 的 lk/lk2 选择与回退行为未确认，不承诺它会自动救回。

条件具备后的测试命令（本轮未执行）：

```powershell
fastboot flash lk .\lk-volup-fastboot-r1.img
fastboot reboot
```

每步确认命令完成；先验证不按键正常开机，再完全关机，用音量上+电源冷启动；电脑 `fastboot devices` 能枚举，`fastboot getvar is-userspace` 预期不是 `yes`，随后用实际可用的 bootloader 命令确认模式。还需验证按键进入后可正常退出，不把反复按键或仅屏幕显示当成功。

备用回写，仅在刷后异常、已通过 BROM 接入并确认本机和原厂备份时使用：

```bash
cd ~/mtkclient
python mtk.py w lk /home/dengxh/S17Promini_LineageOS/stock_imgs/lk.bin
```

如果本轮只写 lk，回退也只回写 lk。没有给出同时覆盖 lk/lk2、擦除 misc 或修改 preloader 的测试步骤。
