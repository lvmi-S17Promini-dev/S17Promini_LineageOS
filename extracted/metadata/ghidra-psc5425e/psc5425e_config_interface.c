/* Stock Image offsets 0xa495a4..0xa49684 */

undefined4 psc5425e_config_interface(uint param_1,uint param_2,uint param_3,uint param_4)

{
  byte bVar1;
  undefined4 uVar2;
  byte abStack_4c [4];
  long lStack_48;

  lStack_48 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  abStack_4c[0] = 0;
  func_0xffffff8008fa5474(0xffffff8009829520);
  psc5425e_read_byte(param_1,abStack_4c);
  bVar1 = abStack_4c[0];
  abStack_4c[0] =
       abStack_4c[0] & ((byte)((param_3 & 0xff) << (ulong)(param_4 & 0x1f)) ^ 0xff) |
       (byte)((param_2 & 0xff) << (ulong)(param_4 & 0x1f));
  uVar2 = psc5425e_write_byte(param_1);
  func_0xffffff8008fa54e4(0xffffff8009829520);
  func_0xffffff8008125e6c(0xffffff80093aaaeb,0xffffff80093fcc4e,param_1 & 0xff,abStack_4c[0],bVar1);
  if (lRamffffff800972db08 == lStack_48) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
