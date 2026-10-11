# OSS flash r1 / r14 kernel

`boot.img`：恢复 S17 原厂 GPIO 闪光灯逻辑，版本 `4.19.127-s17-bringup-r14-flash+`。
完整 bootimage 构建、镜像校验、驱动主机测试通过。
2026-10-11：用户确认闪光灯已修复，r14 已实机验收。

SHA256：`58de7f3564b93d59d09634566ffba78daf2556fe742da22f46b2ef9db8f33f69`。

r13 相机基线保留在 `../source-camera-r2/boot.img`。
修复依据、测试范围与 adb/dmesg 采集方法见 `../../docs/105-flashlight-stock-gpio.md`。
