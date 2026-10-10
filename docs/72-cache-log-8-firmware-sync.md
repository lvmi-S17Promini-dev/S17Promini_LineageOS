# cache-log-8：有效固件 trace 与 KICK 静态核对

更新后的 capture.txt 有 Capture complete；fw-after.txt 为 trace(main mts bif pm rtd spm pow hwr)，日志确已开启。该批 GPU 进程 PID 及 uptime 与 cache-log-7 不同，属于另一次开机会话，不能拼接地址或 IRQ 计数。

固件报告 BootAnimation 的 FWCtx 0xc00285e0 和 0xc0028bc0 都等待 checkpoint 0xc002f000：当前 0xac1，要求 0x519。源码 sync_checkpoint_external.h 定义前者 ACTIVE、后者 SIGNALLED，数值合法，不是独立证据证明 checkpoint 内容损坏。其他 3D 上下文报告 PR-Check 当前 0，需要 >=1。其前序 TA 队列显示 Ready，但未产生完成值。

捕获窗口包含大量 CLEANUP(0x2abc006a)、POW(0x2abc006b) 与上电/下电，不能把 CLEANUP 返回 3 当成 KICK 执行错误。没有捕获首次渲染 KICK，所以目前不能判定任务在创建、提交还是固件执行阶段首次出错。

## 原厂 KICK 逆向结果

原厂 Image 0x568664..0x568688：命令 0x2abc0065，payload 从命令头+8 开始，两次 STP 复制共 32 字节；调用调度函数 0x54593c 时 DM=2。

0x56882c..0x568880：单独 3D 命令同为 0x2abc0065；合并时加 0x10，得到 0x2abc0075。第一份 32 字节 payload 在+8，第二份在+0x28（40）；调度 DM=3。对应当前 KICK=101、COMBINED_TA_3D_KICK=117、GEOM=2、3D=3，以及结构体的 8 字节命令头和两个 32 字节 kick data。

这验证编号、长度、两个 payload 的边界与 DM，不验证每个字段实际值或所有上下文字段布局，也没有证明固件 ABI 完全一致。目前不修改这些格式。

## 无需刷机的补证据方式

采集脚本新增第二参数 --restart-bootanim：在确认 trace 已启用后保存 service.bootanim.exit，临时设为 0，请求 ctl.restart bootanim，使新进程创建上下文并提交任务。采集完成后恢复退出属性和日志过滤器。默认模式继续只采集，不重启进程。该选项会更换开机动画进程；进程重启无法恢复原 PID。若旧进程退出慢，十秒窗口可能仍抓不到新提交，需以记录的状态和 PID 判断，不能把没有 KICK 当成驱动丢弃了 KICK。

这是诊断工具更新，不是黑屏修复；未编译或刷写内核。已做 shell 语法检查，手机实际执行仍待验证。
