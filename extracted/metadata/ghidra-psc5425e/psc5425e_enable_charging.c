/* Stock Image offsets 0xa4ab40..0xa4abac */

undefined8 psc5425e_enable_charging(undefined8 param_1,uint param_2)

{
  func_0xffffff800809f01c();
  func_0xffffff8008125e6c(0xffffff80093676b7,param_2 & 1);
  if ((param_2 & 1) == 0) {
    psc5425e_config_interface(1,1,1,2);
    func_0xffffff8008125e6c(0xffffff80093676d3);
  }
  else {
    psc5425e_config_interface(1,0,1,2);
  }
  return 0;
}
