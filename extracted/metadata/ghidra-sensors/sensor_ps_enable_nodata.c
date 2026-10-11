/* Stock Image offsets 0x631384..0x6313e8 */

undefined8 sensor_ps_enable_nodata(int param_1)

{
  _mcount();
  if (param_1 == 0) {
    sensor_tpd_enable_ps(0);
    func_0xffffff800844ab48(4,lRamffffff80099daae8 + 0x30);
  }
  else {
    sensor_tpd_enable_ps(1);
    func_0xffffff800844abf4(4,lRamffffff80099daae8 + 0x30);
  }
  return 0;
}
