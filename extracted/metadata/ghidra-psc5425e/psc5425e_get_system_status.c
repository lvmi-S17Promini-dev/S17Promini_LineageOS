/* Stock Image offsets 0xa4a214..0xa4a270 */

void psc5425e_get_system_status(void)

{
  undefined1 auStack_1c [4];
  long lStack_18;

  lStack_18 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  auStack_1c[0] = 0;
  psc5425e_read_interface(8,auStack_1c,0xff,0);
  if (lRamffffff800972db08 == lStack_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(auStack_1c[0]);
}
