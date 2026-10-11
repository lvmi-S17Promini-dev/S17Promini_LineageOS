/* Stock Image offsets 0x153d7cc..0x153da08 */

undefined8 sensor_alsps_init(void)

{
  int iVar1;
  int iVar2;
  long lVar3;
  byte bVar4;
  long lVar5;

  _mcount();
  if ((bRamffffff80098a34d2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a34b0,0xffffff800934e960,0xffffff80094efb65);
  }
  if ((bRamffffff80098a34fa >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a34d8,0xffffff800948b876,0xffffff80094057f4);
  }
  lVar5 = func_0xffffff800824fbc4(uRamffffff80094fc450,0x6080c0,800);
  if ((bRamffffff80098a3522 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a3500,0xffffff80092fa24c,0xffffff800946a39b);
  }
  if (lVar5 == 0) {
    func_0xffffff8008125e6c(0xffffff80093813c2);
    lRamffffff80099da9f8 = 0;
    func_0xffffff8008125e6c(0xffffff80094273b4);
    lVar5 = lRamffffff80099da9f8;
  }
  else {
    *(undefined4 *)(lVar5 + 0x16c) = 200;
    *(undefined4 *)(lVar5 + 0x170) = 200;
    *(undefined4 *)(lVar5 + 0x174) = 0;
    *(undefined8 *)(lVar5 + 0xa8) = 0xfffffffe0;
    *(long *)(lVar5 + 0xb0) = lVar5 + 0xb0;
    *(undefined8 *)(lVar5 + 0x78) = 0xfffffffe0;
    *(long *)(lVar5 + 0xb8) = lVar5 + 0xb0;
    *(undefined8 *)(lVar5 + 0xc0) = 0xffffff80086adcf8;
    *(long *)(lVar5 + 0x80) = lVar5 + 0x80;
    *(long *)(lVar5 + 0x88) = lVar5 + 0x80;
    *(undefined8 *)(lVar5 + 0x90) = 0xffffff80086ade28;
    func_0xffffff8008146c94(lVar5 + 0xf8,0xffffff80086adf84,0,0,0);
    func_0xffffff8008146c94(lVar5 + 0x130,0xffffff80086adfc8,0,0,0);
    iVar2 = *(int *)(lVar5 + 0x16c);
    iVar1 = iVar2 + 3;
    if (-1 < iVar2) {
      iVar1 = iVar2;
    }
    *(long *)(lVar5 + 0x108) = lRamffffff8009726980 + (iVar1 >> 2);
    lVar3 = lRamffffff8009726980;
    iVar2 = *(int *)(lVar5 + 0x170);
    *(undefined4 *)(lVar5 + 0x2cc) = 0;
    iVar1 = iVar2 + 3;
    if (-1 < iVar2) {
      iVar1 = iVar2;
    }
    *(long *)(lVar5 + 0x140) = lVar3 + (iVar1 >> 2);
    func_0xffffff800811e840(lVar5 + 0xd8,0xffffff8009370591,0xffffff80099daa30);
    bVar4 = bRamffffff80098a354a;
    *(undefined2 *)(lVar5 + 0x2d0) = 0;
    *(undefined4 *)(lVar5 + 0x2d4) = 0;
    *(undefined4 *)(lVar5 + 0x2dc) = 0;
    *(undefined8 *)(lVar5 + 0x2e8) = 0xffffffffffffffff;
    *(undefined8 *)(lVar5 + 0x2f0) = 0xffffffffffffffff;
    *(undefined8 *)(lVar5 + 0x308) = 0;
    *(undefined8 *)(lVar5 + 0x310) = 0xffffffffffffffff;
    *(undefined8 *)(lVar5 + 0x318) = 0xffffffffffffffff;
    if ((bVar4 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098a3528,0xffffff8009391d98,0xffffff800946a39b);
    }
  }
  lRamffffff80099da9f8 = lVar5;
  func_0xffffff8008125e6c(0xffffff80094ce2e9,0xffffff80094057f4);
  func_0xffffff8008125e6c(0xffffff80094ce2bd);
  return 0xffffffed;
}
