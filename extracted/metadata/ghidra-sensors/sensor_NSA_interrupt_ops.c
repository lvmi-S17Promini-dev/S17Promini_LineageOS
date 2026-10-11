/* Stock Image offsets 0x634270..0x6345bc */

undefined8 sensor_NSA_interrupt_ops(undefined8 param_1,undefined4 *param_2)

{
  int iVar1;
  undefined8 uVar2;
  code *pcVar3;
  byte abStack_3c [4];
  long lStack_38;

  lStack_38 = lRamffffff800972db08;
  _mcount();
  switch(*param_2) {
  case 0:
    sensor_mir3da_register_mask_write(param_1,0x21,0xf,*(undefined1 *)((long)param_2 + 9));
    sensor_mir3da_register_mask_write
              (param_1,0x20,0xf,
               (uint)*(byte *)(param_2 + 2) | param_2[1] << 1 | (uint)*(byte *)(param_2 + 2) << 2 |
               param_2[1] << 3);
    goto LAB_ffffff80086b4524;
  case 1:
    if (param_2[1] == 2) {
      iVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0x16,abStack_3c);
      if (iVar1 != 0) goto LAB_ffffff80086b4524;
      abStack_3c[0] = abStack_3c[0] | 0x30;
    }
    else {
      if ((param_2[1] != 1) ||
         (iVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0x16,abStack_3c), iVar1 != 0))
      goto LAB_ffffff80086b4524;
      abStack_3c[0] = abStack_3c[0] | 7;
    }
    break;
  case 2:
    if (param_2[2] == 2) {
      sensor_mir3da_register_mask_write(param_1,0x2b,0x1f,*(undefined1 *)(param_2 + 3));
      sensor_mir3da_register_mask_write
                (param_1,0x2a,0x67,
                 (uint)*(byte *)((long)param_2 + 0xd) << 6 |
                 (uint)*(byte *)((long)param_2 + 0xe) << 7 | (uint)*(byte *)((long)param_2 + 0xf));
      if (param_2[1] != 1) {
        if ((param_2[1] != 0) ||
           (iVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0x19,abStack_3c), iVar1 != 0))
        goto LAB_ffffff80086b4524;
        abStack_3c[0] = abStack_3c[0] | 0x30;
        goto code_r0xffffff80086b44d4;
      }
      iVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0x1b,abStack_3c);
      if (iVar1 != 0) goto LAB_ffffff80086b4524;
      abStack_3c[0] = abStack_3c[0] | 0x30;
code_r0xffffff80086b45a8:
      pcVar3 = (code *)puRamffffff80099dab20[1];
      uVar2 = 0x1b;
    }
    else {
      if (param_2[2] != 1) goto LAB_ffffff80086b4524;
      (*(code *)puRamffffff80099dab20[1])(param_1,0x28,*(undefined1 *)(param_2 + 3));
      sensor_mir3da_register_mask_write(param_1,0x27,3,*(undefined1 *)((long)param_2 + 0xd));
      if (param_2[1] == 1) {
        iVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0x1b,abStack_3c);
        if (iVar1 != 0) goto LAB_ffffff80086b4524;
        abStack_3c[0] = abStack_3c[0] | 4;
        goto code_r0xffffff80086b45a8;
      }
      if ((param_2[1] != 0) ||
         (iVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0x19,abStack_3c), iVar1 != 0))
      goto LAB_ffffff80086b4524;
      abStack_3c[0] = abStack_3c[0] | 4;
code_r0xffffff80086b44d4:
      pcVar3 = (code *)puRamffffff80099dab20[1];
      uVar2 = 0x19;
    }
    goto code_r0xffffff80086b451c;
  case 3:
    if (param_2[1] == 2) {
      iVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0x16,abStack_3c);
      if (iVar1 != 0) goto LAB_ffffff80086b4524;
      abStack_3c[0] = abStack_3c[0] & 0xcf;
    }
    else {
      if ((param_2[1] != 1) ||
         (iVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0x16,abStack_3c), iVar1 != 0))
      goto LAB_ffffff80086b4524;
      abStack_3c[0] = abStack_3c[0] & 0xf8;
    }
    break;
  default:
    if ((bRamffffff80097db2e8 & 1) != 0) {
      (*(code *)puRamffffff80099dab20[8])(0xffffff80093814fc);
    }
    goto LAB_ffffff80086b4524;
  }
  pcVar3 = (code *)puRamffffff80099dab20[1];
  uVar2 = 0x16;
code_r0xffffff80086b451c:
  (*pcVar3)(param_1,uVar2);
LAB_ffffff80086b4524:
  if (lRamffffff800972db08 != lStack_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}
