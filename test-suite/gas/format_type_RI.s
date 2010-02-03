# TODO: esethi support for %gdop_hix22/lowx10

loop:
  cmp16 %i0, 0xff
  ecmp16 %i0, 0x1fff
  cmp16 %o0, -1
  cmp16 %i0, -128
  cmp16 %i0, 127
  ecmp16 %i0, -8192 # -2^13
  ecmp16 %i0, 8191 # 2^13 - 1
  ecmp16 %i0, -1
  mov16 0xff, %i0
  mov16 -1, %i0
  emov16 0x7ffff, %i0
  esethi16 %hi(0x7ffff000), %i0
  esethi16 loop+3, %i0 
  esethi16 %hi(loop), %i0
