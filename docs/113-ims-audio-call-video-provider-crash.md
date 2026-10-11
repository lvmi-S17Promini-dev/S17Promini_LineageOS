# VoLTE 语音拨号仍创建视频 Provider，导致 IMS JNI 崩溃

2026-10-11 实机更新：提供单独签名 APK 更新方案后，用户确认本次故障修复。记录范围为此次 IMS 拨号崩溃与对方持续响铃问题；其他通话音频、IMS 短信及完整 ZIP 安装路径按各自测试结果记录。

2026-10-11，用户确认 IMS 注册成功，但拨出时崩溃、对方持续响铃。日志位于 volte-debug/ims-r3，原件未修改，normalized 保存 UTF-8 副本。

volte-call-crash:13293–13302：18:21:20.520 com.mediatek.ims 的 ProviderHandlerThread 因 UnsatisfiedLinkError 退出；ImsVTProviderUtil.bindInternal 调用 ImsVTProvider.nInitialization(int,int)，JNI 实现未加载。后续 18:21:31、18:23:31、18:23:46 重复同类崩溃。拨出 profile 的 callType=2 为普通语音，崩溃前却出现 recordAdd、VTSource 初始化。框架随后 hangupAllOngoingConnections cause 14；IMS Binder 已死，框架本地清理不等价于 modem/network 会话完成释放。持续响铃符合该会话失联现象，但是否还有独立挂断故障需修复后复测。

初次 voice-only 补丁只将 ImsVTProviderUtil.isVideoCallOnByPlatform() 置 false，跳过 JNI 库和全局视频初始化；ImsCallSessionProxy 构造函数另读 persist.vendor.vilte_support=1，不看该开关，即使语音拨号也创建、绑定 VTProvider。这是原视频门控不完整造成的用户空间故障。

DT tools/prepare-ims.py 将 ImsCallSessionProxy 构造函数和 updateOutgoingVideoRingtone 的两个直接属性判定改为调用同一个 isVideoCallOnByPlatform()。视频禁用时跳过视频 Provider 创建、bind 与视频铃声路径，不修改语音 start/accept/reject/terminate 实现，不伪造 JNI 返回值、不引入视频 native 库。按固定原厂输入重生成 vendor/ims/ImsService.apk，provenance.json 记录三个视频门控补丁与新 APK 哈希。

验证：smali 重组成功；重新反汇编生成 APK，检查统一门控恒 false、构造函数条件跳转覆盖两个 makeImsVtProvider 和 bind 调用；原厂与生成 APK 的 start、accept、reject、terminate、getVideoCallProvider 方法逐方法相同，既有 getter 可返回 null。APK provenance 哈希、全部五项平台 privileged 权限与两仓库 diff --check 通过。按用户偏好未构建完整 ZIP，实机稳定拨号、接听、音频和挂断仍待验证。
