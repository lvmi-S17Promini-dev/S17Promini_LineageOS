/* Stock Image offsets 0x63214c..0x6321ec */

undefined8 sensor_mir3da_factory_get_cali(int *param_1)

{
  int iVar1;
  int iVar2;
  long lVar3;

  _mcount();
  lVar3 = lRamffffff80099daad8;
  iVar2 = *(short *)(lRamffffff80099daad8 + 0x50) * 0x264f;
  iVar1 = iVar2 + 0x3ff;
  if (-1 < iVar2) {
    iVar1 = iVar2;
  }
  *param_1 = iVar1 >> 10;
  iVar2 = *(short *)(lVar3 + 0x52) * 0x264f;
  iVar1 = iVar2 + 0x3ff;
  if (-1 < iVar2) {
    iVar1 = iVar2;
  }
  param_1[1] = iVar1 >> 10;
  iVar2 = *(short *)(lVar3 + 0x54) * 0x264f;
  iVar1 = iVar2 + 0x3ff;
  if (-1 < iVar2) {
    iVar1 = iVar2;
  }
  param_1[2] = iVar1 >> 10;
  if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff800948b943);
  }
  return 0;
}
