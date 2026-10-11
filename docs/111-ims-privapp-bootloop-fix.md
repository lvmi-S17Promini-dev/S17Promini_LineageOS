# IMS 权限白名单遗漏导致 system_server 启动循环

2026-10-11。用户刷入完整 IMS ZIP 后无法进入系统，提供 volte-debug/bootloop-logcat.txt。原始日志为 UTF-16 LE，normalized 下保留 UTF-8 副本，原件未修改。

日志首个 fatal exception 已明确定位：PermissionManagerService.systemReady 抛出 IllegalStateException，com.mediatek.ims 在 /system/priv-app/S17ImsService 请求 android.permission.PACKET_KEEPALIVE_OFFLOAD，但该项不在 privapp-permissions 白名单。后续多次 system_server 重启均重复同一错误。这是初次 IMS 接入时遗漏权限造成的启动故障，不能归因于内核或 modem。

DT ims/privapp-permissions-s17-ims.xml 补齐该权限。新增 tools/check-ims-permissions.py，读取 APK 的 uses-permission 和平台 AndroidManifest.xml 中的 privileged 权限定义，核对该包的白名单；Android.mk 将检查 stamp 作为 IMS 模块的附加依赖，缺项时在打包前失败。

验证：修复后的白名单覆盖该 APK 请求的全部五项平台 privileged 权限；临时移除 PACKET_KEEPALIVE_OFFLOAD 后，检查脚本非零退出并指出相同缺项。XML、脚本语法和 diff --check 验证通过。按用户偏好未启动构建，完整 OTA 重建与启动验证由用户执行；IMS 注册和实际 VoLTE 通话仍待验证。

日志另外记录 HealthInterfaceService 的充电 HAL 空引用 BOOT FAILURE，后续流程仍运行并最终因上述权限检查 abort；本次未将这条独立错误当作 IMS 启动循环的根因，也未顺带修改充电服务。
