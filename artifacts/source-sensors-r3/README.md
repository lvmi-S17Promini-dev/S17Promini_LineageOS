# r18 传感器基线

`boot.img`：32 MiB，版本 `4.19.127-s17-bringup-r18-sensors+`。

用户已确认 DevCheck 加速度和接近测试通过。内核包括原厂 MIR3DA 地址回退、revision=7 默认分支，以及接近独立采样。

SHA256：`ae171d8df311f6f41dbba488e6718b9b3a04bbff79dfbfcc5a38c26ef0c4ba3c`。

构建和离线验证通过，见 [移植记录](../../docs/109-mir3da-proximity-port.md)。温区和完整 factory 校准接口未验收。
