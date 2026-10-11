/* Stock Image offsets 0x876aa8..0x876c20 */

void gc5025sub_gc5025sub_gcore_enable_otp(int param_1)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined2 auStack_58 [2];
  undefined2 auStack_54 [2];
  undefined1 auStack_50 [4];
  undefined1 uStack_4c;
  byte bStack_4b;
  long lStack_48;

  lStack_48 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  auStack_58[0] = 0;
  auStack_50[0] = 0xfa;
  camera_iReadRegI2C(auStack_50,1,auStack_58,1,0x6e);
  uVar1 = auStack_58[0];
  auStack_54[0] = 0;
  auStack_50[0] = 0xd4;
  camera_iReadRegI2C(auStack_50,1,auStack_54,1,0x6e);
  uVar2 = auStack_54[0];
  func_0xffffff8008f8f060(0x147aeb8);
  if (param_1 == 0) {
    bStack_4b = (byte)uVar2 & 0x7f;
    uStack_4c = 0xd4;
    camera_iWriteRegI2C(&uStack_4c,2,0x6e);
    bStack_4b = (byte)uVar1 & 0xef;
    uStack_4c = 0xfa;
    camera_iWriteRegI2C(&uStack_4c,2,0x6e);
    if ((bRamffffff80098c0582 >> 2 & 1) == 0) goto LAB_ffffff80088f6bd0;
    uVar3 = 0xffffff80098c0560;
    uVar4 = 0xffffff8009375f0c;
  }
  else {
    bStack_4b = (byte)uVar1 | 0x10;
    uStack_4c = 0xfa;
    camera_iWriteRegI2C(&uStack_4c,2,0x6e);
    bStack_4b = (byte)uVar2 | 0x80;
    uStack_4c = 0xd4;
    camera_iWriteRegI2C(&uStack_4c,2,0x6e);
    if ((bRamffffff80098c055a >> 2 & 1) == 0) goto LAB_ffffff80088f6bd0;
    uVar3 = 0xffffff80098c0538;
    uVar4 = 0xffffff8009354587;
  }
  func_0xffffff80084a0cc4(uVar3,uVar4,0xffffff800941bf57);
LAB_ffffff80088f6bd0:
  if (lRamffffff800972db08 == lStack_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
