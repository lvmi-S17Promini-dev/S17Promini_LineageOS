/* Stock Image offsets 0x632978..0x632a28 */

void sensor_mir3da_register_mask_write
               (undefined8 param_1,undefined4 param_2,byte param_3,byte param_4)

{
  int iVar1;
  byte abStack_4c [4];
  long lStack_48;

  lStack_48 = lRamffffff800972db08;
  _mcount();
  iVar1 = (*(code *)*puRamffffff80099dab20)(param_1,param_2,abStack_4c);
  if (iVar1 == 0) {
    abStack_4c[0] = abStack_4c[0] & (param_3 ^ 0xff) | param_4 & param_3;
    (*(code *)puRamffffff80099dab20[1])(param_1,param_2);
  }
  if (lRamffffff800972db08 == lStack_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
