/* Stock Image offsets 0x8c39c4..0x8c3a94 */

undefined8 flash_dummy_pinctrl_set(int param_1)

{
  undefined8 uVar1;
  ulong uVar2;

  func_0xffffff800809f01c();
  if (uRamffffff8009af0070 < 0xfffffffffffff001) {
    uVar2 = uRamffffff8009af0080;
    if (((param_1 == 1) || (uVar2 = uRamffffff8009af0078, param_1 == 0)) &&
       (uVar2 < 0xfffffffffffff001)) {
      func_0xffffff80084b7318();
    }
    else {
      func_0xffffff8008125e6c(0xffffff8009343c76,0xffffff800941c9ce,0,param_1);
    }
    if ((bRamffffff80098c72ba >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c7298,0xffffff8009343cb0,0xffffff800941c9ce,0,param_1);
    }
    uVar1 = 0;
  }
  else {
    func_0xffffff8008125e6c(0xffffff80093d9b8f,0xffffff800941c9ce);
    uVar1 = 0xffffffff;
  }
  return uVar1;
}
