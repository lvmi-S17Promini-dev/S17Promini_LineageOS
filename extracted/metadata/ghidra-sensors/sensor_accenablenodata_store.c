/* Stock Image offsets 0x62f0dc..0x62f1a4 */

long sensor_accenablenodata_store(undefined8 param_1,undefined8 param_2,char *param_3,long param_4)

{
  long lVar1;
  int iVar2;
  undefined4 uVar3;

  _mcount();
  lVar1 = lRamffffff80099daa70;
  if ((bRamffffff80098a37ca >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a37a8,0xffffff800947afcb,param_3);
  }
  func_0xffffff8008fa5474(lRamffffff80099daa70 + 0x70);
  if (*param_3 == '1') {
    uVar3 = 1;
  }
  else {
    if (*param_3 != '0') {
      func_0xffffff8008125e6c(0xffffff80093e3ced);
      iVar2 = -1;
      goto LAB_ffffff80086af148;
    }
    uVar3 = 0;
  }
  *(undefined4 *)(lVar1 + 0x1c4) = uVar3;
  *(char *)(lVar1 + 0x1b8) = (char)uVar3;
  iVar2 = sensor_acc_enable_and_batch();
LAB_ffffff80086af148:
  func_0xffffff8008fa54e4(lRamffffff80099daa70 + 0x70);
  if (iVar2 != 0) {
    param_4 = (long)iVar2;
  }
  return param_4;
}
