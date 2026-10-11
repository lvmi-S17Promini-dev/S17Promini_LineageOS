/* Stock Image offsets 0x8c07f0..0x8c0974 */

undefined8 stock_flashlight_dev_register(undefined8 param_1,undefined8 param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined8 *puVar4;
  int iVar5;
  undefined8 *puVar6;
  ulong uVar7;
  ulong uVar8;
  long lVar9;
  ulong uVar10;

  func_0xffffff800809f01c();
  uVar10 = (ulong)uRamffffff800912cc38;
  if (0 < (int)uRamffffff800912cc38) {
    uVar8 = 0;
    lVar9 = -0x7ff6ed355c;
    do {
      iVar5 = func_0xffffff8008f8f984(param_1,lVar9,0x20);
      if (iVar5 == 0) {
        uVar1 = *(undefined4 *)(lVar9 + -0xc);
        uVar2 = *(undefined4 *)(lVar9 + -8);
        uVar3 = *(undefined4 *)(lVar9 + -4);
        iVar5 = func_0xffffff8008940544(uVar1,uVar2,uVar3);
        if (iVar5 == 0) {
          func_0xffffff8008125e6c(0xffffff80093c9944,0xffffff800940b63b,lVar9,uVar1,uVar2,uVar3);
          func_0xffffff8008fa5474(0xffffff800980be58);
          puVar6 = (undefined8 *)func_0xffffff800824fbc4(uRamffffff80094fc438,0x6080c0,0x60);
          if (puVar6 == (undefined8 *)0x0) {
            func_0xffffff8008fa54e4(0xffffff800980be58);
            return 0xfffffff4;
          }
          puVar6[2] = param_2;
          func_0xffffff8008f8f300(puVar6 + 3,lVar9 + -0xc,0x34);
          puVar4 = puRamffffff800980be50;
          *(undefined8 *)((long)puVar6 + 0x54) = 0x1ffffffff;
          uVar7 = func_0xffffff800845c4c4(puVar6,puVar4,0xffffff800980be48);
          if ((uVar7 & 1) != 0) {
            puRamffffff800980be50 = puVar6;
            *puVar6 = 0xffffff800980be48;
            puVar6[1] = puVar4;
            *puVar4 = puVar6;
          }
          func_0xffffff8008fa54e4(0xffffff800980be58);
        }
        else {
          func_0xffffff8008125e6c(0xffffff8009365976,0xffffff800940b63b,lVar9);
        }
      }
      uVar8 = uVar8 + 1;
      lVar9 = lVar9 + 0x34;
    } while (uVar8 < uVar10);
  }
  return 0;
}
