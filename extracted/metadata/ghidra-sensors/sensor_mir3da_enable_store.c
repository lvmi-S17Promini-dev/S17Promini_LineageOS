/* Stock Image offsets 0x6322b0..0x632320 */

long sensor_mir3da_enable_store(undefined8 param_1,long param_2,int param_3)

{
  undefined8 uVar1;
  int iVar2;
  long lVar3;

  _mcount();
  uVar1 = uRamffffff80099daaf0;
  if (param_2 == 0) {
    lVar3 = -1;
  }
  else {
    lVar3 = func_0xffffff8008f9e4e4(param_2,0,10);
    iVar2 = sensor_mir3da_set_enable(uVar1,lVar3 != 0);
    lVar3 = -0x16;
    if (-1 < iVar2) {
      lVar3 = (long)param_3;
    }
  }
  return lVar3;
}
