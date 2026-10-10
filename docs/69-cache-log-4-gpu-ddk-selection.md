# cache-log-4：纠正 GPU DDK 选择，使用 1.13@5776728

本轮 kmsg 为14:09编译的kernel #6；wrapper1CF80证明上次窗口补丁进入镜像。仍等待 FW started 超时。MIPS_EXCEPTION_STATUS为20，根据当前寄存器定义是SI_SLEEP，不是EXL/ERL；NMI共享数据没有返回，dump仅显示extra debug not available，不能据此宣称没有异常或CPU没执行。

## 更关键的版本证据

实际已提取的原厂 vendor/lib/libsrv_um.so 和 vendor/lib64/libsrv_um.so 都包含明确版本字符串1.13@5776728。当前源码选的是1.11ED5425693，与实际用户态DDK不同。先前按原厂内核中的PVR版本字符串推断1.11、未核对用户态库，是选型依据不足，不能继续以此版本假定接口一致。

仓库已存在m1.13ED5776728，pvrversion.h major1/minor13/build5776728，MT6765用户配置BVNC22.87.104.18；它包含stack-first MIPS boot data、CF800000寄存器窗口、拆分的RGXFWIF_OSINIT与RGXFWIF_SYSINIT。固件反汇编C00091D8返回C1FF1080，旧1.11仍使用合并RGXFWIF_INIT，这也支持需要完整匹配接口，而不只补两个bootstrap字段。此反汇编并不能独立证明FW的精确build号；用户态字符串匹配和实际运行仍须区别。

## 配置修正

S17Promini_defconfig的MTK_GPU_VERSION改为rgx doma m1.13ED5776728，显式关闭MTK_RGX_MIPS_BOOT_DATA_EXTENDED。旧1.11目录中的试验补丁保持隔离，不参与新构建；没有修改或伪造版本号来绕过兼容检查。通用gpufreq的有边界诊断仍保留。

boot验证脚本新增expected-gpu-version参数，默认更新为准确DDK；验证历史1.11镜像时需显式指定旧值，历史报告不重写。

已核对两种用户态库版本/哈希、源码版本、BVNC和关键接口，git diff --check通过。遵照用户要求未启动编译；新DDK是否能编译、实际固件启动、UM/KM兼容检查和显示输出尚待验证。只需源码boot，不需改vendor或格式化data；用正常out编译，下一轮先确认dmesg的Initialized pvr为1.13.5776728，而非1.11.5425693。
