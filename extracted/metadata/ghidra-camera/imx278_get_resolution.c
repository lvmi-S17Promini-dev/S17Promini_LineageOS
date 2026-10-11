/* Stock Image offsets 0x877308..0x877388 */

undefined8 imx278_get_resolution(undefined8 *param_1)

{
  func_0xffffff800809f01c();
  if ((bRamffffff80098c127a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c1258,0xffffff80093fab45,0xffffff80093a85cf);
  }
  *param_1 = 0x616083406160834;
  param_1[1] = 0x1e0052406160834;
  *(undefined4 *)(param_1 + 2) = 0x30e0578;
  return 0;
}
