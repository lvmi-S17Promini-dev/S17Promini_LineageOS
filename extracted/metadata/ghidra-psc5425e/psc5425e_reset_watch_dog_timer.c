/* Stock Image offsets 0xa4adb0..0xa4ae28 */

undefined8 psc5425e_reset_watch_dog_timer(void)

{
  undefined1 auStack_1c [4];
  long lStack_18;

  lStack_18 = lRamffffff800972db08;
  func_0xffffff800809f01c();
  func_0xffffff8008125e6c(0xffffff80093ebbc8);
  psc5425e_read_interface(0,auStack_1c,3,4);
  psc5425e_config_interface(0,auStack_1c[0],3,4);
  if (lRamffffff800972db08 == lStack_18) {
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
