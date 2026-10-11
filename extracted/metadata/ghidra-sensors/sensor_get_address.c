/* Stock Image offsets 0x630c60..0x630cac */

ulong sensor_get_address(long param_1)

{
  ulong uVar1;

  _mcount();
  if (param_1 == 0) {
    if ((bRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff8009427470);
    }
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = (ulong)*(ushort *)(param_1 + 2);
  }
  return uVar1;
}
