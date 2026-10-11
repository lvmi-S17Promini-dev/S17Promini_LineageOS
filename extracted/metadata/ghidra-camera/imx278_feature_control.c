/* Stock Image offsets 0x877388..0x877ff4 */

undefined8 imx278_feature_control(int param_1,ulong *param_2,undefined4 *param_3)

{
  long lVar1;
  uint uVar2;
  bool bVar3;
  bool bVar4;
  uint uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 *puVar9;
  undefined2 uVar10;
  undefined4 uVar11;
  undefined8 *puVar12;
  uint uVar13;
  ushort *puVar14;
  short sVar15;
  ulong uVar16;
  ushort uVar17;
  long lVar18;
  byte bVar19;
  char cVar20;
  ushort auStack_74 [2];
  undefined2 uStack_70;
  undefined1 uStack_6e;
  ushort auStack_6c [2];
  long lStack_68;

  lStack_68 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  if ((bRamffffff80098c12a2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c1280,0xffffff80094801dc,0xffffff8009321c59,param_1);
  }
  if (param_1 < 0xbe2) {
    if (0xbd1 < param_1) {
      if (param_1 != 0xbd2) {
        if (param_1 == 0xbd4) {
          uVar16 = *param_2;
          sVar15 = (short)uVar16;
          if ((bRamffffff80098c1432 >> 2 & 1) != 0) {
            func_0xffffff80084a0cc4
                      (0xffffff80098c1410,0xffffff80093c905e,0xffffff800941bf9f,
                       (uint)uVar16 & 0xffff);
          }
          if ((uVar16 & 0xffff) != 0) {
            func_0xffffff8008fa96b8(0xffffff8009adc50c);
            uVar5 = (uint)uVar16 & 0xffff;
            sRamffffff8009805730 = sVar15;
            if (uVar5 == 0x96) {
              if ((bRamffffff8009adc520 & 1) != 0) {
                sRamffffff8009805730 = 0x92;
              }
            }
            else if ((uVar5 == 300) && ((bRamffffff8009adc520 & 1) != 0)) {
              sRamffffff8009805730 = 0x128;
            }
            func_0xffffff8008fa9838(0xffffff8009adc50c);
            imx278_set_max_framerate(sRamffffff8009805730,1);
          }
        }
        else if (param_1 == 0xbe1) {
          bVar19 = 0x34;
          lVar18 = 0;
          do {
            func_0xffffff8008fa96b8(0xffffff8009adc50c);
            bRamffffff8009805734 = bVar19;
            func_0xffffff8008fa9838(0xffffff8009adc50c);
            cVar20 = '\x02';
            do {
              uVar5 = imx278_return_sensor_id();
              *(uint *)param_2 = uVar5;
              if (uVar5 == 0x278) {
                if ((bRamffffff80098c145a >> 2 & 1) == 0) goto LAB_ffffff80088f7b28;
                uVar5 = (uint)bRamffffff8009805734;
                uVar6 = 0xffffff80098c1438;
                uVar7 = 0xffffff8009375f7f;
                uVar8 = 0xffffff80092ff5d1;
                uVar13 = 0x278;
                goto LAB_ffffff80088f7db0;
              }
              if ((bRamffffff80098c1482 >> 2 & 1) != 0) {
                func_0xffffff80084a0cc4
                          (0xffffff80098c1460,0xffffff80094d3689,0xffffff80092ff5d1,
                           bRamffffff8009805734,uVar5);
              }
              cVar20 = cVar20 + -1;
            } while (cVar20 != '\0');
            lVar1 = lVar18 + 1;
            bVar19 = *(byte *)(lVar18 + -0x7ff6edb0b8);
            lVar18 = lVar1;
          } while (lVar1 != 3);
          if ((uint)*param_2 != 0x278) {
            *(uint *)param_2 = 0xffffffff;
          }
        }
        goto LAB_ffffff80088f7b28;
      }
      uVar5 = 0xffffffff;
LAB_ffffff80088f78fc:
      *(uint *)param_2 = uVar5;
code_r0xffffff80088f7904:
      *param_3 = 4;
      goto LAB_ffffff80088f7b28;
    }
    switch(param_1) {
    case 0xbba:
      uVar10 = 0x1370;
      if (cRamffffff8009adc518 == '\0') {
        uVar10 = 0;
      }
      *(undefined2 *)param_2 = uVar10;
      *(short *)((long)param_2 + 2) = (short)uRamffffff8009adc514;
      goto code_r0xffffff80088f7904;
    case 0xbbb:
      uVar5 = uRamffffff8009adc510;
      if ((bRamffffff80098c12ca >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c12a8,0xffffff800942ce9c,0xffffff8009321c59,uRamffffff8009adc510,
                   sRamffffff8009805730);
        uVar5 = uRamffffff8009adc510;
      }
      goto LAB_ffffff80088f78fc;
    case 0xbbc:
      uVar5 = (uint)*param_2;
      uVar6 = func_0xffffff8008fa9714(0xffffff8009adc50c);
      func_0xffffff8008fa9884(0xffffff8009adc50c,uVar6);
      func_0xffffff8008fa96b8(0xffffff8009adc50c);
      uRamffffff8009adc514 = (uVar5 & 0xffff) + 10;
      if ((uVar5 & 0xffff) <= uRamffffff8009adc51c - 10) {
        uRamffffff8009adc514 = uRamffffff8009adc51c;
      }
      if (0xfffe < uRamffffff8009adc514) {
        uRamffffff8009adc514 = 0xffff;
      }
      func_0xffffff8008fa9838(0xffffff8009adc50c);
      if ((uVar5 & 0xffff) == 0) {
        uVar5 = 1;
      }
      if (0xfff4 < (uVar5 & 0xffff)) {
        uVar5 = 0xfffffff5;
      }
      if (bRamffffff8009adc520 == 1) {
        uVar13 = 0;
        if (uRamffffff8009adc514 != 0) {
          uVar13 = ((uRamffffff8009adc510 / 0x1370) * 10) / uRamffffff8009adc514;
        }
        if ((uVar13 & 0xffff) - 0x129 < 9) {
          uVar6 = 0x128;
        }
        else {
          if (3 < (uVar13 & 0xffff) - 0x93) goto code_r0xffffff80088f7c8c;
          uVar6 = 0x92;
        }
        imx278_set_max_framerate(uVar6,0);
      }
      else {
code_r0xffffff80088f7c8c:
        uStack_70 = 0x401;
        uStack_6e = 1;
        camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
        uStack_70 = 0x4003;
        uStack_6e = (undefined1)(uRamffffff8009adc514 >> 8);
        camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
        uStack_70 = 0x4103;
        uStack_6e = (undefined1)uRamffffff8009adc514;
        camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
        uStack_70 = 0x401;
        uStack_6e = 0;
        camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
      }
      uStack_70 = 0x401;
      uStack_6e = 1;
      camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
      uStack_70 = 0x202;
      uStack_6e = (undefined1)(uVar5 >> 8);
      camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
      uStack_70 = 0x302;
      uStack_6e = (undefined1)uVar5;
      camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
      uStack_70 = 0x401;
      uStack_6e = 0;
      camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
      if ((bRamffffff80098c1392 >> 2 & 1) != 0) {
        uVar5 = uVar5 & 0xffff;
        uVar6 = 0xffffff80098c1370;
        uVar7 = 0xffffff80093a861c;
        uVar8 = 0xffffff800934317f;
        uVar13 = uRamffffff8009adc514;
LAB_ffffff80088f7db0:
        func_0xffffff80084a0cc4(uVar6,uVar7,uVar8,uVar5,uVar13);
      }
      break;
    case 0xbbe:
      uVar17 = (ushort)*param_2;
      if (0x7c0 < uVar17 - 0x40) {
        if ((bRamffffff80098c13e2 >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4(0xffffff80098c13c0,0xffffff8009375fbd,0xffffff80093105df);
        }
        if (0x7ff < uVar17) {
          uVar17 = 0x800;
        }
        if (uVar17 < 0x41) {
          uVar17 = 0x40;
        }
      }
      puVar14 = (ushort *)0xffffff8009125090;
      uVar16 = 0;
      do {
        if (uVar17 <= *puVar14) break;
        uVar16 = uVar16 + 1;
        puVar14 = puVar14 + 2;
      } while (uVar16 != 0x67);
      uVar10 = *(undefined2 *)((uVar16 & 0xff) * 4 + -0x7ff6edaf6e);
      func_0xffffff8008fa96b8(0xffffff8009adc50c);
      func_0xffffff8008fa9838(0xffffff8009adc50c);
      if ((bRamffffff80098c140a >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c13e8,0xffffff8009387055,0xffffff80093105df,uVar17,uVar10);
      }
      uStack_70 = 0x401;
      uStack_6e = 1;
      camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
      uStack_70 = 0x402;
      uStack_6e = (undefined1)((ushort)uVar10 >> 8);
      camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
      uStack_70 = 0x502;
      uStack_6e = (undefined1)uVar10;
      camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
      uStack_70 = 0x401;
      uStack_6e = 0;
      goto code_r0xffffff80088f78ac;
    case 0xbc3:
      uStack_70 = CONCAT11((char)(uint)*param_2,(char)((uint)*param_2 >> 8));
      uStack_6e = (undefined1)*(uint *)((long)param_2 + 4);
code_r0xffffff80088f78ac:
      camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
      break;
    case 0xbc4:
      auStack_6c[0] = (ushort)*param_2 >> 8 | (ushort)*param_2 << 8;
      auStack_74[0] = 0;
      camera_iReadRegI2C(auStack_6c,2,auStack_74,1,bRamffffff8009805734);
      *(uint *)((long)param_2 + 4) = (uint)auStack_74[0];
    }
    goto LAB_ffffff80088f7b28;
  }
  if (param_1 < 0xbf3) {
    if (param_1 == 0xbe2) {
      uVar16 = *param_2;
      if ((bRamffffff80098c14aa >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c1488,0xffffff80092ef3c9,0xffffff80093faafa,(uVar16 & 0xff) != 0,
                   *(undefined2 *)((long)param_2 + 2));
      }
      func_0xffffff8008fa96b8(0xffffff8009adc50c);
      bRamffffff8009adc520 = (uVar16 & 0xff) != 0;
      goto code_r0xffffff80088f7b24;
    }
    if (param_1 == 0xbe3) {
      uVar16 = *param_2;
      if ((bRamffffff80098c1522 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c1500,0xffffff800941bfae,0xffffff800938702f,(char)uVar16 != '\0');
      }
      uStack_70 = 0x106;
      if ((char)uVar16 == '\0') {
        uStack_6e = 0;
      }
      else {
        uStack_6e = 4;
      }
      camera_iWriteRegI2C(&uStack_70,3,bRamffffff8009805734);
      goto code_r0xffffff80088f7a40;
    }
    if (param_1 != 0xbf2) goto LAB_ffffff80088f7b28;
    uVar5 = (uint)*param_2;
    uVar13 = (uint)param_2[1];
    if ((bRamffffff80098c14d2 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c14b0,0xffffff80094e4b09,0xffffff80093a85de,uVar5,uVar13)
      ;
    }
    switch(uVar5) {
    case 0:
      break;
    case 1:
      if (sRamffffff8009805730 != 300) {
        uVar5 = 0;
        if (uVar13 != 0) {
          uVar5 = 388000000 / uVar13;
        }
        func_0xffffff8008fa96b8(0xffffff8009adc50c);
        uRamffffff8009adc514 = ((uVar5 * 10) / 0x1370 + 0xf36c & 0xffff) + 0xc94;
        if (uVar5 * 10 < 0xf49030) {
          uRamffffff8009adc514 = 0xc94;
        }
        goto code_r0xffffff80088f7b24;
      }
      break;
    case 2:
      if (uVar13 == 0) goto LAB_ffffff80088f7b28;
      break;
    case 3:
      uVar5 = 0;
      if (uVar13 != 0) {
        uVar5 = 480000000 / uVar13;
      }
      uVar5 = uVar5 * 10;
      func_0xffffff8008fa96b8(0xffffff8009adc50c);
      uVar13 = 800;
      bVar3 = 0x3cd16e < uVar5;
      bVar4 = uVar5 == 0x3cd16f;
      uRamffffff8009adc514 = (uVar5 / 0x1370 + 0xfce0 & 0xffff) + 800;
      goto code_r0xffffff80088f7b14;
    case 4:
      uVar2 = 0;
      if (uVar13 != 0) {
        uVar2 = 184000000 / uVar13;
      }
      func_0xffffff8008fa96b8(0xffffff8009adc50c);
      uRamffffff8009adc514 = ((uVar2 * 10) / 0x1370 + 0xfb34 & 0xffff) + 0x4cc;
      if (uVar2 * 10 < 0x5d50b0) {
        uRamffffff8009adc514 = 0x4cc;
      }
      uRamffffff8009adc51c = uRamffffff8009adc514;
      func_0xffffff8008fa9838(0xffffff8009adc50c);
    default:
      uVar2 = 0;
      if (uVar13 != 0) {
        uVar2 = 480000000 / uVar13;
      }
      func_0xffffff8008fa96b8(0xffffff8009adc50c);
      uRamffffff8009adc514 = ((uVar2 * 10) / 0x1370 + 0xf380 & 0xffff) + 0xc80;
      if (uVar2 * 10 < 0xf30b70) {
        uRamffffff8009adc514 = 0xc80;
      }
      uRamffffff8009adc51c = uRamffffff8009adc514;
      func_0xffffff8008fa9838(0xffffff8009adc50c);
      if ((bRamffffff80098c14fa >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098c14d8,0xffffff800940ae7b,0xffffff80093a85de,uVar5);
      }
      goto LAB_ffffff80088f7b28;
    }
    uVar5 = 0;
    if (uVar13 != 0) {
      uVar5 = 480000000 / uVar13;
    }
    uVar5 = uVar5 * 10;
    func_0xffffff8008fa96b8(0xffffff8009adc50c);
    uVar13 = 0xc80;
    bVar3 = 0xf30b6e < uVar5;
    bVar4 = uVar5 == 0xf30b6f;
    uRamffffff8009adc514 = (uVar5 / 0x1370 + 0xf380 & 0xffff) + 0xc80;
code_r0xffffff80088f7b14:
    uRamffffff8009adc51c = uRamffffff8009adc514;
    if (!bVar3 || bVar4) {
      uRamffffff8009adc514 = uVar13;
      uRamffffff8009adc51c = uVar13;
    }
    goto code_r0xffffff80088f7b24;
  }
  switch(param_1) {
  case 0xc07:
    if ((bRamffffff80098c12f2 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c12d0,0xffffff80094a1a4a,0xffffff8009321c59,(uint)*param_2);
    }
    func_0xffffff8008fa96b8(0xffffff8009adc50c);
    sRamffffff8009805730 = (short)*param_2;
    break;
  case 0xc08:
    if ((bRamffffff80098c131a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c12f8,0xffffff80094e4adf,0xffffff8009321c59,(char)*param_2);
    }
code_r0xffffff80088f7a40:
    func_0xffffff8008fa96b8(0xffffff8009adc50c);
    break;
  case 0xc09:
    if ((bRamffffff80098c1342 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c1320,0xffffff80093fab61,0xffffff8009321c59,(uint)*param_2);
    }
    puVar9 = (undefined8 *)param_2[1];
    switch((uint)*param_2) {
    case 1:
      puVar12 = (undefined8 *)0xffffff8009124f70;
      break;
    case 2:
      puVar12 = (undefined8 *)0xffffff8009124f90;
      break;
    case 3:
      puVar12 = (undefined8 *)0xffffff8009124fb0;
      break;
    case 4:
      puVar12 = (undefined8 *)0xffffff8009124fd0;
      break;
    default:
      puVar12 = (undefined8 *)0xffffff8009124f50;
    }
    uVar7 = puVar12[3];
    uVar6 = *puVar12;
    uVar8 = puVar12[1];
    puVar9[2] = puVar12[2];
    puVar9[3] = uVar7;
    *puVar9 = uVar6;
    puVar9[1] = uVar8;
  case 0xc0b:
    if ((bRamffffff80098c136a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c1348,0xffffff80094f572a,0xffffff8009321c59,(short)*param_2,
                 (short)param_2[1],(short)param_2[2]);
    }
  case 0xc0a:
    goto LAB_ffffff80088f7b28;
  default:
    if (param_1 == 0xbf3) {
      if ((uint)*param_2 < 5) {
        switch((uint)*param_2) {
        default:
          uVar11 = 300;
          break;
        case 3:
          uVar11 = 0x4b0;
        }
        *(undefined4 *)param_2[1] = uVar11;
      }
      goto LAB_ffffff80088f7b28;
    }
    if (param_1 != 0xbf6) goto LAB_ffffff80088f7b28;
    uVar5 = 0x6b3a163d;
    goto LAB_ffffff80088f78fc;
  }
code_r0xffffff80088f7b24:
  func_0xffffff8008fa9838(0xffffff8009adc50c);
LAB_ffffff80088f7b28:
  if (lRamffffff800972db08 == lStack_68) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
