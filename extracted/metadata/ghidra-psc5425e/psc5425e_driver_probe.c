/* Stock Image offsets 0xa4a55c..0xa4a958 */

ulong psc5425e_driver_probe(long param_1)

{
  ulong *puVar1;
  int iVar2;
  ulong *puVar3;
  undefined8 uVar4;
  ulong uVar5;
  long lVar6;
  ulong auStack_90 [2];
  ulong *puStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  char acStack_4c [4];
  long lStack_48;

  lStack_48 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  uStack_60 = 0;
  uStack_58 = 0;
  uStack_70 = 0;
  uStack_68 = 0;
  puStack_80 = (ulong *)0x0;
  uStack_78 = 0;
  auStack_90[0] = 0;
  auStack_90[1] = 0;
  func_0xffffff8008125e6c(0xffffff800939149e,0xffffff80093786b6);
  uVar5 = param_1 + 0x20;
  puVar3 = (ulong *)func_0xffffff800864ca40(uVar5,200,0x6080c0);
  if (puVar3 == (ulong *)0x0) {
    uVar5 = 0xfffffff4;
  }
  else {
    lRamffffff8009c0c7d0 = param_1;
    puVar3[2] = uVar5;
    lVar6 = *(long *)(param_1 + 0x310);
    func_0xffffff8008125e6c(0xffffff80094da226,0xffffff80093cb5fe);
    if (lVar6 == 0) {
      func_0xffffff8008125e6c(0xffffff8009482936,0xffffff80093cb5fe);
      uVar5 = 0xffffffed;
    }
    else {
      iVar2 = func_0xffffff8008b8e324(lVar6,0xffffff80093fcc68,puVar3 + 3);
      if (iVar2 < 0) {
        puVar3[3] = 0xffffff80094603f8;
        func_0xffffff8008125e6c(0xffffff80094e7278,0xffffff80093cb5fe);
      }
      puVar1 = puVar3 + 1;
      iVar2 = func_0xffffff8008b8e324(lVar6,0xffffff8009324122,puVar1);
      if (iVar2 < 0) {
        *puVar1 = 0xffffff80094c59f3;
        func_0xffffff8008125e6c(0xffffff800940d6e1,0xffffff80093cb5fe);
      }
      acStack_4c[0] = '\0';
      psc5425e_read_interface(0xb,acStack_4c,0xff,0);
      uRamffffff8009c0c7d8 = (uint)(acStack_4c[0] != '\0');
      func_0xffffff8008125e6c(0xffffff800942f70e,0xffffff8009482946);
      psc5425e_config_interface(1,0xf8,0xff,0);
      psc5425e_config_interface(2,0x24,0x3f,2);
      psc5425e_config_interface(4,0x76,0xff,0);
      psc5425e_config_interface(5,0x24,0xff,0);
      func_0xffffff8008125e6c(0xffffff8009301a51,0xffffff800941e87f);
      uVar5 = func_0xffffff8008ae280c(puVar3[3],uVar5,puVar3,0xffffff8009829a38,puVar1);
      *puVar3 = uVar5;
      if ((uVar5 == 0) || (0xfffffffffffff000 < uVar5)) {
        func_0xffffff8008125e6c(0xffffff8009356c7b,0xffffff80093786b6);
        uVar5 = (ulong)(uint)*puVar3;
      }
      else {
        func_0xffffff8008125e6c(0xffffff80093786cc,0xffffff80093786b6,0x664);
        func_0xffffff800810bbe0(0xffffff8009c0c760,0xffffff80094d621e,0xffffff8009c0c7dd);
        uVar5 = func_0xffffff80080dc190(0xffffff8008ac9794,0,0xffffffff,0xffffff800943f60a);
        if (uVar5 < 0xfffffffffffff001) {
          func_0xffffff80080ebb08();
        }
        func_0xffffff8008149e9c(0xffffff8009c0c778,1,1);
        uRamffffff8009c0c7a0 = 0xffffff8008ac96bc;
        func_0xffffff8008149598(0xffffff8009c0c778,10000000000,0,1);
        auStack_90[0] = puVar3[2];
        puStack_80 = puVar3;
        uVar5 = func_0xffffff8008510e94(auStack_90[0],0xffffff80091656a8,auStack_90);
        puVar3[6] = uVar5;
        if (uVar5 < 0xfffffffffffff001) {
          puVar3[7] = 0xffffff80094c59f3;
          puVar3[0xb] = 0xffffff8009829c18;
          puVar3[0xc] = 3;
          puVar3[0xd] = (ulong)psc5425e_chg_get_property;
          *(uint *)(puVar3 + 8) = 0;
          puVar3[9] = 0xffffff8009829c24;
          puVar3[10] = 4;
          puVar3[0x15] = (ulong)puVar3;
          puVar3[0x13] = *(ulong *)(param_1 + 0x310);
          puVar3[0x16] = 0xffffff8009829c38;
          puVar3[0x17] = 2;
          uVar5 = func_0xffffff8008aed73c(puVar3[2],puVar3 + 7,puVar3 + 0x13);
          puVar3[0x18] = uVar5;
          if (uVar5 < 0xfffffffffffff001) {
            uVar5 = 0;
            do {
              iVar2 = psc5425e_read_byte(uVar5 & 0xffffffff,uVar5 - 0x7ff63f3840);
              if (iVar2 == 0) break;
              uVar5 = uVar5 + 1;
            } while (uVar5 != 0xe);
            uVar4 = 0xffffff8009301a45;
            uVar5 = 0xffffff80093786b6;
          }
          else {
            uVar4 = 0xffffff800936769f;
          }
          func_0xffffff8008125e6c(uVar4,uVar5);
          uVar5 = 0;
        }
        else {
          func_0xffffff8008125e6c(0xffffff80093cb5d4,0xffffff80093786b6,uVar5 & 0xffffffff);
        }
      }
    }
  }
  if (lRamffffff800972db08 == lStack_48) {
    return uVar5 & 0xffffffff;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
