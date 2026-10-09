# AXS15260 公开源码检索

2026-10-09。配置基线已提交：内核 `S17Promini-bringup` 的 `c17173daebf2`；工作目录记录 `3381977`。

通过网页搜索及 GitHub repositories/commits API，找到相关源码参考，但尚未找到与 S17 完全对应的 MediaTek `axs15260_dsi_6580_hd` LCM。GitHub code API 未登录返回 401，因此检索结果不能证明公开源码不存在。

## 最有用的参考

[iKalyes/ESP32P4_AXS15260D_MIPI_6.2in](https://github.com/iKalyes/ESP32P4_AXS15260D_MIPI_6.2in)，已下载至本地 `ref/ESP32P4_AXS15260D_MIPI_6.2in`，固定提交 `8741eee3706e83b5ab054cd51c1aeb8707e77e9d`。参考仓库不嵌入工作目录 Git。

- `components/esp_lcd_axs15260/src/esp_lcd_axs15260.c`：MIPI 显示及初始化序列。
- 同目录 `esp_lcd_touch_axs15260.c`：触摸实现。
- `docs/AXS15260D-S1_Datasheet_V1.2_20240306.pdf`：芯片手册。
- `docs/AXS15260通信协议和坐标等说明.pdf`：触摸协议。
- `docs/` 同时含两条/四条 lane 的面板初始化文本。

检查头文件，示例参数为 452×1280、2 lanes；我们原厂为 384×854、3 lanes。示例初始化包含 `0xBB` 解锁、A0/A1/A2 等配置块，能辅助定位原厂表和解释寄存器，但面板初始化、供电、Gamma 及 AXS15260D 与原厂芯片版本关系需要原厂代码核对，不能直接替代。

## 其他候选

- [Deep-start9527/Guition](https://github.com/Deep-start9527/Guition)：ESP LCD 的 AXS15260 显示、触摸及独立 init_commands 文件。
- [kcro227/luban-lite](https://github.com/kcro227/luban-lite/tree/main/bsp/peripheral/touch/axs15260)：AXS15260 触摸源码；不是 MTK LCM。
- [osptek/tft-3.97-480x800-mipi-axs15260](https://github.com/osptek/tft-3.97-480x800-mipi-axs15260)：480×800 模组；当前树未找到 AXS15260 源码文件，历史有移除样例的提交。
- [芯片厂家介绍](https://www.aixiesheng.com/product/module/list-87-1.html)：AXS15260 属显示触控一体芯片，支持 MIPI。显示与触摸复位/供电协同也应核对。

下一步以原厂 kernel/LK 为准恢复 S17 初始化及回调，结合公开手册/实现辅助分析。此次仅保存参考和检索记录，未将不匹配的初始化表接入内核。
