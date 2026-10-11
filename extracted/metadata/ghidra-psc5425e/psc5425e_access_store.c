/* Stock Image offsets 0xa4b19c..0xa4b2fc */

ulong psc5425e_access_store(undefined8 param_1,undefined8 param_2,long param_3,ulong param_4)

{
  undefined8 uVar1;
  undefined8 uStack_38;
  long lStack_30;
  long lStack_28;

  lStack_28 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  uStack_38 = 0;
  lStack_30 = 0;
  func_0xffffff8008125e6c(0xffffff800939149e,0xffffff80093bad70);
  if ((param_3 != 0) && (param_4 != 0)) {
    func_0xffffff8008125e6c(0xffffff800933d202,0xffffff80093bad70,param_3,param_4);
    lStack_30 = param_3;
    if (param_4 < 4) {
      func_0xffffff8008458e74(param_3,0x10,&uStack_38);
      psc5425e_read_interface(uStack_38 & 0xffffffff,0xffffff8009c0c7dc,0xff,0);
      func_0xffffff8008125e6c
                (0xffffff8009450225,0xffffff80093bad70,uStack_38 & 0xffffffff,uRamffffff8009c0c7dc);
      func_0xffffff8008125e6c(0xffffff80094e72db,0xffffff80093bad70);
    }
    else {
      uVar1 = func_0xffffff8008f9dea8(&lStack_30,0xffffff8009339053);
      func_0xffffff8008458e74(uVar1,0x10,&uStack_38);
      uVar1 = func_0xffffff8008f9dea8(&lStack_30,0xffffff8009339053);
      func_0xffffff8008458e74(uVar1,0x10,(long)&uStack_38 + 4);
      func_0xffffff8008125e6c
                (0xffffff80094b527f,0xffffff80093bad70,uStack_38 & 0xffffffff,uStack_38._4_4_);
      psc5425e_config_interface(uStack_38 & 0xffffffff,uStack_38._4_4_,0xff,0);
    }
  }
  if (lRamffffff800972db08 == lStack_28) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
