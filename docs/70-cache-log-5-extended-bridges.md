# cache-log-5：系统已启动，缺 vendor 扩展内存/渲染 bridge

原始文件归档 lineage-debug/cache-log-5。不同会话的logcat不按统一时间轴混用。props显示sys.boot_completed=1；GPU dump的UM/KM/FW均1.13@5776728，FW OS active、HWR OK、CCB有已执行命令。SurfaceFlinger已能完整dumpsys。但采样显示powerMode=Off、无可见HWC层，这本身也可能是自动熄屏，不能当成持续黑屏的独立根因。s17-screen.png为0字节，不能当成有效黑色截图。

## 明确失败接口

kmsg的bridge module6/functions42、44反复返回BRIDGE_EINVAL；logcat明确命名DevmemIntReserveRange2/DevmemIntMapPMR2。module130/functions15、16对应RGXCreateZSBuffer2/RGXCreateFreeList2。另有MM43的清理调用失败。当前公开1.13源MM最高36、TA3D最高14，边界检查拒绝这些新增调用。版本号匹配不意味着已包含vendor后加的接口。

## ABI核对

查过本地全部GPU目录以及较新OPPO远端分支，没有现成实现。参考公开Chromium Imagination驱动：[devicemem_server.c](https://chromium.googlesource.com/chromiumos/third_party/kernel/+/d8738ef369af3d30d2bb29b56e8b9df79b196bdb/drivers/gpu/drm/img-rogue/1.17/devicemem_server.c)。参考文件保存在extracted/metadata/rogue-mm-reference，未作为新DDK安装。

1.17的编号47..50及字段顺序不能直接用于本机。对实际vendor lib64/libsrv_um.so反汇编确认：MM42输入28字节（heap@0、address@8、length@16、32bit flags@24），输出12；MM44输入24（heap@0、reservation@8、PMR@16），输出4；TA3D15输入20/输出12，与旧ZS输入一致；TA3D16输入44/输出12，private ctx、四个page配置、global FL、check flag、reservation@36。ARM32库的Reserve2也有针对64bit内核的28/12字节分支；完整32位应用实测未进行。

## 实现

在m1.13ED5776728增加MM42..45、TA3D15..16。预约对象保存映射flags与mapping；映射/解映射复用现有MMU/PMR实现，返回真实错误，不通过跳过边界检查伪造成功。旧接口保留，opaque reservation handle类型保持兼容。

每个扩展预约带锁和借用计数。FreeList2与ZSBuffer2持有预约引用，实际FW资源销毁后释放；仍被借用时unmap/unreserve返回RETRY。FreeList2从映射取得PMR/虚拟地址，交给旧的渲染资源构造路径；ZS2复用原有构造逻辑。配套销毁、分配失败和handle失败回滚路径同步补齐。

新增预约拒绝零长度、未对齐、地址溢出，以及旧释放路径无法正确表示的>32bit长度。缺失37..41仍由原dispatch的未注册函数检查拒绝，未伪造未知接口。没有修改面板时序、固件或GPU电压频率。

## 验证与限制

extracted/metadata/mm2-lifecycle-test.c直接提取新增C helper，用模拟旧MMU/PMR后端测试错误范围、错误heap、PMR过小、重复mapping、lock/map/unmap失败回滚、借用期间RETRY、abandoned reservation自动释放以及引用溢出。mm2-render-lifecycle-test.c增加实际新增render helper的成功/失败引用测试；ASan/UBSan均通过。mm2-wire-abi-test.c抽取源码结构并断言上述vendor报文尺寸和关键offset，通过。完整kernel编译和真实MMU/稀疏页/固件异步资源生命周期仍未验证，用户负责编译与设备测试。

本轮修复提交b6063932c2fe，794行新增，含生成bridge适配器及生命周期代码。下一次只需更新源码boot。检查MM42/44、TA3D15/16的EINVAL是否消失；然后先唤醒屏幕再截图，区分绘制与显示输出。此补丁尚未证明黑屏已解决。
