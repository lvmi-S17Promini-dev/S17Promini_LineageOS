/* Stock Image offsets 0x62d938..0x62db70 */

undefined8 sensor_ps_enable_and_batch(void)

{
  long lVar1;
  int iVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 uVar5;

  _mcount();
  lVar1 = lRamffffff80099da9f8;
  if (*(int *)(lRamffffff80099da9f8 + 0x308) == 0) {
    if (*(int *)(lRamffffff80099da9f8 + 0x30c) != 1) {
      return 0;
    }
    if ((bRamffffff80098a3342 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098a3320,0xffffff80094ce2a4);
    }
    iVar2 = (**(code **)(lVar1 + 0x270))(1);
    if (iVar2 != 0) {
      uVar5 = 0xffffff80094df147;
      goto LAB_ffffff80086ada94;
    }
    if ((bRamffffff80098a336a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098a3348,0xffffff800946a378);
    }
    *(undefined4 *)(lVar1 + 0x308) = 1;
    if (((bRamffffff80098a3392 >> 2 & 1) != 0) &&
       (func_0xffffff80084a0cc4(0xffffff80098a3370,0xffffff80092e9c91), *(int *)(lVar1 + 0x308) != 1
       )) {
      return 0;
    }
  }
  else {
    if (*(int *)(lRamffffff80099da9f8 + 0x308) != 1) {
      return 0;
    }
    if (*(int *)(lRamffffff80099da9f8 + 0x30c) == 0) {
      if ((bRamffffff80098a32ca >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098a32a8,0xffffff800946a363);
      }
      iVar2 = (**(code **)(lVar1 + 0x270))(0);
      if (iVar2 != 0) {
        uVar5 = 0xffffff8009416072;
        goto LAB_ffffff80086ada94;
      }
      if ((bRamffffff80098a32f2 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098a32d0,0xffffff800934e9e9);
      }
      *(undefined4 *)(lVar1 + 0x308) = 0;
      *(undefined8 *)(lVar1 + 0x310) = 0xffffffffffffffff;
      if ((bRamffffff80098a331a >> 2 & 1) == 0) {
        return 0;
      }
      uVar5 = 0xffffff80098a32f8;
      uVar4 = 0xffffff80094057da;
      goto LAB_ffffff80086adae4;
    }
  }
  lVar3 = *(long *)(lVar1 + 0x310);
  if (lVar3 < 0) {
    return 0;
  }
  if ((bRamffffff80098a33ba >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a3398,0xffffff8009448a13);
    lVar3 = *(long *)(lVar1 + 0x310);
  }
  if (*(char *)(lVar1 + 0x2a9) == '\0') {
    uVar5 = 0;
  }
  else {
    uVar5 = *(undefined8 *)(lVar1 + 0x318);
  }
  iVar2 = (**(code **)(lVar1 + 0x280))(0,lVar3,uVar5);
  if (iVar2 != 0) {
    uVar5 = 0xffffff800949c259;
LAB_ffffff80086ada94:
    func_0xffffff8008125e6c(uVar5,iVar2);
    return 0xffffffff;
  }
  if ((bRamffffff80098a33e2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a33c0,0xffffff80092fa201);
  }
  if ((bRamffffff80098a340a >> 2 & 1) == 0) {
    return 0;
  }
  uVar5 = 0xffffff80098a33e8;
  uVar4 = 0xffffff800937056b;
LAB_ffffff80086adae4:
  func_0xffffff80084a0cc4(uVar5,uVar4);
  return 0;
}
