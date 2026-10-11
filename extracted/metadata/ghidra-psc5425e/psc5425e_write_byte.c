/* Stock Image offsets 0xa49a5c..0xa49b34 */

void psc5425e_write_byte(undefined1 param_1,undefined1 param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uStack_50;
  undefined4 uStack_4e;
  undefined1 *puStack_48;
  undefined1 uStack_40;
  undefined1 uStack_3f;
  long lStack_38;

  lStack_38 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  func_0xffffff8008fa5474(0xffffff8009829500);
  uStack_50 = *(undefined2 *)(lRamffffff8009c0c7d0 + 2);
  puStack_48 = &uStack_40;
  uStack_4e = 0x20000;
  uStack_40 = param_1;
  uStack_3f = param_2;
  iVar2 = func_0xffffff8008a89b44(*(undefined8 *)(lRamffffff8009c0c7d0 + 0x18),&uStack_50,1);
  if (iVar2 == -6) {
    func_0xffffff8008125e6c(0xffffff8009334b76,*(long *)(lRamffffff8009c0c7d0 + 0x18) + 0x424);
  }
  func_0xffffff8008fa54e4(0xffffff8009829500);
  uVar1 = 1;
  if (iVar2 != 1) {
    uVar1 = 0xffffffff;
  }
  if (lRamffffff800972db08 == lStack_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}
