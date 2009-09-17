
  addfp -32, %r0
  addfp -1, %r0
  addfp 31, %r0
  eaddfp -65536, %r0
  eaddfp -1, %r0
  eaddfp 65535, %r0

  addsp -32, %r0
  addsp -1, %r0
  addsp 31, %r0
  eaddsp -65536, %r0
  eaddsp -1, %r0
  eaddsp 65535, %r0

  btst16 -32, %r0
  btst16 -1, %r0
  btst16 31, %r0
  ebtst16 -65536, %r0
  ebtst16 -1, %r0
  ebtst16 65535, %r0

loop:
  ldfp [%lo(loop)], %r0
  ldfp [-32], %r0
  eldfp [%lo(loop)], %r0
  eldfp [-65536], %r0

  stfp %r0, [%lo(loop)]
  stfp %r0, [-32]
  estfp %r0, [%lo(loop)]
  estfp %r0, [-65536]

  ldsp [%lo(loop)], %r0
  ldsp [-32], %r0
  eldsp [%lo(loop)], %r0
  eldsp [-65536], %r0

  stsp %r0, [%lo(loop)]
  stsp %r0, [-32]
  estsp %r0, [%lo(loop)]
  estsp %r0, [-65536]

  clrw [%r0 + -32]
  clrw [-32 + %r0]
  eclrw [%r0 + -65536]
  eclrw [-65536 + %r0]

