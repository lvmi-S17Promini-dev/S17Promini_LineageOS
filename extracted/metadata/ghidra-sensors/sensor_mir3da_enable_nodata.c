/* Stock Image offsets 0x63148c..0x6315a8 */

undefined8 sensor_mir3da_enable_nodata(int param_1)

{
  bool bVar1;
  int iVar2;
  undefined8 uVar3;
  int iVar4;

  _mcount();
  if ((uRamffffff80097db2e8 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff800947b098);
  }
  iVar4 = 3;
  do {
    iVar2 = sensor_mir3da_setPowerMode(uRamffffff80099daae0,param_1 == 1);
    if (iVar2 == 0) {
      if ((uRamffffff80097db2e8 & 1) != 0) {
        func_0xffffff8008125e6c(0xffffff800946a566);
      }
      bVar1 = false;
      goto LAB_ffffff80086b1508;
    }
    if ((uRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff8009459702);
    }
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  bVar1 = true;
LAB_ffffff80086b1508:
  if ((uRamffffff80097db2e8 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff80094ac95f);
  }
  if (bVar1) {
    if ((uRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff80094efcb2);
    }
    uVar3 = 0xffffffff;
  }
  else {
    if ((uRamffffff80097db2e8 >> 2 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff80093d4044);
    }
    if ((uRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff80094bdf41);
    }
    uVar3 = 0;
  }
  return uVar3;
}
