/* Stock Image offsets 0x87bbdc..0x87bca4 */

void imx278_preview(void)

{
  func_0xffffff800809f01c();
  if ((bRamffffff80098c159a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c1578,0xffffff80093fab45,0xffffff80093430f4);
  }
  func_0xffffff8008fa96b8(0xffffff8009adc50c);
  uRamffffff8009adc510 = 480000000;
  uRamffffff8009adc518 = 1;
  uRamffffff8009adc514 = 0xc80;
  uRamffffff8009adc51c = 0xc80;
  uRamffffff8009adc520 = 0;
  func_0xffffff8008fa9838(0xffffff8009adc50c);
  imx278_preview_setting();
  if ((bRamffffff80098c15c2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c15a0,0xffffff80093a86a0,0xffffff80093430f4);
  }
  return;
}
