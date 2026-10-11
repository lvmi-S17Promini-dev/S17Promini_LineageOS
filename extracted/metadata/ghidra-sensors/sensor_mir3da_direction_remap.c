/* Stock Image offsets 0x632a28..0x632ad4 */

undefined4 sensor_mir3da_direction_remap(short *param_1,short *param_2,short *param_3,uint param_4)

{
  long lVar1;
  undefined4 uVar2;
  short sVar3;
  short sVar4;

  _mcount();
  lVar1 = (long)(int)param_4 * 0x10;
  sVar3 = *(short *)(lVar1 + -0x7ff6f014f8);
  sVar4 = *(short *)(lVar1 + -0x7ff6f014f4);
  *param_1 = *param_1 + *param_1 * *(short *)(lVar1 + -0x7ff6f014fc) * -2;
  *param_2 = *param_2 + *param_2 * sVar3 * -2;
  *param_3 = *param_3 + *param_3 * sVar4 * -2;
  if ((0xaaUL >> ((ulong)param_4 & 0x3f) & 1) != 0) {
    sVar3 = *param_1;
    *param_1 = *param_2;
    *param_2 = sVar3;
  }
  uVar2 = 1;
  if ((param_4 & 0xfffffffc) == 4) {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}
