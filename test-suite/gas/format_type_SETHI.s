
loop:
  sethi32 0x3fffff, %i1 # high part of 0xfffffc00
  sethi32 %hi(0xfffffc00), %i1
  sethi32 %hi(loop), %i1
