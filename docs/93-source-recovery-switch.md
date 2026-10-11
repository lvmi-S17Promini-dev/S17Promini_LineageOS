# Recovery 使用 OSS kernel 与源码 DTB

> 2026-10-11 更新：用户确认 OSS 内核 recovery 触摸已修复；下文保留当时的 prebuilt/测试状态。旧自动固件刷写失败的补充说明见 [修复确认](95-recovery-touch-fixed.md)。

2026-10-11。用户确认 cpufreq 修复后希望切换 recovery 并开始测试。编译继续由用户执行，本轮只调整配置和验证工具，未编译或刷写手机。

## 核对结果

公共 BoardConfig 已设置 TARGET_KERNEL_SOURCE=kernel/lvmi/mt6765，两个产品都使用 S17Promini_defconfig + S17Promini_boot_test.config。Android build core 在没有独立 recovery kernel 配置时，recovery_kernel 使用同一个 PRODUCT_OUT/kernel。

但 BoardConfigRom 的 BOARD_RECOVERY_MKBOOTIMG_ARGS 末尾有 `--dtb device/lvmi/S17Promini/prebuilt/dtb.img`，覆盖了 build core 先传入的源码 DTB。现有 out/target/product/S17Promini/recovery.img 确实是 r9 OSS kernel + 原厂 DTB，并带 `_mt_cpufreq_pdrv_init` blacklist；不能当作最新 cpufreq 修复版。其 SHA256 为 d52aa9b5d84523112265c63a7dae2cc8b9076f30fda86dafbee3559ddcb0ce68。这只说明主机现有产物，当前没有 ADB 设备，未确认手机安装了哪个 recovery。

## 修改

- 将 recovery 专用 mkbootimg 参数移到公共 BoardConfig，使 ROM 产品和独立 recovery 产品都显式使用独立参数；保留源码生成 recovery ramdisk。
- 去掉显式 prebuilt DTB 参数。build core 通过 INSTALLED_DTBIMAGE_TARGET 传入由 source-kernel-dtb.mk 包装的源码 S17Promini.dtb，并依赖该产物。
- 保留原厂 recovery_dtbo overlay、加载地址、header v2、page size 和 OS/SPL 参数。原厂 DT table 文件仅作为包装格式模板，不再作为 recovery 的实际基础 DTB。
- 两产品默认命令行均无 cpufreq blacklist，当前源 kernel 配置版本为 r11-connectivity，包含此前 cpufreq SRAM 修复、触摸驱动和本轮 Wi-Fi/BT 接入。

内建 WMT detection 和 connfem 会随内核注册；recovery 没有普通系统的 wmt_loader/wmt_launcher 服务，因此不会自动执行普通系统的 Wi-Fi/BT 初始化链。触摸源码驱动保持无开机自动固件编程，手动修复仍须显式请求。实机返回系统后的触摸状态仍需验证。

## 离线检查

GNU make 求值两产品的 BoardConfig，均指向源码 kernel/config/DTB 规则，recovery 参数无覆盖性的 --dtb 或 --ramdisk，默认 cmdline 无 cpufreq blacklist。报告为 extracted/metadata/source-recovery-config-audit.json。

tools/verify_recovery.py 默认改为检查源码 recovery：kernel 与 KERNEL_OBJ/Image.gz 一致、版本匹配、打包 DT table/原始 S17 DTB 一致、cpufreq 启用、触摸配置、源码 recovery ramdisk、overlay 后触摸和 DVFS SRAM 节点，以及原有 ramdisk/ADB/UI/ELF 检查。历史原厂基线需显式 --kernel-mode stock。

验证脚本用已有 r10 编译产物和已有 recovery ramdisk/DTBO 临时组装测试镜像，全部检查通过；测试镜像在临时目录，未交付或刷写。当前旧 r9 recovery 被正确拒绝。selfcheck 报告为 extracted/metadata/source-recovery-verifier-selfcheck.json。脚本语法和 diff 空白检查通过。

未编译新的 r11 镜像；Wi-Fi/BT 代码的 C 编译/链接和最终镜像大小仍待本次构建。未取得 recovery 实机测试结果。

## 构建与首次验收

建议继续同一个 ROM lunch，生成共用 kernel 的三个镜像：

```sh
cd /home/dengxh/android3/lineage
source build/envsetup.sh
lunch lineage_S17Promini-userdebug
m -j8 bootimage vendorimage recoveryimage

cd /home/dengxh/S17Promini_LineageOS
python3 tools/verify_recovery.py
```

若只构建 recovery，可在上述 ROM lunch 执行 m -j8 recoveryimage；独立 lineage_S17Promini_recovery-userdebug 产品也使用同一 OSS kernel/DTB，但其 AVB 和 SELinux/命令行配置不同。本次建议先测试 ROM 产品生成的 recovery。

验证脚本默认要求 r11-connectivity 和 cpufreq 启用。验证通过后使用既有刷写方法更新 recovery；这次系统 Wi-Fi/BT 测试另需配套 boot/vendor。保留旧可用镜像，不需清 data，也不需先执行固件修复操作。

首次进入 recovery：

1. 确认版本为 r11-connectivity、显示正常、触摸和实体键能操作、ADB 可连接。
2. 保存完整日志与频率信息，便于区分 recovery 的空闲状态与调频异常：

```sh
mkdir -p oss-kernel-debug/recovery-r11
adb shell 'uname -a; cat /proc/cmdline' > oss-kernel-debug/recovery-r11/version.txt
adb shell dmesg > oss-kernel-debug/recovery-r11/dmesg.txt
adb shell 'cat /tmp/recovery.log' > oss-kernel-debug/recovery-r11/recovery.log
adb shell 'for p in /sys/devices/system/cpu/cpufreq/policy*; do echo "$p"; cat "$p/affected_cpus" "$p/scaling_governor" "$p/scaling_cur_freq"; done' > oss-kernel-debug/recovery-r11/cpufreq.txt
```

3. 测试菜单中的进入 fastbootd，再返回 recovery，确认显示/ADB/按键。暂不需要用擦除 data 或完整 ROM 重装来验证内核切换。
4. 从 recovery 重启系统，确认能返回、触摸仍正常；随后单独测试系统 Wi-Fi/BT。用户此前报告重启/关机卡死，cpufreq 修复是否同时消除该问题仍需这次实测，不能预先标为通过。

若卡住，保存完整 pstore 并说明卡在进入 recovery、进入 fastbootd 还是返回系统。若 UI 正常而触摸异常，先保存 touch diagnostics/live_version 与 dmesg，不自动写固件。
