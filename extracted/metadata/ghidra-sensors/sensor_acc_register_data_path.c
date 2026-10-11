/* Stock Image offsets 0x62f6c0..0x62f75c */

undefined8 sensor_acc_register_data_path(long *param_1)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;

  _mcount();
  lVar1 = lRamffffff80099daa70;
  lVar3 = *param_1;
  *(long *)(lRamffffff80099daa70 + 0x1a0) = lVar3;
  *(long *)(lVar1 + 0x1a8) = param_1[1];
  *(int *)(lVar1 + 0x1b0) = (int)param_1[2];
  if ((bRamffffff80098a36b2 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098a3690,0xffffff800932c862);
    lVar3 = *(long *)(lVar1 + 0x1a0);
  }
  if (lVar3 == 0) {
    if ((bRamffffff80098a36da >> 2 & 1) != 0) {
      func_0xffffff80084a0cc4(0xffffff80098a36b8,0xffffff8009391e53);
    }
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}
