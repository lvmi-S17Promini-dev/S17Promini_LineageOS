# MT6765 连接驱动 GitHub 候选

2026-10-10。按用户要求检索 GitHub 并下载候选到工作目录 ref；未启动编译，未替换内核/vendor 的驱动或构建配置。

## 优先候选：Motorola 官方独立模块

[Moto g play (2023) 官方构建说明](https://github.com/MotorolaMobilityLLC/readme/blob/master/MMI-S3SG32.39-191R2) 指定 MT6765、4.19 内核，以及 WMT、connfem、adaptor、gen4m 的模块构建步骤。该说明的内核示例是 ARM，WMT 源码另有 CONFIG_ARM64 分支；仍需在 S17 的 ARM64 构建上验证。

已下载并固定三个仓库的同一发布标签 `MMI-S3SG32.39-191R2`：

| 组件 | 官方仓库 | 本地目录 | commit |
| --- | --- | --- | --- |
| WMT | [common](https://github.com/MotorolaMobilityLLC/vendor-mediatek-kernel_modules-connectivity-common/tree/MMI-S3SG32.39-191R2) | ref/motorola-mt6765-common | 7d4b50bdd13a6f0e9642864c1b9c0a22e02141a2 |
| Wi-Fi 字符设备 | [adaptor](https://github.com/MotorolaMobilityLLC/vendor-mediatek-kernel_modules-connectivity-wlan-adaptor/tree/MMI-S3SG32.39-191R2) | ref/motorola-mt6765-wlan-adaptor | 507897d578adfdab84cf1ba7c016799ca0d2acc7 |
| Wi-Fi 主驱动 | [gen4m](https://github.com/MotorolaMobilityLLC/vendor-mediatek-kernel_modules-connectivity-wlan-core-gen4m/tree/MMI-S3SG32.39-191R2) | ref/motorola-mt6765-wlan-core-gen4m | e98a79d085446a96344450da24282c2881fcef63 |
| connfem 候选 | [connfem / android-12-release-sst](https://github.com/MotorolaMobilityLLC/vendor-mediatek-kernel_modules-connectivity-connfem/tree/android-12-release-sst) | ref/motorola-connfem-sst | a0b2549be29f5cf4f3f212da2baa9523ab64698f |

connfem 没有上述同名发布标签（GitHub API 返回 404），因此这里的 SST 分支只是候选，不能当作整组同版本源码。gen4m 的 pre_cal.c 使用 connfem_api.h 和 EPA/ELNA 信息接口，后续须核对具体类型、字段和条件编译。

静态匹配依据：

- WMT 有 common_main/platform/mt6765.c、mt6765_dbg.c，以及 CONFIG_ARM64 编译选项。
- Kbuild/Makefile 保留 obj-m 模式，可生成与原 vendor init 对应的模块名。
- 官方给 gen4m 的参数为 WLAN_CHIP_ID=6765、MTK_COMBO_CHIP=CONNAC、CONFIG_MTK_COMBO_WIFI_HIF=axi、WIFI_IP_SET=1、MTK_ANDROID_WMT=y、MTK_ANDROID_EMI=y；adaptor 是 CONNAC_VER=1_0。
- gen4m chips/connac/connac.c 构造 WIFI_RAM_CODE_soc1_0_1_1.bin 等文件名，S17 vendor 有该文件及 soc1_0 的 MCU/Wi-Fi/BT 固件。
- WMT 配置解析器有 coex_wmt_antsel_invert_support、coex_wmt_epa_elna、wifi_config，与原厂日志出现的字段同类。
- WMT 中 KERNEL_clk_buf_show_status_info 调用有宏条件保护。当前 S17 adapter 未定义它，新源码编译可能避开原厂二进制强制导入的缺符号；需检查最终构建结果，不能以此证明其他接口已兼容。

构建说明已保存至 extracted/metadata/motorola-mt6765-connectivity-build.txt。不要照搬其 ARCH=arm、maui DT 和工具链，S17 应使用当前内核的 ARM64 配置、工具链和 Module.symvers。

## 备选：三星来源的完整驱动树

[rsuntk-oss/mtk_connectivity_module](https://github.com/rsuntk-oss/mtk_connectivity_module) 声明来自 SM-A055F 的 vendor/mediatek/kernel_modules/connectivity，支持 MT6765/MT6768 和 4.19。已下载到 ref/mtk_connectivity_module，commit `3d48d43caa96b6f9be8a06199ee6eb2241d79153`。

包含 WMT、connfem、gen4m、BT、GPS、FM 和 power_throttling。但该仓库已把多处模块规则改为 obj-y，顶层接入多个子系统，adaptor 名称也改为 wmt_wifi_chrdev。直接覆盖 S17 connectivity 目录会改变初始化、模块部署和现有 connadp，暂作参考而非直接替换。

## 后续适配范围

优先走官方独立 .ko 方案，保留现有 connadp 和 vendor init 加载流程。需要将源码按 common、wlan/adaptor、wlan/core/gen4m、connfem 的目录关系放置或调整 TOP/include 路径，依次处理 connfem/WMT/adaptor/gen4m 的依赖，并将配套模块打包到 vendor。新的驱动构建与实机结果都尚未验证。

目前已找到有针对性的源码候选，无需用户另找仓库，也无需补日志来完成源码检索。源码接入后编译由用户执行；出现编译错误时应保存完整模块构建日志，加载通过后再抓 Wi-Fi 启动日志。
