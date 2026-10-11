/* Stock Image offsets 0x631e80..0x631f04 */

undefined8 sensor_mir3da_factory_clear_cali(void)

{
  long lVar1;

  _mcount();
  if ((bRamffffff80097db2e8 >> 3 & 1) == 0) {
    lVar1 = *(long *)(lRamffffff80099daae0 + 0xd8);
  }
  else {
    func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff80094ac8de,0x3ed);
    lVar1 = *(long *)(lRamffffff80099daae0 + 0xd8);
    if ((bRamffffff80097db2e8 >> 3 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff80094ac8f8,0x10e);
    }
  }
  *(undefined8 *)(lVar1 + 0x50) = 0;
  *(undefined8 *)(lVar1 + 0x58) = 0;
  return 0;
}
