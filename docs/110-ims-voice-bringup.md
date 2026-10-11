# 原厂 IMS 接入：语音 VoLTE bring-up

后续状态：权限启动循环修复见 [111](111-ims-privapp-bootloop-fix.md)，CarrierConfig 接入与用户确认注册成功见 [112](112-ims-carrierconfig-missing.md)，用户确认拨号崩溃修复及单独 APK 更新见 [113](113-ims-audio-call-video-provider-crash.md)。下文保留最初接入时的验证范围。

2026-10-11。用户实机确认 com.mediatek.ims 未安装；日志中的 ImsResolver 配置、服务缓存和活动控制器均为空。仅 3G 可呼入，LTE 主叫可回落，LTE 被叫失败。不能据此认定 IMS 缺失是被叫回落失败的唯一原因。

DT 增加 S17ImsService 预置模块、平台签名、特权/运行时权限、sysconfig，以及 Telephony 的 com.mediatek.ims MMTEL 默认包配置。framework 广告 VoLTE；CarrierConfig 仅为当前联通 46001 SIM 开启 carrier_volte_available_bool，其他运营商保持原有配置。mtkIms Binder 服务使用 radio_service 标签。

tools/prepare-ims.py 从原厂 Android 12 ImsService.apk、mediatek-ims-base.jar 和 mediatek-common.jar 生成 vendor/lvmi/S17Promini/ims/ImsService.apk；输入 SHA256 与 40 个私有依赖类清单位于 DT ims/stock-inputs.json。补入 37 个 ims-base 类与三个 operator factory loader 类，不添加 stock boot JAR、不替换 AOSP framework。原厂 APK 使用 android.uid.phone，因此构建时以 ROM 平台证书重签名。

原厂视频初始化会加载尚未接入的独立系统视频 JNI 栈。本版将 isVideoCallOnByPlatform() 门控为 false；框架 ViLTE/VoWiFi 可用性为 false，先验证语音 IMS。保留原厂 IMS 语音/短信实现，未强制绕过运营商 provisioning。

验证：输入哈希、smali 重组、XML 解析、shell 语法和 DT diff --check 通过。构建已生成并安装 S17ImsService.apk，apksigner 验证签名成功。首次构建发现继承 ims.mk 后 LOCAL_PATH 导致权限 XML 路径重复，已改成显式路径并恢复产品 LOCAL_PATH；第二次已越过该错误。完整 m bacon 在用户要求自行构建时停止，未完成 OTA；已确认没有残留 soong/ninja 构建进程。IMS 绑定、注册、VoLTE 通话、短信和 LTE 呼入均待实机验证。

继续构建：在 ~/android3/lineage 执行 source build/envsetup.sh、lunch lineage_S17Promini-userdebug、m -j8 bacon。vendor/ims 预置 APK 已生成，无需再次准备；重新提取时可向 extract-files.sh 传入原厂 system/system 目录作为第二参数。详细接入说明见 DT ims/README.md。
