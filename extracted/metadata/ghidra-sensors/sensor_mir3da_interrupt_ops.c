/* Stock Image offsets 0x633580..0x6335cc */

void sensor_mir3da_interrupt_ops(undefined8 param_1,undefined8 param_2)

{
  _mcount();
  (**(code **)(lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + 0xa8))(param_1,param_2);
  return;
}
