# wifi-debug-1：空 Motorola 表尾项匹配导致 WMT 初始化回滚

2026-10-11。用户先刷 boot/system，补刷 vendor 后仍打不开 /dev/stpwmt，随后提供 oss-kernel-debug/wifi-debug-1。本轮读取日志并修改内核源码，未启动编译、刷机或提交。

实机为 4.19.127-s17-bringup-r11-connectivity+ #22，2026-10-11 09:16:17 构建。/dev/wmtdetect 为 system:system 0660，存在正确标签；/dev/stpwmt、/dev/wmtWifi、/dev/stpbt 不存在。vendor.connsys.driver.ready=yes 不能作为初始化成功证据，完整 dmesg 明确返回失败。PowerShell 输出为 UTF-16，读取时解码，原始文件不改编码。

## 日志中的失败链

- 0.923 秒 WMT detection 注册成功。
- 7.280 秒 wmt_loader 打开 wmtdetect；硬件准备、EMI 映射、芯片 ID 0x6765 等继续执行。
- 7.295 秒进入 built-in mode，调用内建初始化链。
- 7.296561 秒 WMT major 190 注册；随后设备节点创建流程进入配置读取。
- 7.296891 秒选择 WMT_SOC_.cfg。
- 7.298094 秒 request_firmware 返回 -2（ENOENT）；wmt_conf_read_file 和 wmt_lib_init 失败。
- WMT_init 错误路径清理字符设备/节点，初始化链返回 -1，BT/Wi-Fi 没有继续初始化。
- 此后 wmt_launcher 循环打不开 /dev/stpwmt。

因此当前直接故障是配置文件名错误，launcher 错误是初始化回滚的结果。不是 wmtdetect 权限不足，也不能靠 vendor 属性 yes 判断成功。补刷 vendor 已由用户执行，本轮不再把该故障归为遗漏 vendor 更新。

## 原因与修复

导入的 Motorola WMT 配置选择器遍历 products_list，包括最后一个全空 terminator。S17 没有 Motorola 的 device/radio/revision bootargs，局部匹配字符串均为空，于是错误匹配该项，生成 WMT_SOC_.cfg。vendor 实际存在 WMT_SOC.cfg；调用方已有 CUST_CFG_WMT_SOC 默认路径。

Wi-Fi mot_config.c 也包含全空表尾项；缺失 Motorola bootargs 时会生成 .cfg，影响 wifi/txpowerctrl 配置路径，虽然调用方还可能继续回退。本轮两处循环均在遇到空 hw_device 时停止，保留调用方默认配置逻辑。没有新增伪造配置、重命名 stock 文件或共享 Motorola 其他机型参数。

仅修改：

- connectivity/common/common_main/core/wmt_conf.c。
- connectivity/wlan/core/gen4m/common/mot_config.c。

离线 Python 模型从实际源表提取 15 个 Motorola 产品条目，复现修复前 WMT_SOC_.cfg/.cfg，验证修复后空/未知设备走默认、现有 Motorola 条目保持不变；另外检查实际 C terminator guard、CUST_CFG_WMT_SOC 宏和 vendor 文件存在。报告为 extracted/metadata/wifi-debug-1-config-selector-check.json。这不是内核 C 编译或芯片实测，空白检查通过。

## 下一次测试

用户增量执行 m -j8 bootimage，使用已有方法只更新 boot 即可；这次没有 system/vendor 改动。更新内核后重启，再抓完整 dmesg 和节点状态，保存到 wifi-debug-2。当前初始化失败结果会被缓存，仅重启 wmt_loader 不能替代更新内核并重启。

应不再读取 WMT_SOC_.cfg；关注默认 WMT_SOC.cfg 读取、S17 WMT/BT/Wi-Fi init finished 的 ret，以及 stpwmt/wmtWifi/stpbt 节点是否保留。之后才验证芯片上电、Wi-Fi 固件、wlan0、扫描连接和 BT。本轮只修复日志已证实的这一阶段，不预先标记 Wi-Fi/BT 可用。

后续 wifi-debug-2 确认初始化 ret=0、四个节点都存在，本阶段修复通过；新的 DMA MPU 故障及最终 Wi-Fi/蓝牙扫描验收见 [101](101-wifi-dma-mpu-region.md)。
