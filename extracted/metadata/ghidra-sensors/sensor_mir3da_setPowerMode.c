/* Stock Image offsets 0x632750..0x63282c */

undefined8 sensor_mir3da_setPowerMode(undefined8 param_1,byte param_2)

{
  byte bVar1;
  int iVar2;
  undefined8 uVar3;

  _mcount();
  bVar1 = param_2 & 1;
  if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff80092fa304,bVar1);
  }
  if ((bRamffffff80099dab14 == bVar1) && ((bRamffffff80097db2e8 & 1) != 0)) {
    func_0xffffff8008125e6c(0xffffff80093e3da7);
  }
  iVar2 = sensor_mir3da_set_enable(param_1,bVar1);
  if (iVar2 == 0) {
    bRamffffff80099dab14 = bVar1;
    if (*(char *)(lRamffffff80099daad8 + 0x40) != '\0') {
      if ((param_2 & 1) == 0) {
        *(undefined1 *)(lRamffffff80099daad8 + 0x40) = 0;
        return 0;
      }
      if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
        func_0xffffff8008125e6c(0xffffff80093b35a3);
      }
      sensor_mir3da_flush();
    }
    uVar3 = 0;
  }
  else {
    uVar3 = 0xffffffff;
  }
  return uVar3;
}
