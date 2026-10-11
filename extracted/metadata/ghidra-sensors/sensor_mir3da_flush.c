/* Stock Image offsets 0x6315dc..0x631668 */

int sensor_mir3da_flush(void)

{
  int iVar1;

  _mcount();
  if ((bRamffffff80097db2e8 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff8009370705);
  }
  if (cRamffffff80099dab14 == '\0') {
    iVar1 = 0;
    *(undefined1 *)(lRamffffff80099daad8 + 0x40) = 1;
  }
  else {
    iVar1 = func_0xffffff80086afa24();
    if (-1 < iVar1) {
      *(undefined1 *)(lRamffffff80099daad8 + 0x40) = 0;
    }
    if ((bRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff80093e3de3);
    }
  }
  return iVar1;
}
