/* Stock Image offsets 0x633b04..0x633be0 */

undefined8 sensor_mir3da_core_init(undefined8 param_1)

{
  int iVar1;
  undefined8 uVar2;

  _mcount();
  uRamffffff80099dab28 = 0xffffff80097db2f8;
  if ((int)uRamffffff80097db2ec < 0) {
    iVar1 = sensor_mir3da_module_detect(param_1);
    if (iVar1 != 0) {
      if ((bRamffffff80097db2e8 & 1) == 0) {
        return 0;
      }
      uVar2 = 0xffffff8009391f7e;
      goto LAB_ffffff80086b3bb4;
    }
    if ((int)uRamffffff80097db2ec < 0) {
      return 0;
    }
  }
  if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
    (**(code **)(lRamffffff80099dab20 + 0x40))
              (0xffffff80094161fa,(ulong)uRamffffff80097db2ec * 0xb8 + -0x7ff6824d08);
  }
  iVar1 = sensor_mir3da_chip_resume(param_1);
  if (iVar1 == 0) {
    return param_1;
  }
  if ((bRamffffff80097db2e8 & 1) == 0) {
    return 0;
  }
  uVar2 = 0xffffff800934eb2b;
LAB_ffffff80086b3bb4:
  (**(code **)(lRamffffff80099dab20 + 0x40))(uVar2);
  return 0;
}
