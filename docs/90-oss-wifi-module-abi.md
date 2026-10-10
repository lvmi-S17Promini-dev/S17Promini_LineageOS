# OSS Wi-Fi：原厂连接模块在加载阶段被拒绝

2026-10-10。本轮只做现有日志、构建配置和模块的离线诊断；未编译、刷机或修改内核加载器。

## 直接证据

`oss-kernel-debug/touch-working-1/kmsg.log` 的 r9 工作触摸基线在约 11.70 秒记录：

```text
wmt_drv: disagrees about version of symbol module_layout
finit_module for "/vendor/lib/modules/wmt_drv.ko" failed: Exec format error
```

同次启动 connfem、BT 和 fpsgo 也被 module_layout 校验拒绝。touch-log-4 的 dmesg 同样有 WMT 失败。cache-log-7/logcat.txt 中 wmt_loader 反复打不开 `/dev/wmtdetect`，wmt_launcher 反复读取 `vendor.connsys.driver.ready=no`。

vendor `init.wmt_drv.rc` 在 boot 加载 wmt_drv；`init.wlan_drv.rc` 要等 `vendor.connsys.driver.ready=yes` 才加载 wmt_chrdev_wifi 和 wlan_drv_gen4m。因此当前阻塞链为 WMT 加载失败 → 检测节点不可用 → ready 未置 yes → Wi-Fi 模块加载条件不满足。日志已支持加载阶段故障，尚不能判断通过这一阶段之后芯片上电、固件和 HAL 是否正常。

这些实机日志是 r9 及更早版本，尚未取得 cpufreq 修复后 r10 的 Wi-Fi 日志。以下静态比较使用当前 r10 构建目录的 Module.symvers；不是声称取得了 r10 的实机加载结果。

## 离线 ABI 比较

原厂模块 vermagic 为 `4.19.191 SMP preempt mod_unload modversions aarch64`。当前内核为 `4.19.127-s17-bringup-r10-cpufreq+`，CONFIG_MODVERSIONS=y。

`kernel/module.c` 的 check_modstruct_version 在模块布局分配前调用 check_version；CRC 不一致返回 -ENOEXEC，对应现有日志。带 CRC 的模块在 same_magic 中忽略内核版本号部分，所以单纯修改版本号不能解决此错误。

module_layout 原厂要求 `0x3c6e983b`，当前内核导出 `0xd1cddb94`。用 libkmod 读取全部模块版本和导出表后，与当前 Module.symvers 比较：

| 模块 | 导入数 | 内核 CRC 匹配 | 内核 CRC 不匹配 | 无候选导出 | vendor 模块候选提供的导入 |
| --- | ---: | ---: | ---: | ---: | ---: |
| connfem.ko | 51 | 18 | 33 | 0 | 0 |
| wmt_drv.ko | 244 | 149 | 94 | 1 | 0 |
| wmt_chrdev_wifi.ko | 48 | 28 | 13 | 0 | 7 |
| wlan_drv_gen4m.ko | 303 | 153 | 128 | 0 | 22 |
| bt_drv_connac1x.ko | 102 | 53 | 30 | 0 | 19 |

CRC 差异包括设备树、pinctrl、网络、cfg80211 等接口；WMT 还缺 `connectivity_export_clk_buf_show_status_info`。连接适配层只有 clk_buf_ctrl 的导出包装，当前对应导出不在 Module.symvers 中。缺接口是静态发现的后续问题，实机加载器目前先在 module_layout 拒绝。

CRC 不同不证明每个函数的调用 ABI 必然不同，但说明构建类型描述并不一致；CRC 相同也不能证明二进制完全兼容。报告中的 vendor 提供者是候选，不代表其已加载成功。

完整报告：`extracted/metadata/wifi-module-abi-audit.json`。复现：

```sh
python3 tools/audit_vendor_module_abi.py \
  --symvers /home/dengxh/android3/lineage/out/target/product/S17Promini/obj/KERNEL_OBJ/Module.symvers \
  --modules /home/dengxh/android3/lineage/vendor/lvmi/S17Promini/proprietary/vendor/lib/modules \
  --output extracted/metadata/wifi-module-abi-audit.json
```

脚本只读取 ELF/版本表，不加载模块，依赖主机 modprobe/modinfo。本轮已完整执行，检查了目录内全部九个模块。

## 修复所需输入与路径

当前内核 connectivity 目录只有 connadp 的适配层。Makefile 明确写主驱动已移至 standalone repo，并引用 common、wlan/adaptor、wlan/core/gen4m；当前 Android 树没有 `vendor/mediatek/kernel_modules`。设置 CONFIG_MTK_COMBO_WIFI=y 并没有提供 Wi-Fi 主驱动实现。

下一步补齐匹配 MT6765／CONNAC 的 WMT、Wi-Fi adaptor、gen4m 和 connfem 源码，与当前内核配置、头文件、Module.symvers 配套构建并打包。WMT 的导出接口需核对源码版本和平台适配。仅重建 boot 不会替换 vendor 内的原厂 .ko；模块交付需要同步更新 vendor 或提供明确的测试安装路径。暂未接入未经验证的外部驱动，也没有强行删除 CRC 校验或修改原厂模块 CRC。

目前无需为了定位这次加载错误补抓日志。若另有连接驱动源码，提供所在目录或仓库即可；否则需要寻找适配该平台的独立源码。模块配套后再取完整 dmesg、logcat、`/proc/modules`、连接属性及 `/dev/wmtdetect`／`/dev/wmtWifi` 状态，验证加载、上电、固件、wlan0 和 HAL。若要先确认 r10 仍为同一故障，保存其完整开机 dmesg 和开关 Wi-Fi 时的 logcat；无需反复刷旧 recovery。
