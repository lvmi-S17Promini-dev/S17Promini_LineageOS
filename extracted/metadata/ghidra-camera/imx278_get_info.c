/* Stock Image offsets 0x87721c..0x877308 */

undefined8 imx278_get_info(undefined4 param_1,long param_2)

{
  func_0xffffff800809f01c();
  if ((bRamffffff80098c1252 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c1230,0xffffff80093e957f,0xffffff8009480177,param_1);
  }
  *(undefined1 *)(param_2 + 0x16) = 1;
  *(undefined8 *)(param_2 + 0x18) = 0x500000000;
  *(undefined8 *)(param_2 + 0x20) = 1;
  *(undefined2 *)(param_2 + 0xd) = 0x101;
  *(undefined2 *)(param_2 + 0x14) = 0x101;
  *(undefined8 *)(param_2 + 0x34) = 0x300000003;
  *(undefined8 *)(param_2 + 0x2c) = 0x200000002;
  *(undefined1 *)(param_2 + 0xdd) = 2;
  *(undefined2 *)(param_2 + 0xf1) = 10;
  *(undefined4 *)(param_2 + 0x3c) = 3;
  *(undefined2 *)(param_2 + 0xd8) = 3;
  *(undefined4 *)(param_2 + 0x28) = 3;
  *(undefined1 *)(param_2 + 8) = 0x18;
  *(undefined1 *)(param_2 + 0x11) = 5;
  *(undefined2 *)(param_2 + 0xf) = 0x200;
  *(undefined2 *)(param_2 + 0x12) = 0x203;
  *(undefined4 *)(param_2 + 0xec) = 0;
  *(undefined4 *)(param_2 + 0xf8) = 0;
  *(undefined1 *)(param_2 + 0xda) = 0;
  *(undefined2 *)(param_2 + 0xdb) = 0;
  *(undefined2 *)(param_2 + 0xf4) = 0;
  *(undefined2 *)(param_2 + 0xdf) = 0x5500;
  *(undefined4 *)(param_2 + 0xe6) = 0x1000000;
  *(undefined4 *)(param_2 + 0x84) = 0;
  return 0;
}
