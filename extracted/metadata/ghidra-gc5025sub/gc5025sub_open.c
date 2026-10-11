/* Stock Image offsets 0x873bac..0x875a44 */

void gc5025sub_open(void)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  undefined2 uVar5;
  ushort uVar6;
  ushort uVar7;
  uint uVar8;
  uint uVar9;
  ushort uVar10;
  bool bVar11;
  char cVar12;
  char cVar13;
  byte bVar14;
  ushort uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  undefined8 uVar19;
  undefined8 uVar20;
  byte bVar21;
  uint uVar22;
  short *psVar23;
  ulong uVar24;
  long lVar25;
  ulong uVar26;
  long lVar27;
  short sVar28;
  ushort *puVar29;
  int iVar30;
  byte bVar31;
  int iVar32;
  ushort uVar33;
  ushort uStack_150;
  ushort uStack_14e;
  byte bStack_14c;
  ushort uStack_148;
  ushort uStack_146;
  byte bStack_144;
  byte abStack_140 [7];
  byte bStack_139;
  undefined2 uStack_138;
  undefined1 auStack_bc [76];
  long lStack_70;

  lStack_70 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  if ((bRamffffff80098c04e2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c04c0,0xffffff800944dec7,0xffffff8009487c60);
  }
  func_0xffffff8008fa96b8(0xffffff8009adbffc);
  func_0xffffff8008fa9838(0xffffff8009adbffc);
  cVar12 = '\x02';
  do {
    uStack_138 = 0;
    abStack_140[0] = 0xf0;
    camera_iReadRegI2C(abStack_140,1,&uStack_138,1,0x6e);
    uVar15 = uStack_138;
    uStack_138 = 0;
    abStack_140[0] = 0xf1;
    camera_iReadRegI2C(abStack_140,1,&uStack_138,1,0x6e);
    if (((uint)uStack_138 | (uint)uVar15 << 8) == 0x5025) {
      if ((bRamffffff80098c050a >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c04e8,0xffffff8009343016,0xffffff8009487c60,0x6e,0x5026);
      }
      uStack_138 = 0xfe;
      camera_iWriteRegI2C(&uStack_138,2,0x6e);
      uStack_138 = 0xfe;
      camera_iWriteRegI2C(&uStack_138,2,0x6e);
      uStack_138 = 0xfe;
      camera_iWriteRegI2C(&uStack_138,2,0x6e);
      uStack_138 = 0x1f7;
      camera_iWriteRegI2C(&uStack_138,2,0x6e);
      uStack_138 = 0x11f8;
      camera_iWriteRegI2C(&uStack_138,2,0x6e);
      uStack_138 = 0xf9;
      camera_iWriteRegI2C(&uStack_138,2,0x6e);
      uStack_138 = 0xa0fa;
      camera_iWriteRegI2C(&uStack_138,2,0x6e);
      uStack_138 = 0x2efc;
      camera_iWriteRegI2C(&uStack_138,2,0x6e);
      gc5025sub_gc5025sub_gcore_enable_otp(1);
      lVar27 = -0x7ff6524000;
      func_0xffffff8008f8f600(0xffffff8009adc000,0,0x4f8);
      gc5025sub_gc5025sub_select_page_otp(0);
      uVar16 = gc5025sub_gc5025sub_read_otp(0);
      if ((bRamffffff80098c05aa >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c0588,0xffffff800940ae3c,0xffffff80094c3361,uVar16 & 0xff);
      }
      uVar17 = gc5025sub_gc5025sub_read_otp(0x7f);
      if ((uVar16 & 3) - 2 < 2) {
        if ((bRamffffff80098c069a >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4(0xffffff80098c0678,0xffffff80094d3607,0xffffff80094c3361);
        }
        bVar31 = 0;
        sRamffffff8009adc4c8 = 2;
        goto LAB_ffffff80088f4080;
      }
      if ((uVar16 & 3) != 1) {
        if ((bRamffffff80098c05d2 >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4(0xffffff80098c05b0,0xffffff8009343057,0xffffff80094c3361);
        }
        sRamffffff8009adc4c8 = 0;
        bVar31 = 0;
        goto LAB_ffffff80088f4080;
      }
      if ((bRamffffff80098c05fa >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098c05d8,0xffffff80094915b6,0xffffff80094c3361);
      }
      cVar12 = gc5025sub_gc5025sub_read_otp(1);
      cVar13 = gc5025sub_gc5025sub_read_otp(2);
      bVar31 = cVar13 + cVar12;
      uVar16 = (uint)bVar31;
      if ((bRamffffff80098c0622 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098c0600,0xffffff800943cfda,0xffffff80094c3361,uVar16);
      }
      if (uVar16 < 0x20) {
        gc5025sub_gc5025sub_read_otp_group(3,&uStack_138,uVar16 * 4);
        if (bVar31 == 0) {
          bVar14 = 0;
          goto LAB_ffffff80088f4070;
        }
      }
      else {
        gc5025sub_gc5025sub_read_otp_group(3,&uStack_138,0x7c);
        gc5025sub_gc5025sub_select_page_otp(1);
        gc5025sub_gc5025sub_read_otp_group(0x29,auStack_bc,uVar16 * 4 + -0x7c);
      }
      uVar24 = 0;
      bVar14 = 0;
      goto LAB_ffffff80088f3ec4;
    }
    if ((bRamffffff80098c0532 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c0510,0xffffff800946fe37,0xffffff8009487c60,0x6e);
    }
    cVar12 = cVar12 + -1;
  } while (cVar12 != '\0');
  uVar19 = 0x10;
  goto LAB_ffffff80088f56d4;
LAB_ffffff80088f3ec4:
  do {
    uVar26 = uVar24 * 4;
    if ((bRamffffff80098c064a >> 2 & 1) == 0) {
      bVar21 = *(byte *)((long)&uStack_138 + (uVar26 | 3));
      uVar33 = (ushort)bVar21;
      uVar15 = uVar33;
      if ((bVar21 >> 4 & 1) != 0) goto LAB_ffffff80088f3f70;
LAB_ffffff80088f3edc:
      if ((bRamffffff80098c0672 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c0650,0xffffff80092ef379,0xffffff80094c3361,uVar24 & 0xffffffff,
                   uVar15 & 0xf0);
      }
    }
    else {
      bVar21 = *(byte *)((long)&uStack_138 + (uVar26 | 3));
      uVar33 = (ushort)bVar21;
      uVar15 = (ushort)bVar21;
      func_0xffffff80084a0cc4
                (0xffffff80098c0628,0xffffff80094c337f,0xffffff80094c3361,uVar24 & 0xffffffff,
                 *(undefined1 *)(&uStack_138 + uVar24 * 2),
                 *(undefined1 *)((long)&uStack_138 + (uVar26 | 1)),
                 *(undefined1 *)((long)&uStack_138 + (uVar26 | 2)),bVar21);
      if ((bVar21 >> 4 & 1) == 0) goto LAB_ffffff80088f3edc;
LAB_ffffff80088f3f70:
      if ((uVar33 & 0xf) == 3) {
        bVar2 = *(byte *)((long)&uStack_138 + (uVar26 | 1));
        bVar3 = *(byte *)(&uStack_138 + uVar24 * 2);
        bVar4 = *(byte *)((long)&uStack_138 + (uVar26 | 2));
        bVar21 = 0;
        do {
          lVar25 = (ulong)bVar14 * 2;
          uVar15 = (ushort)bVar21;
          bVar14 = bVar14 + 1;
          bVar11 = bVar21 < 3;
          bVar21 = bVar21 + 1;
          *(ushort *)(lVar25 + -0x7ff6523fea) = (ushort)bVar3 | (bVar2 & 0xf) << 8;
          *(ushort *)(lVar25 + -0x7ff6523e5a) = ((ushort)(bVar2 >> 4) | (ushort)bVar4 << 4) + uVar15
          ;
          *(undefined2 *)(lVar25 + -0x7ff6523cca) = 2;
        } while (bVar11);
      }
      else if ((uVar33 & 0xf) == 4) {
        bVar21 = *(byte *)((long)&uStack_138 + (uVar26 | 1));
        bVar2 = *(byte *)(&uStack_138 + uVar24 * 2);
        bVar3 = *(byte *)((long)&uStack_138 + (uVar26 | 2));
        sVar28 = 0;
        do {
          bVar11 = sVar28 == 0;
          lVar25 = (ulong)bVar14 * 2;
          bVar14 = bVar14 + 1;
          *(ushort *)(lVar25 + -0x7ff6523e5a) =
               ((ushort)(bVar21 >> 4) | (ushort)bVar3 << 4) + sVar28;
          sVar28 = 1;
          *(ushort *)(lVar25 + -0x7ff6523fea) = (ushort)bVar2 | (bVar21 & 0xf) << 8;
          *(undefined2 *)(lVar25 + -0x7ff6523cca) = 2;
        } while (bVar11);
      }
      else {
        bVar21 = *(byte *)((long)&uStack_138 + (uVar26 | 1));
        bVar2 = *(byte *)((long)&uStack_138 + (uVar26 | 2));
        lVar25 = (ulong)bVar14 * 2;
        bVar14 = bVar14 + 1;
        *(ushort *)(lVar25 + -0x7ff6523fea) =
             (ushort)*(byte *)(&uStack_138 + uVar24 * 2) | (bVar21 & 0xf) << 8;
        *(ushort *)(lVar25 + -0x7ff6523e5a) = (ushort)(bVar21 >> 4) | (ushort)bVar2 << 4;
        *(ushort *)(lVar25 + -0x7ff6523cca) = uVar33 & 0xf;
      }
    }
    uVar24 = uVar24 + 1;
  } while (uVar24 != bVar31);
LAB_ffffff80088f4070:
  uRamffffff8009adc4c6 = (ushort)bVar14;
  sRamffffff8009adc4c8 = 1;
LAB_ffffff80088f4080:
  gc5025sub_gc5025sub_select_page_otp(1);
  bVar14 = gc5025sub_gc5025sub_read_otp(0x75);
  uVar16 = (uint)bVar31;
  if (bVar31 != 0) {
    uVar24 = 0;
    uVar18 = uVar16 << 2;
    uVar16 = (uint)bVar31;
    do {
      uVar26 = uVar24 & 0xff;
      uVar22 = (int)uVar24 + 1;
      uVar24 = (ulong)uVar22;
      uVar16 = uVar16 + *(byte *)((long)&uStack_138 + uVar26);
    } while ((uVar22 & 0xff) < uVar18);
  }
  uVar16 = ((uVar16 & 0xffff) / 0xff) * -0xff + (uVar16 & 0xffff) + 1;
  if (uVar16 == bVar14) {
    if ((bRamffffff80098c06c2 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c06a0,0xffffff80094915ef,0xffffff80094c3361,uVar16);
    }
  }
  else if ((bRamffffff80098c06ea >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c06c8,0xffffff8009343091,0xffffff80094c3361,bVar14);
  }
  gc5025sub_gc5025sub_select_page_otp(1);
  uVar16 = gc5025sub_gc5025sub_read_otp(0);
  uVar18 = gc5025sub_gc5025sub_read_otp(0x1b);
  uVar16 = uVar16 & 0xff;
  if ((bRamffffff80098c0712 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c06f0,0xffffff800944def9,0xffffff80094c3361,uVar16,uVar18 & 0xff);
  }
  iVar32 = 0;
  do {
    uVar8 = iVar32 * 2;
    uVar22 = uVar16 >> (ulong)(uVar8 + 4 & 0x1f) & 3;
    if (uVar22 - 2 < 2) {
      if ((bRamffffff80098c07b2 >> 2 & 1) != 0) {
        uVar19 = 0xffffff80098c0790;
        uVar20 = 0xffffff80094c3438;
LAB_ffffff80088f45a0:
        func_0xffffff80084a0cc4(uVar19,uVar20,0xffffff80094c3361,iVar32 + 1);
      }
    }
    else if (uVar22 == 1) {
      if ((bRamffffff80098c0762 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098c0740,0xffffff80094c33f3,0xffffff80094c3361,iVar32 + 1)
        ;
      }
      gc5025sub_gc5025sub_read_otp_group(iVar32 << 3 | 1,abStack_140,8);
      lVar25 = 0;
      uVar22 = 0;
      do {
        pbVar1 = abStack_140 + lVar25;
        lVar25 = lVar25 + 1;
        uVar22 = uVar22 + *pbVar1;
      } while (lVar25 != 7);
      if (((uVar22 & 0xffff) / 0xff) * -0xff + (uVar22 & 0xffff) + 1 == (uint)bStack_139) {
        uRamffffff8009adc000 = (ushort)abStack_140[0];
        uRamffffff8009adc002 = (ushort)abStack_140[1];
        uRamffffff8009adc006 = (ushort)abStack_140[2];
        uRamffffff8009adc004 = (ushort)abStack_140[3];
        uRamffffff8009adc008 = (ushort)abStack_140[4];
        uRamffffff8009adc00a = (ushort)abStack_140[5];
        uRamffffff8009adc00c = (ushort)abStack_140[6];
      }
      else if ((bRamffffff80098c078a >> 2 & 1) != 0) {
        uVar19 = 0xffffff80098c0768;
        uVar20 = 0xffffff80094d3643;
        goto LAB_ffffff80088f45a0;
      }
    }
    else if ((bRamffffff80098c073a >> 2 & 1) != 0) {
      uVar19 = 0xffffff80098c0718;
      uVar20 = 0xffffff80094e4a53;
      goto LAB_ffffff80088f45a0;
    }
    uVar22 = uVar16 >> (ulong)(uVar8 & 0x1f) & 3;
    if (uVar22 - 2 < 2) {
      if ((bRamffffff80098c0852 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098c0830,0xffffff80094b25f5,0xffffff80094c3361,iVar32 + 1)
        ;
      }
      uRamffffff8009adc012 = uRamffffff8009adc012 | 2;
    }
    else if (uVar22 == 1) {
      if ((bRamffffff80098c0802 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098c07e0,0xffffff80094a19cb,0xffffff80094c3361,iVar32 + 1)
        ;
      }
      gc5025sub_gc5025sub_read_otp_group(iVar32 * 5 + 0x11,&uStack_148,5);
      lVar25 = 0;
      uVar22 = 0;
      do {
        pbVar1 = (byte *)((long)&uStack_148 + lVar25);
        lVar25 = lVar25 + 1;
        uVar22 = uVar22 + *pbVar1;
      } while (lVar25 != 4);
      if (((uVar22 & 0xffff) / 0xff) * -0xff + (uVar22 & 0xffff) + 1 == (uint)bStack_144) {
        uVar22 = (uStack_148 & 0xff00) << 8 | (uint)uStack_148 << 0x18;
        uVar9 = (uStack_146 & 0xff00) << 8 | (uint)uStack_146 << 0x18;
        uRamffffff8009adc00e = 0x400;
        if (uVar22 != 0) {
          uRamffffff8009adc00e = (short)(uVar22 >> 0x10);
        }
        uRamffffff8009adc012 = uRamffffff8009adc012 | 1;
        uRamffffff8009adc010 = 0x400;
        if (uVar9 != 0) {
          uRamffffff8009adc010 = (short)(uVar9 >> 0x10);
        }
      }
      else if ((bRamffffff80098c082a >> 2 & 1) != 0) {
        uVar19 = 0xffffff80098c0808;
        uVar20 = 0xffffff8009386f7e;
        goto LAB_ffffff80088f45c4;
      }
    }
    else if ((bRamffffff80098c07da >> 2 & 1) != 0) {
      uVar19 = 0xffffff80098c07b8;
      uVar20 = 0xffffff80093a854e;
LAB_ffffff80088f45c4:
      func_0xffffff80084a0cc4(uVar19,uVar20,0xffffff80094c3361,iVar32 + 1);
    }
    uVar22 = (uVar18 & 0xff) >> (ulong)(uVar8 & 0x1f) & 3;
    if (uVar22 - 2 < 2) {
      if ((bRamffffff80098c08f2 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098c08d0,0xffffff80093b89e5,0xffffff80094c3361,iVar32 + 1)
        ;
      }
      uRamffffff8009adc014 = uRamffffff8009adc014 | 2;
    }
    else if (uVar22 == 1) {
      if ((bRamffffff80098c08a2 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4(0xffffff80098c0880,0xffffff800943d019,0xffffff80094c3361,iVar32 + 1)
        ;
      }
      gc5025sub_gc5025sub_read_otp_group(iVar32 * 5 + 0x1c,&uStack_150,5);
      lVar25 = 0;
      uVar22 = 0;
      do {
        pbVar1 = (byte *)((long)&uStack_150 + lVar25);
        lVar25 = lVar25 + 1;
        uVar22 = uVar22 + *pbVar1;
      } while (lVar25 != 4);
      if (((uVar22 & 0xffff) / 0xff) * -0xff + (uVar22 & 0xffff) + 1 == (uint)bStack_14c) {
        uVar22 = (uStack_150 & 0xff00) << 8 | (uint)uStack_150 << 0x18;
        uVar8 = (uStack_14e & 0xff00) << 8 | (uint)uStack_14e << 0x18;
        uRamffffff8009adc4ca = 0x400;
        if (uVar22 != 0) {
          uRamffffff8009adc4ca = (short)(uVar22 >> 0x10);
        }
        uRamffffff8009adc014 = uRamffffff8009adc014 | 1;
        uRamffffff8009adc4cc = 0x400;
        if (uVar8 != 0) {
          uRamffffff8009adc4cc = (short)(uVar8 >> 0x10);
        }
      }
      else if ((bRamffffff80098c08ca >> 2 & 1) != 0) {
        uVar19 = 0xffffff80098c08a8;
        uVar20 = 0xffffff80093d9303;
        goto LAB_ffffff80088f45e8;
      }
    }
    else if ((bRamffffff80098c087a >> 2 & 1) != 0) {
      uVar19 = 0xffffff80098c0858;
      uVar20 = 0xffffff80094e4a98;
LAB_ffffff80088f45e8:
      func_0xffffff80084a0cc4(uVar19,uVar20,0xffffff80094c3361,iVar32 + 1);
    }
    iVar32 = iVar32 + 1;
  } while (iVar32 != 2);
  if ((bRamffffff80098c091a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c08f8,0xffffff80093545bf,0xffffff80094c3361,uVar17 & 0xff);
  }
  uVar16 = uVar17 >> 4 & 3;
  if (uVar16 == 2) {
    if ((bRamffffff80098c09ba >> 2 & 1) != 0) {
      uVar19 = 0xffffff80098c0998;
      uVar20 = 0xffffff800945f038;
LAB_ffffff80088f4660:
      func_0xffffff80084a0cc4(uVar19,uVar20,0xffffff80094c3361);
    }
  }
  else if (uVar16 == 1) {
    if ((bRamffffff80098c096a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c0948,0xffffff800944df48,0xffffff80094c3361);
    }
    uVar24 = 0;
    do {
      iVar30 = (int)uVar24;
      iVar32 = iVar30 * 2;
      uVar15 = gc5025sub_gc5025sub_read_otp(iVar32 + 0x62);
      lVar25 = uVar24 * 2;
      *(ushort *)(lVar25 + -0x7ff6523b32) = uVar15 & 0xff;
      uVar15 = gc5025sub_gc5025sub_read_otp(iVar32 + 99);
      bVar31 = bRamffffff80098c0992;
      *(ushort *)(lVar25 + -0x7ff6523b1e) = uVar15 & 0xff;
      if ((bVar31 >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c0970,0xffffff80094c347f,0xffffff80094c3361,uVar24 & 0xffffffff,
                   *(undefined2 *)(lVar25 + -0x7ff6523b32),uVar24 & 0xffffffff);
      }
      uVar26 = uVar24 + 1;
    } while ((uVar24 < 9) && (uVar24 = uVar26, *(short *)((long)iVar30 * 2 + -0x7ff6523b32) != 0));
    uRamffffff8009adc4f6 = (short)uVar26 - 1;
  }
  else if ((uVar16 == 0) && ((bRamffffff80098c0942 >> 2 & 1) != 0)) {
    uVar19 = 0xffffff80098c0920;
    uVar20 = 0xffffff8009331f62;
    goto LAB_ffffff80088f4660;
  }
  if ((bRamffffff80098c09e2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c09c0,0xffffff8009491645,0xffffff80094c3361,uRamffffff8009adc000);
  }
  if ((bRamffffff80098c0a0a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c09e8,0xffffff800943d060,0xffffff80094c3361,uRamffffff8009adc002);
  }
  if ((bRamffffff80098c0a32 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c0a10,0xffffff800942cdd9,0xffffff80094c3361,uRamffffff8009adc004);
  }
  if ((bRamffffff80098c0a5a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c0a38,0xffffff80092ff58e,0xffffff80094c3361,uRamffffff8009adc006);
  }
  if ((bRamffffff80098c0a82 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c0a60,0xffffff80093a8591,0xffffff80094c3361,uRamffffff8009adc008,
               uRamffffff8009adc00a,uRamffffff8009adc00c);
  }
  if ((bRamffffff80098c0aaa >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c0a88,0xffffff8009491684,0xffffff80094c3361,uRamffffff8009adc00e);
  }
  if ((bRamffffff80098c0ad2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c0ab0,0xffffff800946fe82,0xffffff80094c3361,uRamffffff8009adc010);
  }
  if ((bRamffffff80098c0afa >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c0ad8,0xffffff800946feb9,0xffffff80094c3361,uRamffffff8009adc4ca);
  }
  if ((bRamffffff80098c0b22 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c0b00,0xffffff80093b8a2e,0xffffff80094c3361,uRamffffff8009adc4cc);
  }
  gc5025sub_gc5025sub_gcore_enable_otp(0);
  if ((bRamffffff80098c0b4a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c0b28,0xffffff80093faa51,0xffffff800945f07d);
  }
  uStack_138 = 0x80fe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x80fe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x80fe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x1f7;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x11f8;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xf9;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xa0fa;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2afc;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x3fe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x701;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2efc;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xfe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x388;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xcce7;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x703;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x804;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x205;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x5806;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x808;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x1c0a;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x40c;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x70d;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x9c0e;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xa0f;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x3010;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xc317;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x218;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x1719;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x1a1a;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x1c1c;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x901e;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xb01f;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2b20;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2b21;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2b26;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xc125;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x6427;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x28;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x3f29;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x802b;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x4a2f;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x1130;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2031;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xa032;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x33;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x5534;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x238;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x39;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x3a;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x3b;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x23c;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x23d;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x6081;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2cb;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xadcd;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x50cf;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xb3d0;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x18d1;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xccd3;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xaad9;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x3dc;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xaadd;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xe0;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xae1;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2ae3;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xa0e4;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x6e5;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x10e6;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xc2e7;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x131d;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x1080;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x389;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x1fe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xf788;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x38a;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xc78e;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xfe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2240;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x343;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x40ae;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x60;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x8061;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x58b0;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x1b1;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xb2;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xb6;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x91;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x192;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x294;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x3fe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x102;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x8e03;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x8006;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x15;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x916;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xa18;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x1021;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x522;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2023;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x224;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x2025;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x826;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x629;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xa2a;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x82b;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0xfe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  uStack_138 = 0x913f;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  if (sRamffffff8009adc4c8 == 1) {
    if (uRamffffff8009adc4c6 != 0) {
      uVar24 = 0;
      do {
        if (*(short *)(lVar27 + 0x336) == 0) {
          psVar23 = (short *)(lVar27 + 0x16);
          sVar28 = 0xa31 - *psVar23;
        }
        else {
          psVar23 = (short *)(uVar24 * 2 + -0x7ff6523fea);
          if (*(short *)(lVar27 + 0x336) == 1) {
            sVar28 = 0xa2f - *(short *)(lVar27 + 0x16);
          }
          else {
            sVar28 = 0xa30 - *(short *)(lVar27 + 0x16);
          }
        }
        *psVar23 = sVar28;
        uVar24 = uVar24 + 1;
        *(short *)(lVar27 + 0x1a6) = 0x79d - *(short *)(lVar27 + 0x1a6);
        uVar26 = (ulong)uRamffffff8009adc4c6;
        lVar27 = lVar27 + 2;
      } while (uVar24 < uVar26);
      if (1 < uRamffffff8009adc4c6) {
        uVar15 = 1;
        uVar16 = 0;
        do {
          uVar17 = uVar16 + 1;
          if ((uVar17 & 0xffff) < (uint)uVar26) {
            lVar27 = (ulong)(ushort)uVar16 * 2;
            lVar25 = (ulong)uVar15 * 2 + -0x7ff6524000;
            puVar29 = (ushort *)(lVar27 + -0x7ff6523e5a);
            uVar16 = uVar17;
            do {
              if (*(ushort *)(lVar25 + 0x1a6) < *puVar29) {
                uVar5 = *(undefined2 *)(lVar27 + -0x7ff6523fea);
                *(undefined2 *)(lVar27 + -0x7ff6523fea) = *(undefined2 *)(lVar25 + 0x16);
                *(undefined2 *)(lVar25 + 0x16) = uVar5;
                uVar33 = *puVar29;
                *puVar29 = *(ushort *)(lVar25 + 0x1a6);
                *(ushort *)(lVar25 + 0x1a6) = uVar33;
                bVar31 = *(byte *)(lVar27 + -0x7ff6523cca);
                *(undefined2 *)(lVar27 + -0x7ff6523cca) = *(undefined2 *)(lVar25 + 0x336);
                *(ushort *)(lVar25 + 0x336) = (ushort)bVar31;
                uVar26 = (ulong)uRamffffff8009adc4c6;
              }
              uVar16 = uVar16 + 1;
              lVar25 = lVar25 + 2;
            } while ((uVar16 & 0xffff) < (uint)uVar26);
          }
          uVar15 = uVar15 + 1;
          uVar16 = uVar17;
        } while ((int)(uVar17 & 0xffff) < (int)uVar26 + -1);
      }
      if ((int)uVar26 != 0) {
        uVar15 = 0;
        do {
          sVar28 = *(short *)((ulong)uVar15 * 2 + -0x7ff6523e5a);
          if (sVar28 == *(short *)((ulong)uVar15 * 2 + -0x7ff6523e58)) {
            uVar24 = (ulong)uVar15;
            uVar16 = (int)uVar26 - 1;
            uVar33 = uVar15;
            do {
              uVar33 = uVar33 + 1;
              if (sVar28 != *(short *)((ulong)(uVar33 + 1) * 2 + -0x7ff6523e5a)) break;
            } while (uVar16 != uVar33 && (int)(uint)uVar33 <= (int)uVar16);
            bVar11 = uVar15 < uVar33;
            uVar15 = uVar33;
            if (bVar11) {
              do {
                uVar26 = uVar24 + 1;
                if (uVar24 < uVar33) {
                  lVar27 = uVar24 * 2;
                  uVar24 = uVar26 & 0xffffffff;
                  do {
                    uVar6 = *(ushort *)(lVar27 + -0x7ff6523fea);
                    puVar29 = (ushort *)((uVar24 & 0xffff) * 2 + -0x7ff6523fea);
                    uVar7 = *puVar29;
                    if (uVar7 < uVar6) {
                      *(ushort *)(lVar27 + -0x7ff6523fea) = uVar7;
                      *puVar29 = uVar6;
                      lVar25 = (uVar24 & 0xffff) * 2;
                      uVar5 = *(undefined2 *)(lVar27 + -0x7ff6523e5a);
                      *(undefined2 *)(lVar27 + -0x7ff6523e5a) =
                           *(undefined2 *)(lVar25 + -0x7ff6523e5a);
                      *(undefined2 *)(lVar25 + -0x7ff6523e5a) = uVar5;
                      bVar31 = *(byte *)(lVar27 + -0x7ff6523cca);
                      *(undefined2 *)(lVar27 + -0x7ff6523cca) =
                           *(undefined2 *)(lVar25 + -0x7ff6523cca);
                      *(ushort *)(lVar25 + -0x7ff6523cca) = (ushort)bVar31;
                    }
                    uVar16 = (int)(uVar24 & 0xffff) + 1;
                    uVar24 = (ulong)uVar16;
                  } while ((uVar16 & 0xffff) <= (uint)uVar33);
                }
                uVar24 = uVar26;
              } while (uVar26 != uVar33);
              uVar26 = (ulong)uRamffffff8009adc4c6;
            }
          }
          uVar15 = uVar15 + 1;
        } while ((uint)uVar15 < (uint)uVar26);
      }
    }
    uStack_138 = 0x1fe;
    camera_iWriteRegI2C(&uStack_138,2,0x6e);
    uStack_138 = 0xa8;
    camera_iWriteRegI2C(&uStack_138,2,0x6e);
    uStack_138 = 0x49d;
    camera_iWriteRegI2C(&uStack_138,2,0x6e);
    uStack_138 = 0xbe;
    camera_iWriteRegI2C(&uStack_138,2,0x6e);
    uStack_138 = 0x1a9;
    camera_iWriteRegI2C(&uStack_138,2,0x6e);
    if (uRamffffff8009adc4c6 != 0) {
      uVar24 = 0;
      do {
        lVar27 = uVar24 * 2;
        uVar33 = *(ushort *)(lVar27 + -0x7ff6523fea);
        uVar6 = *(ushort *)(lVar27 + -0x7ff6523e5a);
        uVar7 = uVar33 >> 8 & 0xf;
        uStack_138 = CONCAT11((char)uVar24,0xaa);
        camera_iWriteRegI2C(&uStack_138,2,0x6e);
        uStack_138 = CONCAT11((char)uVar33,0xac);
        camera_iWriteRegI2C(&uStack_138,2,0x6e);
        uVar15 = uVar7 | (uVar6 & 0xf) << 4;
        uStack_138 = CONCAT11((char)uVar15,0xac);
        camera_iWriteRegI2C(&uStack_138,2,0x6e);
        uVar10 = uVar6 >> 4 & 0xff;
        uStack_138 = CONCAT11((char)(uVar6 >> 4),0xac);
        camera_iWriteRegI2C(&uStack_138,2,0x6e);
        uStack_138 = CONCAT11(*(undefined1 *)(lVar27 + -0x7ff6523cca),0xac);
        camera_iWriteRegI2C(&uStack_138,2,0x6e);
        if ((bRamffffff80098c0b72 >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4
                    (0xffffff80098c0b50,0xffffff80093977e8,0xffffff800942ce15,uVar33 & 0xff,uVar15,
                     uVar10);
        }
        if ((bRamffffff80098c0b9a >> 2 & 1) != 0) {
          func_0xffffff80084a0cc4
                    (0xffffff80098c0b78,0xffffff80093c901f,0xffffff800942ce15,
                     uVar33 & 0xff | uVar7 << 8,uVar6 & 0xf | uVar10 << 4);
        }
        uVar24 = uVar24 + 1;
      } while (uVar24 < uRamffffff8009adc4c6);
    }
    uStack_138 = 0x1be;
    camera_iWriteRegI2C(&uStack_138,2,0x6e);
    uStack_138 = 0xfe;
    camera_iWriteRegI2C(&uStack_138,2,0x6e);
  }
  if ((bRamffffff80098c0bc2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4
              (0xffffff80098c0ba0,0xffffff800935460f,0xffffff8009386fc2,uRamffffff8009adc4f6);
  }
  uStack_138 = 0xfe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  if (uRamffffff8009adc4f6 != 0) {
    bVar31 = 0;
    do {
      lVar27 = (ulong)bVar31 * 2;
      uStack_138 = CONCAT11(*(undefined1 *)(lVar27 + -0x7ff6523b1e),
                            *(undefined1 *)(lVar27 + -0x7ff6523b32));
      camera_iWriteRegI2C(&uStack_138,2,0x6e);
      if ((bRamffffff80098c0bea >> 2 & 1) != 0) {
        func_0xffffff80084a0cc4
                  (0xffffff80098c0bc8,0xffffff800936506e,0xffffff8009386fc2,
                   *(undefined2 *)(lVar27 + -0x7ff6523b32),*(undefined2 *)(lVar27 + -0x7ff6523b1e));
      }
      bVar31 = bVar31 + 1;
    } while (bVar31 < uRamffffff8009adc4f6);
  }
  uStack_138 = 0xfe;
  camera_iWriteRegI2C(&uStack_138,2,0x6e);
  func_0xffffff8008fa96b8(0xffffff8009adbffc);
  uRamffffff8009adc508 = 0;
  uRamffffff8009adc4f8 = 1;
  uRamffffff8009adc4fc = 0x3dc;
  uRamffffff8009adc500 = 1;
  uRamffffff8009adc504 = 0x3dc;
  uRamffffff8009805698 = 300;
  func_0xffffff8008fa9838(0xffffff8009adbffc);
  uVar19 = 0;
  uRamffffff8009adbff8 = 0;
LAB_ffffff80088f56d4:
  if (lRamffffff800972db08 != lStack_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar19);
  }
  return;
}
