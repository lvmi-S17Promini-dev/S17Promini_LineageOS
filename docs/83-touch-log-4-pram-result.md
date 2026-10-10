# touch-log-4：PRAM完成传输但未得到目标版本

firmware_status：busy0、failed、pages197/197、result-71(EPROTO)、versionffff。内核日志164.602..166.027秒完整走到197包和结束后的版本查询，未在路径、权限、CRC验证或中间传输阶段失败。失败清理复位后实时版本0000，符合实现，不能用它冒充加载后成功值。

该结果说明本次独立PRAM测试没有启动可验证的0007程序；197包成功仅表示I2C报告完整发送，不证明芯片接受并执行，也不证明任何flash内容已改变。停止重复PRAM尝试，改查原厂自动路径所用的存储接口。

## 新增读取探测

root节点firmware_probe写1。复用同一受校验原厂bin用于比较，选择Flash的握手最后字节0（PRAM为1）。只执行原厂9f/05读取、内部read_init和0b读取，没有06、C7、02等写使能/擦除/编程步骤。退出执行GPIO174低10ms/高后120ms复位。不启用原厂自动升级，不在启动时执行。

stock 9f读取发送9f+两个dummy后读1字节，因此命名id_tail，不能当作完整JEDEC ID或厂家号。05读取与原厂flash_is_ready相同格式。read_init为原厂Image0x9fb5a0的12字节桥接命令；0b读取为Image0x9fc00c格式，每次读取payload+1，丢弃首dummy字节。全部197页完成后输出CRC32、差异字节总数及首32字节。flash_status数据应在firmware_status busy0后查看；其读锁会等待传输结束。

probe成功result0表示完整读取结束，不等于CRC一致、不等于触摸修复。若错误中止，CRC可能仍0，pages和result必须一起看。CRC/全部字节一致时才有依据继续查启动/映射；全FF或大量差异也不能单独证明坏flash，还可能是该读取模式未正确建立。

验证：提取实际flash_read和共享GPIO会话C函数，mock确认197页地址/长度、dummy跳过、与独立Python zlib预计算CRC一致、一个差异字节、短读中止、无GPIO及退出复位；ASan/UBSan和-Wall -Wextra -Werror通过。源码checkpatch/空白检查及脚本语法通过。未编译内核，未在手机执行该探测。

刷入新boot后更新test_s17_touch_pram.sh，已推送的50192字节bin可继续使用。在root shell执行：

```
sh /data/local/tmp/test_s17_touch_pram.sh /data/local/tmp/s17-flash-probe /data/local/tmp --probe
```

拉取s17-flash-probe。脚本仅触发firmware_probe并保存flash-status.txt，保留之前的load功能但此次不使用。目标证据是id_tail/status、CRC与差异字节及退出后实时版本。
