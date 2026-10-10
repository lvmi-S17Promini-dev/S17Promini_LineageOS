# cache-log-1：/data 的 encrypted casefold 被 ext4 拒绝

原始三个文件归档 lineage-debug/cache-log-1。kmsg 明确显示这轮已进入 fs/post-fs/late-fs，没有旧的 GSI reboot bootloader 调用。主要失败链：

- 9.273750、10.288539、11.177716 秒：EXT4-fs (dm-2): Can't mount with encoding and encryption。
- vold 挂载 metadata-encrypted /data 返回 EINVAL，e2fsck 后重试仍失败。
- init 后续 /data 下 mkdir 报只读/不存在，vold init_user0 无法建立 user_keys。
- 11.700668 秒：init 请求 recovery，reason=init_user0_failed。

SurfaceFlinger/bootanimation 尚未启动。keystore2 和 tombstoned 的后续报错不能越过此挂载错误直接当成主要根因。logcat 文件包含前次会话记录以及末尾零字节，应使用当次 kmsg 的时间和 vold PID350/keystore PID389 交叉定位，不能把前次 vold PID405 的成功路径混入。

源码 fs/ext4/super.c 的 casefold + encrypt 检查拒绝该 superblock，尽管 inlinecrypt/DM_DEFAULT_KEY 等配置已启用。fscrypt 已有 fscrypt_fname_siphash、match_name、dirhash 派生和 v2/casefold 策略校验；缺的是 ext4 对加密且 casefolded 目录项的完整处理。

恢复仓库历史中的 Android 原补丁 5e0550f5d0ec（Handle casefolding with encryption）和 09f7a7cbe57c（Optimize match for casefolded encrypted dirs）。两者干净应用；涉及 dir.c/ext4.h/hash.c/ialloc.c/inline.c/namei.c/super.c，补充目录项存储 SipHash、按 hash 过滤/解密匹配、各查找/插入/重命名/目录枚举路径，不仅删除 mount guard。

git diff --check 通过，配套 fscrypt 接口及策略/dirhash 支持已核对。遵照用户要求不执行编译：源码实际编译、/data 挂载以及已有加密文件的读写兼容性均待用户验证。无需改 fstab、禁用加密、wipe/format data，也不需重刷 system 来应用内核修复。更新源码 boot 后检查是否消失 Can't mount with encoding and encryption、/data 成功挂载以及 init_user0 成功，再继续分析显示。
