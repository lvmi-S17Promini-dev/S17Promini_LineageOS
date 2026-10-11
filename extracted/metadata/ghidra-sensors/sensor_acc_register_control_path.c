/* Stock Image offsets 0x62f75c..0x62f874 */

undefined8 sensor_acc_register_control_path(long param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  long lVar6;

  _mcount();
  lVar1 = lRamffffff80099daa70;
  lVar6 = *(long *)(param_1 + 8);
  *(long *)(lRamffffff80099daa70 + 0x170) = lVar6;
  lVar5 = *(long *)(param_1 + 0x18);
  *(long *)(lVar1 + 0x180) = lVar5;
  lVar4 = *(long *)(param_1 + 0x20);
  *(long *)(lVar1 + 0x188) = lVar4;
  *(undefined8 *)(lVar1 + 400) = *(undefined8 *)(param_1 + 0x28);
  *(undefined1 *)(lVar1 + 0x199) = *(undefined1 *)(param_1 + 0x31);
  *(undefined1 *)(lVar1 + 0x198) = *(undefined1 *)(param_1 + 0x30);
  if (((lVar6 == 0) || (lVar5 == 0)) || (lVar4 == 0)) {
    if ((bRamffffff80098a3702 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098a36e0,0xffffff800932c899);
    }
    uVar3 = 0xffffffff;
  }
  else {
    *(undefined1 *)(lVar1 + 8) = 0;
    *(undefined8 *)(lVar1 + 0x10) = 0xffffff800935fb7d;
    *(undefined8 *)(lVar1 + 0x18) = 0xffffff80090fe690;
    iVar2 = func_0xffffff80086ab408();
    if (iVar2 == 0) {
      iVar2 = func_0xffffff8008307cdc
                        (*(long *)(lRamffffff80099daa70 + 0x38) + 0x10,0xffffff80097daeb8);
      if (iVar2 < 0) {
        func_0xffffff8008125e6c(0xffffff80094bde22);
        uVar3 = 0xfffffffd;
      }
      else {
        func_0xffffff8008f9779c(*(long *)(lRamffffff80099daa70 + 0x38) + 0x10,0);
        uVar3 = 0;
      }
    }
    else {
      func_0xffffff8008125e6c(0xffffff80093a2a9d);
      func_0xffffff8008125e6c(0xffffff80093a2a9d);
      uVar3 = 0xfffffffe;
    }
  }
  return uVar3;
}
