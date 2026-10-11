/* Stock Image offsets 0x62c590..0x62c61c */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sensor_ps_report_interrupt_data(int value)

{
  long lVar1;

  _mcount();
  lVar1 = lRamffffff80099da9f8;
  func_0xffffff8008125e6c(0xffffff800940577f,0xffffff800932c767,value);
  if ((value != -1) && (*(char *)(lVar1 + 0x2d2) == '\0')) {
    *(undefined1 *)(lVar1 + 0x2d2) = 1;
    DataMemoryBarrier(2,3);
    func_0xffffff8008147e24(lVar1 + 0x130);
    DataMemoryBarrier(2,3);
    func_0xffffff80080d4940(lVar1 + 0x78);
  }
  if (*(char *)(lVar1 + 0x2d1) == '\0') {
    func_0xffffff80086ac23c(value,3,0);
  }
  return 0;
}
