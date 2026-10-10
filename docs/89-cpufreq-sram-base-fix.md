# r10：修正 cpufreq PV 表与控制区的 SRAM 基址不一致

2026-10-10。用户确认 OSS 显示、触摸、声音、震动正常，要求继续处理此前跳过的 cpufreq。本轮修复 SRAM 地址错误后，用户确认实机启动成功，DevCheck 中出现 CPU 频率变化；启动卡点已解除，动态调频恢复。

## 地址证据

S17 原厂 DT、LK 内置 DT 和当前 `s17promini-stock.dtsi` 的 `dvfsp@00110c00`，两个 reg resource 均为物理 `0x110c00`、大小 `0x1400`。

继承的 MT6765 平台头文件却定义 `CSRAM_BASE=0x110800`。hybrid.c 的控制字段和日志通过 `of_iomap(node, 1)` 访问 DT 地址，PV 表另用 `ioremap_nocache(CSRAM_BASE + 0x10, 0x240)`。因此 PV 表实际写在 `0x110810`，比预期低 `0x400`；EEM 后续更新也沿用这个表指针。静态表内容相同不能证明写入地址正确，docs/59 和 docs/61 的表内容审计没有检查到这个差异。

原厂 gzip kernel SHA256 为 `5851e16038244576d0a863d9f36b4ad602af68ea45477f5c6f44a9dfb0127286`。其 Image 中 PV 表创建路径：

```text
0153a32c: mov  w0, #0xc10
0153a334: movk w0, #0x11, lsl #16
0153a338: mov  w1, #0x240
0153a33c: bl   #0xffffff80080a681c
```

这构造的物理地址是 `0x110c10`，长度为 `0x240`，与原厂 DT 基址加 `0x10` 一致。`tools/check_stock_dvfs_sram.py` 检查原厂镜像 hash 与精确指令，并生成 `extracted/metadata/stock-dvfs-sram-address.txt`。本轮用 Capstone 直接核对指令，尚未运行 Ghidra；后续深度反编译可用用户提供的 `/home/dengxh/ghidra_12.1.4_PUBLIC/support/analyzeHeadless`。

地址错误很可能与首次 cluster-on 启用 DVFS 时卡住相关，但这是因果推断。现有 r5 日志没有证明所有轮询执行完，也没有证明固件的具体故障机制。

## 源码修复

`kernel/lvmi/mt6765/drivers/misc/mediatek/base/power/cpufreq_v1/src/mtk_cpufreq_hybrid.c`：

- PV 表使用 `csram_base + OFFS_TBL_S`，复用已经成功建立的 DT 映射。
- 条件编译的 CCI 表也使用同一个映射加其偏移；当前 MT6765 没有开启 CCI_MAP_TBL_SUPPORT。
- probe 检查第二个 DT resource 的长度覆盖 `CSRAM_SIZE`，记录实际物理基址。
- 调试区域大小/清理使用大小和相对偏移，去掉不再需要的硬编码物理地址派生宏。

未改变 OPP 表值、频率、电压、cluster 编号、IPI 命令、ACK 协议、锁顺序、轮询预算及 watchdog。r6 诊断 observer 仍保留。

`device/lvmi/S17Promini/BoardConfigRom.mk`：恢复 `S17_SKIP_CPUFREQ_INIT` 构建开关，默认值为 false，直接编译即启用 cpufreq；显式指定 true 可恢复 blacklist 旁路。该变量属于 Android 构建配置，不是内核 defconfig 选项。测试配置版本改为 `4.19.127-s17-bringup-r10-cpufreq`。

## 构建和离线验证

用户表示后续编译自行执行；检查时本轮已启动的构建已经结束（01:54），没有再启动编译。命令为：

```sh
cd /home/dengxh/android3/lineage
source build/envsetup.sh
lunch lineage_S17Promini-userdebug
S17_SKIP_CPUFREQ_INIT=false m -j8 bootimage
```

产物 `artifacts/source-boot-r10/boot-source-r10-cpufreq.img`，32 MiB，SHA256 `1c8e5cfad3ce6da40391d92333d648d3cc240f0ad2d63c0613ae30e0b08a4c76`。原工作 boot 备份为同目录的 `boot-before-cpufreq.img`。没有刷写手机，ADB 当前没有设备。

验证通过：内核编译和 bootimage 打包；内核补丁 checkpatch 无错误/警告；三个仓库 diff 空白检查；原厂地址指令检查；Python 语法；boot 内源 kernel/DTB、gzip、加载地址、AXS/GPU 配置、AVB hash 和原厂 overlay 后的触摸及 DVFS resource。新版 ramdisk/DTB 与备份逐字节相同。Android libufdt 独立工具本轮未运行。

`tools/verify_source_boot.py` 新增 `--cpufreq-init enabled|disabled`，支持恢复初始化的 collector 镜像，验证 boot cmdline 中 blacklist 的实际状态；本轮使用 `--cpufreq-init enabled`，确认产物没有 cpufreq blacklist。报告为 `extracted/metadata/source-boot-r10-validation.json`。本产物使用原厂 ramdisk，没有新增 early-init wrapper。

## 实机验收

用户后续确认：修复后成功进入系统，DevCheck 中观察到 CPU 频率变化。这支持 SRAM 地址错误是此前启动卡点的原因，并确认动态调频已恢复。尚未收到 policy/sysfs、PPM、熄屏唤醒、持续负载或热限制的日志/测试结果，不将这些项目记为通过。

只需测试 boot，无需为了这次改动重刷 system/vendor。启动成功后，先确认 `/proc/cmdline` 没有 `_mt_cpufreq_pdrv_init` blacklist、dmesg 的 SRAM resource 为 `0x110c00`，并看到 cluster command send-sync 返回及 policy init 完成。

随后读取 `/sys/devices/system/cpu/cpufreq/policy*/` 的 `affected_cpus`、`scaling_available_frequencies`、`scaling_governor`、`scaling_cur_freq`、`cpuinfo_cur_freq`（存在时）；比较空闲和应用负载期间频率、`stats/time_in_state`（存在时），核对 PPM 是否仍报告缺 DVFS table。还需测试熄屏/唤醒、持续负载与热限制；能进系统不等于动态调频完整恢复。

若仍卡住，回 recovery 保存完整 pstore 至 `oss-kernel-debug/s17-pstore-r10`，尤其看现有 observer 的 phase/mailbox_step。若 phase=5 且 ret 非零，才转向 ACK/固件处理；若仍不推进，结合多次 observer 样本区分锁、MMIO 和 delay。不要以忽略 cluster-on 或伪造 ACK 作为恢复 DVFS。
