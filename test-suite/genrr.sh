
for i in "addx_reg16 00100" "subx_reg16 01100" "btst_reg16 00001" \
  "clrw16 00111" "clrb16 00101" "clrh16 00110" "cmp_reg16 10100" \
  "stb_reg16 00101" "sth_reg16 00110" "std_reg16 001111" \
  "lduh_reg16 00010" "ldub_reg16 00001" "ldd_reg16 00011" \
  "ldsb_reg16 01001" "ldsh_reg16 01010" "sra_reg16 10111" \
  "srl_reg16 10110" "sll_reg16 10101" "orn_reg16 00010" \
  "xnor_reg16 00001" "andn_reg16 01101" "neg_reg16 11101" \
  "restore16 00000"; do

  set $i
  name="$1"
  op2=`echo "obase=16; ibase=2; $2" | bc`
  op2="0x$op2"
  matchload="ld"
  matchstore="st"

  if [[ "$name" =~ "${matchload}" ]]; then
    echo "    $name.set_asm(\"$name [%reg], %reg\", rs, rd);"
  fi

  if [[ "$name" =~ "${matchstore}" ]]; then
    echo "    $name.set_asm(\"$name %reg, [%reg]\", rd, rs);"
  fi

  if [[ ! "$name" =~ "${matchload}" ]] && [[ ! "$name" =~ "${matchstore}" ]]; then
    echo "    $name.set_asm(\"$name %reg, %reg\", rs, rd);"
  fi

  echo "    $name.set_decoder(op=0xa, op2=${op2});"

  if [[ "$name" =~ "${matchload}" ]]; then
    echo "    e_$name.set_asm(\"e$name [%reg + %reg], %reg\", rsext, rs, rd);"
  fi

  if [[ "$name" =~ "${matchstore}" ]]; then
    echo "    e_$name.set_asm(\"e$name %reg, [%reg + %reg]\", rd, rsext, rs);"
  fi

  if [[ ! "$name" =~ "${matchload}" ]] && [[ ! "$name" =~ "${matchstore}" ]]; then
    echo "    e_$name.set_asm(\"e$name %reg, %reg, %reg\", rs, rsext, rd);"
  fi

  echo "    e_$name.set_decoder(opext=0x0b, op=0xa, op2=${op2});"
  echo ""
done
