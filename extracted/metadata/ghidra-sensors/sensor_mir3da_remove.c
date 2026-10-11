/* Stock Image offsets 0x6312c4..0x63136c */

undefined8 sensor_mir3da_remove(long param_1)

{
  long lVar1;
  long lVar2;

  _mcount();
  lVar1 = lRamffffff80097db048 + 0x28;
  if (lVar1 == 0) {
    if ((bRamffffff80097db2e8 & 1) != 0) {
      func_0xffffff8008125e6c(0xffffff80093a2ba2,0xffffffea);
    }
  }
  else {
    lVar2 = 0;
    do {
      func_0xffffff8008648cd4(lVar1,*(undefined8 *)(lVar2 + -0x7ff6f01548));
      lVar2 = lVar2 + 8;
    } while (lVar2 != 0x40);
  }
  uRamffffff80099daae0 = 0;
  func_0xffffff8008a885c8(param_1);
  func_0xffffff80086b0268(0xffffff80097db198);
  func_0xffffff8008251b9c(*(undefined8 *)(param_1 + 0xd8));
  return 0;
}
