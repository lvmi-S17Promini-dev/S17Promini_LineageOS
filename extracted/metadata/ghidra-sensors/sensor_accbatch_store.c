/* Stock Image offsets 0x62f328..0x62f430 */

void sensor_accbatch_store(undefined8 param_1,undefined8 param_2,undefined8 param_3,long param_4)

{
  long lVar1;
  int iVar2;
  undefined8 uStack_40;
  long lStack_38;

  lStack_38 = lRamffffff800972db08;
  _mcount();
  lVar1 = lRamffffff80099daa70;
  uStack_40 = 0;
  if ((bRamffffff80098a39fa >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a39d8,0xffffff80092fa2ab,0xffffff80093e3d26,param_3);
  }
  iVar2 = func_0xffffff8008fa0c70
                    (param_3,0xffffff800937053e,(long)&uStack_40 + 4,&uStack_40,lVar1 + 0x1c8,
                     lVar1 + 0x1d0);
  if (iVar2 == 4) {
    func_0xffffff8008fa5474(lRamffffff80099daa70 + 0x70);
    iVar2 = sensor_acc_enable_and_batch();
    func_0xffffff8008fa54e4(lRamffffff80099daa70 + 0x70);
    if (iVar2 != 0) {
      param_4 = (long)iVar2;
    }
  }
  else {
    func_0xffffff8008125e6c(0xffffff800946a4c8,0xffffff80093e3d26,iVar2);
    param_4 = -1;
  }
  if (lRamffffff800972db08 != lStack_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_4);
  }
  return;
}
