/* Stock Image offsets 0x8c407c..0x8c40c4 */

void flash_dummy_work_disable(void)

{
  func_0xffffff800809f01c();
  if ((bRamffffff80098c7382 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c7360,0xffffff80094c3ec1,0xffffff80093d9bc7);
  }
  flash_dummy_pinctrl_set(0);
  return;
}
