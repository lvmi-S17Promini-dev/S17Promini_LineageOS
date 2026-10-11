/* Stock Image offsets 0x6323f4..0x6324c0 */

undefined8 sensor_mir3da_reg_data_store(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  int iVar1;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  long lStack_28;

  lStack_28 = lRamffffff800972db08;
  _mcount();
  func_0xffffff8008fa0c70(param_2,0xffffff80094274bd,&uStack_2c,&uStack_30);
  iVar1 = sensor_mir3da_register_write(uRamffffff80099daaf0,uStack_2c,uStack_30);
  if (iVar1 != 0) {
    func_0xffffff8008125e6c
              (0xffffff800935fc44,0xffffff8009391f39,0x232,0xffffff800949c348,0xffffff80094df263);
  }
  if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff8009405a12,uStack_2c,uStack_30);
  }
  if (lRamffffff800972db08 == lStack_28) {
    return param_3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
