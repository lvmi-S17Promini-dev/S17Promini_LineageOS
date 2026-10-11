/* Stock Image offsets 0x633448..0x6334d8 */

undefined8 sensor_mir3da_read_offset(undefined8 param_1,long param_2)

{
  short sVar1;
  undefined8 uVar2;
  long lVar3;

  _mcount();
  lVar3 = 0x46;
  while( true ) {
    sVar1 = *(short *)(lRamffffff80099dab28 + (long)iRamffffff80097db2ec * 0xb8 + lVar3);
    if (sVar1 < 0) {
      return 0;
    }
    uVar2 = (*(code *)*puRamffffff80099dab20)(param_1,(long)sVar1,param_2);
    if ((int)uVar2 != 0) break;
    lVar3 = lVar3 + 4;
    param_2 = param_2 + 1;
    if (lVar3 == 0x6a) {
      return 0;
    }
  }
  return uVar2;
}
