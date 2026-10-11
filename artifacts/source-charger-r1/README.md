# PSC5425E r1 / r15 kernel

`boot.img`：PSC5425E 驱动及充电初始化修复，版本 `4.19.127-s17-bringup-r15-charger+`。
bootimage 编译、离线镜像验证、原厂 Ghidra 核对和主机回调测试通过。
用户确认实机 CPU 占用下降、深度休眠恢复；充电和 OTG 尚未专项验收。

SHA256：`24ae1eac72a597901d1c7e7ec09d1a04e7f71ddcf1345155cfac94f5a1b70a06`。

内核提交：`d997bb2c0f5e`、`40d6cd858f18`。
输入电流有限档最高 800 mA，大电流充电可能比原厂慢。
详细记录见 `../../docs/107-psc5425e-port.md`。
