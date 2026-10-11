/* Stock Image offsets 0xa4af40..0xa4afe8 */

undefined8 psc5425e_enable_otg(undefined8 param_1,uint param_2)

{
  func_0xffffff800809f01c();
  func_0xffffff8008125e6c(0xffffff8009461c72,0xffffff800943f650,param_2 & 1);
  if ((param_2 & 1) == 0) {
    psc5425e_config_interface(1,0,1,0);
    uRamffffff8009c0c758 = 0;
  }
  else {
    psc5425e_config_interface(1,1,1,0);
    uRamffffff8009c0c758 = 1;
    func_0xffffff8008125e6c(0xffffff800949386c);
    func_0xffffff8008149598(0xffffff8009c0c778,10000000000,0,1);
  }
  return 0;
}
