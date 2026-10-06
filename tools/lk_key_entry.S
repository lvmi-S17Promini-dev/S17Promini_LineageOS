/* S17Promini stock LK only. Reuse the removed 50 ms idle loop. */
.syntax unified
.cpu cortex-a53
.thumb

.section .key_hook,"ax",%progbits
.global key_hook
.thumb_func
key_hook:
    b.w key_entry

.section .key_slot,"ax",%progbits
.global key_slot
.thumb_func
key_slot:
    /* Normal fallthrough after misc: skip the trampoline. */
    b.n slot_end
.global key_entry
.thumb_func
key_entry:
    /* Stock unlock confirmation uses HW key 0x11 for Volume UP. */
    movs r0, #0x11
    bl stock_detect_key
    cbz r0, no_key
    movs r3, #0x63
    str r3, [r5]
    /* Return directly from boot_mode_select, using its existing frame. */
    pop {r3, r4, r5, r6, r7, pc}
no_key:
    /* Replay the original call replaced by the hook. */
    bl stock_detect_pmic_just_rst
    b.w original_continue
slot_end:
