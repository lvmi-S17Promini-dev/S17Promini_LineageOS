/* Stock Image offsets 0x6323c0..0x6323f4 */

long sensor_mir3da_reg_data_show(undefined8 param_1,undefined8 param_2)

{
  int iVar1;

  _mcount();
  iVar1 = sensor_mir3da_get_reg_data(uRamffffff80099daaf0,param_2);
  return (long)iVar1;
}
