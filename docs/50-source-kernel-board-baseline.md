# 源码内核 S17Promini 配置与设备树基线

2026-10-09：内核仓库从 `marlon-bringup` 的 `9fd0d5ca5f05` 创建并切换到 `S17Promini-bringup`。

- 新增 `arch/arm64/configs/S17Promini_defconfig`：以原厂完整配置为输入，在当前源码 Kconfig 下归一化并用 savedefconfig 缩减到 453 行。保留原厂工程、屏幕尺寸、GPU DDK、相机字符串及板级功能；启用 DSI 诊断和 SELinux bootparam。
- 新增 `arch/arm64/boot/dts/mediatek/s17promini-stock.dtsi`：由原厂 boot DTB 合并原厂 DTBO entry 0 后反编译得到，保留数字 phandle 和全部板级属性。
- 新增 `S17Promini.dts`：声明机型和 compatible，将 BSP 屏幕名改为 LK 实际识别的 AXS15260。新增工程对应 DTB 构建目标，产物为完整独立 DTB。
- 清理指向已删除 OPPO 目录的 Kconfig source，恢复标准 ext4 入口。初始化已有 MySU 子模块供 Kconfig 解析，S17 配置不启用 MySU。

验证：defconfig、内核 dtbs 目标成功；savedefconfig 再生成与保存版本一致。DTB 二进制属性对照确认 806 个节点路径一致、3652 个属性中仅根节点 model/compatible 和 chosen 屏幕名三项变化，其余字节均与合并后的原厂树一致。反编译树的数字引用及原厂 BSP bindings 仍有 dtc 警告，未为消除警告修改硬件描述。

配置差异和 DTB 核对结果：`extracted/metadata/s17-source-kernel-baseline.json`。构建输出及详细日志位于 `/tmp/s17-kernel-bringup`。内核中 `Documentation/S17Promini-bringup.md` 记录输入哈希、复现命令、配置前置修复和驱动缺口。

尚未完整编译内核、生成 boot 或实机启动。缺失 AXS15260、AXS 触摸、PSC5425E、IMX278 及原厂 MIR3DA/GSENSOR_PROXIMITY 对应实现，原厂专用电池 profile 符号也不存在；WUSB3801X 已有源码，但绑定和全局节点名查找与原厂不同，暂不启用。当前字符串配置保留原厂名称，不代表这些驱动已存在。下一步先逆向 AXS15260 及接入 LCM 驱动列表，相机后置。

完整 DTB 已包含原厂 overlay 内容，后续打包/启动需要避免重复应用同一 overlay。当前正式 ROM/recovery 继续使用已验证原厂内核，本轮未更改设备树仓库的 BoardConfig，未刷写设备。
