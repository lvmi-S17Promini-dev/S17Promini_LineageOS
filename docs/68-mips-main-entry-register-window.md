# boot-layout 补丁后的新 MIPS 异常与寄存器窗口

用户更新 cache-log-3。新 gpu-debug 的原因从 Breakpoint（EPC 0x9fc0295c）变为 Reserved instruction（EPC 0xc0015604），栈为 CF600FF8，TLB出现 CF000000 的页表、CF600000 的栈和 C0014000 的代码页。与仍停在 bootstrap 参数断言的旧 dump 有明显差异，支持其已越过旧断言。原始更新文件归档 lineage-debug/cache-log-3-after-boot-layout。

本目录 kmsg 仍记录先前 11:27 的内核编译时间和旧错误时间戳，不能把它与新 proc dump 直接拼成同一次启动。当前编译输出 .config 包含 MTK_RGX_MIPS_BOOT_DATA_EXTENDED=y，但实际镜像对应还应由新 dmesg 核对。

EPC在本机 firmware .text.startup 中，对应有效 microMIPS jals 指令；仅凭 reserved exception 不能断言固件文件错误或CPU不支持，实际GPU读到的代码、映射与现场ISA状态尚需验证。

另有确定的接口差异：firmware bootstrap 设置 CF800000 的寄存器窗口（可由之前的 microMIPS lui a1,0xcf80 和当前TLB0观察），新驱动同样定义CF800000。旧m1.11定义CF400000，RGXInitMipsProcWrapper 对物理总线宽度>32时将这个值右移16并叠加 microMIPS ISA bit，现场 wrapper为1CF40，正好是旧值。

在已有 S17兼容选项下把REGISTERS_VIRTUAL_BASE改为CF800000，legacy未启用分支保留CF400000。期望下一次wrapper为1CF80，不变更ISA、页表分配、时钟电压或PVR版本。源码diff检查通过，用户负责编译，实机待验证。这是必需的窗口兼容修复，但是否解释当前全部异常尚未证实。

下一次取 fresh dmesg 和 /proc/gpulog，优先看wrapper是否1CF80、FW started是否返回、是否还有新的EPC。无需重刷system或wipe。
