/* Stock Image offsets 0xa494d8..0xa495a4 */

undefined4 psc5425e_read_interface(uint param_1,undefined1 *param_2,uint param_3,uint param_4)

{
  uint uVar1;
  undefined4 uVar2;
  byte abStack_4c [4];
  long lStack_48;

  lStack_48 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  abStack_4c[0] = 0;
  uVar2 = psc5425e_read_byte(param_1,abStack_4c);
  func_0xffffff8008125e6c(0xffffff80093786a3,0xffffff800942f6f6,param_1 & 0xff,abStack_4c[0]);
  uVar1 = (param_3 & 0xff) << (ulong)(param_4 & 0x1f) & (uint)abStack_4c[0];
  abStack_4c[0] = (byte)uVar1;
  *param_2 = (char)(uVar1 >> (ulong)(param_4 & 0x1f));
  func_0xffffff8008125e6c(0xffffff8009324113,0xffffff800942f6f6);
  if (lRamffffff800972db08 == lStack_48) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
