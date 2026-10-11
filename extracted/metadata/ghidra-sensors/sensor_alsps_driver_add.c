/* Stock Image offsets 0x62c44c..0x62c574 */

undefined8 sensor_alsps_driver_add(long param_1)

{
  int iVar1;
  undefined8 uVar2;
  long lVar3;

  _mcount();
  if ((bRamffffff80098a2e1a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a2df8,0xffffff800934e960,0xffffff8009427326);
  }
  if (param_1 == 0) {
    uVar2 = 0xffffff80093d3f28;
  }
  else {
    lVar3 = 0;
    do {
      if ((lVar3 == 0) && (lRamffffff80099daa00 == 0)) {
        if ((bRamffffff80098a2e42 >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4(0xffffff80098a2e20,0xffffff80092e9c5e);
        }
        iVar1 = func_0xffffff800864a47c(0xffffff80097da9a8,0);
        if (iVar1 != 0) {
          func_0xffffff8008125e6c(0xffffff800934e96d);
        }
      }
      if (*(long *)(lVar3 + -0x7ff6625600) == 0) {
        *(undefined8 *)(param_1 + 0x18) = 0xffffff80097da9a8;
        *(long *)(lVar3 + -0x7ff6625600) = param_1;
        return 0;
      }
      lVar3 = lVar3 + 8;
    } while (lVar3 != 0x28);
    uVar2 = 0xffffff80094489a5;
  }
  func_0xffffff8008125e6c(uVar2);
  return 0xffffffff;
}
