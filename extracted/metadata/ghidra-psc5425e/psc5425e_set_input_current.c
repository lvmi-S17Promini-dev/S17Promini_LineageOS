/* Stock Image offsets 0xa4ad58..0xa4adb0 */

undefined8 psc5425e_set_input_current(undefined8 param_1,uint param_2)

{
  undefined4 uVar1;

  func_0xffffff800809f01c();
  uVar1 = 3;
  if (param_2 < 0xc351) {
    uVar1 = 1;
  }
  psc5425e_config_interface(1,uVar1,3,6);
  func_0xffffff8008125e6c(0xffffff80094c59fc,uVar1);
  return 1;
}
