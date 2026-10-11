/* Stock Image offsets 0x62fb50..0x62fe88 */

undefined8 sensor_acc_enable_and_batch(void)

{
  long lVar1;
  int iVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 uVar5;

  _mcount();
  lVar1 = lRamffffff80099daa70;
  if (*(int *)(lRamffffff80099daa70 + 0x1c0) == 0) {
    if (*(int *)(lRamffffff80099daa70 + 0x1c4) != 1) {
      return 0;
    }
    if ((bRamffffff80098a3892 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098a3870,0xffffff80093f4d2b);
    }
    if ((*(char *)(lVar1 + 0x1b9) != '\0') || (*(char *)(lVar1 + 0x1b8) != '\0')) {
      iVar2 = (**(code **)(lVar1 + 0x170))(1);
      if (iVar2 != 0) {
        uVar5 = 0xffffff8009427433;
        goto LAB_ffffff80086afd68;
      }
      if ((bRamffffff80098a38ba >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098a3898,0xffffff800946a460);
      }
    }
    *(undefined4 *)(lVar1 + 0x1c0) = 1;
    if (((bRamffffff80098a38e2 >> 2 & 1) != 0) &&
       (func_0xffffff80084a0cc4(0xffffff80098a38c0,0xffffff80093c333d), *(int *)(lVar1 + 0x1c0) != 1
       )) {
      return 0;
    }
  }
  else {
    if (*(int *)(lRamffffff80099daa70 + 0x1c0) != 1) {
      return 0;
    }
    if (*(int *)(lRamffffff80099daa70 + 0x1c4) == 0) {
      if ((bRamffffff80098a37f2 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098a37d0,0xffffff800933da27);
      }
      if (*(char *)(lVar1 + 0x1b9) == '\0') {
        if ((*(char *)(lVar1 + 0x198) == '\0') && (*(char *)(lVar1 + 0x1bb) != '\0')) {
          DataMemoryBarrier(2,3);
          func_0xffffff8008149b90(lVar1 + 0xd0);
          DataMemoryBarrier(2,3);
          func_0xffffff80080d4940(lVar1 + 0x40);
          *(undefined1 *)(lVar1 + 0x1bb) = 0;
          *(undefined8 *)(lVar1 + 0x138) = 0xffffffffffffffff;
          *(undefined4 *)(lVar1 + 0x140) = 0xffffffff;
          if ((bRamffffff80098a381a >> 2 & 1) != 0) {
            func_0xffffff80084a0cc4(0xffffff80098a37f8,0xffffff800932c8c9);
          }
        }
        if ((*(char *)(lVar1 + 0x1b9) == '\0') && (*(char *)(lVar1 + 0x1b8) == '\0')) {
          iVar2 = (**(code **)(lVar1 + 0x170))(0);
          if (iVar2 != 0) {
            uVar5 = 0xffffff80093b352c;
            goto LAB_ffffff80086afd68;
          }
          if ((bRamffffff80098a3842 >> 2 & 1) != 0) {
            func_0xffffff80084a0cc4(0xffffff80098a3820,0xffffff8009459610);
          }
        }
      }
      *(undefined4 *)(lVar1 + 0x1c0) = 0;
      *(undefined8 *)(lVar1 + 0x1c8) = 0xffffffffffffffff;
      if ((bRamffffff80098a386a >> 2 & 1) == 0) {
        return 0;
      }
      uVar5 = 0xffffff80098a3848;
      uVar4 = 0xffffff80094ac85b;
      goto LAB_ffffff80086afdc8;
    }
  }
  lVar3 = *(long *)(lVar1 + 0x1c8);
  if (lVar3 < 0) {
    return 0;
  }
  if ((bRamffffff80098a390a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a38e8,0xffffff8009416131);
    lVar3 = *(long *)(lVar1 + 0x1c8);
  }
  if (*(char *)(lVar1 + 0x199) == '\0') {
    uVar5 = 0;
  }
  else {
    uVar5 = *(undefined8 *)(lVar1 + 0x1d0);
  }
  iVar2 = (**(code **)(lVar1 + 0x180))(0,lVar3,uVar5);
  if (iVar2 != 0) {
    uVar5 = 0xffffff800947affb;
LAB_ffffff80086afd68:
    func_0xffffff8008125e6c(uVar5,iVar2);
    return 0xffffffff;
  }
  if ((bRamffffff80098a3932 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a3910,0xffffff800946a488);
  }
  if ((*(char *)(lVar1 + 0x1b9) != '\0') && (*(char *)(lVar1 + 0x198) == '\0')) {
    *(int *)(lVar1 + 0x90) = (int)(*(ulong *)(lVar1 + 0x1c8) / 1000000);
    if (*(char *)(lVar1 + 0x1bb) == '\0') {
      *(undefined2 *)(lVar1 + 0x1ba) = 0x101;
      func_0xffffff80086afe88(lVar1 + 0xd0,*(undefined4 *)(lVar1 + 0x90),1);
    }
    if ((bRamffffff80098a395a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098a3938,0xffffff800947b028,*(undefined4 *)(lVar1 + 0x90));
    }
  }
  if ((bRamffffff80098a3982 >> 2 & 1) == 0) {
    return 0;
  }
  uVar5 = 0xffffff80098a3960;
  uVar4 = 0xffffff800934ea93;
LAB_ffffff80086afdc8:
  func_0xffffff80084a0cc4(uVar5,uVar4);
  return 0;
}
