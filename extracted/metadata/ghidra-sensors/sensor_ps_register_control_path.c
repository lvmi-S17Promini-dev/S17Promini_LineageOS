/* Stock Image offsets 0x62d3dc..0x62d524 */

undefined8 sensor_ps_register_control_path(undefined8 *param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  long lVar6;

  _mcount();
  lVar1 = lRamffffff80099da9f8;
  *(undefined8 *)(lRamffffff80099da9f8 + 0x278) = param_1[2];
  *(undefined8 *)(lVar1 + 0x268) = *param_1;
  lVar6 = param_1[1];
  *(long *)(lVar1 + 0x270) = lVar6;
  lVar5 = param_1[3];
  *(long *)(lVar1 + 0x280) = lVar5;
  lVar4 = param_1[4];
  *(long *)(lVar1 + 0x288) = lVar4;
  *(undefined1 *)(lVar1 + 0x2a9) = *(undefined1 *)((long)param_1 + 0x41);
  *(undefined1 *)(lVar1 + 0x2a8) = *(undefined1 *)(param_1 + 8);
  *(undefined8 *)(lVar1 + 0x298) = param_1[6];
  *(undefined8 *)(lVar1 + 0x2a0) = param_1[7];
  *(undefined1 *)(lVar1 + 0x2ab) = *(undefined1 *)((long)param_1 + 0x43);
  *(undefined1 *)(lVar1 + 0x2aa) = *(undefined1 *)((long)param_1 + 0x42);
  if (((lVar6 == 0) || (lVar5 == 0)) || (lVar4 == 0)) {
    if ((bRamffffff80098a2f32 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098a2f10,0xffffff800948b800);
    }
    uVar3 = 0xffffffff;
  }
  else {
    *(undefined1 *)(lVar1 + 0x40) = 7;
    *(undefined8 *)(lVar1 + 0x48) = 0xffffff8009391d8e;
    *(undefined8 *)(lVar1 + 0x50) = 0xffffff80090fe180;
    iVar2 = func_0xffffff80086ab408();
    if (iVar2 == 0) {
      iVar2 = func_0xffffff8008307cdc
                        (*(long *)(lRamffffff80099da9f8 + 0x70) + 0x10,0xffffff80097dabf0);
      if (iVar2 < 0) {
        func_0xffffff8008125e6c(0xffffff8009459587);
        uVar3 = 0xfffffffd;
      }
      else {
        func_0xffffff8008f9779c(*(long *)(lRamffffff80099da9f8 + 0x70) + 0x10,0);
        uVar3 = 0;
      }
    }
    else {
      func_0xffffff8008125e6c(0xffffff80093f4c68);
      func_0xffffff8008125e6c(0xffffff80093f4c68);
      uVar3 = 0xfffffffe;
    }
  }
  return uVar3;
}
