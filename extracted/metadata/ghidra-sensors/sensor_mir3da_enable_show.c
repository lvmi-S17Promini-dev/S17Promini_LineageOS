/* Stock Image offsets 0x632204..0x6322b0 */

void sensor_mir3da_enable_show(undefined8 param_1,undefined8 param_2)

{
  undefined8 uVar1;
  int iVar2;
  long lVar3;
  undefined1 auStack_2c [4];
  long lStack_28;

  lStack_28 = lRamffffff800972db08;
  _mcount();
  uVar1 = uRamffffff80099daaf0;
  if ((bRamffffff80097db2e8 >> 3 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff800949c335,0x1ed);
  }
  iVar2 = sensor_mir3da_get_enable(uVar1,auStack_2c);
  if (iVar2 < 0) {
    lVar3 = -0x16;
  }
  else {
    iVar2 = func_0xffffff8008f9fb58(param_2,0xffffff80094010a2,auStack_2c[0]);
    lVar3 = (long)iVar2;
  }
  if (lRamffffff800972db08 == lStack_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}
