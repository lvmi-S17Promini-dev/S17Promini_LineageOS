# r11：内建 MT6765 WMT、Wi-Fi 与 BT

2026-10-11。用户授权接入连接驱动并同意内建，随后明确加入 BT、不需要 GPS/FM。已修改内核和 vendor 源码；编译由用户执行，本轮没有启动内核/Android 编译、打包或刷机。

## 实现

内核 `drivers/misc/mediatek/connectivity/` 导入 Motorola 官方 WMT、Wi-Fi adaptor、gen4m 和 BT（同一 MMI-S3SG32.39-191R2 标签），connfem 使用 Android 12 SST 分支。完整 commit/目录/变更说明见内核中的 `UPSTREAM.md`。connfem 与其他组件并非同一发布标签，其消费者接口已静态检查，尚未完成编译与实机兼容验证。

原有 connadp 平台桥接层保留。新 CONFIG_WLAN_DRV_BUILD_IN 只在 MT6765、COMBO/WIFI/BT 都为 y 时可选；S17 默认开启。顶层直接接入 common、connfem、wlan/adaptor、wlan/core/gen4m、bt/mt66xx/wmt，无外部 vendor/mediatek 源码依赖或构建时创建符号链接。

参数为 AXI、CONNAC、WLAN_CHIP_ID=6765、WIFI_IP_SET=1、MTK_ANDROID_WMT=y、MTK_ANDROID_EMI=y、CONNAC_VER=1_0、BT_PLATFORM=connac1x。驱动目标分别为 wmt_drv.o、connfem.o、wmt_chrdev_wifi.o、wlan_drv_gen4m.o、bt_drv_connac1x.o，均为 obj-y。修正了 gen4m 的重复 obj-y/obj-m 规则，设置 CFG_BUILT_IN_DRIVER=1，保留现有 adapter 与驱动间的桥接实现。

WMT detection 在内核 initcall 注册 `/dev/wmtdetect`；connfem 自行注册。保留原厂 wmt_loader/wmt_launcher：loader 在 vendor 挂载后通过 ioctl 完成硬件准备和 WMT → BT → Wi-Fi 初始化，成功后发布 ready 属性。Wi-Fi 字符设备和主驱动使用已有内建入口，不提前在内核启动时加载 firmware。现有 firmware loader 已搜索 `/vendor/firmware`；S17 的 WIFI_RAM_CODE_soc1_0_1_1.bin 与选中 CONNAC 分支的命名匹配。

初始化串行执行并保存真实返回值，防止重复 ioctl 把首次失败误当成功、或者重复注册。WMT 失败不继续 BT；BT 失败不继续 Wi-Fi；重复失败需保留日志并重启修复后的内核，不能通过再次调用伪造成功。关闭内建 Wi-Fi/BT 的 weak 空实现回退，使缺入口在链接时暴露。GPS/FM 不在调用链或构建对象中，其 S17 配置关闭；不据此判断设备物理上是否存在 GPS/FM。

显式设置 CFG_MOT_REM_CH12_CH13=0，关闭 Motorola 对 channel 12/13 的额外排除，沿用驱动原有监管域/NVRAM 路径。最初直接移除宏定义，后续构建在 p2p_func.c 的数字 #elif 触发 -Wundef；已改为明确定义 0。没有开启额外 NAN、CONNINFRA 或连接 power_throttling 驱动。

vendor 的普通、factory 和 meta init 中去除 WMT、connfem、Wi-Fi、BT、GPS、FM 的 insmod 命令。保留 HAL、固件、loader、launcher、wlan_assistant 启动以及 BT proc 节点权限处理。原厂 .ko 文件仍作为未使用的原始 blob 保留，启动流程不再加载它们；本轮没有改 vendor 文件清单/元数据或删除二进制。

内核导入中删去 Android.mk 外部模块配方，避免 Android 扫描后重复注册这些包。其余上游版权/许可证保留。测试内核版本为 `4.19.127-s17-bringup-r11-connectivity`；cpufreq 修复和默认启用状态保留。

## 本轮验证和边界

