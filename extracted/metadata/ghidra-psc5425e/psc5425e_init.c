/* Stock Image offsets 0x1550ab0..0x1550b6c */

int psc5425e_init(void)

{
  int iVar1;
  undefined8 uVar2;

  func_0xffffff800809f01c();
  func_0xffffff8008125e6c(0xffffff80094e72bc,0xffffff80093aab0e);
  iVar1 = func_0xffffff8008a89350(0,0xffffff8009829928);
  uVar2 = 0xffffff8009334b9a;
  if (iVar1 != 0) {
    uVar2 = 0xffffff8009389315;
  }
  func_0xffffff8008125e6c(uVar2,0xffffff80093aab0e);
  iVar1 = func_0xffffff8008649e00(0xffffff8009829540);
  if (iVar1 == 0) {
    iVar1 = func_0xffffff800864a47c(0xffffff8009829c48,0);
    if (iVar1 == 0) {
      return 0;
    }
    uVar2 = 0xffffff80093bad45;
  }
  else {
    uVar2 = 0xffffff80093aab1c;
  }
  func_0xffffff8008125e6c(uVar2,0xffffff80093aab0e,iVar1);
  return iVar1;
}
