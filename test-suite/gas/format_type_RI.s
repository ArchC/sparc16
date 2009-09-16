# TODO: esethi support for %gdop_hix22/lowx10

loop:
  cmp16 %g0, 0xff
  ecmp16 %g0, 0xddff
  cmp16 %g2, -1
  cmp16 %g0, -128
  cmp16 %g0, 127
  ecmp16 %g0, -262144 # -2^18
  ecmp16 %g0, 262143 # 2^18 - 1
  ecmp16 %g0, -1
  mov16 0xff, %g0
  mov16 -1, %g0
  emov16 0x7ffff, %g0
  esethi16 %hi(0x7ffff000), %g0
  esethi16 loop+3, %g0 
  esethi16 %hi(loop), %g0
