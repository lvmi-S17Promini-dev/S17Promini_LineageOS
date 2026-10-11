/* Stock Image offsets 0x633e04..0x634030 */

void sensor_mir3da_get_primary_offset
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined4 *param_4)

{
  long lVar1;
  short sVar2;
  int iVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  long lVar6;
  undefined8 uStack_78;
  undefined1 uStack_70;
  long lStack_68;

  lStack_68 = lRamffffff800972db08;
  _mcount();
  uStack_70 = 0;
  uStack_78 = 0;
  puVar5 = &uStack_78;
  lVar6 = 0x46;
  do {
    sVar2 = *(short *)(lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + lVar6);
    if (sVar2 < 0) break;
    iVar3 = (*(code *)*puRamffffff80099dab20)(param_1,(long)sVar2,puVar5);
    if (iVar3 != 0) {
      if ((bRamffffff80097db2e8 & 1) == 0) goto LAB_ffffff80086b3ee8;
      uVar4 = 0xffffff800933db53;
      goto LAB_ffffff80086b3ee0;
    }
    lVar6 = lVar6 + 4;
    puVar5 = (undefined8 *)((long)puVar5 + 1);
  } while (lVar6 != 0x6a);
  iVar3 = sensor_mir3da_soft_reset(param_1);
  if (iVar3 == 0) {
    lVar6 = 0;
    do {
      sVar2 = *(short *)(lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + lVar6 + 0x1a);
      if (sVar2 < 0) break;
      lVar1 = lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + lVar6;
      iVar3 = sensor_mir3da_register_mask_write
                        (param_1,(long)sVar2,*(undefined1 *)(lVar1 + 0x1c),
                         *(undefined1 *)(lVar1 + 0x1d));
      if (iVar3 != 0) {
        if ((bRamffffff80097db2e8 & 1) != 0) {
          (*(code *)puRamffffff80099dab20[8])
                    (0xffffff80093c3427,
                     (long)*(short *)(lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 +
                                      lVar6 + 0x1a));
        }
        sensor_mir3da_write_offset(param_1,&uStack_78);
        goto LAB_ffffff80086b3ee8;
      }
      lVar6 = lVar6 + 4;
    } while (lVar6 != 0x2c);
    (*(code *)puRamffffff80099dab20[10])(100);
    sensor_cycle_read_xyz(param_1,param_2,param_3,param_4,0x14);
    sensor_mir3da_write_offset(param_1,&uStack_78);
    if ((((cRamffffff80099dab30 == 'K') || (cRamffffff80099dab30 == -0x74)) ||
        (cRamffffff80099dab30 == -0x36)) || (iRamffffff80099dab3c == 5)) {
      uVar4 = 0;
      *param_4 = 0;
    }
    else {
      uVar4 = 0;
    }
  }
  else {
    if ((bRamffffff80097db2e8 & 1) != 0) {
      uVar4 = 0xffffff80093b3628;
LAB_ffffff80086b3ee0:
      (*(code *)puRamffffff80099dab20[8])(uVar4);
    }
LAB_ffffff80086b3ee8:
    uVar4 = 0xffffffff;
  }
  if (lRamffffff800972db08 == lStack_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}
