/* Stock Image offsets 0x632de0..0x633324 */

int sensor_mir3da_temp_calibrate(int *param_1,int *param_2,int *param_3)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  ulong uVar11;
  uint uVar12;

  _mcount();
  iVar9 = iRamffffff80099dac0c + *param_3;
  *param_3 = iVar9;
  if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
    (**(code **)(lRamffffff80099dab20 + 0x40))(0xffffff80093b3648);
  }
  iVar7 = iRamffffff80099dac0c;
  iVar6 = *param_1;
  iVar9 = iVar9 % 10;
  iVar10 = -iVar6;
  if (-1 < iVar6) {
    iVar10 = iVar6;
  }
  if (iVar10 < 200) {
    iVar2 = *param_2;
    iVar3 = -iVar2;
    if (-1 < iVar2) {
      iVar3 = iVar2;
    }
    if (199 < iVar3) goto LAB_ffffff80086b2ef0;
    iVar6 = sensor_mir3da_temp_calibrate_detect_static(iVar6,iVar2,*param_3 - iRamffffff80099dac0c);
    iVar10 = 0x100000 - (*param_2 * *param_2 + *param_1 * *param_1);
    if (iVar10 < 0) {
      uVar8 = 0;
    }
    else {
      uVar8 = 0;
      uVar11 = 0xfffffffffffffffe;
      do {
        uVar11 = uVar11 + 2;
        uVar12 = (uint)(0x40000000L >> (uVar11 & 0x3f));
        iVar7 = uVar8 + uVar12;
        iVar3 = 0;
        if (iVar7 <= iVar10) {
          iVar3 = iVar7;
        }
        uVar1 = 0;
        if (iVar7 <= iVar10) {
          uVar1 = uVar12;
        }
        iVar10 = iVar10 - iVar3;
        uVar8 = uVar1 | (int)uVar8 >> 1;
      } while (uVar11 < 0x1e);
    }
    iVar9 = uVar8 + iVar9;
    if (iRamffffff80099dac0c == 0) {
      iVar10 = *param_3;
      if (iVar6 == 1) {
        iRamffffff80099dac0c = -iVar9;
        if (-1 < iVar10) {
          iRamffffff80099dac0c = iVar9;
        }
        iRamffffff80099dac0c = iRamffffff80099dac0c - iVar10;
        iVar7 = iRamffffff80099dac0c;
      }
      else {
        iVar7 = 0;
      }
      iVar6 = iVar9;
      if (iVar10 < 0) {
        iVar6 = -iVar9;
      }
LAB_ffffff80086b3218:
      *param_3 = iVar6;
    }
    else {
      iVar7 = iRamffffff80099dac0c;
      if (iVar6 != 0) {
        iVar3 = *param_3;
        iVar10 = -iVar3;
        if (-1 < iVar3) {
          iVar10 = iVar3;
        }
        iVar6 = 0x400 - iVar10;
        if (-1 < iVar10 + -0x400) {
          iVar6 = iVar10 + -0x400;
        }
        if (0x82 < iVar6) {
          iRamffffff80099dac0c = 0;
        }
        iVar6 = -iVar9;
        iVar7 = iRamffffff80099dac0c;
        if (-1 < iVar3) {
          iVar6 = iVar9;
        }
        goto LAB_ffffff80086b3218;
      }
    }
    *param_1 = (*param_1 * 0x82) / 200;
    iVar9 = *param_2;
LAB_ffffff80086b32a4:
    *param_2 = (iVar9 * 0x82) / 200;
    goto LAB_ffffff80086b32c0;
  }
