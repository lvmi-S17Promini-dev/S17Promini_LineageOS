/* Stock Image offsets 0x630d80..0x6312c4 */

void sensor_mir3da_probe(long param_1)

{
  long lVar1;
  bool bVar2;
  char cVar3;
  undefined1 uVar4;
  int iVar5;
  int iVar6;
  long *plVar7;
  undefined8 uVar8;
  byte bVar9;
  undefined8 *puVar10;
  long lVar11;
  code *pcStack_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  code *pcStack_108;
  code *pcStack_100;
  code *pcStack_f8;
  code *pcStack_f0;
  code *pcStack_e8;
  undefined2 uStack_c8;
  undefined1 uStack_c5;
  code *pcStack_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  code *pcStack_a8;
  code *pcStack_a0;
  code *pcStack_98;
  code *pcStack_90;
  code *pcStack_88;
  undefined8 uStack_80;
  long lStack_78;
  long lStack_70;

  lStack_70 = lRamffffff800972db08;
  _mcount();
  uStack_80 = 0;
  lStack_78 = 0;
  pcStack_90 = (code *)0x0;
  pcStack_88 = (code *)0x0;
  pcStack_a0 = (code *)0x0;
  pcStack_98 = (code *)0x0;
  pcStack_a8 = (code *)0x0;
  uStack_b8 = 0;
  uStack_b0 = 0;
  pcStack_c0 = (code *)0x0;
  func_0xffffff8008f8f600(&pcStack_108,0,0x48);
  uStack_118 = 0;
  uStack_110 = 0;
  pcStack_120 = (code *)0x0;
  if ((bRamffffff80097db2e8 >> 3 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff80094efc37,0x444);
  }
  if (*(short *)(param_1 + 2) != 0x26) {
    *(undefined2 *)(param_1 + 2) = 0x26;
  }
  plVar7 = (long *)func_0xffffff800824fbc4(uRamffffff80094fc438,0x6080c0,0x68);
  if (plVar7 == (long *)0x0) {
    if ((bRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff800949c319);
    }
    iVar5 = -0xc;
  }
  else {
    iVar5 = func_0xffffff80086aa848(*(undefined8 *)(param_1 + 0x310),plVar7 + 1);
    if (-1 < iVar5) {
      *(undefined4 *)((long)plVar7 + 0xc) = 4;
      iVar6 = func_0xffffff80086aa800(4,plVar7 + 7);
      if (iVar6 == 0) {
        *plVar7 = param_1;
        *(long **)(param_1 + 0xd8) = plVar7;
        *(undefined4 *)((long)plVar7 + 0x44) = 0;
        *(undefined4 *)(plVar7 + 9) = 0;
        uRamffffff80099daaf8 = 0x100000000;
        uRamffffff80099dab00 = 0xffffff80099dab00;
        uRamffffff80099dab08 = 0xffffff80099dab00;
        plRamffffff80099daad8 = plVar7;
        lRamffffff80099daae0 = param_1;
        iVar5 = sensor_mir3da_install_general_ops(0xffffff80097daf90);
        if (iVar5 == 0) {
          cVar3 = func_0xffffff8008a8bb00(lRamffffff80099daae0,1);
          if (cVar3 != '\x13') {
            bVar9 = 0;
            do {
              func_0xffffff8008f8f060(0x147aeb8);
              cVar3 = func_0xffffff8008a8bb00(lRamffffff80099daae0,1);
              if (cVar3 == '\x13') goto LAB_ffffff80086b0fd8;
              bVar2 = bVar9 < 4;
              bVar9 = bVar9 + 1;
            } while (bVar2);
            *(undefined2 *)(param_1 + 2) = 0x27;
          }
LAB_ffffff80086b0fd8:
          uRamffffff80099daae8 = func_0xffffff800824fbc4(uRamffffff80094fc438,0x6080c0,0x48);
          uRamffffff80099daac8 = func_0xffffff800865ad24(0,0xffffff800947b084);
          uStack_c5 = 0;
          pcStack_108 = sensor_ps_open_report_data;
          pcStack_100 = sensor_ps_enable_nodata;
          pcStack_f8 = sensor_ps_set_delay;
          pcStack_f0 = sensor_ps_batch;
          pcStack_e8 = sensor_ps_flush;
          uStack_c8 = 0;
          iVar5 = sensor_ps_register_control_path(&pcStack_108);
          if (iVar5 != 0) {
            func_0xffffff8008125e6c(0xffffff80092e9d4c,iVar5);
          }
          pcStack_120 = sensor_ps_get_data;
          uStack_110 = CONCAT44(uStack_110._4_4_,100);
          iVar5 = sensor_ps_register_data_path(&pcStack_120);
          if (iVar5 != 0) {
            func_0xffffff8008125e6c(0xffffff8009448ad4,iVar5);
          }
          uVar4 = func_0xffffff8008a8bb00(lRamffffff80099daae0,1);
          func_0xffffff8008125e6c(0xffffff8009437aba,0xffffff80094efc37,0x493,uVar4);
          lRamffffff80099daaf0 = sensor_mir3da_core_init(lRamffffff80099daae0);
          if (lRamffffff80099daaf0 != 0) {
            func_0xffffff80086745dc(0xffffff80097db160);
            uRamffffff80099dab1a = 1;
            uRamffffff80099dab18 = 1;
            uRamffffff80099dab16 = 1;
            if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
              func_0xffffff8008125e6c(0xffffff80094059ad,0xffffff800946a545,0x400,0x400,0x400);
            }
            lStack_78._0_3_ = (uint3)(ushort)lStack_78;
            iVar5 = func_0xffffff80086b0214(0xffffff80097db198);
            if (iVar5 == 0) {
              lVar1 = lRamffffff80097db048 + 0x28;
              if (lVar1 == 0) {
                iVar5 = -0x16;
LAB_ffffff80086b1204:
                if ((bRamffffff80097db2e8 & 1) != 0) {
                  func_0xffffff8008125e6c(0xffffff8009405983,iVar5);
                }
                iVar5 = -0x16;
              }
              else {
                lVar11 = 0;
                do {
                  puVar10 = *(undefined8 **)(lVar11 + -0x7ff6f01548);
                  iVar5 = func_0xffffff8008648c94(lVar1,puVar10);
                  if (iVar5 != 0) {
                    if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
                      func_0xffffff8008125e6c(0xffffff80094059e8,*puVar10,iVar5);
                    }
                    goto LAB_ffffff80086b1204;
                  }
                  lVar11 = lVar11 + 8;
                } while (lVar11 != 0x40);
                pcStack_a8 = sensor_mir3da_open_report_data;
                pcStack_a0 = sensor_mir3da_enable_nodata;
                pcStack_90 = sensor_mir3da_batch;
                pcStack_88 = sensor_mir3da_flush;
                pcStack_98 = sensor_mir3da_set_delay;
                lStack_78 = (ulong)CONCAT61((int6)((ulong)lStack_78 >> 0x10),
                                            *(undefined1 *)((long)plVar7 + 0x34)) << 8;
                iVar5 = sensor_acc_register_control_path(&pcStack_a8);
                if (iVar5 == 0) {
                  pcStack_c0 = sensor_mir3da_get_data;
                  uStack_b0 = CONCAT44(uStack_b0._4_4_,1000);
                  iVar5 = sensor_acc_register_data_path(&pcStack_c0);
                  if (iVar5 == 0) {
                    uRamffffff80099daad4 = 1;
                    func_0xffffff8008125e6c(0xffffff80094ce36c);
                    uVar8 = 0;
                    goto LAB_ffffff80086b0f04;
                  }
                  if ((bRamffffff80097db2e8 & 1) != 0) {
                    uVar8 = 0xffffff80094efc44;
                    iVar6 = iVar5;
                    goto LAB_ffffff80086b0e70;
                  }
                }
                else if ((bRamffffff80097db2e8 & 1) != 0) {
                  uVar8 = 0xffffff80093c33b0;
                  goto LAB_ffffff80086b1124;
                }
              }
            }
            else if ((bRamffffff80097db2e8 & 1) != 0) {
              uVar8 = 0xffffff80094ac897;
LAB_ffffff80086b1124:
              func_0xffffff8008125e6c(uVar8);
            }
            goto LAB_ffffff80086b0edc;
          }
          if ((bRamffffff80097db2e8 & 1) != 0) {
            uVar8 = 0xffffff8009427470;
            goto LAB_ffffff80086b1134;
          }
        }
        else if ((bRamffffff80097db2e8 & 1) != 0) {
          uVar8 = 0xffffff80094596e0;
LAB_ffffff80086b1134:
          func_0xffffff8008125e6c(uVar8);
        }
        iVar5 = 0;
      }
      else {
        iVar5 = iVar6;
        if ((bRamffffff80097db2e8 & 1) != 0) {
          iVar5 = *(int *)((long)plVar7 + 0xc);
          uVar8 = 0xffffff800938144d;
LAB_ffffff80086b0e70:
          func_0xffffff8008125e6c(uVar8,iVar5);
          iVar5 = iVar6;
        }
      }
    }
LAB_ffffff80086b0edc:
    func_0xffffff8008251b9c(plVar7);
  }
  if ((bRamffffff80097db2e8 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff80093b3589,0xffffff80094efc37,iVar5);
  }
  uVar8 = 0xffffffff;
  lRamffffff80099daaf0 = 0;
  lRamffffff80099daae0 = 0;
  plRamffffff80099daad8 = (long *)0x0;
  uRamffffff80099daad4 = 0;
LAB_ffffff80086b0f04:
  if (lRamffffff800972db08 != lStack_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar8);
  }
  return;
}
