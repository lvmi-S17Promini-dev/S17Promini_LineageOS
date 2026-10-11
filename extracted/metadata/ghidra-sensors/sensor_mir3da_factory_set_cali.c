/* Stock Image offsets 0x631f04..0x63214c */

undefined8 sensor_mir3da_factory_set_cali(int *param_1)

{
  int iVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  bool bVar5;
  undefined8 uVar6;
  int iVar7;
  int iVar8;
  long lVar9;
  long lVar10;

  _mcount();
  if ((uRamffffff80097db2e8 >> 3 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff80094059d0,0x3fc);
  }
  if ((uRamffffff80097db2e8 >> 2 & 1) == 0) {
    bVar5 = true;
  }
  else {
    func_0xffffff8008125e6c(0xffffff800948b912,*param_1,param_1[1],param_1[2]);
    bVar5 = (uRamffffff80097db2e8 & 4) == 0;
  }
  iVar7 = 0x400;
  iVar1 = iVar7;
  if (cRamffffff80099dab16 == '\0') {
    iVar1 = 0;
  }
  if (cRamffffff80099dab18 == '\0') {
    iVar7 = 0;
  }
  iVar8 = 0x400;
  if (cRamffffff80099dab1a == '\0') {
    iVar8 = 0;
  }
  iVar1 = (iVar1 * *param_1) / 0x264f;
  iVar7 = (iVar7 * param_1[1]) / 0x264f;
  iVar8 = (iVar8 * param_1[2]) / 0x264f;
  if (!bVar5) {
    func_0xffffff8008125e6c(0xffffff800933db03,iVar1,iVar7,iVar8);
  }
  lVar9 = lRamffffff80099daae0;
  lVar10 = *(long *)(lRamffffff80099daae0 + 0xd8);
  if ((uRamffffff80097db2e8 >> 3 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff800931bae5,0x129);
  }
  if (lVar10 == 0) {
    if (((uRamffffff80097db2e8 & 1) != 0) &&
       (func_0xffffff8008125e6c(0xffffff800930aea9), (uRamffffff80097db2e8 & 1) != 0)) {
      func_0xffffff8008125e6c(0xffffff8009427490);
    }
    uVar6 = 0xffffffff;
  }
  else {
    lVar9 = *(long *)(lVar9 + 0xd8);
    if ((uRamffffff80097db2e8 >> 3 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff80093e3d90,0x11a);
    }
    sVar2 = *(short *)(lVar9 + 0x50);
    sVar3 = *(short *)(lVar9 + 0x52);
    sVar4 = *(short *)(lVar9 + 0x54);
    if ((uRamffffff80097db2e8 >> 2 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff80094ce382,(int)sVar2,(int)sVar3,(int)sVar4,iVar1,iVar7,iVar8)
      ;
    }
    sVar4 = (short)iVar8 + sVar4;
    sVar2 = sVar2 + (short)iVar1;
    sVar3 = sVar3 + (short)iVar7;
    *(short *)(lVar10 + 0x50) = sVar2;
    *(short *)(lVar10 + 0x52) = sVar3;
    *(short *)(lVar10 + 0x54) = sVar4;
    if (((byte)uRamffffff80097db2e8 >> 2 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff80094ac910,(int)sVar2,(int)sVar3,(int)sVar4);
    }
    func_0xffffff8008f8f060(0x418958);
    uVar6 = 0;
  }
  return uVar6;
}