后续构建发现 adaptor 的 MODULE_NAME 赋值顺序错误，原始直接 include 的 Makefile 检查因延迟展开而漏检；现已修复并使用真实 Makefile.build/Makefile.lib 复查，详见 [97](97-connectivity-adaptor-module-name-order.md)。下面的原始规则求值不作为赋值时序正确的证据。

随后 gen4m 编译暴露遗漏 MTK_WLAN_SERVICE=yes 的头文件依赖，现已按官方参数补齐，详见 [98](98-connectivity-wlan-service-build-fix.md)。当前 gen4m 选择 104 个源码目标，较下面最初记录增加 6 个服务对象。

继续构建至 vmlinux 链接后发现 WMT 与 gen4m 都定义了 mmi_get_bootarg。已将只在 wmt_conf.c 内使用的 WMT 版本设为 static；保留 Wi-Fi 全局实现供 wlan_lib.c 调用，两份 bootargs 缓冲区不共享。只读扫描用户已编译的 170 个连接驱动叶对象，重复强全局符号仅这一项；报告 extracted/metadata/connectivity-builtin-symbol-audit.json 记录的是重新编译修复前的对象状态。本轮未启动编译/链接。

- 用现有 Kconfig conf 工具在临时目录解析 defconfig + boot_test 配置，CONFIG_WLAN_DRV_BUILD_IN、COMBO_BT、COMBO_WIFI 都为 y，GPS/FM 关闭；输出保存为 extracted/metadata/connectivity-r11-resolved.config。没有修改现有 KERNEL_OBJ 配置或编译内核。覆盖 CUSTOM_KERNEL_IMGSENSOR/LOG_BUF_SHIFT 的两条提示来自正常片段合并。
- GNU make 只求值真实顶层和子目录规则，五个目标均无 obj-m，所选 162 个驱动 C 文件均存在，FM/GPS helper 未选中。报告为 extracted/metadata/connectivity-r11-kbuild-audit.json。现有 connadp 的两个 C 文件不计入这个数。
- 10 个修改过的 vendor init 文件通过现有 host_init_verifier 的单文件语法检查。
- 内核/vendor/workspace diff 空白检查通过。

用户随后确认上述构建问题已修复，要求提交。编译由用户执行，本助手没有启动编译；没有收到完整构建报告或最终镜像大小/验证结果，不据此宣称全部镜像通过。尚未收到 Wi-Fi/BT 实机验收：上电、固件协议、MAC/NVRAM、扫描连接、蓝牙 HAL、配对、音频和休眠恢复仍待确认。历史 r9 的原厂模块加载失败不能当作 r11 运行结果。

提交：内核 `d498ec5e9666`（内建驱动及所有构建修复），vendor `87b924b`（停止加载原厂连接模块）；配套 recovery DTB 配置为 DT `0041d99`。GPU 与重启/关机诊断改动未纳入这组提交。

## 用户构建与测试

```sh
cd /home/dengxh/android3/lineage
source build/envsetup.sh
lunch lineage_S17Promini-userdebug
m -j8 bootimage vendorimage
```

本次需同时交付 boot 和 vendor：boot 包含内建驱动，vendor 更新 init 加载流程。无 system 源码变化，无需为了这次驱动接入单独重建 system。若使用标准全量包则按现有 bacon 流程构建。先保留当前可用 r10 boot/vendor，使用既有刷写方法，不需清 data。

若编译失败，提供第一处报错及其上下文或完整构建日志；不要删掉 -Werror 或伪造接口来掩盖错误。

启动后保存完整 dmesg 和开关 Wi-Fi/蓝牙时的 logcat，核对 `/proc/version`、`getprop vendor.connsys.driver.ready`、`ls -l /dev/wmtdetect /dev/wmtWifi /dev/stpbt`、`ip link`。内建驱动不应以 .ko 出现在 `/proc/modules`，空模块列表不能判断内建初始化失败。关键日志为 built-in mode、S17 WMT/BT/Wi-Fi init finished 的 ret、CONSYS 硬件版本和 firmware 下载结果。

先测 Wi-Fi 开关、扫描、连接及 BT 开关、扫描、配对，再测熄屏/唤醒和 BT 音频。若启动卡住则保存完整 pstore；无需进入旧 prebuilt recovery 反复测试触摸。
