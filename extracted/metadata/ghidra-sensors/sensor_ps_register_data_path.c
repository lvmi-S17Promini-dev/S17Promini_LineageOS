/* Stock Image offsets 0x62d1f0..0x62d28c */

undefined8 sensor_ps_register_data_path(long *param_1)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;

  _mcount();
  lVar1 = lRamffffff80099da9f8;
  lVar3 = *param_1;
  *(long *)(lRamffffff80099da9f8 + 0x2b0) = lVar3;
  *(int *)(lVar1 + 0x2c0) = (int)param_1[2];
  *(long *)(lVar1 + 0x2b8) = param_1[1];
  if ((bRamffffff80098a2eba >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a2e98,0xffffff80093704da);
    lVar3 = *(long *)(lVar1 + 0x2b0);
  }
  if (lVar3 == 0) {
    if ((bRamffffff80098a2ee2 >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098a2ec0,0xffffff80093f4c43);
    }
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}
