
loop:
  add16 %g0, -16, %g1
  add16 %g0, -1, %g2
  add16 %g0, 15, %g3
  add16 %g0, %lo(0xff), %g3
  eadd16 %g0, -32768, %g1
  eadd16 %g0, -1, %g2
  eadd16 %g0, 32767, %g3
  eadd16 %g0, %lo(0xffff), %g3
  and16 %g0, -16, %g1
  and16 %g0, -1, %g2
  and16 %g0, 15, %g3
  eand16 %g0, -32768, %g1
  eand16 %g0, -1, %g2
  eand16 %g0, 32767, %g3

  ld16 [%g0 + -1], %g1 
  ld16 [-1 + %g0], %g1 
  ld16 [%g0 + %lo(0xffff)], %g1
  eld16 [%g0 + -1], %g1 
  eld16 [-1 + %g0], %g1 
  eld16 [%g0 + %lo(0xffffeeee)], %g1
  ldd16 [%g0 + -1], %g1 
  ldd16 [-1 + %g0], %g1 
  ldd16 [%g0 + %lo(0xffff)], %g1
  eldd16 [%g0 + -1], %g1 
  eldd16 [-1 + %g0], %g1 
  eldd16 [%g0 + %lo(0xffffeeee)], %g1

  st16 %g1, [%g0 + -1]
  st16 %g1, [-1 + %g0]
  st16 %g1, [%g0 + %lo(0xffff)]
  est16 %g1, [%g0 + -1]
  est16 %g1, [-1 + %g0]
  est16 %g1, [%g0 + %lo(0xffffeeee)]
  std16 %g1, [%g0 + -1]
  std16 %g1, [-1 + %g0]
  std16 %g1, [%g0 + %lo(0xffff)]
  estd16 %g1, [%g0 + -1]
  estd16 %g1, [-1 + %g0]
  estd16 %g1, [%g0 + %lo(0xffffeeee)]

  srl16 %g0, 0x1f, %g3
  srl16 %g0, %lo(0xffffeeee), %g3
  sll16 %g0, 0x1f, %g3
  sll16 %g0, %lo(0xffffeeee), %g3
  sra16 %g0, 0x1f, %g3
  sra16 %g0, %lo(0xffffeeee), %g3
  


