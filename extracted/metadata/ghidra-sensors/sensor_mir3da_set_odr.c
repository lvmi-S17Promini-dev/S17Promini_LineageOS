/* Stock Image offsets 0x633734..0x63379c */

void sensor_mir3da_set_odr(undefined8 param_1,int param_2)

{
  long lVar1;
  ulong uVar2;

  _mcount();
  uVar2 = 2;
  if (5 < param_2) {
    uVar2 = (ulong)(param_2 < 0xb);
  }
  lVar1 = lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + uVar2 * 4;
  sensor_mir3da_register_mask_write
            (param_1,*(undefined2 *)(lVar1 + 0x6a),*(undefined1 *)(lVar1 + 0x6c),
             *(undefined1 *)(lVar1 + 0x6d));
  return;
}
