/* Exact stock S17Promini LK boot_mode_select replacement, Thumb ABI. */
.syntax unified
.cpu cortex-a53
.thumb
.section .boot_mode_r2,"ax",%progbits
.global boot_mode_r2
.thumb_func
boot_mode_r2:
    push {r3, r4, r5, r6, r7, lr}
    ldr r5, =0x480b05cc /* stock g_boot_mode */
    /* Preserve the linked stock dump, META and reboot-to-fastboot paths. */
    bl stock_kedump_mini
    cmp r0, #0
    beq check_meta
    bl stock_mrdump_detection
    cbz r0, dump_post
    bl stock_backlight_on
    ldr r3, [r5]
    cmp r3, #0x63
    beq done
    bl stock_mrdump_run
dump_post:
    ldr r3, [r5]
    cmp r3, #0x63
    beq done
    pop.w {r3, r4, r5, r6, r7, lr}
    b.w stock_arch_reset

check_meta:
    bl stock_meta_detection
    cmp r0, #0
    bne done
    ldr r7, =0x4805aba8 /* reboot_meta_flag */
    mov r0, r7
    bl stock_get_env
    cbz r0, check_fastboot
    ldr r1, =0x4805abbc /* meta */
    movs r2, #5 /* strlen("meta") + 1 */
    bl stock_strncmp
    cmp r0, #0
    bne check_fastboot
    movs r3, #1
    str r3, [r5]
    mov r0, r7
    ldr r1, =0x48070e58 /* stock "0" used to clear reboot_meta_flag */
    bl stock_set_env
    b done

check_fastboot:
    bl stock_rtc_fastboot_check
    cbz r0, check_rgu_fastboot
    movs r3, #0x63
    str r3, [r5]
    movs r0, #0
    bl stock_rtc_fastboot_clear
check_rgu_fastboot:
    bl stock_rgu_fastboot_check
    cbz r0, fastboot_result
    movs r3, #0x63
    str r3, [r5]
    movs r0, #0
    bl stock_rgu_fastboot_clear
fastboot_result:
    ldr r3, [r5]
    cmp r3, #0x63
    beq done

    /* Original factory-forbidden branch only logged; no factory branch
       exists in this stock build. Rescue keys intentionally ignore it. */
    movs r0, #0
    bl stock_get_timer
    mov r4, r0
    movw r6, #300 /* hard cap if timer fails to advance */
poll_keys:
    movs r0, #0 /* keypad Volume DOWN */
    bl stock_detect_key
    cmp r0, #0
    bne key_fastboot
    movs r0, #1 /* keypad Volume UP */
    bl stock_detect_key
    cmp r0, #0
    bne key_fastboot
    movs r0, #17 /* PMIC home/reset, stock affirmative unlock key */
    bl stock_detect_key
    cmp r0, #0
    bne key_fastboot
    movs r0, #19 /* keypad KEY_CAMERA from stock dtbo map */
    bl stock_detect_key
    cmp r0, #0
    bne key_fastboot
    bl stock_wdt_restart
    movs r0, #10
    bl stock_mdelay
    mov r0, r4
    bl stock_get_timer
    movw r3, #3000
    cmp r0, r3
    bhs no_key
    subs r6, #1
    bne poll_keys
no_key:
    bl stock_detect_pmic_just_rst
    bl stock_rtc_recovery_check
    cmp r0, #0
    bne recovery
    bl stock_rgu_mode
    cmp r0, #2
    beq recovery
    bl stock_misc_recovery
    cmp r0, #0
    bne done
    bl stock_charging_detection
    b done
recovery:
    movs r3, #2
    str r3, [r5]
    b done
key_fastboot:
    movs r3, #0x63
    str r3, [r5]
done:
    pop {r3, r4, r5, r6, r7, pc}
    .ltorg
.global boot_mode_r2_end
boot_mode_r2_end:
