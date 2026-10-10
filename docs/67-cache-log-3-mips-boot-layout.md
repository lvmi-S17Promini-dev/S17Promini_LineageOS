# cache-log-3：MIPS 固件 bootstrap 参数布局不匹配

ADB 读取的 gpu-debug/gpu-power 原始文件为 Windows PowerShell 输出的 UTF-16 文本。解码副本在 extracted/metadata/cache-log-3-gpu-*.txt，原件未改写。clk_summary 文件只有节点不可读错误，未作为硬件故障证据。

GPU 实测 680000 KHz、80000（10uV 单位）=0.8V、SRAM 87500=0.875V，电压状态启用。先前启动时 mux 460MHz 与此为不同采样时刻，不能混为一个固定运行值。HW APM=1 时 core0 的 clk framework enable count 为0是原调用路径，未作为断电证明。

GPULOG 显示 MIPS PC 0x9fc02338，EPC 0x9fc0295c，CAUSE 0x00800024，即 breakpoint exception（FW assert）。现场 BIF fault flags 为0；FW info UNINITIALIZED、UM/KM info全零、KM Arch32 都在固件初始化未完成的上下文，不能当成有效版本比较。默认 debug dump 写入 MTK_DEBUG_PROC_PRINT 的 /proc/gpulog，并按原实现 OSWarnOn(TRUE) 产生 WARN；不是 Invalid verbosity，MAX 实际为HIGH=2。

## 对照本机固件

本项目 vendor/firmware/rgx.fw 为 little-endian microMIPS ELF。ELF第一 LOAD 映射 file0xd4→VA0x9fc02000。EPC 0x9fc0295c 对应 bytes 8046，反汇编为 break16 0。

bootstrap 在 0x9fc0290e/12 从 boot-data cached alias 0x9fc01008/0c 读 GPU registers 的64位物理地址；0x9fc0293c/44 读0x1030和0x1034；后续检查计数并在 EPC 触发 break。读字段位置与仓库 m1.13 的 RGXMIPSFW_BOOT_DATA 一致：stack@0，regs@8，PT地址数组@0x10..0x28，log2-size@0x30，count@0x34，reserved@0x38。

当前 m1.11 仅填旧的四个64位字段：regs@0、PT@8、stack@0x10、reserved@0x18，未填0x30/0x34。这是直接可验证的 ABI 差异，不能通过加电压/延时或跳过 FW started 修复。

## 源码修改

新增 CONFIG_MTK_RGX_MIPS_BOOT_DATA_EXTENDED，默认n，仅 S17Promini_defconfig 启用；不全局切换 GPU DDK，legacy 分支保留。

m1.11 的 boot字段索引按新布局设置，保留现有连续 firmware PT 的单一物理基址，其余三个数组槽清零。内部 boot params 增加log2-size，从psFirmwareMMUDevAttrs->ui32BaseAlign获取（本实现15，32KB连续表）；count=1，与较新驱动处理连续分配的方法一致。reserved清零，stack/regs放正确位置。PDUMP的已有字段写入使用同一组索引，因此地址字段跟随新布局。

未改变 MIPS页表分配、PTE、GPU功耗/APM/频率电压、vendor firmware或用户空间接口。实际设备固件hash尚未通过ADB核对，当前基于已提取的vendor文件及完全吻合的EPC指令判断。源码差异检查通过，固件断言反汇编已保存；用户负责编译，编译及现场固件启动仍待验证。
