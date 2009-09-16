
for i in "n 0" "g 10" "le 2" "ge 11" "l 3" "gu 12" "leu 4" "cc 13" "cs 5" \
  "pos 14" "neg 6" "vc 15" "vs 7"; do
  set $i
  name="$1"
  cc=$2
  opcode=`python -c "print hex( ((0x7 << 4) + $cc) >> 2 );"`
  cond=`python -c "print hex( ((0x7 << 4) + $cc) & 0x3 );"`
  echo "    b${name}16.set_asm(\"b${name}16%[anul] %exp(pcreli)\", an, simm8);"
  echo "    b${name}16.set_decoder(op=$opcode, cond=$cond);"
  echo "    e_b${name}16.set_asm(\"eb${name}16%[anul] %exp(pcreli)\", an, simm8+immext);"
  echo "    e_b${name}16.set_decoder(opext=0x0b, op=$opcode, cond=$cond);"
  echo ""
done
