/* Stock Image offsets 0x633acc..0x633b04 */

undefined8 sensor_mir3da_install_general_ops(long param_1)

{
  undefined8 uVar1;

  _mcount();
  if (param_1 == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = 0;
    lRamffffff80099dab20 = param_1;
  }
  return uVar1;
}
