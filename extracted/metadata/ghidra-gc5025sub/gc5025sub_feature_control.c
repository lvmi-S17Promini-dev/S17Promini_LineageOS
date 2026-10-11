/* Stock Image offsets 0x875ba0..0x8766b8 */

undefined8 gc5025sub_feature_control(int param_1,ulong *param_2,undefined4 *param_3)

{
  uint uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  uint uVar5;
  undefined1 uVar6;
  uint uVar7;
  undefined8 *puVar8;
  undefined2 uVar9;
  undefined8 *puVar10;
  ushort uVar11;
  ulong uVar12;
  char cVar13;
  undefined1 auStack_70 [4];
  undefined2 uStack_6c;
  long lStack_68;

  lStack_68 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  if ((bRamffffff80098c0c62 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c0c40,0xffffff800939783f,0xffffff8009321c59,param_1);
  }
  if (param_1 < 0xbe2) {
    if (param_1 < 0xbd2) {
      switch(param_1) {
      case 0xbba:
        uVar9 = 0x960;
        if (cRamffffff8009adc500 == '\0') {
          uVar9 = 0;
        }
        *(undefined2 *)param_2 = uVar9;
        *(short *)((long)param_2 + 2) = (short)uRamffffff8009adc4fc;
        goto code_r0xffffff80088f60ac;
      case 0xbbb:
        uVar7 = 72000000;
        if (cRamffffff8009adc4f8 == '\0') {
          uVar7 = 0;
        }
        *(uint *)param_2 = uVar7;
        *param_3 = 4;
        break;
      case 0xbbc:
        uVar7 = (uint)*param_2;
        uVar2 = func_0xffffff8008fa9714(0xffffff8009adbffc);
        func_0xffffff8008fa9884(0xffffff8009adbffc,uVar2);
        func_0xffffff8008fa96b8(0xffffff8009adbffc);
        uRamffffff8009adc4fc = uVar7 & 0xffff;
        if ((uVar7 & 0xffff) <= uRamffffff8009adc504) {
          uRamffffff8009adc4fc = uRamffffff8009adc504;
        }
        if (0x3ffe < uRamffffff8009adc4fc) {
          uRamffffff8009adc4fc = 0x3fff;
        }
        func_0xffffff8008fa9838(0xffffff8009adbffc);
        if ((uVar7 & 0xffff) == 0) {
          uVar7 = 1;
        }
        uVar5 = uVar7 << 1;
        if (0x1ffe < (uVar7 & 0xffff)) {
          uVar5 = 0x3ffe;
        }
        uStack_6c = 0xfe;
        camera_iWriteRegI2C(&uStack_6c,2,0x6e);
        uStack_6c = CONCAT11((char)(uVar5 >> 8),3) & 0x3fff;
        camera_iWriteRegI2C(&uStack_6c,2,0x6e);
        uStack_6c = CONCAT11((char)uVar5,4);
        camera_iWriteRegI2C(&uStack_6c,2,0x6e);
        if ((bRamffffff80098c0d2a >> 2 & 1) != 0) {
          uVar5 = uVar5 & 0xffff;
          uVar2 = 0xffffff80098c0d08;
          uVar3 = 0xffffff800942ce5c;
          uVar4 = 0xffffff8009386fe5;
          uVar7 = uRamffffff8009adc4fc;
code_r0xffffff80088f6250:
          func_0xffffff80084a0cc4(uVar2,uVar3,uVar4,uVar5,uVar7);
        }
        break;
      case 0xbbe:
        uVar7 = (uint)(ushort)*param_2;
        if ((ushort)*param_2 < 0x41) {
          uVar7 = 0x40;
        }
        uStack_6c = 0xfe;
        camera_iWriteRegI2C(&uStack_6c,2,0x6e);
        if (uVar7 < 0x5c) {
          uStack_6c = 0xb6;
          camera_iWriteRegI2C(&uStack_6c,2,0x6e);
          uStack_6c = CONCAT11((char)(uVar7 >> 6),0xb1);
          camera_iWriteRegI2C(&uStack_6c,2,0x6e);
          uStack_6c = CONCAT11((char)(uVar7 << 2),0xb2);
          camera_iWriteRegI2C(&uStack_6c,2,0x6e);
          if ((bRamffffff80098c0d52 >> 2 & 1) == 0) break;
          uVar2 = 0xffffff80098c0d30;
          uVar3 = 0xffffff8009480180;
          uVar4 = 0xffffff80093105df;
        }
        else {
          uStack_6c = 0x1b6;
          camera_iWriteRegI2C(&uStack_6c,2,0x6e);
          uStack_6c = CONCAT11((char)((uVar7 << 6) / 0x1700),0xb1);
          camera_iWriteRegI2C(&uStack_6c,2,0x6e);
          uVar7 = (uVar7 << 6) / 0x5c;
          uStack_6c = CONCAT11((char)(uVar7 << 2),0xb2);
          camera_iWriteRegI2C(&uStack_6c,2,0x6e);
          if ((bRamffffff80098c0d7a >> 2 & 1) == 0) break;
          uVar2 = 0xffffff80098c0d58;
          uVar3 = 0xffffff80093faa6f;
          uVar4 = 0xffffff80093105df;
        }
code_r0xffffff80088f6394:
        func_0xffffff80084a0cc4(uVar2,uVar3,uVar4,uVar7);
        break;
      case 0xbc3:
        uStack_6c = CONCAT11((char)*(uint *)((long)param_2 + 4),(char)(uint)*param_2);
        camera_iWriteRegI2C(&uStack_6c,2,0x6e);
        break;
      case 0xbc4:
        uStack_6c = 0;
        auStack_70[0] = (undefined1)(uint)*param_2;
        camera_iReadRegI2C(auStack_70,1,&uStack_6c,1,0x6e);
        *(uint *)((long)param_2 + 4) = (uint)uStack_6c;
      }
      goto LAB_ffffff80088f64b8;
    }
    if (param_1 == 0xbd2) {
      uVar7 = 0xffffffff;
      goto LAB_ffffff80088f60a4;
    }
    if (param_1 != 0xbd4) {
      if (param_1 != 0xbe1) goto LAB_ffffff80088f64b8;
      func_0xffffff8008fa96b8(0xffffff8009adbffc);
      func_0xffffff8008fa9838(0xffffff8009adbffc);
      cVar13 = '\x02';
      do {
        uStack_6c = 0;
        auStack_70[0] = 0xf0;
        camera_iReadRegI2C(auStack_70,1,&uStack_6c,1,0x6e);
        uVar11 = uStack_6c;
        uStack_6c = 0;
        auStack_70[0] = 0xf1;
        camera_iReadRegI2C(auStack_70,1,&uStack_6c,1,0x6e);
        uVar7 = ((uint)uStack_6c | (uint)uVar11 << 8) + 1;
        *(uint *)param_2 = uVar7;
        if (uVar7 == 0x5026) {
          if ((bRamffffff80098c0dca >> 2 & 1) == 0) goto LAB_ffffff80088f64b8;
          uVar2 = 0xffffff80098c0da8;
          uVar3 = 0xffffff8009343016;
          uVar4 = 0xffffff80092ff5d1;
          uVar5 = 0x6e;
          uVar7 = 0x5026;
          goto code_r0xffffff80088f6250;
        }
        if ((bRamffffff80098c0df2 >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4(0xffffff80098c0dd0,0xffffff800946fe37,0xffffff80092ff5d1,0x6e);
        }
        cVar13 = cVar13 + -1;
      } while (cVar13 != '\0');
      if ((uint)*param_2 == 0x5026) goto LAB_ffffff80088f64b8;
      uVar7 = 0xffffffff;
      goto code_r0xffffff80088f6304;
    }
    uVar12 = *param_2;
    uVar11 = (ushort)uVar12;
    if ((bRamffffff80098c0da2 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c0d80,0xffffff80093faacd,0xffffff800941bf9f,(uint)uVar12 & 0xffff);
    }
    if ((uVar12 & 0xffff) == 0) goto LAB_ffffff80088f64b8;
    func_0xffffff8008fa96b8(0xffffff8009adbffc);
    uVar7 = (uint)uVar12 & 0xffff;
    uRamffffff8009805698 = uVar11;
    if (uVar7 == 0x96) {
      if ((bRamffffff8009adc508 & 1) != 0) {
        uRamffffff8009805698 = 0x92;
      }
    }
    else if ((uVar7 == 300) && ((bRamffffff8009adc508 & 1) != 0)) {
      uRamffffff8009805698 = 0x128;
    }
    func_0xffffff8008fa9838(0xffffff8009adbffc);
    uVar7 = 72000000;
    if (cRamffffff8009adc4f8 == '\0') {
      uVar7 = 0;
    }
    uVar5 = 0;
    if (uRamffffff8009805698 != 0) {
      uVar5 = uVar7 / uRamffffff8009805698;
    }
    func_0xffffff8008fa96b8(0xffffff8009adbffc);
    uRamffffff8009adc4fc = uVar5 / 0xf0;
    if (uVar5 / 0xf0 <= uRamffffff8009adc504) {
      uRamffffff8009adc4fc = uRamffffff8009adc504;
    }
    uRamffffff8009adc504 = uRamffffff8009adc4fc;
    if (0x3ffe < uRamffffff8009adc4fc) {
      uRamffffff8009adc4fc = 0x3fff;
      uRamffffff8009adc504 = uRamffffff8009adc4fc;
    }
    goto code_r0xffffff80088f64b4;
  }
  if (param_1 < 0xbf3) {
    if (param_1 == 0xbe2) {
      uVar12 = *param_2;
      if ((bRamffffff80098c0e1a >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c0df8,0xffffff8009375f45,0xffffff80093faafa,(uVar12 & 0xff) != 0,
                   *(undefined2 *)((long)param_2 + 2));
      }
      func_0xffffff8008fa96b8(0xffffff8009adbffc);
      bRamffffff8009adc508 = (uVar12 & 0xff) != 0;
    }
    else {
      if (param_1 == 0xbe3) {
        uVar12 = *param_2;
        if ((bRamffffff80098c0ee2 >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4
                    (0xffffff80098c0ec0,0xffffff800943d104,0xffffff800938702f,(char)uVar12 != '\0');
        }
        uStack_6c = 0xfe;
        camera_iWriteRegI2C(&uStack_6c,2,0x6e);
        if ((char)uVar12 == '\0') {
          uVar6 = 0x10;
        }
        else {
          uVar6 = 0x11;
        }
        uStack_6c = CONCAT11(uVar6,0x8c);
        camera_iWriteRegI2C(&uStack_6c,2,0x6e);
        goto code_r0xffffff80088f6210;
      }
      if (param_1 != 0xbf2) goto LAB_ffffff80088f64b8;
      uVar7 = (uint)*param_2;
      uVar5 = (uint)param_2[1];
      if ((bRamffffff80098c0e42 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c0e20,0xffffff8009386ff1,0xffffff80093a85de,uVar7,uVar5);
      }
      switch(uVar7) {
      case 1:
        if (((uRamffffff8009805698 != 300) && (uRamffffff8009805698 != 0xf0)) &&
           ((bRamffffff80098c0e6a >> 2 & 1) != 0)) {
          func_0xffffff80084a0cc4
                    (0xffffff80098c0e48,0xffffff80092ff5e2,0xffffff80093a85de,uVar5,0x1e);
        }
        uVar7 = 0;
        if (uVar5 != 0) {
          uVar7 = 72000000 / uVar5;
        }
        break;
      case 2:
        if (uVar5 == 0) goto LAB_ffffff80088f64b8;
      case 0:
      case 3:
      case 4:
        uVar7 = 0;
        if (uVar5 != 0) {
          uVar7 = 72000000 / uVar5;
        }
        break;
      default:
        uVar1 = 0;
        if (uVar5 != 0) {
          uVar1 = 72000000 / uVar5;
        }
        func_0xffffff8008fa96b8(0xffffff8009adbffc);
        uRamffffff8009adc4fc = (uVar1 / 0xf0 + 0xfc24 & 0xffff) + 0x3dc;
        if (0x12f < uVar5) {
          uRamffffff8009adc4fc = 0x3dc;
        }
        uRamffffff8009adc504 = uRamffffff8009adc4fc;
        func_0xffffff8008fa9838(0xffffff8009adbffc);
        if ((bRamffffff80098c0e92 >> 2 & 1) == 0) goto LAB_ffffff80088f64b8;
        uVar2 = 0xffffff80098c0e70;
        uVar3 = 0xffffff8009321c69;
        uVar4 = 0xffffff80093a85de;
        goto code_r0xffffff80088f6394;
      }
      func_0xffffff8008fa96b8(0xffffff8009adbffc);
      uRamffffff8009adc4fc = (uVar7 / 0xf0 + 0xfc24 & 0xffff) + 0x3dc;
      uRamffffff8009adc504 = uRamffffff8009adc4fc;
      if (0x12f < uVar5) {
        uRamffffff8009adc4fc = 0x3dc;
        uRamffffff8009adc504 = uRamffffff8009adc4fc;
      }
    }
    goto code_r0xffffff80088f64b4;
  }
  switch(param_1) {
  case 0xc07:
    if ((bRamffffff80098c0c8a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c0c68,0xffffff800941bf72,0xffffff8009321c59,(uint)*param_2);
    }
    func_0xffffff8008fa96b8(0xffffff8009adbffc);
    uRamffffff8009805698 = (ushort)*param_2;
    goto code_r0xffffff80088f64b4;
  case 0xc08:
    if ((bRamffffff80098c0cb2 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c0c90,0xffffff800942ce2f,0xffffff8009321c59,(char)*param_2);
    }
code_r0xffffff80088f6210:
    func_0xffffff8008fa96b8(0xffffff8009adbffc);
code_r0xffffff80088f64b4:
    func_0xffffff8008fa9838(0xffffff8009adbffc);
    break;
  case 0xc09:
    if ((bRamffffff80098c0cda >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c0cb8,0xffffff800943d0bc,0xffffff8009321c59,(uint)*param_2);
    }
    puVar8 = (undefined8 *)param_2[1];
    switch((uint)*param_2) {
    case 1:
      puVar10 = (undefined8 *)0xffffff8009124d4a;
      break;
    case 2:
      puVar10 = (undefined8 *)0xffffff8009124d6a;
      break;
    case 3:
      puVar10 = (undefined8 *)0xffffff8009124d8a;
      break;
    case 4:
      puVar10 = (undefined8 *)0xffffff8009124daa;
      break;
    default:
      puVar10 = (undefined8 *)0xffffff8009124d2a;
    }
    uVar3 = puVar10[3];
    uVar2 = *puVar10;
    uVar4 = puVar10[1];
    puVar8[2] = puVar10[2];
    puVar8[3] = uVar3;
    *puVar8 = uVar2;
    puVar8[1] = uVar4;
    break;
  case 0xc0a:
    break;
  case 0xc0b:
    if ((bRamffffff80098c0d02 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c0ce0,0xffffff80093b8a6f,0xffffff8009321c59,(short)*param_2,
                 (short)param_2[1],(short)param_2[2]);
    }
    if ((bRamffffff80098c0f0a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4
                (0xffffff80098c0ee8,0xffffff80093105e8,0xffffff80093d936d,(short)*param_2,
                 (short)param_2[1],(short)param_2[2]);
    }
    break;
  default:
    if (param_1 == 0xbf3) {
      uVar12 = *param_2;
      param_2 = (ulong *)param_2[1];
      if ((bRamffffff80098c0eba >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c0e98,0xffffff8009331fa5,0xffffff80093d934b,(uint)uVar12);
      }
      switch((uint)uVar12) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
        uVar7 = 300;
        break;
      default:
        goto LAB_ffffff80088f64b8;
      }
code_r0xffffff80088f6304:
      *(uint *)param_2 = uVar7;
      break;
    }
    if (param_1 != 0xbf6) break;
    uVar7 = 0xf7375923;
LAB_ffffff80088f60a4:
    *(uint *)param_2 = uVar7;
code_r0xffffff80088f60ac:
    *param_3 = 4;
  }
LAB_ffffff80088f64b8:
  if (lRamffffff800972db08 == lStack_68) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
