/* Stock Image offsets 0xa4abc4..0xa4acb4 */

undefined8 psc5425e_set_current(undefined8 param_1,uint param_2)

{
  undefined4 uVar1;

  func_0xffffff800809f01c();
  if (param_2 < 0xa0281) {
    uVar1 = 0;
  }
  else if (param_2 < 0xbfe51) {
    uVar1 = 1;
  }
  else if (param_2 < 0xffdc1) {
    uVar1 = 2;
  }
  else if (param_2 < 0x120161) {
    uVar1 = 3;
  }
  else if (param_2 < 0x1600d1) {
    uVar1 = 4;
  }
  else if (param_2 < 0x180471) {
    uVar1 = 5;
  }
  else {
    uVar1 = 6;
    if (0x1c03e0 < param_2) {
      uVar1 = 7;
    }
  }
  psc5425e_config_interface(4,uVar1,7,4);
  func_0xffffff8008125e6c(0xffffff800943f61d,param_2,uVar1);
  return 1;
}
