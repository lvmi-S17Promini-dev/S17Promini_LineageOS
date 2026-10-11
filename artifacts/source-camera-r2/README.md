# OSS camera r2 / r13 kernel

`boot.img`：S17 专用 GC5025sub 副摄修正，主摄保持 r12 实现。
版本 `4.19.127-s17-bringup-r13-camera+`，完整编译及镜像验证通过，未刷写。

SHA256：`dbde29902a14046d73f02d60816b1e993da814c5569fe6226481baff4f6dc7a6`。

r12 镜像保留在 `../source-camera-r1/boot.img`。需分别复测 camera 0/BACK
和 camera 1/FRONT 的预览、拍照、录像和切换。此前日志实际失败的是前摄。

详见 `docs/104-camera-front-gc5025sub-stock.md`。

2026-10-11：用户确认修复成功，并纠正此前默认打开的是副摄。r13 相机修复已实机验收。
