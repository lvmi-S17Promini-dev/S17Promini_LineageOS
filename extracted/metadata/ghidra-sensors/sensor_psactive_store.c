/* Stock Image offsets 0x62ccac..0x62cdac */

long sensor_psactive_store(undefined8 param_1,undefined8 param_2,char *param_3,long param_4)

{
  long lVar1;
  int iVar2;
  undefined4 uVar3;

  _mcount();
  lVar1 = lRamffffff80099da9f8;
  if ((bRamffffff80098a327a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a3258,0xffffff800931ba1f,0xffffff800948b853,param_3);
  }
  func_0xffffff8008fa5474(lRamffffff80099da9f8 + 0xd8);
  if (*param_3 == '1') {
    uVar3 = 1;
  }
  else {
    if (*param_3 != '0') {
      func_0xffffff8008125e6c(0xffffff800932c7ee,0xffffff800948b853);
      iVar2 = -1;
      goto LAB_ffffff80086acd14;
    }
    uVar3 = 0;
  }
  *(undefined4 *)(lVar1 + 0x30c) = uVar3;
  iVar2 = sensor_ps_enable_and_batch();
LAB_ffffff80086acd14:
  func_0xffffff8008fa54e4(lRamffffff80099da9f8 + 0xd8);
  if ((bRamffffff80098a32a2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a3280,0xffffff80094efb42,0xffffff800948b853);
  }
  if (iVar2 != 0) {
    param_4 = (long)iVar2;
  }
  return param_4;
}
