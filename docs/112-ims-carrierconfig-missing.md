# IMS 已绑定，但缺少 CarrierConfig 导致框架关闭 IMS

后续实机结果：补齐 CarrierConfig 后，用户确认 IMS 注册成功；随后的拨号崩溃与修复验收见 [113](113-ims-audio-call-video-provider-crash.md)。

2026-10-11，用户采集 volte-debug/ims-r2。原始 UTF-16 文件保留，normalized 下生成 UTF-8 分析副本。

IMS 权限启动循环已不再出现。com.mediatek.ims 进程 1680 完成 ImsApp.onCreate，MtkDynamicImsService 绑定成功；phone-after 的 ImsResolver 显示 isBound=true，两个 slot 的 MMTEL 均为 READY。不是上次的 IMS APK 未安装/没有服务。

registration-logcat:8531–8539 明确记录 com.android.carrierconfig 不存在，CarrierConfigLoader 获取包版本和绑定 default CarrierService 失败。carrier-config 只有 CarrierConfigManager 默认值，默认 app 配置为空；carrier_volte_available_bool=false。registration-logcat:8928 显示 available=false、enabled=true、provisioned=true、voLteFeatureOn=false，8964 执行 reevaluateCapabilities: turnOffIms，8989 向 modem 发出 AT+EIMSCFG=0,0,0,0,1,1。props 中 persist.vendor.mtk.volte.enable=0 与此一致，不能简单当作需要强制 setprop 的独立原因。

原因：ROM 产品继承 common_full_phone，却未包含 AOSP CarrierConfig 模块；之前添加的 CarrierConfig vendor.xml overlay 没有对应应用可打包。DT lineage_S17Promini.mk 将 CarrierConfig 与 Dialer 一起明确列入 PRODUCT_PACKAGES，使用已有 46001 VoLTE overlay。标准 CarrierConfig 模块自动依赖其 READ_PRIVILEGED_PHONE_STATE 权限白名单。

ClassNotFoundException: com.mediatek.imsplugin.ExtensionPluginFactoryImpl 在工厂捕获后走 ExtensionPluginFactoryBase 默认实现，后续 ImsApp.onCreate end 与服务绑定均成功；本次未据这条可回退异常额外修改 APK。CODE_REGISTRATION_ERROR 1000 也不足以认定运营商拒绝注册，目前应先恢复框架启用 IMS 的前置条件。

验证：核对 AOSP CarrierConfig 模块、manifest 服务声明、required 白名单、vendor.xml 的 MCC/MNC 匹配规则与加载顺序；XML、IMS 特权权限检查、DT diff --check 通过。遵照用户自行构建的偏好，没有运行构建。修复后的 CarrierConfig 有效配置、IMS 启用、注册及通话保持 LTE 仍需实机验证。

刷入后先检查 pm path com.android.carrierconfig；dumpsys carrier_config 中 Phone0 的 mConfigFromDefaultApp 应有实际配置且 carrier_volte_available_bool=true。再观察 ImsManager 是否 available=true / voLteFeatureOn=true，以及是否 turnOnIms。若仍未注册，继续采集注册过程，不预先绕过 provisioning 或强制更改持久 vendor 属性。
