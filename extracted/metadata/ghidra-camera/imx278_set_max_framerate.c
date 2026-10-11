/* Stock Image offsets 0x87bb18..0x87bbdc */

void imx278_set_max_framerate(ushort param_1,uint param_2)

{
  uint uVar1;

  func_0xffffff800809f01c();
  if ((bRamffffff80098c13ba >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c1398,0xffffff80093a8653,0xffffff800945f094,(uint)param_1,param_2 & 1);
  }
  uVar1 = 0;
  if (param_1 != 0) {
    uVar1 = uRamffffff8009adc510 / param_1;
  }
  func_0xffffff8008fa96b8(0xffffff8009adc50c);
  uRamffffff8009adc514 = (uVar1 * 10) / 0x1370;
  if ((uVar1 * 10) / 0x1370 <= uRamffffff8009adc51c) {
    uRamffffff8009adc514 = uRamffffff8009adc51c;
  }
  if (0xfffe < uRamffffff8009adc514) {
    uRamffffff8009adc514 = 0xffff;
  }
  if ((param_2 & 1) != 0) {
    uRamffffff8009adc51c = uRamffffff8009adc514;
  }
  func_0xffffff8008fa9838(0xffffff8009adc50c);
  return;
}
