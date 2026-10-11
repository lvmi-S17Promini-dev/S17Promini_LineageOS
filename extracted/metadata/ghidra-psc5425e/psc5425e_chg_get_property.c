/* Stock Image offsets 0xa4a958..0xa4aaf0 */

undefined4 psc5425e_chg_get_property(undefined8 param_1,int param_2,undefined4 *param_3)

{
  byte bVar1;
  undefined4 uVar2;
  long lVar3;
  undefined4 uVar4;
  byte abStack_3c [4];
  long lStack_38;

  lStack_38 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  lVar3 = func_0xffffff8008aedef8(param_1);
  abStack_3c[0] = 0;
  *param_3 = 0;
  if ((bRamffffff80098d45fa >> 2 & 1) != 0) {
    func_0xffffff80084a0eb0
              (0xffffff80098d45d8,*(undefined8 *)(lVar3 + 0x10),0xffffff80094938a0,
               0xffffff80093bad2b,param_2);
  }
  uVar2 = 0;
  if (param_2 < 0x10) {
    if (param_2 == 4) goto LAB_ffffff8008acaa2c;
    if (param_2 == 8) {
      if (*(int *)(lVar3 + 0x40) != 4) goto LAB_ffffff8008acaa1c;
      uVar4 = 5000000;
      goto LAB_ffffff8008acaa10;
    }
    if (param_2 != 0) goto LAB_ffffff8008acaa28;
    uVar2 = psc5425e_read_interface(0,abStack_3c,3,4);
    bVar1 = abStack_3c[0];
    func_0xffffff8008125e6c(0xffffff80094b524e,abStack_3c[0]);
    switch(bVar1) {
    case 0:
    case 3:
      uVar4 = 3;
      break;
    case 1:
      uVar4 = 1;
      break;
    case 2:
      uVar4 = 4;
      break;
    default:
      *param_3 = 0;
      goto LAB_ffffff8008acaa2c;
    }
  }
  else {
    if (param_2 == 0x10) {
      if (*(int *)(lVar3 + 0x40) != 4) {
LAB_ffffff8008acaa1c:
        uVar2 = 0;
        goto LAB_ffffff8008acaa2c;
      }
      uVar4 = 500000;
    }
    else {
      if (param_2 == 0x3b) goto LAB_ffffff8008acaa2c;
      if (param_2 != 0x3a) {
LAB_ffffff8008acaa28:
        uVar2 = 0xffffffc3;
        goto LAB_ffffff8008acaa2c;
      }
      uVar4 = *(undefined4 *)(lVar3 + 0x40);
    }
LAB_ffffff8008acaa10:
    uVar2 = 0;
  }
  *param_3 = uVar4;
LAB_ffffff8008acaa2c:
  if (lRamffffff800972db08 != lStack_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar2;
}
