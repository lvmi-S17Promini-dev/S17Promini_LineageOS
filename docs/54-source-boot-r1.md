# 首版完整源码内核测试 boot

2026-10-10，LineageOS 19.1 `m -j8 bootimage` 完整构建通过。

测试镜像：`artifacts/source-boot-r1/boot-source-r1.img`，33554432 字节。
SHA256：`7ad0c6aeda51561a093bb1aed824faebe1cfea4e4e38fd0932347c53570efb24`。

回退镜像：`artifacts/source-boot-r1/boot-stock-rollback.img`，复制本轮前普通 Lineage 输出中的 stock-kernel boot。
SHA256：`3e4748cc7671ccf15bac2813f293fde63eecc5177cffef2f07062828f38590b9`。
回退镜像解包后 kernel 与原厂导出 kernel 相同，ramdisk 与设备树中的已验证 stock first-stage ramdisk 相同。

## 实际构建配置

使用 `kernel/lvmi/mt6765` 的 `S17Promini-bringup` 分支，基础提交 `60faf9d34423`，叠加本轮构建修复。
使用 **S17Promini_defconfig + S17Promini_boot_test.config**，没有沿用 OPPO/marlon defconfig。

生成配置明确包含 r65v3_pmz_w1a、AXS15260 LCM、AXS 基本触摸及 PowerVR m1.11ED5425693。
测试 fragment 仅把缺失 IMX278 的相机列表缩减为已有 GC5025sub，增加 `-s17-bringup-r1` release 标识及 2 MiB 日志缓冲。

Lineage 输出目录为独立的绝对路径 `~/android3/lineage/out-s17-sourceboot`，普通 `out/` 的已验证 boot 未覆盖。
设备树 BoardConfig 新增仅 ROM 产品使用的 `S17_SOURCE_KERNEL_BOOT=true` 开关；普通构建默认继续使用 stock kernel/DTB。

```sh
cd ~/android3/lineage
export OUT_DIR="$PWD/out-s17-sourceboot"
export S17_SOURCE_KERNEL_BOOT=true
source build/envsetup.sh
lunch lineage_S17Promini-userdebug
m -j8 bootimage
```

非默认 OUT_DIR 必须使用绝对路径：当前 Lineage kernel builder 对自定义相对输出目录不会补 BUILD_TOP 前缀。
测试只使用 bootimage 目标；普通构建在新的 shell 中运行，或先 unset 上述两个变量。

## 编译错误的来源与修复

`oppo_ion` 与 defconfig 无关。原 Makefile 写成 `#ifdef VENDOR_EDIT` / `obj-y += oppo_ion/` / `#endif`；GNU make 把两条 # 指令当成注释，因此它无条件进入构建。此前仅验证了 LCM/触摸局部编译，没有证明完整树已清理这些入口。

本轮处理：

- 移除缺失的 OPPO audio 和 oppo_ion 无条件构建入口；MTK ION 仍启用。
- 移除指向缺失 vendor/oplus 目录的 12 个 include 符号链接，解除 Soong kernel header glob 阻塞。
- vDSO 使用配置好的交叉 LD，避免 Clang 调用 Android PATH 禁止的系统 linker。
- TCPC 的警告选项按编译器支持即时求值，兼容 Clang 12 及 modules.builtin 阶段。
- USB PHY 的通用函数前置声明移出 OPPO 专用宏，修正关闭 OPPO 功能时的声明错误。
- 去掉 OPPO tempntc 专用构建和工程老化模式依赖；BTS/BTSMDPA 使用现有 IIO 通道读数。标准 MTK 温控及过温 reset/poweroff 保持启用，没有用固定温度或空函数代替传感器。
- 新 BPF backport 的 vmlinux BTF 解析补上 DEBUG_INFO_BTF 条件，支持原厂配置关闭 BTF 的情况。

当前配置的剩余缺失 vendor 路径入口已核查，没有仍然命中缺失目录的无条件 obj-y 项。未启用的可选 vendor 链接仍存在。

## 内容验证

- `Linux version 4.19.127-s17-bringup-r1+`，确认为本轮源码编译的 Image.gz。
- Boot header v2，page size 2048，kernel/ramdisk/DTB 地址分别为 0x40080000 / 0x51b00000 / 0x47880000。
- 原厂 first-stage ramdisk 逐字节保持一致，cmdline 为 `bootopt=64S3,32N2,64N2 buildvariant=user`。
- 使用源码生成 S17Promini.dtb，按原厂单条目 MTK DT table 格式封装；没有追加 DTB 到 gzip kernel。
- AVB NONE metadata/footer 和 boot SHA256 descriptor 校验通过，镜像为 boot 分区规定的 32 MiB。
- 静态叠加原厂 DTBO 后，新 0x3b 触摸节点仍保留。LK 仍可能写入动态 chosen/memory 属性；此检查不代替实机启动。

报告：`extracted/metadata/source-boot-r1-validation.json`；header：`source-boot-r1-header.txt`。
复现校验：`python3 tools/verify_source_boot.py --image artifacts/source-boot-r1/boot-source-r1.img`。
构建日志位于 `extracted/metadata/source-boot-r1-lineage-build.log`。

## 测试范围

尚未刷机。先测启动、显示、背光、触摸、息屏唤醒及 ADB，记录版本日志和 dmesg/pstore。
PSC5425E 充电、IMX278 主摄、MIR3DA 等仍待适配；新内核版本/CRC 与原厂无线模块不同，模块加载和无线功能也未验证。
这份 boot 不携带新 vendor 模块，不适合作为完整功能 ROM 验收。

只更新 boot，保留当前 recovery、vendor、dtbo 和 vbmeta。当前设备沿用此前已验证的 unlocked/verification-disabled 配置。

```sh
fastboot flash boot boot-source-r1.img
fastboot reboot
```

需要回退时刷回 `boot-stock-rollback.img`。此次没有通过工具执行任何设备刷写。

后续 GPU 选型更正：cache-log-4 的用户态 libsrv_um.so 明确为 1.13@5776728，当前配置已切换精确匹配驱动。本篇记录的 1.11 选择属于历史测试状态，参见 docs/69-cache-log-4-gpu-ddk-selection.md。
