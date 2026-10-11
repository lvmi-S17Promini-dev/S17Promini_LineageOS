/* Stock Image offsets 0xa4b000..0xa4b074 */

undefined8 psc5425e_get_charging_status(undefined8 param_1,undefined8 param_2)

{
  long lVar1;
  char acStack_2c [4];
  long lStack_28;

  lStack_28 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  psc5425e_read_interface(0,acStack_2c,3,4);
  lVar1 = lRamffffff800972db08;
  *(bool *)param_2 = acStack_2c[0] == '\x02';
  if (lVar1 == lStack_28) {
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
