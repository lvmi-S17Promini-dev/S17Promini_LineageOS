/* Stock Image offsets 0x6338a8..0x633acc */

void sensor_mir3da_parse_chip_info(undefined8 param_1)

{
  byte bVar1;
  uint uVar2;
  undefined4 uVar3;
  ulong uVar4;
  undefined1 auStack_44 [4];
  byte abStack_40 [4];
  byte abStack_3c [4];
  long lStack_38;

  lStack_38 = lRamffffff800972db08;
  _mcount();
  uVar2 = 0xff;
  abStack_3c[0] = 0xff;
  abStack_40[0] = 0xff;
  auStack_44[0] = 0xff;
  if (iRamffffff80097db2ec == -1) goto LAB_ffffff80086b3aa0;
  bVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0xc0,abStack_3c);
  uVar2 = (uint)bVar1;
  if (bVar1 != 0) goto LAB_ffffff80086b3aa0;
  bRamffffff80099dab30 = abStack_3c[0];
  if (abStack_3c[0] < 0x40) {
    uVar2 = 0xffffffff;
    goto LAB_ffffff80086b3aa0;
  }
  uRamffffff80099dab34 = (uint)(abStack_3c[0] >> 6);
  bVar1 = abStack_3c[0] & 0x38;
  uRamffffff80099dab38 = 0x200000002;
  if (bVar1 == 8) {
    uVar3 = 2;
LAB_ffffff80086b397c:
    uRamffffff80099dab38 = CONCAT44(2,uVar3);
  }
  else {
    if (bVar1 == 0x18) {
      uVar3 = 4;
      goto LAB_ffffff80086b397c;
    }
    if (bVar1 == 0x10) {
      uVar3 = 3;
      goto LAB_ffffff80086b397c;
    }
  }
  bVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0xc1,abStack_40);
  uVar2 = (uint)bVar1;
  if (bVar1 == 0) {
    if ((int)uRamffffff80099dab38 == 2) {
      bVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0x8f,abStack_3c);
      uVar2 = (uint)bVar1;
      if (bVar1 != 0) goto LAB_ffffff80086b3aa0;
      uVar4 = (ulong)((uint)(abStack_40[0] >> 6) | (abStack_3c[0] & 1) << 2);
    }
    else {
      uVar4 = (ulong)(abStack_40[0] >> 5);
    }
    bVar1 = (*(code *)*puRamffffff80099dab20)(param_1,0x8f,auStack_44);
    uVar2 = (uint)bVar1;
    if ((bVar1 == 0) && ((uint)uVar4 < 5)) {
                    /* WARNING: Could not recover jumptable at 0xffffff80086b3a24. Too many branches
                        */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)((ulong)*(byte *)(uVar4 - 0x7ff6f01508) * 4 + -0x7ff794c5d8))(0);
      return;
    }
  }
LAB_ffffff80086b3aa0:
  if (lRamffffff800972db08 == lStack_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}
