/* Stock Image offsets 0x876c20..0x876cec */

void gc5025sub_gc5025sub_select_page_otp(int param_1)

{
  ushort uVar1;
  ushort auStack_34 [2];
  undefined2 uStack_30;
  undefined1 auStack_2c [4];
  long lStack_28;

  lStack_28 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  uStack_30 = 0xfe;
  camera_iWriteRegI2C(&uStack_30,2,0x6e);
  auStack_34[0] = 0;
  auStack_2c[0] = 0xd4;
  camera_iReadRegI2C(auStack_2c,1,auStack_34,1,0x6e);
  if (param_1 == 1) {
    uVar1 = auStack_34[0] | 4;
  }
  else {
    uVar1 = auStack_34[0];
    if (param_1 == 0) {
      uVar1 = auStack_34[0] & 0xfffb;
    }
  }
  func_0xffffff8008f8f060(0x147aeb8);
  uStack_30 = CONCAT11((char)uVar1,0xd4);
  camera_iWriteRegI2C(&uStack_30,2,0x6e);
  if (lRamffffff800972db08 == lStack_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
