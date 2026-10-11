/* Stock Image offsets 0x8c42dc..0x8c4338 */

undefined8 flash_dummy_strobe_store(long param_1)

{
  func_0xffffff800809f01c();
  flash_dummy_set_driver(1);
  flash_dummy_pinctrl_set(0);
  uRamffffff8009af0100 = 0;
  flash_dummy_pinctrl_set(1);
  func_0xffffff80081489d0(*(undefined4 *)(param_1 + 0x14));
  flash_dummy_pinctrl_set(0);
  flash_dummy_set_driver(0);
  return 0;
}
