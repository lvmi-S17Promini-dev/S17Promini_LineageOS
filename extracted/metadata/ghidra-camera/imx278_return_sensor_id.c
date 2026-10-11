/* Stock Image offsets 0x879c6c..0x879d70 */

uint imx278_return_sensor_id(void)

{
  ushort uVar1;
  ushort uStack_30;
  undefined1 uStack_2e;
  undefined2 auStack_2c [2];
  long lStack_28;

  lStack_28 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  uStack_30 = 0x20a;
  uStack_2e = 0x13;
  camera_iWriteRegI2C(&uStack_30,3,uRamffffff8009805734);
  uStack_30 = 10;
  uStack_2e = 1;
  camera_iWriteRegI2C(&uStack_30,3,uRamffffff8009805734);
  uStack_30 = 0x10a;
  uStack_2e = 1;
  camera_iWriteRegI2C(&uStack_30,3,uRamffffff8009805734);
  uStack_30 = 0;
  auStack_2c[0] = 0x280a;
  camera_iReadRegI2C(auStack_2c,2,&uStack_30,1,uRamffffff8009805734);
  uVar1 = uStack_30;
  uStack_30 = 0;
  auStack_2c[0] = 0x290a;
  camera_iReadRegI2C(auStack_2c,2,&uStack_30,1,uRamffffff8009805734);
  if (lRamffffff800972db08 == lStack_28) {
    return (uStack_30 & 0xf0 | (uint)uVar1 << 8) >> 4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
