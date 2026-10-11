/* Stock Image offsets 0x63253c..0x6325c4 */

long sensor_mir3da_primary_offset_show(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  long lStack_28;

  lStack_28 = lRamffffff800972db08;
  _mcount();
  uStack_2c = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  sensor_mir3da_get_primary_offset(uRamffffff80099daaf0,&uStack_2c,&uStack_30,&uStack_34);
  iVar1 = func_0xffffff8008f9fb58(param_2,0xffffff800946a554,uStack_2c,uStack_30,uStack_34);
  if (lRamffffff800972db08 == lStack_28) {
    return (long)iVar1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
