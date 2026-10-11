/* Stock Image offsets 0x631430..0x631474 */

undefined8 sensor_ps_get_data(uint *param_1,undefined4 *param_2)

{
  _mcount();
  *param_1 = ~(uint)bRamffffff80099dab11 & 1;
  *param_2 = 2;
  return 0;
}
