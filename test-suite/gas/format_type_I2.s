
loop:
  movra %i1, %r20
  movrb %r20, %i0
  savesp -256
  savesp -1
  savesp 255
  esavesp -524288
  esavesp -1
  esavesp 524287

  bn16 loop
  ble16 loop
  bl16 loop
  bleu16 loop
  bcs16 loop
  bneg16 loop
  bvs16 loop
  bg16 loop
  bge16 loop
  bgu16 loop
  bcc16 loop
  bpos16 loop
  bvc16 loop
  ebn16 loop
  eble16 loop
  ebl16 loop
  ebleu16 loop
  ebcs16 loop
  ebneg16 loop
  ebvs16 loop
  ebg16 loop
  ebge16 loop
  ebgu16 loop
  ebcc16 loop
  ebpos16 loop
  ebvc16 loop

  # with annul
  bn16,a loop
  ble16,a loop
  bl16,a loop
  bleu16,a loop
  bcs16,a loop
  bneg16,a loop
  bvs16,a loop
  bg16,a loop
  bge16,a loop
  bgu16,a loop
  bcc16,a loop
  bpos16,a loop
  bvc16,a loop
  ebn16,a loop
  eble16,a loop
  ebl16,a loop
  ebleu16,a loop
  ebcs16,a loop
  ebneg16,a loop
  ebvs16,a loop
  ebg16,a loop
  ebge16,a loop
  ebgu16,a loop
  ebcc16,a loop
  ebpos16,a loop
  ebvc16,a loop

  sparc16bx loop
