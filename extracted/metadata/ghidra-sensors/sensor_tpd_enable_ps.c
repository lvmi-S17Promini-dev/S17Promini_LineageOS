/* Stock Image offsets 0x631b98..0x631c0c */

void sensor_tpd_enable_ps(int param_1)

{
  undefined8 uVar1;

  _mcount();
  func_0xffffff8008125e6c(0xffffff800946a51f,param_1);
  if (param_1 == 0) {
    uRamffffff80099dab10 = 0;
    func_0xffffff800865aab8(uRamffffff80099daac8);
    uVar1 = 0xffffff80094bdf0e;
  }
  else {
    func_0xffffff800865b1a8(uRamffffff80099daac8);
    uRamffffff80099dab10 = 1;
    uVar1 = 0xffffff80094efc70;
  }
  func_0xffffff8008125e6c(uVar1);
  return;
}
