/* Stock Image offsets 0x876cec..0x876dfc */

void gc5025sub_gc5025sub_read_otp(uint param_1)

{
  byte bVar1;
  undefined2 auStack_48 [2];
  undefined2 auStack_44 [2];
  undefined1 auStack_40 [4];
  undefined2 uStack_3c;
  long lStack_38;

  lStack_38 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  auStack_48[0] = 0;
  auStack_40[0] = 0xd4;
  camera_iReadRegI2C(auStack_40,1,auStack_48,1,0x6e);
  bVar1 = (byte)auStack_48[0];
  uStack_3c = 0xfe;
  camera_iWriteRegI2C(&uStack_3c,2,0x6e);
  uStack_3c = CONCAT11(bVar1 & 0xfc | (byte)(param_1 >> 5) & 3,0xd4);
  camera_iWriteRegI2C(&uStack_3c,2,0x6e);
  uStack_3c = CONCAT11((char)(param_1 << 3),0xd5);
  camera_iWriteRegI2C(&uStack_3c,2,0x6e);
  uStack_3c = 0x20f3;
  camera_iWriteRegI2C(&uStack_3c,2,0x6e);
  auStack_44[0] = 0;
  auStack_40[0] = 0xd7;
  camera_iReadRegI2C(auStack_40,1,auStack_44,1,0x6e);
  if (lRamffffff800972db08 == lStack_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail((undefined1)auStack_44[0]);
}
