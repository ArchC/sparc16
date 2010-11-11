
constant:
  addx16 %i0, -1, %i1
  addx16 %i0, %lo(0xeeff), %i1
  eaddx16 %i0, -1, %i1
  eaddx16 %i0, %lo(0xeeeffff), %i1

  subx16 %i0, -1, %i1
  subx16 %i0, %lo(0xeeff), %i1
  esubx16 %i0, -1, %i1
  esubx16 %i0, %lo(0xeeeffff), %i1

  smul16 %i0, -1, %i1
  smul16 %i0, %lo(0xeeff), %i1
  esmul16 %i0, -1, %i1
  esmul16 %i0, %lo(0xeeeffff), %i1

  umul16 %i0, -1, %i1
  umul16 %i0, %lo(0xeeff), %i1
  eumul16 %i0, -1, %i1
  eumul16 %i0, %lo(0xeeeffff), %i1

  sdiv16 %i0, -1, %i1
  sdiv16 %i0, %lo(0xeeff), %i1
  esdiv16 %i0, -1, %i1
  esdiv16 %i0, %lo(0xeeeffff), %i1

  udiv16 %i0, -1, %i1
  udiv16 %i0, %lo(0xeeff), %i1
  eudiv16 %i0, -1, %i1
  eudiv16 %i0, %lo(0xeeeffff), %i1

  or16 %i0, -1, %i1
  or16 %i0, %lo(0xeeff), %i1
  eor16 %i0, -1, %i1
  eor16 %i0, %lo(0xeeeffff), %i1

  xor16 %i0, -1, %i1
  xor16 %i0, %lo(0xeeff), %i1
  exor16 %i0, -1, %i1
  exor16 %i0, 0xaa, %i1
  exor16 %i0, %lo(0xeeeffff), %i1

  orn16 %i0, -1, %i1
  orn16 %i0, %lo(0xeeff), %i1
  eorn16 %i0, -1, %i1
  eorn16 %i0, %lo(0xeeeffff), %i1

  xnor16 %i0, -1, %i1
  xnor16 %i0, %lo(0xeeff), %i1
  exnor16 %i0, -1, %i1
  exnor16 %i0, %lo(0xeeeffff), %i1

  
