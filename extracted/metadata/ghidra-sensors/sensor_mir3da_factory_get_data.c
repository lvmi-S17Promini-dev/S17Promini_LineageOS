/* Stock Image offsets 0x631ca0..0x631d2c */

undefined8 sensor_mir3da_factory_get_data(undefined4 *param_1,undefined8 param_2)

{
  _mcount();
  if ((bRamffffff80097db2e8 >> 3 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff8009381470,0x3cc);
  }
  sensor_mir3da_get_data(param_1,param_1 + 1,param_1 + 2,param_2);
  if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff80093706d8,*param_1,param_1[1],param_1[2]);
  }
  return 0;
}
