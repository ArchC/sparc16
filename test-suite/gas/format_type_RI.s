# TODO: esethi support for %gdop_hix22/lowx10

loop:
  cmp16 %i1, 0xff
  ecmp16 %i1, 0x1fff
  cmp16 %o0, -1
  cmp16 %i1, -128
  cmp16 %i1, 127
  ecmp16 %i1, -4096 # -2^12
  ecmp16 %i1, 4095 # 2^12 - 1
  ecmp16 %i1, -1
  mov16 0xff, %i1
  mov16 -1, %i1
  emov16 0x7ffff, %i1
  esethi16 %hi(0x7ffff000), %i1
  esethi16 loop+3, %i1 
  esethi16 %hi(loop), %i1
