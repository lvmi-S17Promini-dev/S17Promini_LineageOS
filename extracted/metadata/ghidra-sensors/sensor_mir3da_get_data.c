/* Stock Image offsets 0x6316d8..0x631b98 */

undefined8
sensor_mir3da_get_data(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined4 *param_4)

{
  long lVar1;
  int iVar2;
  int iVar3;
  char cVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  bool bVar8;
  int iVar9;
  ulong uVar10;
  ulong uVar11;
  short sVar12;
  short sVar13;
  ulong uVar14;
  char cVar15;
  short sVar16;
  long lVar17;
  undefined1 auStack_17c [256];
  int aiStack_7c [3];
  long lStack_70;

  lStack_70 = lRamffffff800972db08;
  _mcount();
  func_0xffffff800811eae0(0xffffff80099daaf8);
  lVar1 = lRamffffff80099daae0;
  if (lRamffffff80099daae0 == 0) {
    auStack_17c[0] = 0;
    goto LAB_ffffff80086b19f8;
  }
  lVar17 = *(long *)(lRamffffff80099daae0 + 0xd8);
  if (cRamffffff80099dab14 == '\0') {
    iVar9 = sensor_mir3da_setPowerMode(lRamffffff80099daae0,1);
    if ((iVar9 != 0) && ((uRamffffff80097db2e8 & 1) != 0)) {
      func_0xffffff8008125e6c(0xffffff80094161a3,iVar9);
    }
    func_0xffffff80081489d0(0x14);
  }
  iVar9 = sensor_mir3da_read_data(lVar1,lVar17 + 0x60,lVar17 + 0x62,lVar17 + 100);
  if (iVar9 != 0) {
    if ((uRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff8009381488,iVar9);
    }
    goto LAB_ffffff80086b19f8;
  }
  if ((uRamffffff80097db2e8 >> 2 & 1) != 0) {
    func_0xffffff8008125e6c
              (0xffffff800946a58c,*(undefined1 *)(lVar17 + 0x3c),*(undefined1 *)(lVar17 + 0x3d),
               *(undefined1 *)(lVar17 + 0x3e),(long)*(char *)(lVar17 + 0x38),
               (long)*(char *)(lVar17 + 0x39),(long)*(char *)(lVar17 + 0x3a));
    if (((byte)uRamffffff80097db2e8 >> 2 & 1) != 0) {
      func_0xffffff8008125e6c
                (0xffffff80093b35e5,(long)*(short *)(lVar17 + 0x60),(long)*(short *)(lVar17 + 0x62),
                 (long)*(short *)(lVar17 + 100),(long)*(short *)(lVar17 + 0x50),
                 (long)*(short *)(lVar17 + 0x52),(long)*(short *)(lVar17 + 0x54));
    }
  }
  uVar10 = (ulong)*(byte *)(lVar17 + 0x3c);
  lVar1 = lVar17 + 0x50;
  cVar15 = *(char *)(lVar17 + 0x38);
  uVar11 = (ulong)*(byte *)(lVar17 + 0x3d);
  cVar4 = *(char *)(lVar17 + 0x39);
  sVar12 = *(short *)(lVar17 + 0x60) + *(short *)(lVar1 + uVar10 * 2) * (short)cVar15;
  *(short *)(lVar17 + 0x60) = sVar12;
  uVar14 = (ulong)*(byte *)(lVar17 + 0x3e);
  cVar5 = *(char *)(lVar17 + 0x3a);
  sVar13 = *(short *)(lVar17 + 0x62) + *(short *)(lVar1 + uVar11 * 2) * (short)cVar4;
  *(short *)(lVar17 + 0x62) = sVar13;
  sVar16 = *(short *)(lVar17 + 100) + *(short *)(lVar1 + uVar14 * 2) * (short)cVar5;
  *(short *)(lVar17 + 100) = sVar16;
  if (((byte)uRamffffff80097db2e8 >> 2 & 1) == 0) {
    bVar8 = true;
  }
  else {
    func_0xffffff8008125e6c(0xffffff800934eafc,(int)sVar12,(int)sVar13,(int)sVar16);
    cVar15 = *(char *)(lVar17 + 0x38);
    sVar12 = *(short *)(lVar17 + 0x60);
    uVar10 = (ulong)*(byte *)(lVar17 + 0x3c);
    cVar4 = *(char *)(lVar17 + 0x39);
    sVar13 = *(short *)(lVar17 + 0x62);
    uVar11 = (ulong)*(byte *)(lVar17 + 0x3d);
    cVar5 = *(char *)(lVar17 + 0x3a);
    sVar16 = *(short *)(lVar17 + 100);
    uVar14 = (ulong)*(byte *)(lVar17 + 0x3e);
    bVar8 = (uRamffffff80097db2e8 & 4) == 0;
  }
  aiStack_7c[uVar10] = (int)cVar15 * (int)sVar12;
  aiStack_7c[uVar11] = (int)cVar4 * (int)sVar13;
  aiStack_7c[uVar14] = (int)cVar5 * (int)sVar16;
  if (!bVar8) {
    func_0xffffff8008125e6c(0xffffff80093814ac,aiStack_7c[uVar10],aiStack_7c[uVar11]);
  }
  sVar12 = *(short *)(lVar17 + 0x54);
  iVar9 = -(int)sVar12;
  if (-1 < sVar12) {
    iVar9 = (int)sVar12;
  }
  if (0x514 < iVar9) {
    aiStack_7c[*(byte *)(lVar17 + 0x3e)] = aiStack_7c[*(byte *)(lVar17 + 0x3e)] + -0x800;
  }
  if ((uRamffffff80097db2e8 >> 4 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff80093e3dfe,aiStack_7c[0],aiStack_7c[1],aiStack_7c[2]);
  }
  aiStack_7c[0] = aiStack_7c[0] * 0x264f;
  iVar6 = aiStack_7c[1] * 0x264f;
  iVar7 = aiStack_7c[2] * 0x264f;
  iVar9 = aiStack_7c[0] + 0x3ff;
  if (-1 < aiStack_7c[0]) {
    iVar9 = aiStack_7c[0];
  }
  iVar9 = iVar9 >> 10;
  iVar2 = iVar6 + 0x3ff;
  if (-1 < iVar6) {
    iVar2 = iVar6;
  }
  iVar2 = iVar2 >> 10;
  iVar3 = iVar7 + 0x3ff;
  if (-1 < iVar7) {
    iVar3 = iVar7;
  }
  iVar3 = iVar3 >> 10;
  aiStack_7c[0] = iVar9;
  aiStack_7c[1] = iVar2;
  aiStack_7c[2] = iVar3;
  if ((uRamffffff80097db2e8 >> 2 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff800931bafd,iVar9,iVar2,iVar3);
  }
  func_0xffffff8008f9fb58(auStack_17c,0xffffff80093f4db3,iVar9,iVar2,iVar3);
  if (((byte)uRamffffff80097db2e8 >> 4 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff80093f4de3,iVar9,iVar2,iVar3);
  }
  if (cRamffffff80099dab10 != '\x01') goto LAB_ffffff80086b19f8;
  iVar3 = -iVar9;
  if (-1 < iVar9) {
    iVar3 = iVar9;
  }
  if (iVar3 < 0x2135) {
    iVar9 = -iVar2;
    if (-1 < iVar2) {
      iVar9 = iVar2;
    }
    if ((((0x1d4c < iVar3) || (iVar7 < -0x3ff && 7000 < iVar9)) ||
        ((5000 < iVar3 && (5000 < iVar9)))) ||
       ((iVar6 - 0x4e2400U >> 0xe < 0x7d && (iVar7 + 0x4e1fffU < 0x4e1c00))))
    goto LAB_ffffff80086b190c;
    uRamffffff80099daa94 = 1;
    iRamffffff80099daa88 = 0;
    bRamffffff80099dab11 = 0;
  }
  else {
LAB_ffffff80086b190c:
    iRamffffff80099daa88 = iRamffffff80099daa88 + 1;
    if (iRamffffff80099daa88 < 4) {
      uRamffffff80099daa94 = ~(uint)bRamffffff80099dab11 & 1;
    }
    else {
      uRamffffff80099daa94 = 0;
      bRamffffff80099dab11 = 1;
    }
  }
  uRamffffff80099daaac = 1;
  uRamffffff80099daab0 = 2;
  iVar9 = sensor_ps_report_interrupt_data(uRamffffff80099daa94);
  if (iVar9 != 0) {
    func_0xffffff8008125e6c(0xffffff800948b970);
  }
LAB_ffffff80086b19f8:
  func_0xffffff800811ec68(0xffffff80099daaf8);
  func_0xffffff8008fa0c70(auStack_17c,0xffffff80092fa2c2,param_1,param_2,param_3);
  *param_4 = 2;
  if (lRamffffff800972db08 == lStack_70) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
