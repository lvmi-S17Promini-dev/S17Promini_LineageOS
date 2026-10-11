/* Stock Image offsets 0xa4acb4..0xa4ad40 */

undefined8 psc5425e_set_cv_voltage(undefined8 param_1,uint param_2)

{
  undefined4 uVar1;

  func_0xffffff800809f01c();
  if (param_2 < 0x3e8fa1) {
    uVar1 = 0;
  }
  else if (param_2 < 0x401641) {
    uVar1 = 2;
  }
  else {
    uVar1 = 0x2c;
    if (0x426030 < param_2) {
      uVar1 = 0x2d;
    }
  }
  psc5425e_config_interface(2,uVar1,0x3f,2);
  func_0xffffff8008125e6c(0xffffff80094e728d,uVar1);
  return 1;
}
