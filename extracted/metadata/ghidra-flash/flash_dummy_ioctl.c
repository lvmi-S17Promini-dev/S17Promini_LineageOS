/* Stock Image offsets 0x8c4124..0x8c42dc */

undefined8 flash_dummy_ioctl(uint param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  undefined8 uVar2;

  func_0xffffff800809f01c();
  uVar1 = *param_2;
  if (param_1 == 0x80045373) {
    if ((bRamffffff80098c73fa >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c73d8,0xffffff80093224b2,0xffffff80093224a6,uVar1,param_2[1]);
    }
    if (param_2[1] != 1) {
      flash_dummy_pinctrl_set(0);
      func_0xffffff8008149b90(0xffffff8009af00b8);
      return 0;
    }
    if (uRamffffff8009af0100 != 0) {
      func_0xffffff8008149598
                (0xffffff8009af00b8,
                 (ulong)((uRamffffff8009af0100 % 1000) * 1000000) +
                 ((ulong)uRamffffff8009af0100 / 1000) * 1000000000,0,1);
    }
    uVar2 = 1;
  }
  else {
    if (param_1 != 0x8004536e) {
      if (param_1 != 0x80045364) {
        func_0xffffff8008125e6c
                  (0xffffff8009310e5d,0xffffff80093224a6,uVar1,param_1 & 0xff,param_2[1]);
        return 0xffffffe7;
      }
      if ((bRamffffff80098c73aa >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c7388,0xffffff80094e551b,0xffffff80093224a6,uVar1,param_2[1]);
      }
      uRamffffff8009af0100 = param_2[1];
      return 0;
    }
    if ((bRamffffff80098c73d2 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c73b0,0xffffff800940b7a4,0xffffff80093224a6,uVar1,param_2[1]);
    }
    uVar2 = 0;
  }
  flash_dummy_pinctrl_set(uVar2);
  return 0;
}
