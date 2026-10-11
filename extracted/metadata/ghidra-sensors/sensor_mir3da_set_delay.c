/* Stock Image offsets 0x631668..0x6316d8 */

undefined8 sensor_mir3da_set_delay(int param_1)

{
  _mcount();
  if ((uRamffffff80097db2e8 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff80092fa333);
  }
  if ((uRamffffff80097db2e8 >> 2 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff80093c33db,param_1 / 1000000);
  }
  return 0;
}
