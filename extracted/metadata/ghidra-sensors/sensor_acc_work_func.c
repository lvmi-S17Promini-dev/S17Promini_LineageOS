/* Stock Image offsets 0x630004..0x630214 */

void sensor_acc_work_func(void)

{
  long lVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  undefined1 auVar7 [16];
  uint uStack_90;
  int iStack_8c;
  int iStack_88;
  int iStack_84;
  long lStack_80;
  undefined4 uStack_78;
  int iStack_74;
  int iStack_70;
  int iStack_6c;
  undefined8 uStack_68;
  undefined4 uStack_60;
  long lStack_58;

  lStack_58 = lRamffffff800972db08;
  _mcount();
  lVar1 = lRamffffff80099daa70;
  lVar5 = (long)*(int *)(lRamffffff80099daa70 + 0x90);
  if (*(long *)(lRamffffff80099daa70 + 0x1a0) == 0) {
    func_0xffffff8008125e6c(0xffffff800930ae42);
  }
  else {
    func_0xffffff800814b4dc(1);
    auVar7 = func_0xffffff8008144a44();
    iVar2 = (**(code **)(lVar1 + 0x1a0))(&iStack_84,&iStack_88,&iStack_8c,&uStack_90);
    if (iVar2 == 0) {
      if ((iStack_88 != 0 || iStack_84 != 0) || iStack_8c != 0) {
        *(int *)(lVar1 + 0x138) = iStack_84;
        *(int *)(lVar1 + 0x13c) = iStack_88;
        *(int *)(lVar1 + 0x140) = iStack_8c;
        lVar3 = auVar7._0_8_ * 1000000000;
        lVar6 = lVar3 + auVar7._8_8_;
        *(char *)(lVar1 + 0x134) = (char)uStack_90;
        lVar4 = *(long *)(lVar1 + 0x144);
        *(long *)(lVar1 + 0x144) = lVar6;
        if ((*(char *)(lVar1 + 0x1ba) == '\0') ||
           (((*(undefined1 *)(lVar1 + 0x1ba) = 0, iStack_84 != -1 && (iStack_88 != -1)) &&
            (lVar4 = lVar6, iStack_8c != -1)))) {
          if (lVar5 * 1800000 <= lVar6 - lVar4) {
            lVar6 = ((auVar7._8_8_ + lVar3) - lVar4) + lVar5 * -1000000;
            while( true ) {
              lVar4 = lVar4 + lVar5 * 1000000;
              uStack_60 = 0;
              uStack_68 = 0;
              uStack_78 = (uStack_90 & 0xff) << 0x10;
              _iStack_70 = CONCAT44(iStack_8c,iStack_88);
              uStack_78 = CONCAT13(*(undefined1 *)(lVar1 + 0x135),(undefined3)uStack_78);
              _uStack_78 = CONCAT44(iStack_84,uStack_78);
              lStack_80 = lVar4;
              func_0xffffff80086ab708(*(undefined1 *)(lRamffffff80099daa70 + 8),&lStack_80);
              if (lVar6 < lVar5 * 1800000) break;
              uStack_90 = (uint)*(byte *)(lVar1 + 0x134);
              iStack_84 = *(int *)(lVar1 + 0x138);
              iStack_88 = *(int *)(lVar1 + 0x13c);
              iStack_8c = *(int *)(lVar1 + 0x140);
              lVar6 = lVar6 + lVar5 * -1000000;
            }
          }
          func_0xffffff80086af874(lVar1 + 0x134);
        }
        else if ((bRamffffff80098a3bda >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4(0xffffff80098a3bb8,0xffffff8009405905);
        }
      }
    }
    else {
      func_0xffffff8008125e6c(0xffffff800932c91c);
    }
    if (*(char *)(lVar1 + 0x1bb) != '\0') {
      func_0xffffff80086afe88(lVar1 + 0xd0,*(undefined4 *)(lVar1 + 0x90),0);
    }
  }
  if (lRamffffff800972db08 == lStack_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
