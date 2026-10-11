/* Stock Image offsets 0xa4ae28..0xa4af28 */

void psc5425e_do_event(long param_1,uint param_2)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;
  ulong uVar4;
  undefined8 *puVar5;
  long lVar6;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  long lStack_48;

  puVar5 = &uStack_60;
  lStack_48 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  lVar6 = *(long *)(param_1 + 0xf0);
  lVar2 = *(long *)(lVar6 + 0xc0);
  if (lVar2 == 0) {
    func_0xffffff80086445fc(*(undefined8 *)(lVar6 + 0x10),0xffffff8009389301,0xffffff8009450206);
    uVar3 = 0xffffffed;
  }
  else {
    if (param_2 < 3) {
      uVar4 = 0;
      uStack_58 = 0;
      uStack_50 = 0;
      uStack_60 = 0;
      do {
        iVar1 = psc5425e_chg_get_property(lVar2,*(undefined4 *)(uVar4 * 4 + -0x7ff67d63e8),puVar5);
        if (iVar1 < 0) {
          func_0xffffff80086445fc
                    (*(undefined8 *)(lVar6 + 0x10),0xffffff80094b522f,0xffffff80093786e0,iVar1,
                     uVar4 & 0xffffffff);
        }
        lVar2 = *(long *)(lVar6 + 0xc0);
        uVar4 = uVar4 + 1;
        puVar5 = (undefined8 *)((long)puVar5 + 8);
      } while (uVar4 != 3);
      func_0xffffff8008aecc78();
    }
    uVar3 = 0;
  }
  if (lRamffffff800972db08 == lStack_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}
