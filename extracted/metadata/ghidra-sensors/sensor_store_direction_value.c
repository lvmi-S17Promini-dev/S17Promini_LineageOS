/* Stock Image offsets 0x632698..0x632750 */

undefined8 sensor_store_direction_value(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  int iVar1;
  long lVar2;
  undefined4 uStack_3c;
  long lStack_38;

  lStack_38 = lRamffffff800972db08;
  _mcount();
  uStack_3c = 0;
  lVar2 = *(long *)(lRamffffff80099daaf0 + 0xd8);
  iVar1 = func_0xffffff8008458ef4(param_2,10,&uStack_3c);
  if (iVar1 == 0) {
    *(undefined4 *)(lRamffffff80099daad8 + 0xc) = uStack_3c;
    func_0xffffff80086aa800(uStack_3c,lVar2 + 0x38);
    func_0xffffff8008125e6c(0xffffff80094ac94a,uStack_3c);
  }
  else {
    func_0xffffff8008125e6c(0xffffff800933db34);
  }
  if (lRamffffff800972db08 == lStack_38) {
    return param_3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
