ENTRY(boot_mode_r2)
stock_kedump_mini = 0x4802174d;
stock_mrdump_detection = 0x4801e289;
stock_backlight_on = 0x48014fa1;
stock_mrdump_run = 0x4801e30d;
stock_arch_reset = 0x48044671;
stock_meta_detection = 0x48017cfd;
stock_get_env = 0x4803efad;
stock_strncmp = 0x480291c5;
stock_set_env = 0x4803f1c5;
stock_rtc_fastboot_check = 0x4800831d;
stock_rtc_fastboot_clear = 0x480082d9;
stock_rgu_fastboot_check = 0x480443ed;
stock_rgu_fastboot_clear = 0x4804442d;
stock_get_timer = 0x48043cfd;
stock_detect_key = 0x48004aed;
stock_wdt_restart = 0x48044281;
stock_mdelay = 0x48043da1;
stock_detect_pmic_just_rst = 0x48004b79;
stock_rtc_recovery_check = 0x48008341;
stock_rgu_mode = 0x48044225;
stock_misc_recovery = 0x48017e39;
stock_charging_detection = 0x480152a1;
SECTIONS {
    .boot_mode_r2 0x480037fc : { *(.boot_mode_r2) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) }
}
ASSERT(SIZEOF(.boot_mode_r2) <= 408, "Replacement must fit the original function and literal pool")
