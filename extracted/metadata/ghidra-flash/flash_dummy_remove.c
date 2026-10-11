/* Stock Image offsets 0x8c3f98..0x8c407c */

undefined8 flash_dummy_remove(long param_1)

{
  long lVar1;
  int *piVar2;
  long lVar3;

  func_0xffffff800809f01c();
  piVar2 = *(int **)(param_1 + 0xc0);
  if ((bRamffffff80098c7472 >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c7450,0xffffff8009365a57,0xffffff800941c9e0);
  }
  *(undefined8 *)(param_1 + 0xc0) = 0;
  if ((piVar2 == (int *)0x0) || (*piVar2 == 0)) {
    func_0xffffff8008940974(0xffffff8009343cdf);
  }
  else if (0 < *piVar2) {
    lVar1 = 0;
    lVar3 = 0;
    do {
      func_0xffffff8008940c48(*(long *)(piVar2 + 2) + lVar1);
      lVar3 = lVar3 + 1;
      lVar1 = lVar1 + 0x34;
    } while (lVar3 < *piVar2);
  }
  func_0xffffff80080d4734(0xffffff8009af0088);
  if ((bRamffffff80098c749a >> 2 & 1) != 0) {
    func_0xffffff80084a0cc4(0xffffff80098c7478,0xffffff80093981ff,0xffffff800941c9e0);
  }
  return 0;
}
