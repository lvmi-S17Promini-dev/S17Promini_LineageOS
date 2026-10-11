/* Stock Image offsets 0x153da68..0x153ddc8 */

undefined8 sensor_acc_init(void)

{
  byte bVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  undefined8 *puVar5;
  ulong uVar6;

  _mcount();
  if ((bRamffffff80098a3b12 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a3af0,0xffffff80093f4d17,0xffffff800931bab6);
  }
  if ((bRamffffff80098a3b3a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a3b18,0xffffff800948b8a6);
  }
  lVar3 = func_0xffffff800824fbc4(uRamffffff80094fc448,0x6080c0,0x1d8);
  if ((bRamffffff80098a3b8a >> 2 & 1) == 0) {
    if (lVar3 == 0) goto LAB_ffffff80095bdd0c;
LAB_ffffff80095bdacc:
    *(undefined4 *)(lVar3 + 0x90) = 200;
    *(undefined4 *)(lVar3 + 0x94) = 0;
    *(undefined8 *)(lVar3 + 0x40) = 0xfffffffe0;
    *(long *)(lVar3 + 0x48) = lVar3 + 0x48;
    *(long *)(lVar3 + 0x50) = lVar3 + 0x48;
    *(code **)(lVar3 + 0x58) = sensor_acc_work_func;
    *(undefined8 *)(lVar3 + 0x128) = 0;
    lVar4 = func_0xffffff80080d4fd4(0xffffff8009338f3a,0x40008,1,0,0,0xffffff8009459667);
    *(long *)(lVar3 + 0x128) = lVar4;
    if (lVar4 != 0) {
      func_0xffffff8008149e9c(lVar3 + 0xd0,1,0);
      *(undefined8 *)(lVar3 + 0xf8) = 0xffffff80086aef70;
      *(undefined4 *)(lVar3 + 0x1b8) = 0;
      func_0xffffff800811e840(lVar3 + 0x70,0xffffff8009370673,0xffffff80099daa7c);
      *(undefined1 *)(lVar3 + 0x1bc) = 0;
      *(undefined8 *)(lVar3 + 0x154) = 0;
      bVar1 = bRamffffff80098a3bb2;
      *(undefined4 *)(lVar3 + 0x15c) = 0;
      *(undefined8 *)(lVar3 + 0x1c0) = 0;
      *(undefined8 *)(lVar3 + 0x1c8) = 0xffffffffffffffff;
      *(undefined8 *)(lVar3 + 0x1d0) = 0xffffffffffffffff;
      if ((bVar1 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098a3b90,0xffffff8009448a8b,0xffffff80093d3f83);
      }
      lRamffffff80099daa70 = lVar3;
      if ((bRamffffff80098a3c02 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098a3be0,0xffffff8009381433,0xffffff80093a2b3f);
      }
      uVar6 = 0;
      do {
        if ((bRamffffff80098a3c2a >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4(0xffffff80098a3c08,0xffffff80093f4d49,uVar6 & 0xffffffff);
        }
        puVar5 = *(undefined8 **)(uVar6 * 8 + -0x7ff66255b8);
        if (puVar5 != (undefined8 *)0x0) {
          if ((bRamffffff80098a3c52 >> 2 & 1) != 0) {
            func_0xffffff80084a0cc4(0xffffff80098a3c30,0xffffff80094058d9,*puVar5);
            puVar5 = *(undefined8 **)(uVar6 * 8 + -0x7ff66255b8);
          }
          iVar2 = (*(code *)puVar5[1])();
          if (iVar2 == 0) {
            if ((bRamffffff80098a3c7a >> 2 & 1) != 0) {
              func_0xffffff80084a0cc4
                        (0xffffff80098a3c58,0xffffff800932c944,
                         **(undefined8 **)(uVar6 * 8 + -0x7ff66255b8));
            }
            if ((bRamffffff80098a3b62 >> 2 & 1) != 0) {
              func_0xffffff80084a0cc4(0xffffff80098a3b40,0xffffff800949c2cd);
            }
            return 0;
          }
        }
        uVar6 = uVar6 + 1;
      } while (uVar6 != 5);
      if ((bRamffffff80098a3ca2 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098a3c80,0xffffff800941618a,0xffffff80093a2b3f);
      }
      func_0xffffff8008125e6c(0xffffff80093e3d3c);
      func_0xffffff8008251b9c(lRamffffff80099daa70);
      goto LAB_ffffff80095bdc50;
    }
    func_0xffffff8008251b9c(lVar3);
  }
  else {
    func_0xffffff80084a0cc4(0xffffff80098a3b68,0xffffff8009381433,0xffffff80093d3f83);
    if (lVar3 != 0) goto LAB_ffffff80095bdacc;
LAB_ffffff80095bdd0c:
    func_0xffffff8008125e6c(0xffffff8009370646);
  }
  lRamffffff80099daa70 = 0;
  func_0xffffff8008125e6c(0xffffff8009459639);
LAB_ffffff80095bdc50:
  func_0xffffff8008125e6c(0xffffff800934eab3);
  func_0xffffff8008125e6c(0xffffff800949c29c);
  return 0xffffffed;
}
