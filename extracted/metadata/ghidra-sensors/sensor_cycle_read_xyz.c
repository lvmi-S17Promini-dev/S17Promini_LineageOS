/* Stock Image offsets 0x633324..0x633448 */

undefined8
sensor_cycle_read_xyz(undefined8 param_1,int *param_2,int *param_3,int *param_4,int param_5)

{
  int iVar1;
  long lVar2;
  int iVar3;
  short asStack_64 [2];
  short asStack_60 [2];
  short asStack_5c [2];
  long lStack_58;

  lStack_58 = lRamffffff800972db08;
  _mcount();
  *param_4 = 0;
  *param_3 = 0;
  *param_2 = 0;
  iVar3 = param_5;
  if (param_5 == 0) {
    iVar3 = 0;
  }
  else {
    do {
      asStack_64[0] = 0;
      asStack_60[0] = 0;
      asStack_5c[0] = 0;
      sensor_mir3da_read_raw_data(param_1,asStack_5c,asStack_60,asStack_64);
      *param_2 = *param_2 + (int)asStack_5c[0];
      *param_3 = *param_3 + (int)asStack_60[0];
      *param_4 = *param_4 + (int)asStack_64[0];
      (**(code **)(lRamffffff80099dab20 + 0x50))(5);
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    iVar3 = *param_2;
  }
  iVar1 = 0;
  if (param_5 != 0) {
    iVar1 = iVar3 / param_5;
  }
  *param_2 = iVar1;
  lVar2 = lRamffffff800972db08;
  iVar3 = 0;
  if (param_5 != 0) {
    iVar3 = *param_3 / param_5;
  }
  *param_3 = iVar3;
  iVar3 = 0;
  if (param_5 != 0) {
    iVar3 = *param_4 / param_5;
  }
  *param_4 = iVar3;
  if (lVar2 != lStack_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}
