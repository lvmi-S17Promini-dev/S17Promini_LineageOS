/* Stock Image offsets 0xa49dc8..0xa49e24 */

void psc5425e_get_ce(void)

{
  undefined4 uStack_1c;
  long lStack_18;

  lStack_18 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  uStack_1c = 0;
  psc5425e_config_interface(1,&uStack_1c,1,2);
  if (lRamffffff800972db08 == lStack_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uStack_1c);
}
