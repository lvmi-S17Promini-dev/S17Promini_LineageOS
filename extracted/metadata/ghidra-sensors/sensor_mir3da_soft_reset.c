/* Stock Image offsets 0x633d9c..0x633e04 */

undefined4 sensor_mir3da_soft_reset(undefined8 param_1)

{
  undefined4 uVar1;
  long lVar2;

  _mcount();
  lVar2 = lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8;
  uVar1 = sensor_mir3da_register_mask_write
                    (param_1,*(undefined2 *)(lVar2 + 0x12),*(undefined1 *)(lVar2 + 0x14),
                     *(undefined1 *)(lVar2 + 0x15));
  (**(code **)(lRamffffff80099dab20 + 0x50))(5);
  return uVar1;
}
