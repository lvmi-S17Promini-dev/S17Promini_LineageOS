/* Stock Image offsets 0x877044..0x87721c */

undefined8 imx278_open(void)

{
  long lVar1;
  int iVar2;
  long lVar3;
  undefined1 uVar4;
  char cVar5;

  func_0xffffff800809f01c();
  if ((bRamffffff80098c1162 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c1140,0xffffff80094a1a1e,0xffffff8009487c60);
  }
  if ((bRamffffff80098c118a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c1168,0xffffff80093430fc,0xffffff8009487c60);
  }
  uVar4 = 0x34;
  lVar3 = 0;
  do {
    func_0xffffff8008fa96b8(0xffffff8009adc50c);
    uRamffffff8009805734 = uVar4;
    func_0xffffff8008fa9838(0xffffff8009adc50c);
    cVar5 = '\x02';
    do {
      iVar2 = imx278_return_sensor_id();
      if (iVar2 == 0x278) {
        if ((bRamffffff80098c11b2 >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4
                    (0xffffff80098c1190,0xffffff8009375f7f,0xffffff8009487c60,uRamffffff8009805734,
                     0x278);
        }
        imx278_sensor_init();
        func_0xffffff8008fa96b8(0xffffff8009adc50c);
        uRamffffff8009adc510 = 480000000;
        uRamffffff8009adc518 = 1;
        uRamffffff8009adc514 = 0xc80;
        uRamffffff8009adc51c = 0xc80;
        uRamffffff8009adc520 = 0;
        uRamffffff8009805730 = 300;
        func_0xffffff8008fa9838(0xffffff8009adc50c);
        return 0;
      }
      if ((bRamffffff80098c11da >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c11b8,0xffffff80094d3689,0xffffff8009487c60,uRamffffff8009805734,
                   iVar2);
      }
      cVar5 = cVar5 + -1;
    } while (cVar5 != '\0');
    lVar1 = lVar3 + 1;
    uVar4 = *(undefined1 *)(lVar3 + -0x7ff6edb0b8);
    lVar3 = lVar1;
  } while (lVar1 != 3);
  return 0x10;
}
