/* Stock Image offsets 0x62cde8..0x62cf20 */

void sensor_psbatch_store(undefined8 param_1,undefined8 param_2,undefined8 param_3,long param_4)

{
  long lVar1;
  int iVar2;
  undefined8 uStack_40;
  long lStack_38;

  lStack_38 = lRamffffff800972db08;
  _mcount();
  lVar1 = lRamffffff80099da9f8;
  uStack_40 = 0;
  if ((bRamffffff80098a3432 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a3410,0xffffff80093813b2,0xffffff800949c27e,param_3);
  }
  iVar2 = func_0xffffff8008fa0c70
                    (param_3,0xffffff800937053e,(long)&uStack_40 + 4,&uStack_40,lVar1 + 0x310,
                     lVar1 + 0x318);
  if (iVar2 == 4) {
    func_0xffffff8008fa5474(lRamffffff80099da9f8 + 0xd8);
    iVar2 = sensor_ps_enable_and_batch();
    func_0xffffff8008fa54e4(lRamffffff80099da9f8 + 0xd8);
    if ((bRamffffff80098a345a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098a3438,0xffffff80094ce28e,0xffffff800949c27e,
                 *(undefined1 *)(lVar1 + 0x2d1));
    }
    if (iVar2 != 0) {
      param_4 = (long)iVar2;
    }
  }
  else {
    func_0xffffff8008125e6c(0xffffff800931b9fa,0xffffff800949c27e,iVar2);
    param_4 = -1;
  }
  if (lRamffffff800972db08 != lStack_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_4);
  }
  return;
}
