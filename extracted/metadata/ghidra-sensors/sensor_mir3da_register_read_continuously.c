/* Stock Image offsets 0x6328d0..0x63292c */

bool sensor_mir3da_register_read_continuously
               (undefined8 param_1,undefined4 param_2,uint param_3,undefined8 param_4)

{
  uint uVar1;

  _mcount();
  uVar1 = (**(code **)(lRamffffff80099dab20 + 0x10))(param_1,param_2,param_3,param_4);
  return uVar1 != (param_3 & 0xff);
}
