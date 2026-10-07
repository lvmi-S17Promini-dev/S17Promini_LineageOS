# 相机键唤醒

用户希望电源键下面的相机键也能唤醒正常 Android 系统。

实机结果：用户确认修正 523 映射后的功能符合预期，标记为验证通过。
未单独记录拔掉 USB 后的深度休眠测试结果。

getevent 枚举显示 mtk-kpd 包含 KEY_CAMERA（Linux 212），但这只能证明
设备声明该键码。实机按电源键下面的相机键实际上报 KEY_NUMERIC_POUND，
其 Linux 编号为 0x20b（523）。原厂 keypad DTBO 的
mediatek,kpd-hw-init-map 在索引 3 映射为 0x20b。
LineageOS 默认 config_deviceHardwareKeys 和 config_deviceHardwareWakeKeys
均为 64，仅包含音量键。PhoneWindowManager 还要求 camera_wake_screen=1，
否则会过滤熄屏时的 CAMERA/FOCUS 唤醒。

设备仓库修改：

- ROM 产品增加设备资源 overlay，将两项硬件键掩码设为 96（相机 32 + 音量 64）。
- 增加 system/usr/keylayout/mtk-kpd.kl，523 映射为 CAMERA WAKE；保留该设备
  声明的 ALT_LEFT、ALT_RIGHT、VOLUME_DOWN、VOLUME_UP，以及 212 的 CAMERA 映射。
- 保持正常的 CAMERA 功能，通过 Lineage 按钮设置控制是否唤醒。

本次修改 Android 设备配置，不修改内核 DTB/DTBO。没有证据证明内核支持
通用 wakeup-source 属性，盲加该属性不足以完成 Android 唤醒策略。

重编 ROM 并刷入后，在设置的按钮页面开启相机键唤醒。当前源码读取这个
选项的默认值为 0，没有可用于设备 overlay 的默认开关资源，本次没有改
公共 framework 或每次开机强制覆盖用户设置。

如需通过 root ADB 开启当前用户的选项：

```powershell
adb shell content insert --uri content://lineagesettings/system --bind name:s:camera_wake_screen --bind value:s:1
```

这条命令不能替代新 ROM 中的硬件能力 overlay。

实机验证：

1. adb shell dumpsys input：确认 mtk-kpd 的 KeyLayoutFile 为新的系统文件；
   厂商/odm 若存在优先级更高的同名或 Vendor/Product 专用映射，需相应调整。
2. adb shell getevent -lt /dev/input/event0：亮屏按相机键，确认 KEY_NUMERIC_POUND。
   getevent 查看内核原始事件，修改 Android keylayout 后仍显示这个名称；
   Android 接收到的功能才会变成 CAMERA。
3. 开启相机唤醒后，短按电源熄屏，再短按相机键，确认亮屏；拔掉 USB 等待
   一分钟后复测，区分仅关闭显示与深度休眠。
4. 确认亮屏相机键功能、电源键及音量键正常。

若开启后仍不唤醒，采集熄屏相机键事件和内核日志；若深度休眠无事件，
需要继续检查 MTK keypad 的 suspend 和 IRQ 唤醒，系统 keylayout 不能补上
内核未启用的硬件唤醒中断。当前用户已确认功能符合预期，无需继续此排查。
