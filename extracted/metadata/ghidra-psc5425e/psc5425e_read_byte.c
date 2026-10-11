/* Stock Image offsets 0xa49978..0xa49a5c */

void psc5425e_read_byte(undefined1 param_1,undefined8 param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 auStack_4c [4];
  undefined2 uStack_48;
  undefined4 uStack_46;
  undefined1 *puStack_40;
  undefined2 uStack_38;
  undefined4 uStack_36;
  undefined8 uStack_30;
  long lStack_28;

  lStack_28 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  auStack_4c[0] = param_1;
  func_0xffffff8008fa5474(0xffffff8009829500);
  puStack_40 = auStack_4c;
  uStack_48 = *(undefined2 *)(lRamffffff8009c0c7d0 + 2);
  uStack_46 = 0x10000;
  uStack_38 = *(undefined2 *)(lRamffffff8009c0c7d0 + 2);
  uStack_36 = 0x10001;
  uStack_30 = param_2;
  iVar2 = func_0xffffff8008a89b44(*(undefined8 *)(lRamffffff8009c0c7d0 + 0x18),&uStack_48,2);
  if (iVar2 == -6) {
    func_0xffffff8008125e6c(0xffffff8009334b76,*(long *)(lRamffffff8009c0c7d0 + 0x18) + 0x424);
  }
  func_0xffffff8008fa54e4(0xffffff8009829500);
  uVar1 = 0xffffffff;
  if (iVar2 == 2) {
    uVar1 = 1;
  }
  if (lRamffffff800972db08 == lStack_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}