LAB_ffffff80086b2ef0:
  iVar4 = iVar10 + -0x400;
  iVar2 = *param_2;
  iVar3 = 0x400 - iVar10;
  if (-1 < iVar4) {
    iVar3 = iVar4;
  }
  if (iVar3 < 200) {
    iVar3 = -iVar2;
    if (-1 < iVar2) {
      iVar3 = iVar2;
    }
    if ((iRamffffff80099dac0c != 0) && (iVar3 < 200)) {
      if (iVar10 < 0x401) {
        if (iVar6 < 1) {
          iVar9 = (0x400 - iVar10) * 0x46;
          goto LAB_ffffff80086b3284;
        }
        iVar4 = 0x400 - iVar6;
LAB_ffffff80086b325c:
        iVar9 = (iVar4 * 0x46) / 200;
      }
      else {
        if (iVar6 < 1) goto LAB_ffffff80086b325c;
        iVar9 = iVar6 * 0x46 + -0x11800;
LAB_ffffff80086b3284:
        iVar9 = -(iVar9 / 200);
      }
      *param_1 = iVar6 + iVar9;
      iVar9 = *param_2;
      goto LAB_ffffff80086b32a4;
    }
  }
  iVar3 = -iVar2;
  if (-1 < iVar2) {
    iVar3 = iVar2;
  }
  iVar5 = iVar3 + -0x400;
  iVar4 = 0x400 - iVar3;
  if (-1 < iVar5) {
    iVar4 = iVar5;
  }
  if (((199 < iVar10) || (iRamffffff80099dac0c == 0)) || (199 < iVar4)) {
    if (iRamffffff80099dac0c != 0) goto LAB_ffffff80086b32c0;
    if (iVar10 < 200) {
      iVar7 = iVar2;
      if (-1 < iVar2) {
        iVar7 = -iVar2;
      }
      iVar4 = -0x400 - iVar7;
      if (-0x401 < iVar7) {
        iVar4 = iVar7 + 0x400;
      }
      if (199 < iVar4) goto LAB_ffffff80086b30e0;
      iVar9 = -iVar10;
      if (-1 < *param_3) {
        iVar9 = iVar10;
      }
LAB_ffffff80086b3134:
      iVar10 = (iVar9 * 0x82) / 200;
    }
    else {
LAB_ffffff80086b30e0:
      if (iVar3 < 200) {
        iVar10 = iVar6;
        if (-1 < iVar6) {
          iVar10 = -iVar6;
        }
        iVar7 = -0x400 - iVar10;
        if (-0x401 < iVar10) {
          iVar7 = iVar10 + 0x400;
        }
        if (iVar7 < 200) {
          iVar9 = -iVar3;
          if (-1 < *param_3) {
            iVar9 = iVar3;
          }
          goto LAB_ffffff80086b3134;
        }
      }
      iVar10 = 0x100000 - (iVar2 * iVar2 + iVar6 * iVar6);
      if (iVar10 < 0) {
        uVar8 = 0;
      }
      else {
        uVar8 = 0;
        uVar11 = 0xfffffffffffffffe;
        do {
          uVar11 = uVar11 + 2;
          uVar12 = (uint)(0x40000000L >> (uVar11 & 0x3f));
          iVar6 = uVar8 + uVar12;
          iVar7 = 0;
          if (iVar6 <= iVar10) {
            iVar7 = iVar6;
          }
          uVar1 = 0;
          if (iVar6 <= iVar10) {
            uVar1 = uVar12;
          }
          iVar10 = iVar10 - iVar7;
          uVar8 = uVar1 | (int)uVar8 >> 1;
        } while (uVar11 < 0x1e);
      }
      iVar9 = uVar8 + iVar9;
      iVar10 = -iVar9;
      if (-1 < *param_3) {
        iVar10 = iVar9;
      }
    }
    iVar7 = 0;
    *param_3 = iVar10;
    goto LAB_ffffff80086b32c0;
  }
  if (iVar3 < 0x401) {
    if (iVar2 < 1) {
      iVar9 = (0x400 - iVar3) * 0x46;
      goto LAB_ffffff80086b31c8;
    }
    iVar5 = 0x400 - iVar2;
LAB_ffffff80086b31a0:
    iVar9 = (iVar5 * 0x46) / 200;
  }
  else {
    if (iVar2 < 1) goto LAB_ffffff80086b31a0;
    iVar9 = iVar2 * 0x46 + -0x11800;
LAB_ffffff80086b31c8:
    iVar9 = -(iVar9 / 200);
  }
  *param_2 = iVar2 + iVar9;
  *param_1 = (*param_1 * 0x82) / 200;
LAB_ffffff80086b32c0:
  if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
    (**(code **)(lRamffffff80099dab20 + 0x40))(0xffffff80093e3e85);
    iVar7 = iRamffffff80099dac0c;
  }
  return -(uint)(iVar7 == 0);
}
