/* Stock Image offsets 0x6345bc..0x6346d8 */

int sensor_NSA_get_reg_data(undefined8 param_1,long param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  undefined1 auStack_5c [4];
  long lStack_58;

  lStack_58 = lRamffffff800972db08;
  _mcount();
  iVar1 = (*(code *)puRamffffff80099dab20[9])(param_2,0xffffff800947b0bb);
  uVar3 = 0;
  do {
    if ((uVar3 & 0xf) == 0) {
      iVar2 = (*(code *)puRamffffff80099dab20[9])(param_2 + iVar1,0xffffff800949c35e,uVar3);
      iVar1 = iVar2 + iVar1;
    }
    (*(code *)*puRamffffff80099dab20)(param_1,uVar3,auStack_5c);
    iVar2 = (*(code *)puRamffffff80099dab20[9])(param_2 + iVar1,0xffffff80093a2bcb,auStack_5c[0]);
    uVar3 = uVar3 + 1;
    iVar1 = iVar2 + iVar1;
  } while (uVar3 != 0xd3);
  iVar2 = (*(code *)puRamffffff80099dab20[9])(param_2 + iVar1,0xffffff80094ac982);
  if (lRamffffff800972db08 == lStack_58) {
    return iVar2 + iVar1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
