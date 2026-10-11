/* Stock Image offsets 0x8766b8..0x876a64 */

undefined8 gc5025sub_control(undefined4 param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;

  func_0xffffff800809f01c();
  if ((bRamffffff80098c0f32 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c0f10,0xffffff8009331fa5,0xffffff8009499190,param_1);
  }
  func_0xffffff8008fa96b8(0xffffff8009adbffc);
  func_0xffffff8008fa9838(0xffffff8009adbffc);
  switch(param_1) {
  case 0:
    gc5025sub_preview();
    return 0;
  case 1:
    if ((bRamffffff80098c0fd2 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c0fb0,0xffffff800943d09d,0xffffff80093f1c33);
    }
    func_0xffffff8008fa96b8(0xffffff8009adbffc);
    if (((sRamffffff8009805698 != 0xf0) && (sRamffffff8009805698 != 300)) &&
       ((bRamffffff80098c0ffa >> 2 & 1) != 0)) {
      func_0xffffff80084a0cc4
                (0xffffff80098c0fd8,0xffffff80092ff5e2,0xffffff80093f1c33,sRamffffff8009805698,0x1e)
      ;
    }
    uRamffffff8009adc4f8 = 1;
    uRamffffff8009adc500 = 1;
    uRamffffff8009adc4fc = 0x3dc;
    uRamffffff8009adc504 = 0x3dc;
    uRamffffff8009adc508 = 0;
    func_0xffffff8008fa9838(0xffffff8009adbffc);
    if ((bRamffffff80098c1022 >> 2 & 1) == 0) {
      return 0;
    }
    uVar1 = 0xffffff80098c1000;
    uVar2 = 0xffffff80094a1a0e;
    break;
  case 2:
    if ((bRamffffff80098c104a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c1028,0xffffff800943d09d,0xffffff8009331fd3);
    }
    func_0xffffff8008fa96b8(0xffffff8009adbffc);
    uRamffffff8009adc4f8 = 1;
    uRamffffff8009adc500 = 1;
    uRamffffff8009adc4fc = 0x3dc;
    uRamffffff8009adc504 = 0x3dc;
    uRamffffff8009adc508 = 0;
    func_0xffffff8008fa9838(0xffffff8009adbffc);
    if ((bRamffffff80098c1072 >> 2 & 1) == 0) {
      return 0;
    }
    uVar1 = 0xffffff80098c1050;
    uVar2 = 0xffffff80093e9557;
    break;
  case 3:
    if ((bRamffffff80098c109a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c1078,0xffffff800943d09d,0xffffff80093d9385);
    }
    func_0xffffff8008fa96b8(0xffffff8009adbffc);
    uRamffffff8009adc4f8 = 1;
    uRamffffff8009adc500 = 1;
    uRamffffff8009adc4fc = 0x3dc;
    uRamffffff8009adc504 = 0x3dc;
    uRamffffff8009adc508 = 0;
    func_0xffffff8008fa9838(0xffffff8009adbffc);
    if ((bRamffffff80098c10c2 >> 2 & 1) == 0) {
      return 0;
    }
    uVar1 = 0xffffff80098c10a0;
    uVar2 = 0xffffff80093b8ab9;
    goto code_r0xffffff80088f6a38;
  case 4:
    if ((bRamffffff80098c10ea >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c10c8,0xffffff800943d09d,0xffffff800945f089);
    }
    func_0xffffff8008fa96b8(0xffffff8009adbffc);
    uRamffffff8009adc4f8 = 1;
    uRamffffff8009adc500 = 1;
    uRamffffff8009adc4fc = 0x3dc;
    uRamffffff8009adc504 = 0x3dc;
    uRamffffff8009adc508 = 0;
    func_0xffffff8008fa9838(0xffffff8009adbffc);
    if ((bRamffffff80098c1112 >> 2 & 1) == 0) {
      return 0;
    }
    uVar1 = 0xffffff80098c10f0;
    uVar2 = 0xffffff80093e956c;
code_r0xffffff80088f6a38:
    func_0xffffff80084a0cc4(uVar1,0xffffff800943d09d,uVar2);
    return 0;
  default:
    if ((bRamffffff80098c0f5a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c0f38,0xffffff80093fab10,0xffffff8009499190);
    }
    gc5025sub_preview();
    return 4;
  }
  func_0xffffff80084a0cc4(uVar1,0xffffff80093650bb,uVar2,sRamffffff8009805698);
  return 0;
}
