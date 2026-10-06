# 使用 mtkclient 导出 S17Promini 分区

本机已有原厂分区备份（除 userdata），保存在 `stock_imgs/`。下面记录如何重新导出；本轮仅核对命令，没有连接设备执行读写。
当前设备已经刷入 Lineage/LK 补丁，重新 dump 得到的是**当前分区状态**，不能替代旧的原厂备份。使用新目录避免覆盖旧文件。

## 环境与连接

- 使用已有 `~/mtkclient` 及其 venv；本地参考提交 `cd25cf9c1ff6d36e82697ac2c798e69e9cfb78c3`。
- S17Promini 的 BROM 接入：完全关机，按住音量 + 和音量 -，插入 USB 数据线；工具建立连接后松开按键。
- 驱动、USB 访问权限和 Python 依赖使用此前成功的环境。当前 Linux venv 导入时提示 `Unable to find libfuse`，重新运行前需要补齐系统 FUSE 依赖；不必为了 dump 给 Android 刷 Magisk。
- 如该固件的 DA/DRAM 初始化需要 `--preloader`，使用本机匹配的原厂 preloader 文件，并按已有成功参数调用。

## 导出除 userdata 外的 GPT 分区

在 Linux 中：

```bash
cd ~/mtkclient
S17_BACKUP_DIR="$HOME/S17Promini_LineageOS/stock_imgs/dump-$(date +%Y%m%d-%H%M%S)"
mkdir -p "$S17_BACKUP_DIR"

# 查看分区表；运行命令后按上面的方式连接关机设备。
./venv/bin/python mtk.py printgpt > "$S17_BACKUP_DIR/gpt.txt" 2>&1

# 逐分区读取，跳过 userdata。
./venv/bin/python mtk.py rl "$S17_BACKUP_DIR" --skip=userdata
```

`rl` 将 GPT 中各分区写为 `<分区名>.bin`，并生成 `gpt.bin`、`gpt_backup.bin`。每次命令如等待连接，可重新按上述方式接入。
应先确认 GPT 正常；本地代码在无法取得 GPT 时可能回退整块 flash 读取，出现该情况先检查连接/DA，不将结果误认为逐分区备份。

## 单独读取

```bash
./venv/bin/python mtk.py r boot "$S17_BACKUP_DIR/boot.bin"
./venv/bin/python mtk.py r recovery "$S17_BACKUP_DIR/recovery.bin"
./venv/bin/python mtk.py r super "$S17_BACKUP_DIR/super.bin"
```

system/vendor/product 是 super 内的逻辑分区，不是同名 GPT 分区；保留 super.bin 后再用本项目的 `tools/extract_stock.sh` 解包。

preloader 位于 eMMC boot 区域，不能认为 GPT 的 `rl` 已覆盖它；本地工具支持单独读取：

```bash
./venv/bin/python mtk.py r preloader "$S17_BACKUP_DIR/preloader.bin" --parttype=boot1
```

该命令语法已与当前 mtkclient 使用说明核对；本轮未在手机上重新验证。不要把 `dumppreloader` 的运行时 RAM dump 当成完整 flash 分区备份。

## 校验与归档

```bash
cd "$S17_BACKUP_DIR"
sha256sum *.bin > SHA256SUMS
```

检查工具是否报告每个分区读取成功、文件大小是否与 GPT/读取长度相符。SHA256SUMS 用于后续对比，完整备份另行存储。
Git 仅纳入已选定且不超过 50 MiB 的参考镜像；整套备份、新 dump 目录及设备独有状态分区留在本地。此文档只描述读取，不包含解锁、擦除或回写操作。

来源：本地 `mtk.py` argparse、`mtkclient/Library/DA/mtk_da_handler.py` 和 [mtkclient README-USAGE](https://github.com/bkerler/mtkclient/blob/main/README-USAGE.md)。
