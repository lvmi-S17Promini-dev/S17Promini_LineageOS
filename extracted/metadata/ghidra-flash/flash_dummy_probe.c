/* Stock Image offsets 0x8c3a94..0x8c3f98 */

void flash_dummy_probe(long param_1)

{
  long lVar1;
  long lVar2;
  undefined4 *puVar3;
  undefined8 uVar4;
  int iVar5;
  int iVar6;
  undefined8 uVar7;
  int *piVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  undefined4 uStack_6c;
  long lStack_68;

  lStack_68 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  piVar8 = *(int **)(param_1 + 0xc0);
  lVar1 = param_1 + 0x10;
  if ((bRamffffff80098c72e2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c72c0,0xffffff80094a2354,0xffffff80093879bc);
  }
  uRamffffff8009af0070 = func_0xffffff80084b7494(lVar1);
  if (uRamffffff8009af0070 < 0xfffffffffffff001) {
    uRamffffff8009af0080 = func_0xffffff80084b71dc(uRamffffff8009af0070,0xffffff80093981bd);
    if (uRamffffff8009af0080 < 0xfffffffffffff001) {
      iVar5 = 0;
    }
    else {
      func_0xffffff8008125e6c(0xffffff80094f5ed0,0xffffff800940b6ce,0xffffff80093981bd);
      iVar5 = (int)uRamffffff8009af0080;
    }
    uRamffffff8009af0078 = func_0xffffff80084b71dc(uRamffffff8009af0070,0xffffff80094e54d9);
    if (0xfffffffffffff000 < uRamffffff8009af0078) {
      func_0xffffff8008125e6c(0xffffff80094f5ed0,0xffffff800940b6ce,0xffffff80094e54d9);
      iVar5 = (int)uRamffffff8009af0078;
    }
  }
  else {
    func_0xffffff8008125e6c(0xffffff800939817c,0xffffff800940b6ce);
    iVar5 = (int)uRamffffff8009af0070;
  }
  if (iVar5 == 0) {
    if (piVar8 == (int *)0x0) {
      piVar8 = (int *)func_0xffffff800864ca40(lVar1,0x10,0x6080c0);
      if (piVar8 == (int *)0x0) {
LAB_ffffff8008943eec:
        uVar7 = 0xfffffff4;
        goto LAB_ffffff8008943b48;
      }
      *(int **)(param_1 + 0xc0) = piVar8;
      uStack_6c = 0;
      if ((lVar1 == 0) || (lVar10 = *(long *)(param_1 + 0x300), lVar10 == 0)) {
        uVar7 = 0xffffffed;
        goto LAB_ffffff8008943b48;
      }
      lVar9 = func_0xffffff8008b8a9b4(lVar10,0);
      if (lVar9 == 0) {
        *piVar8 = 0;
      }
      else {
        iVar5 = 0;
        do {
          iVar5 = iVar5 + 1;
          lVar9 = func_0xffffff8008b8a9b4(lVar10,lVar9);
        } while (lVar9 != 0);
        *piVar8 = iVar5;
        if (iVar5 != 0) {
          func_0xffffff8008125e6c(0xffffff80094e54e8,0xffffff800940b6e1,iVar5);
          iVar5 = func_0xffffff8008b8e110(lVar10,0xffffff800945cde7,&uStack_6c,1,0);
          if (iVar5 < 0) {
            func_0xffffff8008125e6c(0xffffff800940b6f0,0xffffff800940b6e1);
          }
          lVar9 = func_0xffffff800864ca40(lVar1,(long)*piVar8 * 0x34,0x6080c0);
          *(long *)(piVar8 + 2) = lVar9;
          if (lVar9 != 0) {
            lVar9 = func_0xffffff8008b8a9b4(lVar10,0);
            if (lVar9 != 0) {
              iVar5 = 0;
              do {
                iVar6 = func_0xffffff8008b8e110
                                  (lVar9,0xffffff80094b94be,
                                   *(long *)(piVar8 + 2) + (long)iVar5 * 0x34,1,0);
                if (iVar6 < 0) {
LAB_ffffff8008943f14:
                  uVar7 = 0xffffffea;
                  goto LAB_ffffff8008943b48;
                }
                lVar11 = (long)iVar5;
                iVar6 = func_0xffffff8008b8e110
                                  (lVar9,0xffffff800945fbf1,
                                   *(long *)(piVar8 + 2) + lVar11 * 0x34 + 4,1,0);
                if ((iVar6 < 0) ||
                   (iVar6 = func_0xffffff8008b8e110
                                      (lVar9,0xffffff8009491da3,
                                       *(long *)(piVar8 + 2) + lVar11 * 0x34 + 8,1,0),
                   uVar7 = uRamffffff8009343cdf, iVar6 < 0)) goto LAB_ffffff8008943f14;
                uVar4 = CONCAT17(uRamffffff8009343cee,uRamffffff8009343ce7);
                lVar11 = lVar11 * 0x34;
                lVar2 = *(long *)(piVar8 + 2) + lVar11;
                *(ulong *)(lVar2 + 0x1b) = CONCAT71(uRamffffff8009343cef,uRamffffff8009343cee);
                *(undefined8 *)(lVar2 + 0x14) = uVar4;
                *(undefined8 *)(lVar2 + 0xc) = uVar7;
                *(int *)(*(long *)(piVar8 + 2) + lVar11 + 0x2c) = iVar5;
                *(undefined4 *)(*(long *)(piVar8 + 2) + lVar11 + 0x30) = uStack_6c;
                puVar3 = (undefined4 *)(*(long *)(piVar8 + 2) + lVar11);
                func_0xffffff8008125e6c
                          (0xffffff800940b726,0xffffff800940b6e1,*puVar3,puVar3[1],puVar3[2],
                           puVar3 + 3,puVar3[0xb],puVar3[0xc]);
                iVar5 = iVar5 + 1;
                lVar9 = func_0xffffff8008b8a9b4(lVar10,lVar9);
              } while (lVar9 != 0);
            }
            goto LAB_ffffff8008943b80;
          }
          goto LAB_ffffff8008943eec;
        }
      }
      func_0xffffff8008125e6c(0xffffff80093981cd,0xffffff800940b6e1);
    }
LAB_ffffff8008943b80:
    uRamffffff8009af0088 = 0xfffffffe0;
    uRamffffff8009af0090 = 0xffffff8009af0090;
    uRamffffff8009af0098 = 0xffffff8009af0090;
    pcRamffffff8009af00a0 = flash_dummy_work_disable;
    func_0xffffff8008149e9c(0xffffff8009af00b8,1,1);
    pcRamffffff8009af00e0 = flash_dummy_timer_func;
    uRamffffff8009af0100 = 100;
    uRamffffff8009af0104 = 0;
    if (*piVar8 == 0) {
      iVar5 = stock_flashlight_dev_register(0xffffff8009343cdf,0xffffff800980c1b8);
      if (iVar5 != 0) goto LAB_ffffff8008943b44;
    }
    else if (0 < *piVar8) {
      lVar9 = 0;
      lVar10 = 0;
      do {
        iVar5 = func_0xffffff8008940b2c(*(long *)(piVar8 + 2) + lVar9,0xffffff800980c1b8);
        if (iVar5 != 0) goto LAB_ffffff8008943b44;
        lVar10 = lVar10 + 1;
        lVar9 = lVar9 + 0x34;
      } while (lVar10 < *piVar8);
    }
    iVar5 = func_0xffffff8008641d50(lVar1,0xffffff800980c0b8);
    if ((iVar5 < 0) && ((bRamffffff80098c7332 >> 2 & 1) != 0)) {
      func_0xffffff80084a0cc4(0xffffff80098c7310,0xffffff80093c99d7,0xffffff80093879bc);
    }
    if ((bRamffffff80098c735a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c7338,0xffffff8009354d90,0xffffff80093879bc);
    }
    uVar7 = 0;
  }
  else {
    if ((bRamffffff80098c730a >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098c72e8,0xffffff8009491d6e,0xffffff80093879bc);
    }
LAB_ffffff8008943b44:
    uVar7 = 0xfffffff2;
  }
LAB_ffffff8008943b48:
  if (lRamffffff800972db08 != lStack_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar7);
  }
  return;
}
