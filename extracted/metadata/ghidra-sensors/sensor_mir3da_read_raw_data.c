/* Stock Image offsets 0x632c30..0x632de0 */

void sensor_mir3da_read_raw_data
               (undefined8 param_1,short *param_2,short *param_3,undefined2 *param_4)

{
  int iVar1;
  undefined8 uVar2;
  long lVar3;
  undefined4 uStack_50;
  ushort uStack_4c;
  long lStack_48;

  lStack_48 = lRamffffff800972db08;
  _mcount();
  uStack_4c = 0;
  uStack_50 = 0;
  iVar1 = (**(code **)(lRamffffff80099dab20 + 0x10))
                    (param_1,*(undefined1 *)
                              (lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + 0x76),6,
                     &uStack_50);
  if (iVar1 == 6) {
    lVar3 = lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8;
    *param_2 = (short)((int)(short)((ushort)uStack_50._1_1_ <<
                                    (ulong)(*(byte *)(lVar3 + 0x8e) & 0x1f) |
                                   (ushort)(byte)uStack_50) >> (8 - *(byte *)(lVar3 + 0x8f) & 0x1f))
    ;
    lVar3 = lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8;
    *param_3 = (short)((int)(short)((ushort)uStack_50._3_1_ <<
                                    (ulong)(*(byte *)(lVar3 + 0x8e) & 0x1f) |
                                   (ushort)uStack_50._2_1_) >> (8 - *(byte *)(lVar3 + 0x8f) & 0x1f))
    ;
    lVar3 = lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8;
    *param_4 = (short)((int)(short)((uStack_4c >> 8) << (ulong)(*(byte *)(lVar3 + 0x8e) & 0x1f) |
                                   uStack_4c & 0xff) >> (8 - *(byte *)(lVar3 + 0x8f) & 0x1f));
    if ((bRamffffff80097db2e8 >> 4 & 1) != 0) {
      (**(code **)(lRamffffff80099dab20 + 0x40))(0xffffff8009437b02,(long)*param_2,(long)*param_3);
    }
    uVar2 = 0;
  }
  else {
    if ((bRamffffff80097db2e8 & 1) != 0) {
      (**(code **)(lRamffffff80099dab20 + 0x40))(0xffffff800931bb2c);
    }
    uVar2 = 0xffffffff;
  }
  if (lRamffffff800972db08 == lStack_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}
