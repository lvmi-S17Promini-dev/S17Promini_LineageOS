/* Stock Image offsets 0x630cac..0x630d34 */

int sensor_mir3da_local_init(void)

{
  int iVar1;

  _mcount();
  if ((bRamffffff80097db2e8 >> 3 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff80094df247,0x507);
  }
  iVar1 = func_0xffffff8008a89350(0,0xffffff80097db050);
  if (iVar1 == 0) {
    iVar1 = -(~(uint)bRamffffff80099daad4 & 1);
  }
  else {
    if ((bRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff800933dae5);
    }
    iVar1 = -1;
  }
  return iVar1;
}
