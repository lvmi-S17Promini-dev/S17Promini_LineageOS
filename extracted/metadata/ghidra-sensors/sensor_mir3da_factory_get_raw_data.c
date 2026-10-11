/* Stock Image offsets 0x631d2c..0x631e40 */

undefined8 sensor_mir3da_factory_get_raw_data(void)

{
  int iVar1;
  long lVar2;
  undefined1 auStack_148 [256];
  long lStack_48;

  lStack_48 = lRamffffff800972db08;
  _mcount();
  func_0xffffff8008f8f600(auStack_148,0,0x100);
  if ((bRamffffff80097db2e8 >> 3 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff8009448ab1,0xffffff8009437ae6,0x3da);
  }
  if (lRamffffff80099daae0 != 0) {
    lVar2 = *(long *)(lRamffffff80099daae0 + 0xd8);
    iVar1 = sensor_mir3da_read_data
                      (lRamffffff80099daae0,(short *)(lVar2 + 0x60),(short *)(lVar2 + 0x62),
                       (short *)(lVar2 + 100));
    if (iVar1 == 0) {
      func_0xffffff8008f9fb58
                (auStack_148,0xffffff80093f4db3,(long)*(short *)(lVar2 + 0x60),
                 (long)*(short *)(lVar2 + 0x62),(long)*(short *)(lVar2 + 100));
    }
    else if ((bRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff8009381488,iVar1);
    }
  }
  if ((bRamffffff80097db2e8 >> 2 & 1) != 0) {
    func_0xffffff8008125e6c(0xffffff800930ae77);
  }
  if (lRamffffff800972db08 == lStack_48) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
