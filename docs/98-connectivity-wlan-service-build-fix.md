# gen4m QA 头文件缺少 LIST_SEG_MAX

2026-10-11。用户继续构建时，gl_qa_agent.h 的 PARAM_LIST_MODE_STATUS.u4TxStatus 数组报 LIST_SEG_MAX 未定义，多个包含 precomp.h 的目标同时失败。

这份上游 include/config.h 默认 CFG_SUPPORT_QA_TOOL=1。LIST_SEG_MAX=100 和 list_mode_event 等类型来自 wlan_service/glue/osal/include/net_adaption.h；CONFIG_WLAN_SERVICE=1 时 gl_os.h 才包含 agent.h 并引入这条依赖。初始接入遗漏了 Motorola 官方构建说明中的 MTK_WLAN_SERVICE=yes，导致服务源码和头文件路径未选中。BUILD_QA_DBG 控制的调试信息并不是这个依赖的开关，不能靠调整它修复。

已在 connectivity/Makefile 的内建分支 export MTK_WLAN_SERVICE=yes，沿用上游完整类型和实现，没有手工补 LIST_SEG_MAX，也没有关闭 QA 来掩盖依赖。

用真实 scripts/Makefile.build 和 Makefile.lib、当前 auto.conf 求值：gen4m 选择 104 个源码目标，比之前增加 6 个 wlan_service 文件；所有文件存在，CONFIG_WLAN_SERVICE=1 和 CONFIG_TEST_ENGINE_OFFLOAD=1 生效，服务头文件目录已进入 include 路径。按实际目录查到的头文件链为 gl_os.h → agent.h → service_test.h → operation.h → net_adaption.h，末端存在上游 LIST_SEG_MAX 定义。对照关闭服务的旧参数，确实缺少这些对象和 include 路径。

报告为 extracted/metadata/connectivity-wlan-service-fix.json。以上是规则和头文件依赖检查，没有启动 C 编译/链接，也没有修改其他关机/GPU 工作。可以直接增量重跑原构建命令；后续编译错误仍须按具体接口继续处理。
