/* Stock Image offsets 0x63282c..0x632884 */

void sensor_squareRoot(int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  ulong uVar5;
  uint uVar6;

  _mcount();
  uVar4 = 0;
  if (-1 < param_1) {
    uVar5 = 0xfffffffffffffffe;
    do {
      uVar5 = uVar5 + 2;
      uVar6 = (uint)(0x40000000L >> (uVar5 & 0x3f));
      iVar1 = uVar4 + uVar6;
      iVar2 = 0;
      if (iVar1 <= param_1) {
        iVar2 = iVar1;
      }
      uVar3 = 0;
      if (iVar1 <= param_1) {
        uVar3 = uVar6;
      }
      param_1 = param_1 - iVar2;
      uVar4 = uVar3 | (int)uVar4 >> 1;
    } while (uVar5 < 0x1e);
  }
  return;
}
