/* Stock Image offsets 0x633be0..0x633d9c */

int sensor_mir3da_chip_resume(undefined8 param_1)

{
  long lVar1;
  short sVar2;
  char cVar3;
  int iVar4;
  long lVar5;
  byte abStack_4c [4];
  long lStack_48;

  lStack_48 = lRamffffff800972db08;
  _mcount();
  iVar4 = sensor_mir3da_soft_reset(param_1);
  if (iVar4 == 0) {
    lVar5 = 0;
    do {
      sVar2 = *(short *)(lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + lVar5 + 0x1a);
      if (sVar2 < 0) break;
      lVar1 = lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + lVar5;
      iVar4 = sensor_mir3da_register_mask_write
                        (param_1,(long)sVar2,*(undefined1 *)(lVar1 + 0x1c),
                         *(undefined1 *)(lVar1 + 0x1d));
      if (iVar4 != 0) goto LAB_ffffff80086b3d68;
      lVar5 = lVar5 + 4;
    } while (lVar5 != 0x2c);
    (*(code *)puRamffffff80099dab20[10])(10);
    if ((iRamffffff80097db2f0 < 0) &&
       (iRamffffff80097db2f0 = sensor_mir3da_parse_chip_info(param_1), iRamffffff80097db2f0 < 0)) {
      if ((bRamffffff80097db2e8 & 1) != 0) {
        (*(code *)puRamffffff80099dab20[8])(0xffffff80093e3e55);
      }
      iVar4 = -1;
    }
    else {
      if ((iRamffffff80099dab38 == 2) &&
         (((cVar3 = (*(code *)puRamffffff80099dab20[6])(param_1), cVar3 == 'L' || (cVar3 == '&')) &&
          (iVar4 = (*(code *)*puRamffffff80099dab20)(param_1,0x8c,abStack_4c), iVar4 == 0)))) {
        abStack_4c[0] = abStack_4c[0] & 0x3f;
        (*(code *)puRamffffff80099dab20[1])(param_1,0x8c);
      }
      iVar4 = 0;
    }
  }
  else if ((bRamffffff80097db2e8 & 1) != 0) {
    (*(code *)puRamffffff80099dab20[8])(0xffffff80093b3628);
  }
LAB_ffffff80086b3d68:
  if (lRamffffff800972db08 != lStack_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar4;
}
