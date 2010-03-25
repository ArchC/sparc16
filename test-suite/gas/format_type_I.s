
# this instructions all use pcrel operands, so
# they use an addend to hold the symbol, we should
# test if they fit only in the linker

loop:
  b16 loop
  eb16 loop
  be16 loop
  ebe16 loop
  bne16 loop
  ebne16 loop

  #b16,a loop
  #eb16,a loop
  #be16,a loop
  #ebe16,a loop
  #bne16,a loop
  #ebne16,a loop

  call16 loop
  ecall16 loop
  callx loop
