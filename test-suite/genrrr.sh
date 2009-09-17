
for i in "add_reg16 10101 00" "sub_reg16 10101 01" "and_reg16 10101 10" \
"or_reg16 10101 11" "smul_reg16 10110 00" "sdiv_reg16 10110 01" \
"umul_reg16 10110 10" "udiv_reg16 10110 11" "st_reg16 10111 00" \
"ld_reg16 10111 01" "xor_reg16 10111 10"; do

  set $i
  name="$1"

  op=`echo "obase=16; ibase=2; $2" | bc`
  op="0x$op"
  op2=`echo "obase=16; ibase=2; $3" | bc`
  op2="0x$op2"

  matchload="ld"
  matchstore="st"

  if [[ "$name" =~ "${matchload}" ]]; then
    echo "    $name.set_asm(\"$name [%reg + %reg], %reg\", rs1, rs2, rd);"
  fi

  if [[ "$name" =~ "${matchstore}" ]]; then
    echo "    $name.set_asm(\"$name %reg, [%reg + %reg]\", rd, rs1, rs2);"
  fi

  if [[ ! "$name" =~ "${matchload}" ]] && [[ ! "$name" =~ "${matchstore}" ]]; then
    echo "    $name.set_asm(\"$name %reg, %reg, %reg\", rs1, rs2, rd);"
  fi

  echo "    $name.set_decoder(op=${op}, op2=${op2});"
  echo ""
done
