/* Stock Image offsets 0x6335cc..0x633684 */

void sensor_mir3da_get_enable(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  byte abStack_3c [4];
  long lStack_38;

  lStack_38 = lRamffffff800972db08;
  _mcount();
  abStack_3c[0] = 0;
  iVar1 = (*(code *)*puRamffffff80099dab20)
                    (param_1,*(undefined1 *)
                              (lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + 0x16),
                     abStack_3c);
  if (iVar1 == 0) {
    *(bool *)param_2 =
         (*(byte *)(lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + 0x18) & abStack_3c[0]
         ) == 0;
  }
  if (lRamffffff800972db08 == lStack_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
