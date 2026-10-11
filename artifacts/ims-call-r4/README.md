# IMS 语音拨号崩溃修复 APK

2026-10-11：用户确认本次 IMS 拨号崩溃修复，使用单独 APK 更新方案验证。

S17ImsService-voice-fix.apk 由 vendor 提交 945d16d 的修复 APK 使用 ROM 标准平台证书签名，配套 DT 2d391a6。不是完整 ROM 构建产物。已核对与现有构建 IMS APK 的证书一致、签名与 4 字节对齐有效，DEX/manifest/resources 与修复版 vendor APK 相同；哈希见 validation.json。适用于已刷入当前 IMS/CarrierConfig ROM 的系统。

结束现有通话后，在 PowerShell 中运行：

```powershell
adb install -r .\S17ImsService-voice-fix.apk
adb reboot
```

作为已存在系统 IMS 应用的更新安装，不需 root/remount 或重新刷 ZIP。成功后 pm path com.mediatek.ims 应指向 /data/app 下的更新 APK。不要先卸载 IMS。未来的 ROM 若更新该 APK，需注意已有 /data/app 更新副本是否仍覆盖 ROM 内预置版本；同版本测试结束后可使用 cmd package uninstall-system-updates com.mediatek.ims 回到 ROM 预置版本，再重启。
