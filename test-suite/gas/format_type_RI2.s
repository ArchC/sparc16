
  addfp -32, %i0
  addfp -1, %i0
  addfp 31, %i0
  eaddfp -65536, %i0
  eaddfp -1, %i0
  eaddfp 65535, %i0

  addsp -32, %i0
  addsp -1, %i0
  addsp 31, %i0
  eaddsp -65536, %i0
  eaddsp -1, %i0
  eaddsp 65535, %i0

  btst16 -32, %i0
  btst16 -1, %i0
  btst16 31, %i0
  ebtst16 -65536, %i0
  ebtst16 -1, %i0
  ebtst16 65535, %i0

loop:
  ldfp [%lo(loop)], %i0
  ldfp [-32], %i0
  eldfp [%lo(loop)], %i0
  eldfp [-65536], %i0

  stfp %i0, [%lo(loop)]
  stfp %i0, [-32]
  estfp %i0, [%lo(loop)]
  estfp %i0, [-65536]

  ldsp [%lo(loop)], %i0
  ldsp [-32], %i0
  eldsp [%lo(loop)], %i0
  eldsp [-65536], %i0

  stsp %i0, [%lo(loop)]
  stsp %i0, [-32]
  estsp %i0, [%lo(loop)]
  estsp %i0, [-65536]

  clrw [%i0 + -32]
  clrw [-32 + %i0]
  eclrw [%i0 + -65536]
  eclrw [-65536 + %i0]

