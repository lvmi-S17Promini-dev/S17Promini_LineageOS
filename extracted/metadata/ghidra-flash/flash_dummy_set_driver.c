/* Stock Image offsets 0x8c4338..0x8c4424 */

undefined4 flash_dummy_set_driver(int param_1)

{
  undefined4 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;

  func_0xffffff800809f01c();
  func_0xffffff8008fa5474(0xffffff800980c1e0);
  if (param_1 == 0) {
    iRamffffff8009af0104 = iRamffffff8009af0104 + -1;
    if (iRamffffff8009af0104 == 0) {
      uVar1 = flash_dummy_pinctrl_set(0);
    }
    else {
      uVar1 = 0;
    }
    if (iRamffffff8009af0104 < 0) {
      iRamffffff8009af0104 = 0;
    }
    if ((bRamffffff80098c744a >> 2 & 1) == 0) goto LAB_ffffff80089443d0;
    uVar2 = 0xffffff80098c7428;
    uVar3 = 0xffffff80092ffeed;
  }
  else {
    if (iRamffffff8009af0104 == 0) {
      uVar1 = flash_dummy_pinctrl_set(0);
    }
    else {
      uVar1 = 0;
    }
    iRamffffff8009af0104 = iRamffffff8009af0104 + 1;
    if ((bRamffffff80098c7422 >> 2 & 1) == 0) goto LAB_ffffff80089443d0;
    uVar2 = 0xffffff80098c7400;
    uVar3 = 0xffffff800942d774;
  }
  func_0xffffff80084a0cc4(uVar2,uVar3,0xffffff80094a237e,iRamffffff8009af0104);
LAB_ffffff80089443d0:
  func_0xffffff8008fa54e4(0xffffff800980c1e0);
  return uVar1;
}
