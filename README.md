# S17Promini LineageOS 移植工作目录

用于保存 LineageOS 19.1 移植文档、分析工具、诊断日志和小型镜像产物。
DT、vendor 与本工作目录使用独立 Git 仓库。

- DT：`~/android3/lineage/device/lvmi/S17Promini`
- Vendor：`~/android3/lineage/vendor/lvmi/S17Promini`
- [移植记录](docs/README.md) / [Roadmap](docs/ROADMAP.md)
- [mtkclient 分区导出方法](docs/STOCK_BACKUP.md)

## 跟踪范围

- `docs/`：移植步骤、问题记录、roadmap。
- `tools/`：解包、分析、LK patch 和验证工具。
- `charger-debug/`、`recovery-debug/`、`lineage-debug/`：设备诊断日志及其规范化版本。
- `extracted/metadata/`：分区信息、镜像头、验证结果及文本分析；常规构建/解包进度日志忽略。
- `ref/内核config.gz.txt`：原厂内核配置；其他参考仓库留在本地，不嵌入此仓库。
- `artifacts/`：小型 boot/recovery/LK/DT 产物及元数据，二进制采用显式清单。
- `stock_imgs/`：仅选定的小型 boot/recovery/LK/DTBO/vbmeta 等参考备份。

单文件上限 **50 MiB**。system/vendor/super、ROM zip、解包文件系统和整套分区备份在本地保存。
原厂备份不能仅依赖此 Git 仓库；需要完整备份时按导出文档准备外部存储。
历史失败镜像保留用于对照，使用前查看对应 docs 和 artifacts/README.md。

## 添加新文件

通常文本记录和设备日志可直接 `git add`；新二进制默认忽略，确认用途及大小后再增加 `.gitignore` 的显式例外。
每次提交会检查暂存区，拒绝超过 50 MiB 的文件和误加入的嵌套仓库。

```bash
git config core.hooksPath .githooks
python3 tools/check_git_assets.py
```

重新 clone 后执行第一行启用版本化提交检查。保留原始日志换行和编码，不因格式整理改变二进制内容。
