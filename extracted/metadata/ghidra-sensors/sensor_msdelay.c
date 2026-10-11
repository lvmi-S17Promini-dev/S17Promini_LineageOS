/* Stock Image offsets 0x630c24..0x630c60 */

void sensor_msdelay(int param_1)

{
  long lVar1;

  _mcount();
  if (param_1 != 0) {
    lVar1 = (long)param_1;
    do {
      lVar1 = lVar1 + -1;
      func_0xffffff8008f8f060(0x418958);
    } while (lVar1 != 0);
  }
  return;
}
