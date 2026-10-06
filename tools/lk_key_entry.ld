/* Virtual addresses, excluding the stock 0x200-byte MTK header. */
ENTRY(key_hook)
stock_detect_key = 0x48004aed;
stock_detect_pmic_just_rst = 0x48004b79;
original_continue = 0x4800386e;
charging_continue = 0x48003908;
SECTIONS {
    .key_hook 0x4800386a : { *(.key_hook) }
    .key_slot 0x480038f0 : { *(.key_slot) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) }
}
ASSERT(SIZEOF(.key_hook) == 4, "Hook must replace exactly one BL")
ASSERT(SIZEOF(.key_slot) == 24, "Trampoline must fit stock idle loop")
