/* Stock Image offsets 0x632ad4..0x632c30 */

int sensor_mir3da_read_data(undefined8 param_1,short *param_2,short *param_3,short *param_4)

{
  int iVar1;
  int iVar2;
  int iStack_44;
  int iStack_40;
  int iStack_3c;
  long lStack_38;

  lStack_38 = lRamffffff800972db08;
  _mcount();
  iStack_3c = 0;
  iStack_44 = 0;
  iStack_40 = 0;
  iVar1 = sensor_mir3da_read_raw_data(param_1,param_2,param_3,param_4);
  if (iVar1 == 0) {
    iVar2 = 0;
    if ((((cRamffffff80099dab30 != 'K') && (cRamffffff80099dab30 != -0x74)) &&
        (iVar2 = 0, cRamffffff80099dab30 != -0x36)) && (iRamffffff80099dab3c != 5)) {
      iStack_3c = (int)*param_2;
      iStack_40 = (int)*param_3;
      iStack_44 = (int)*param_4;
      sensor_mir3da_temp_calibrate(&iStack_3c,&iStack_40,&iStack_44);
      *param_2 = (short)iStack_3c;
      *param_3 = (short)iStack_40;
      iVar2 = iStack_44;
    }
    *param_4 = (short)iVar2;
    if ((bRamffffff80097db2e8 >> 4 & 1) != 0) {
      (**(code **)(lRamffffff80099dab20 + 0x40))
                (0xffffff80093e3e28,(long)*param_2,(long)*param_3,(int)(short)iVar2);
    }
  }
  else if ((bRamffffff80097db2e8 & 1) != 0) {
    (**(code **)(lRamffffff80099dab20 + 0x40))(0xffffff80094161ca,iVar1);
  }
  if (lRamffffff800972db08 == lStack_38) {
    return iVar1;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
