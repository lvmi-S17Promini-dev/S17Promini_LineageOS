/* Stock Image offsets 0x876dfc..0x876f54 */

void gc5025sub_gc5025sub_read_otp_group(uint param_1,long param_2,int param_3)

{
  uint uVar1;
  byte bVar2;
  ulong uVar3;
  ulong uVar4;
  undefined2 auStack_58 [2];
  undefined2 auStack_54 [2];
  undefined1 auStack_50 [4];
  undefined2 uStack_4c;
  long lStack_48;

  lStack_48 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  auStack_58[0] = 0;
  auStack_50[0] = 0xd4;
  camera_iReadRegI2C(auStack_50,1,auStack_58,1,0x6e);
  bVar2 = (byte)auStack_58[0];
  uStack_4c = 0xfe;
  camera_iWriteRegI2C(&uStack_4c,2,0x6e);
  uStack_4c = CONCAT11(bVar2 & 0xfc | (byte)(param_1 >> 5) & 3,0xd4);
  camera_iWriteRegI2C(&uStack_4c,2,0x6e);
  uStack_4c = CONCAT11((char)(param_1 << 3),0xd5);
  camera_iWriteRegI2C(&uStack_4c,2,0x6e);
  uStack_4c = 0x20f3;
  camera_iWriteRegI2C(&uStack_4c,2,0x6e);
  uStack_4c = 0x88f3;
  camera_iWriteRegI2C(&uStack_4c,2,0x6e);
  if (0 < param_3) {
    uVar4 = 0;
    do {
      auStack_54[0] = 0;
      auStack_50[0] = 0xd7;
      camera_iReadRegI2C(auStack_50,1,auStack_54,1,0x6e);
      uVar3 = uVar4 & 0xff;
      uVar1 = (int)uVar4 + 1;
      uVar4 = (ulong)uVar1;
      *(undefined1 *)(param_2 + uVar3) = (undefined1)auStack_54[0];
    } while ((int)(uVar1 & 0xff) < param_3);
  }
  if (lRamffffff800972db08 == lStack_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
