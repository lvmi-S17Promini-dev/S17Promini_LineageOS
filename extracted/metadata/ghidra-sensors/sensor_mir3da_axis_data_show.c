/* Stock Image offsets 0x632320..0x6323c0 */

void sensor_mir3da_axis_data_show(undefined8 param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  int iVar2;
  long lVar3;
  short asStack_34 [2];
  short asStack_30 [2];
  short asStack_2c [2];
  long lStack_28;

  lStack_28 = lRamffffff800972db08;
  _mcount();
  iVar2 = sensor_mir3da_read_data(uRamffffff80099daaf0,asStack_2c,asStack_30,asStack_34);
  uVar1 = uRamffffff80093f4dca;
  if (iVar2 == 0) {
    iVar2 = func_0xffffff8008f9fb58
                      (param_2,0xffffff8009391f28,(long)asStack_2c[0],(long)asStack_30[0],
                       (long)asStack_34[0]);
    lVar3 = (long)iVar2;
  }
  else {
    lVar3 = 0xf;
    *param_2 = uRamffffff80093f4dc2;
    param_2[1] = uVar1;
  }
  if (lRamffffff800972db08 == lStack_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}
