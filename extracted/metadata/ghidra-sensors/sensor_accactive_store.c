/* Stock Image offsets 0x62f214..0x62f2ec */

long sensor_accactive_store(undefined8 param_1,undefined8 param_2,char *param_3,long param_4)

{
  long lVar1;
  int iVar2;
  undefined4 uVar3;

  _mcount();
  lVar1 = lRamffffff80099daa70;
  if ((bRamffffff80098a39d2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a39b0,0xffffff8009416150,0xffffff800946a4b8,param_3);
  }
  func_0xffffff8008fa5474(lRamffffff80099daa70 + 0x70);
  if (*param_3 == '1') {
    uVar3 = 1;
  }
  else {
    if (*param_3 != '0') {
      func_0xffffff8008125e6c(0xffffff80094efbf3,0xffffff800946a4b8);
      iVar2 = -1;
      goto LAB_ffffff80086af280;
    }
    uVar3 = 0;
  }
  *(undefined4 *)(lVar1 + 0x1c4) = uVar3;
  *(char *)(lVar1 + 0x1b9) = (char)uVar3;
  iVar2 = sensor_acc_enable_and_batch();
LAB_ffffff80086af280:
  func_0xffffff8008fa54e4(lRamffffff80099daa70 + 0x70);
  if (iVar2 != 0) {
    param_4 = (long)iVar2;
  }
  return param_4;
}
