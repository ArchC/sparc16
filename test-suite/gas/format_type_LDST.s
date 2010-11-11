
constant:
  ## load and store
  ldd16 [%i0 + -1], %g1 
  ldd16 [-1 + %i0], %g1 
  ldd16 [%i0 + %lo(0xffff)], %g1
  eldd16 [%i0 + -1], %g1 
  eldd16 [-1 + %i0], %g1 
  eldd16 [%i0 + %lo(0xffffeeee)], %g1

  std16 %g1, [%i0 + -1]
  std16 %g1, [-1 + %i0]
  std16 %g1, [%i0 + %lo(0xffff)]
  estd16 %g1, [%i0 + -1]
  estd16 %g1, [-1 + %i0]
  estd16 %g1, [%i0 + %lo(0xffffeeee)]

  ldsh16 [%i0 + -1], %g1 
  ldsh16 [-1 + %i0], %g1 
  ldsh16 [%i0 + %lo(0xffff)], %g1
  eldsh16 [%i0 + -1], %g1 
  eldsh16 [-1 + %i0], %g1 
  eldsh16 [%i0 + %lo(0xffffeeee)], %g1

  lduh16 [%i0 + -1], %g1 
  lduh16 [-1 + %i0], %g1 
  lduh16 [%i0 + %lo(0xffff)], %g1
  elduh16 [%i0 + -1], %g1 
  elduh16 [-1 + %i0], %g1 
  elduh16 [%i0 + %lo(0xffffeeee)], %g1

  ldsb16 [%i0 + -1], %g1 
  ldsb16 [-1 + %i0], %g1 
  ldsb16 [%i0 + %lo(0xffff)], %g1
  eldsb16 [%i0 + -1], %g1 
  eldsb16 [-1 + %i0], %g1 
  eldsb16 [%i0 + %lo(0xffffeeee)], %g1

  ldub16 [%i0 + -1], %g1 
  ldub16 [-1 + %i0], %g1 
  ldub16 [%i0 + %lo(0xffff)], %g1
  eldub16 [%i0 + -1], %g1 
  eldub16 [-1 + %i0], %g1 
  eldub16 [%i0 + %lo(0xffffeeee)], %g1

  stb16 %g1, [%i0 + -1]
  stb16 %g1, [-1 + %i0]
  stb16 %g1, [%i0 + %lo(0xffff)]
  estb16 %g1, [%i0 + -1]
  estb16 %g1, [-1 + %i0]
  estb16 %g1, [%i0 + %lo(0xffffeeee)]

  sth16 %g1, [%i0 + -1]
  sth16 %g1, [-1 + %i0]
  sth16 %g1, [%i0 + %lo(0xffff)]
  esth16 %g1, [%i0 + -1]
  esth16 %g1, [-1 + %i0]
  esth16 %g1, [%i0 + %lo(0xffffeeee)]
