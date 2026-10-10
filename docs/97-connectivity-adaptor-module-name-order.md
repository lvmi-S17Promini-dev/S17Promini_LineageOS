# Wi-Fi adaptor 的空 .o 构建目标

2026-10-11。用户构建报 `wlan/adaptor/.o` 无规则，并且 warning 中模块名为空。

原因是本轮接入在 adaptor/Makefile 中把 `MODULE_NAME := wmt_chrdev_wifi` 放到了 obj-y/obj-m 使用之后。scripts/Makefile.build 首先用 `obj-y :=` 初始化简单展开变量，因此读取 `obj-y += $(MODULE_NAME).o` 时模块名仍为空，立即生成 `.o`。原来的直接 include 检查没有初始化 obj-y 的变量类型，使用了延迟展开；后续 MODULE_NAME 定义掩盖了这个错误。这是接入和离线检查的遗漏。

已将 MODULE_NAME 定义移动到目标规则之前。没有启动内核编译，也没有修改其他正在进行的 GPU/关机诊断改动。

这次通过真实 scripts/Makefile.build 和 scripts/Makefile.lib 求值，使用当前 KERNEL_OBJ/include/config/auto.conf，并从真实 connectivity 顶层获取参数：

- connfem.o：7 个 C 文件。
- wmt_drv.o：49 个 C 文件。
- wmt_chrdev_wifi.o：4 个 C 文件。
- wlan_drv_gen4m.o：98 个 C 文件。
- bt_drv_connac1x.o：4 个 C 文件。

五个目标均为期望的内建对象，无空 .o，所有最终源码目标存在。临时回退赋值顺序的 fixture 在同一真实 Kbuild 环境中重现 `.o`，证明该检查能捕获原错误。报告保存在 extracted/metadata/connectivity-kbuild-module-name-fix.json。

可以直接增量重跑原构建命令，无需清理整个输出目录。上述检查只验证 Kbuild 目标展开和文件存在，不能代替 C 编译、链接或实机验证。
