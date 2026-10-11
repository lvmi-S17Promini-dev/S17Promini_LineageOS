/* Stock Image offsets 0xa4aaf0..0xa4ab40 */

undefined8 psc5425e_dump_register(void)

{
  int iVar1;
  ulong uVar2;

  func_0xffffff800809f01c();
  uVar2 = 0;
  do {
    iVar1 = psc5425e_read_byte(uVar2 & 0xffffffff,uVar2 - 0x7ff63f3840);
    if (iVar1 == 0) {
      return 1;
    }
    uVar2 = uVar2 + 1;
  } while (uVar2 != 0xe);
  return 0;
}
