# 手动原厂PRAM下载：Ghidra复核与测试入口

## 实现范围

新增firmware_pram(root写load)与firmware_status(实时近似进度)。启动时不自动加载，不执行原厂flash_init/erase/write路径。选择原厂独立PRAM路径，暂未在S17上实测，不能宣称触摸修复。

只接受axs15260-s17.bin，50192字节、CRC32 0x1655298b、内嵌版本0007。文件来自已验证原厂内核Image0x10d0e89，SHA256 92332cbd1e3da9e39647951a3bd198ee65a43ef199b9db01aad83cd7d2536e1d；已复制到artifacts/axs15260-s17.bin。不下载其他设备固件，不修改vendor镜像。固件通过request_firmware_direct加载；缺文件/校验失败发生在复位与设备写入之前。

PRAM选择：GPIO174低，等待1..1.1ms，发送A5 5A B5 AB 01，释放为高，等待15..16ms。256字节分包，11字节头：AB B5 A5 5A、BE16(payload_len+3)、两个0、BE24目标偏移、payload。完整50192字节共197包，最后16字节，末包地址0xc400。结束发送AB B5 A5 5A 00 00 00 00；等待100ms，通过0x0c读取版本，只有0007判定成功。不在成功后做硬件复位，避免丢失刚下载的程序。

firmware_pram同步执行：debug_lock排除重复请求与轮询控制切换，state_lock防止暂停交错，disable_irq等待线程退出，report_lock阻止并发报文读取；LCM共享GPIO所有者的bias_lock覆盖整个选择/下载阶段，避免LCD回调中触摸复位插入。无自动重试。协议十秒deadline在每个传输前后检查，不能抢占正在执行的I2C控制器等待；失败释放GPIO为高并执行低10ms/高后20ms恢复序列。释放固件/packet并恢复IRQ深度。firmware_status无阻塞锁，字段逐个READ_ONCE，进行中不是一个原子的完整快照。

## Ghidra复核

使用用户提供/home/dengxh/ghidra_12.1.4_PUBLIC/support/analyzeHeadless，Java21。全Image按BinaryLoader、AARCH64:LE:64:v8A、基址0xffffff8008080000导入，不运行全内核分析。项目/tmp/s17-ghidra-project/S17Touch，脚本tools/S17TouchDecompile.java，输出extracted/metadata/ghidra-touch。

首轮未标记__stack_chk_fail为noreturn，反编译出现相邻函数合并；纠正该标记及独立汇编确认的函数边界，并设置已确认的返回类型后重新生成。函数命名和边界由脚本赋予，不是Ghidra自主恢复的符号；不能把变量猜测类型当成独立事实。

axs_select_pram(Image0x9fc82c)、axs_pram_write(0x9fca64)、axs_download_finish(0x9fcbf8)、axs_pram_download(0x9fcc8c)确认上述选择字节、分包格式/长度和结束/版本顺序，与Capstone提取一致。自动原厂work选的是另一份flash函数表，手动PRAM是替代验证路径，不假装等同于已验证的自动启动流程。

## 验证与限制

主机测试提取实际PRAM会话、写入、下载C函数，mock GPIO、I2C和时间。ASan/UBSan、-Wall -Wextra -Werror通过：197包载荷/地址/末包、模式/结束命令、版本查询、短写、错误、超时、错误版本、无GPIO和恢复路径。数据为合成payload，测试不能证明真实芯片接受该镜像或PRAM容量。实际原厂固件hash/长度/版本检查通过，脚本sh -n、checkpatch、diff空白检查通过。没有运行内核/Lineage编译，没有向手机写入固件。

## 手机测试

编译刷入带这些入口的boot后，电脑push artifacts/axs15260-s17.bin及tools/test_s17_touch_pram.sh到/data/local/tmp。手机root shell亮屏运行：

```
sh /data/local/tmp/test_s17_touch_pram.sh
```

脚本明确执行一次manual load。临时设置firmware_class path到/data/local/tmp，并恢复旧路径；本内核param_set_copystring不剔除newline，所以用NUL结尾的单次写入设置准确路径，也能清空原本为空的path。不设置环境变量，不重挂vendor。默认输出/data/local/tmp/s17-pram-test，拒绝覆盖已有目录和打断已有诊断轮询。

成功后有八秒getevent窗口供点击/滑动。只看到pages197不算成功，须stage=done,result=0,version=0007，并观察IRQ/实际事件。失败时保留stage/pages/result及dmesg，不立即重复擦写或切换到flash路径。先不要熄屏或手动reset_touch，因为后续复位可能丢失PRAM程序；若验证成功，另行完善启动与休眠唤醒协同。
