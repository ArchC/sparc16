
loop:
  add16 %i0, -16, %g1
  add16 %i0, -1, %o0
  add16 %i0, 15, %o0
  add16 %i0, %lo(0xff), %o0
  eadd16 %i0, -32768, %g1
  eadd16 %i0, -1, %o0
  eadd16 %i0, 32767, %o0
  eadd16 %i0, %lo(0xffff), %o0
  and16 %i0, -16, %g1
  and16 %i0, -1, %o0
  and16 %i0, 15, %o0
  eand16 %i0, -32768, %g1
  eand16 %i0, -1, %o0
  eand16 %i0, 32767, %o0

  ld16 [%i0 + -1], %g1 
  ld16 [-1 + %i0], %g1 
  ld16 [%i0 + %lo(0xffff)], %g1
  eld16 [%i0 + -1], %g1 
  eld16 [-1 + %i0], %g1 
  eld16 [%i0 + %lo(0xffffeeee)], %g1
  ldd16 [%i0 + -1], %g1 
  ldd16 [-1 + %i0], %g1 
  ldd16 [%i0 + %lo(0xffff)], %g1
  eldd16 [%i0 + -1], %g1 
  eldd16 [-1 + %i0], %g1 
  eldd16 [%i0 + %lo(0xffffeeee)], %g1

  st16 %g1, [%i0 + -1]
  st16 %g1, [-1 + %i0]
  st16 %g1, [%i0 + %lo(0xffff)]
  est16 %g1, [%i0 + -1]
  est16 %g1, [-1 + %i0]
  est16 %g1, [%i0 + %lo(0xffffeeee)]
  std16 %g1, [%i0 + -1]
  std16 %g1, [-1 + %i0]
  std16 %g1, [%i0 + %lo(0xffff)]
  estd16 %g1, [%i0 + -1]
  estd16 %g1, [-1 + %i0]
  estd16 %g1, [%i0 + %lo(0xffffeeee)]

  srl16 %i0, 0x1f, %o0
  srl16 %i0, %lo(0xffffeeee), %o0
  sll16 %i0, 0x1f, %o0
  sll16 %i0, %lo(0xffffeeee), %o0
  sra16 %i0, 0x1f, %o0
  sra16 %i0, %lo(0xffffeeee), %o0
  


