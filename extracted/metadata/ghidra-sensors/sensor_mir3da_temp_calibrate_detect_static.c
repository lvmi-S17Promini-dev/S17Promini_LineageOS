/* Stock Image offsets 0x634030..0x634240 */

bool sensor_mir3da_temp_calibrate_detect_static(ushort param_1,ushort param_2,ushort param_3)

{
  short *psVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  ushort uVar9;
  ushort uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  long lVar16;
  uint uVar17;

  _mcount();
  if ((bRamffffff80099dac0a & 1) == 0) {
    lVar16 = 4;
    uRamffffff80099dab40 = param_1;
    uRamffffff80099dab42 = param_1;
    uRamffffff80099dab7c = param_2;
    uRamffffff80099dab7e = param_2;
    uRamffffff80099dabb8 = param_3;
    uRamffffff80099dabba = param_3;
    do {
      uVar10 = uRamffffff80099dabb8;
      uVar9 = uRamffffff80099dab7c;
      *(ushort *)(lVar16 + -0x7ff66254c0) = uRamffffff80099dab40;
      *(ushort *)(lVar16 + -0x7ff6625484) = uVar9;
      *(ushort *)(lVar16 + -0x7ff6625448) = uVar10;
      lVar16 = lVar16 + 2;
    } while (lVar16 != 0x3c);
    bRamffffff80099dac0a = 1;
  }
  else {
    uVar12 = (uint)uRamffffff80099dab40;
    uVar13 = (uint)uRamffffff80099dab7c;
    uVar14 = (uint)uRamffffff80099dabb8;
    lVar16 = 0;
    uVar11 = uVar12;
    uVar17 = uVar13;
    uVar15 = uVar14;
    do {
      sVar3 = *(short *)(lVar16 + -0x7ff66254be);
      sVar4 = *(short *)(lVar16 + -0x7ff6625482);
      *(short *)(lVar16 + -0x7ff66254c0) = sVar3;
      psVar1 = (short *)(lVar16 + -0x7ff6625448);
      *(short *)(lVar16 + -0x7ff6625484) = sVar4;
      sVar5 = *(short *)(lVar16 + -0x7ff6625446);
      if ((short)uVar12 <= sVar3) {
        uVar12 = (int)sVar3;
      }
      sRamffffff80099dabf4 = (short)uVar12;
      if ((short)uVar13 <= sVar4) {
        uVar13 = (int)sVar4;
      }
      sRamffffff80099dabf8 = (short)uVar13;
      if ((short)uVar14 <= sVar5) {
        uVar14 = (int)sVar5;
      }
      sRamffffff80099dabfc = (short)uVar14;
      if ((int)sVar3 <= (int)(short)uVar11) {
        uVar11 = (uint)sVar3;
      }
      sRamffffff80099dac00 = (short)uVar11;
      lVar16 = lVar16 + 2;
      if ((int)sVar4 <= (int)(short)uVar17) {
        uVar17 = (uint)sVar4;
      }
      sRamffffff80099dac04 = (short)uVar17;
      if ((int)sVar5 == (int)(short)uVar15 || (int)sVar5 < (int)(short)uVar15) {
        uVar15 = (int)sVar5;
      }
      sRamffffff80099dac08 = (short)uVar15;
      *psVar1 = sVar5;
      uRamffffff80099dab7a = param_1;
      uRamffffff80099dabb6 = param_2;
      uRamffffff80099dabf2 = param_3;
    } while (lVar16 != 0x3a);
  }
  iVar6 = (int)sRamffffff80099dabf4 - (int)sRamffffff80099dac00;
  iVar7 = (int)sRamffffff80099dabf8 - (int)sRamffffff80099dac04;
  iVar8 = (int)sRamffffff80099dabfc - (int)sRamffffff80099dac08;
  iVar2 = -iVar6;
  if (-1 < iVar6) {
    iVar2 = iVar6;
  }
  iVar6 = -iVar7;
  if (-1 < iVar7) {
    iVar6 = iVar7;
  }
  iVar7 = -iVar8;
  if (-1 < iVar8) {
    iVar7 = iVar8;
  }
  return iVar6 + iVar2 + iVar7 < 0x3c;
}
