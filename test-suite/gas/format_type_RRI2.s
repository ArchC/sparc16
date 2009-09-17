
constant:
  addx16 %r0, -1, %r1
  addx16 %r0, %lo(0xeeff), %r1
  eaddx16 %r0, -1, %r1
  eaddx16 %r0, %lo(0xeeeffff), %r1

  subx16 %r0, -1, %r1
  subx16 %r0, %lo(0xeeff), %r1
  esubx16 %r0, -1, %r1
  esubx16 %r0, %lo(0xeeeffff), %r1

  smul16 %r0, -1, %r1
  smul16 %r0, %lo(0xeeff), %r1
  esmul16 %r0, -1, %r1
  esmul16 %r0, %lo(0xeeeffff), %r1

  umul16 %r0, -1, %r1
  umul16 %r0, %lo(0xeeff), %r1
  eumul16 %r0, -1, %r1
  eumul16 %r0, %lo(0xeeeffff), %r1

  sdiv16 %r0, -1, %r1
  sdiv16 %r0, %lo(0xeeff), %r1
  esdiv16 %r0, -1, %r1
  esdiv16 %r0, %lo(0xeeeffff), %r1

  udiv16 %r0, -1, %r1
  udiv16 %r0, %lo(0xeeff), %r1
  eudiv16 %r0, -1, %r1
  eudiv16 %r0, %lo(0xeeeffff), %r1

  or16 %r0, -1, %r1
  or16 %r0, %lo(0xeeff), %r1
  eor16 %r0, -1, %r1
  eor16 %r0, %lo(0xeeeffff), %r1

  xor16 %r0, -1, %r1
  xor16 %r0, %lo(0xeeff), %r1
  exor16 %r0, -1, %r1
  exor16 %r0, %lo(0xeeeffff), %r1

  orn16 %r0, -1, %r1
  orn16 %r0, %lo(0xeeff), %r1
  eorn16 %r0, -1, %r1
  eorn16 %r0, %lo(0xeeeffff), %r1

  xnor16 %r0, -1, %r1
  xnor16 %r0, %lo(0xeeff), %r1
  exnor16 %r0, -1, %r1
  exnor16 %r0, %lo(0xeeeffff), %r1

  ## load and store
  ldsh16 [%g0 + -1], %g1 
  ldsh16 [-1 + %g0], %g1 
  ldsh16 [%g0 + %lo(0xffff)], %g1
  eldsh16 [%g0 + -1], %g1 
  eldsh16 [-1 + %g0], %g1 
  eldsh16 [%g0 + %lo(0xffffeeee)], %g1

  lduh16 [%g0 + -1], %g1 
  lduh16 [-1 + %g0], %g1 
  lduh16 [%g0 + %lo(0xffff)], %g1
  elduh16 [%g0 + -1], %g1 
  elduh16 [-1 + %g0], %g1 
  elduh16 [%g0 + %lo(0xffffeeee)], %g1

  ldsb16 [%g0 + -1], %g1 
  ldsb16 [-1 + %g0], %g1 
  ldsb16 [%g0 + %lo(0xffff)], %g1
  eldsb16 [%g0 + -1], %g1 
  eldsb16 [-1 + %g0], %g1 
  eldsb16 [%g0 + %lo(0xffffeeee)], %g1

  ldub16 [%g0 + -1], %g1 
  ldub16 [-1 + %g0], %g1 
  ldub16 [%g0 + %lo(0xffff)], %g1
  eldub16 [%g0 + -1], %g1 
  eldub16 [-1 + %g0], %g1 
  eldub16 [%g0 + %lo(0xffffeeee)], %g1

  stb16 %g1, [%g0 + -1]
  stb16 %g1, [-1 + %g0]
  stb16 %g1, [%g0 + %lo(0xffff)]
  estb16 %g1, [%g0 + -1]
  estb16 %g1, [-1 + %g0]
  estb16 %g1, [%g0 + %lo(0xffffeeee)]

  sth16 %g1, [%g0 + -1]
  sth16 %g1, [-1 + %g0]
  sth16 %g1, [%g0 + %lo(0xffff)]
  esth16 %g1, [%g0 + -1]
  esth16 %g1, [-1 + %g0]
  esth16 %g1, [%g0 + %lo(0xffffeeee)]
