# 原厂电量估算参数提取（2026-10-12）

用户反馈：设置 → 电池使用情况 → 自上次充满电以来的用量，长期显示没有电池使用数据；prebuilt 与源码内核都有此现象。

## 提取结果

从已有原厂 ROM 解包目录扫描 system framework 与 product/vendor 的全部 overlay APK，找到三份正式 `res/xml/power_profile.xml`：

| 来源 APK（相对 extracted/filesystems） | 解码文件（相对 extracted/metadata/stock-power-profiles） |
| --- | --- |
| system/system/framework/framework-res.apk | system-framework-res.xml |
| vendor/overlay/FrameworkResOverlay/FrameworkResOverlay.apk | vendor-FrameworkResOverlay.xml |
| product/overlay/framework-res__auto_generated_rro_product.apk | product-framework-res__auto_generated_rro_product.xml |

三份均是 legacy 屏幕字段：`ambient.on=0.1`、`screen.on=0.1`、`screen.full=0.1`，没有 `.display0` 字段。system 的 `power_profile_test.xml` 是测试资源，不作为正式参数提取。

vendor 配置有 4+4 核、battery.capacity=2500，但 cpu.core_speeds 使用 300/1000/2000 MHz 和 300/1000/2500/3000 MHz；cpu.core_power 为 10/20/30 和 25/35/50/60，cpu.idle=1.11、cpu.active=2.55。这些 CPU 参数与原厂 framework 的 power_profile_test 中对应参数一致，明显不能据此认定为本机实测值。

product 配置仍为单核、400 MHz、battery.capacity=1000。system 配置也是默认占位表。当前没有从这些资源中找到可直接用于修复的实际屏幕功耗值。

## 复现

```bash
python3 tools/extract_stock_power_profiles.py
```

默认使用 `~/android3/lineage/out/host/linux-x86/bin/aapt2`，可通过 `--aapt2` 指定路径。脚本保存可读 XML、aapt2 原始树，以及 sources.json 中的 APK、二进制 XML、解码 XML SHA256。

可读 XML 还原元素、属性、数值及数组顺序；编译资源不含原始注释，无法恢复原始源码的格式。vendor/product 中原有的根节点杂文本 `. ` 也保留。没有修改来源 APK 或设备树。

## 与 LineageOS 19.1 页面条件的关系

本地 `packages/apps/Settings/src/com/android/settings/fuelgauge/BatteryAppListPreferenceController.java` 的 `shouldShowBatteryAttributionList()` 检查 `screen.full.display0 >= 10 mA`；不成立就跳过添加耗电列表，随后显示 `power_usage_not_available`。

本地已构建的 `out/target/product/S17Promini/system/framework/framework-res.apk` 中 `screen.full.display0=0.1`，低于该阈值。现有采样日志有 UID 活动，但当次无百分比掉电，也会导致条目被过滤；最终验证需要实际放电。

PowerProfile.initDisplays() 支持旧屏幕字段的兼容映射。因此三份原厂表的 0.1 并不能绕过页面阈值。原厂表直接复制到设备 overlay 不能解决此问题。

后续应收集适配本机的功耗参数，并核对实际 CPU cluster 编号和频率表。如果仅作页面诊断，可临时使用满足阈值的屏幕参数，但必须标明临时估算，不能作为准确耗电统计。

设备仍接入原厂 prebuilt vendor，blob 清单包含 FrameworkResOverlay 与 FrameworkResOverlayExt；引入 system 的新 power_profile 时，应检查运行时 `cmd overlay list` 和实际资源解析结果，确认 vendor RRO 是否覆盖新表。仅查看 framework-res.apk 内容不足以证明运行时最终值。

本轮完成提取和 XML/数值完整性检查，未刷机，未确认运行时 RRO 优先级。
