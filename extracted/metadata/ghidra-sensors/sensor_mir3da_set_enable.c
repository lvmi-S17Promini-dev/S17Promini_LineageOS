/* Stock Image offsets 0x633684..0x6336e8 */

void sensor_mir3da_set_enable(undefined8 param_1,char param_2)

{
  undefined1 uVar1;
  long lVar2;

  _mcount();
  if (param_2 == '\0') {
    uVar1 = *(undefined1 *)(lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + 0x19);
  }
  else {
    uVar1 = 0;
  }
  lVar2 = lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8;
  sensor_mir3da_register_mask_write
            (param_1,*(undefined2 *)(lVar2 + 0x16),*(undefined1 *)(lVar2 + 0x18),uVar1);
  return;
}
