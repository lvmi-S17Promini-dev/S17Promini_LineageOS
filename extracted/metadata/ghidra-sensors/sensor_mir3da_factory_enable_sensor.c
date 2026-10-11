/* Stock Image offsets 0x631c0c..0x631ca0 */

undefined8 sensor_mir3da_factory_enable_sensor(uint param_1)

{
  int iVar1;
  undefined8 uVar2;

  _mcount();
  if (((byte)uRamffffff80097db2e8 >> 3 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff80094ac8c1,0x3b8);
  }
  iVar1 = sensor_mir3da_enable_nodata(param_1 & 1);
  if (iVar1 == 0) {
    if ((uRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff800932c9a5);
    }
    uVar2 = 0;
  }
  else {
    if ((uRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff800931babf,0xffffff80094ac8c1);
    }
    uVar2 = 0xffffffff;
  }
  return uVar2;
}
