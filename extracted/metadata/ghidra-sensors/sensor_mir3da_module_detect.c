/* Stock Image offsets 0x63379c..0x6338a8 */

void sensor_mir3da_module_detect(undefined8 param_1)

{
  undefined8 uVar1;
  byte abStack_30 [4];
  byte abStack_2c [4];
  long lStack_28;

  lStack_28 = lRamffffff800972db08;
  _mcount();
  uVar1 = (*(code *)*puRamffffff80099dab20)(param_1,uRamffffff80097db302,abStack_2c);
  if ((int)uVar1 != 0) goto LAB_ffffff80086b3880;
  abStack_2c[0] = abStack_2c[0] & bRamffffff80097db304;
  if (bRamffffff80097db305 == abStack_2c[0]) {
    uVar1 = (*(code *)*puRamffffff80099dab20)(param_1,uRamffffff80097db306,abStack_30);
    if ((int)uVar1 != 0) goto LAB_ffffff80086b3880;
    abStack_30[0] = abStack_30[0] & bRamffffff80097db308;
    if (bRamffffff80097db309 == abStack_30[0]) {
      if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
        (*(code *)puRamffffff80099dab20[8])(0xffffff80093814db);
      }
      uVar1 = 0;
      uRamffffff80097db2ec = 0;
      goto LAB_ffffff80086b3880;
    }
  }
  uVar1 = 0xffffffff;
LAB_ffffff80086b3880:
  if (lRamffffff800972db08 == lStack_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}
