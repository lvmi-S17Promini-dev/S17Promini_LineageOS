# probe/运行时复位未恢复触摸：固件启动路径继续核对

用户刷入带probe复位的boot并执行reset_touch=120：reset_requests2、last_reset_result0，IRQ无暂停，实时版本0000。没有读报文时last_read/last_decode=-61为初始化ENODATA，并非本次I2C传输失败。复位与额外等待单独不足以恢复触摸，不能继续只调整延时。

在此前未充分检索的lineage-debug/cache/recovery/last_kmsg.5、.6发现原厂4.19.191内核的另两次恢复会话：probe版本0000，升级线程ffff、host7，随后分别4.437/4.485秒记录upgrade success,new fw version07。这些会话本身是recovery，日志也有recovery进程，不能称作正常LineageOS触摸成功的运行日志，更不能据此认定recovery触摸成功。它们说明“原厂升级总失败”不成立。

独立地，用户确认LineageOS+prebuilt触摸正常。因此完整固件处理链路是最明显的未恢复差异，不能只以最早的恢复升级失败排除此路径。此前cache-log-1 runtime kmsg也曾见源码驱动probe版本0007，现有固件状态可能随启动路径变化；不能据0000断言永久损坏。

厂商AXS15260产品页明确列出触控支持0-Flash应用、支持外挂PSRAM：
https://www.aixiesheng.com/product/module/list-87-1.html
这只是该芯片支持的应用形式，不证明S17板上的具体存储配置；但表明不能仅依据原厂函数名flash_init/flash_erase推定都是普通持久NOR升级。

已保存Image0x9fb4d4..0x9fcdcc的完整固件函数反汇编到extracted/metadata/axs-stock-firmware-functions.txt，继续区别进入编程模式、擦写/加载、读回验证、复位启动各阶段。实际二进制包含SPI命令06/9f/05/c7；目标存储类型和写入效果仍须核实，尚未加入自动写入路径。

当前没有新触摸修复补丁。保留已恢复的probe复位和诊断入口，后续工作的重点转向完整固件启动链路，不再把复位延时或vendor文件缺失当成已验证根因。
