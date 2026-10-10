# 原厂 AXS 固件路径：不按“缺 vendor RAM 固件”处理

用户手动读取报文时 IRQ 核心 activated/started，depth0；驱动亮屏状态无禁用。读请求返回1，但没有有效接触报文，出现全零、全FF和两轮重复序列。熄屏后暂停手动读取符合实现，不能解释亮屏时失效。probe版本0000为旧快照。

本地 vendor 树没有已识别的 AXS 固件文件。这个搜索结果不证明原厂所有可选外部固件文件都不存在；但正常自动初始化的实际来源已由原厂二进制确认是内嵌固件。

原厂自动初始化 Image0x9fc350..0x9fc358 选择0x17a5948的函数表，表内版本基准0x5034、函数指针对应Image0x9fc1e8。自动work0x9fc454..0x9fc470使用Image0x10d0e89的50192字节固件，调用0x9fb644，force参数0。

0x9fc1e8依次调用flash_init(0x9fba7c)、flash_erase(0x9fbcb8)、flash_write(0x9fbdf8)、flash_read_check(0x9fc00c)。独立函数表0x17a5958中的0x9fcc8c是PRAM/download路径，未被该自动work初始化选中。不能把自动流程移植成每次启动都向RAM加载，更不能直接让当前设备执行flash擦写。

自动升级判断0x9fb778..0x9fb798要求当前版本与内嵌版本不同且当前版本非0；当前版本0时跳过。因此0000不等于“原厂每次启动会下载而当前漏了下载”，也不证明芯片中无固件。当前基本驱动没有重建所有升级和工作模式逻辑，芯片运行状态仍待确认。

I2C控制器实际CONFIG_I2C_MTK=y、MT65XX关闭。i2c-mtk对读取使用自身coherent DMA buffer，完成后memcpy到调用者buf；当前touch栈上buf并未被该适配器直接dma_map_single。故没有依据仅凭VMAP_STACK和异常报文就宣布DMA栈地址错误。

新增live_version sysfs入口复用原厂0x0c查询，与报文读取串行；同时输出regulator启用状态和电压。暂停状态拒绝查询，传输不完整返回错误；不改probe版本快照，不写flash/PRAM。源码检查和原报文验证通过，未启动内核编译，运行时效果待测试。
