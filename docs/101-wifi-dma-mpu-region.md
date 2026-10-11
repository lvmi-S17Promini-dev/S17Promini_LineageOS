# wifi-debug-2：Wi-Fi DMA 池使用的 MPU region 与原厂不同

2026-10-11。用户用配置选择修复后的 #23 boot 复测，提供 wifi-debug-2。本轮没有启动内核编译、刷机或提交。

## 已通过与新故障

WMT/BT/Wi-Fi 内建初始化在 7.320722 秒返回 0；wmtdetect、stpwmt、wmtWifi、stpbt 均存在，权限和标签正常。WMT 已进入芯片上电和 soc1_0 的 BT/Wi-Fi/MCU patch 下载路径，上电返回 0。上一轮配置选择和节点回滚故障已解除，仍不代表系统 Wi-Fi 可用。

19.515 秒系统 Wi-Fi HAL 写 wmtWifi 开启 Wi-Fi，进入 wlanProbe。driver-own 获取完成，首条初始化命令发出后，19.538179 秒 EMI MPU 报：

```text
corrupted address is 0xbe002000, in region 31
violation master is MT6765_M7_AXI_MST_CONNSYS_WIFI_PDMA, from domain 0x2
read violation
```

BootAnimation 是中断发生时正在运行的 CPU 任务名；硬件报告的违规 master 是 Wi-Fi PDMA，不据该任务名归因到 GPU。

随后 halRxWaitResponse 超时/Chip No Ack，访问寄存器事件不匹配，MCU 触发 assert。WIFI_RAM_CODE_soc1_0_1_1.bin 能打开，但下载配置/传输失败，最终 wlanProbe 失败、wmtWifi 返回失败，HAL 写驱动控制节点收到 I/O error。

wifi_sigma.cfg/wifi.cfg 缺失走驱动已有默认配置逻辑，早于失败但没有中止 wlanProbe。WMT 的 unspecified wifi_hif 日志也是继续执行的分支。本轮优先处理有明确硬件违规记录的 DMA 内存保护，不跳过固件 ACK 或修改这些旁支来掩盖错误。

## 原厂模块的直接证据

Wi-Fi 预留池由 DT 建立，起始 0xbe000000，长度 0x300000（3 MiB）。当前驱动日志显示 kalSetDrvEmiMpuProtection 使用这个池，DMA 故障地址在范围内。

原厂 vendor/lib/modules/wlan_drv_gen4m.ko 保留符号。使用 AArch64 objdump 直接反汇编 kalSetDrvEmiMpuProtection（.text 0x104），核对构造 emi_region_info_t 的指令：

- 0x168 mov x9,#0x18，即 region 24。
- 0x170/0x174 movk 构造 apc[0]=0x80b6da28，与 region 组成 64 位值后写入 sp+24。
- 0x178/0x180 构造 apc[1]=0x00b6db6d，写入 sp+32。
- 0x198 调用 emi_mpu_set_protection。

现有导入源码在相同函数使用 region 29。按当前 MT6765 的 16-domain SET_ACCESS_PERMISSION 宏计算，权限位与原厂上述两个字完全一致：LOCK、domain 0/2 NO_PROTECTION、其余域 FORBIDDEN。差别是 region 编号，不是需要向所有域开放权限。

用户提供的 Ghidra 路径可用于更深度逆向；本轮这个小函数用直接反汇编和精确 opcode 检查已经充分定位参数，没有启动 Ghidra 分析。报告 extracted/metadata/stock-wifi-dma-mpu-check.json 包含原厂模块 hash、指令、region、权限字和地址范围。

## 修复与验证边界

mt6765/plat_priv.c 的 Wi-Fi 驱动 DMA MPU region 改为原厂的 24，沿用实际分配池的 base/offset/size，保持全部权限字和锁位。新增 region/start/end/apc 日志，便于下一次实机核对。

没有关闭 EMI MPU，没有改 region 31 的全局 DRAM 保护，没有改变其他域权限、DT 保留区、DMA 描述符格式或固件文件。

离线检查精确匹配原厂 7 条 AArch64 指令，验证源码宏 region=24、权限字=0x80b6da28/0x00b6db6d，以及首个故障地址落在目标池内。源码空白检查通过；未运行 C 编译或设备测试。

region 29 与原厂不同且紧邻首次 DMA 违规，支持该修复方向；当前 emi_mpu_set_protection 包装没有检查 SMC 返回值，因此主机侧尚不能证明此前 region 29 设置是否被固件拒绝，也不能提前宣称 region 24 运行时一定生效。

用户增量构建 m -j8 bootimage，只更新 boot 后重启，保存完整日志到 wifi-debug-3。重点核对新的 region=24、start=0xbe000000、end=0xbe2fffff、apc=80b6da28/00b6db6d 日志，Wi-Fi PDMA 违规是否消失、固件传输是否完成、wlan0 是否建立。若 DMA 拒绝解除后出现新的协议错误，再以新的首个失败点处理，不以此次修复假定 Wi-Fi/BT 已验收。

## 实机验收

用户随后确认 Wi-Fi 成功启动，进一步确认扫描、连接、实际联网均正常。Wi-Fi 基本功能修复已通过实机验收，支持将 DMA MPU region 对齐原厂的修复有效。编译和测试由用户执行；未取得此次修复后完整 dmesg，不将 MPU 寄存器/SMC 返回值写成实测读回结果。

用户另外确认蓝牙能搜索设备，蓝牙扫描通过。熄屏/唤醒、休眠恢复、长时间连接稳定性、蓝牙配对与音频尚未反馈，不标记为通过。

用户要求先提交：内核配置选择修复 `71d533d5b48b`，DMA MPU region 修复与验收记录 `b6b2235d5f6b`。这次没有 system/vendor 或 recovery 配置变更。
