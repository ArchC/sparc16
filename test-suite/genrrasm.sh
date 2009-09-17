
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
  matchload="ld"
  matchstore="st"

  if [[ "$name" =~ "${matchload}" ]]; then
    echo "    $name [%r0], %r2"
    echo "    e$name [%r0+%r1], %r2"
  fi

  if [[ "$name" =~ "${matchstore}" ]]; then
    echo "    $name %r2, [%r0]"
    echo "    e$name %r2, [%r0+%r1]"
  fi

  if [[ ! "$name" =~ "${matchload}" ]] && [[ ! "$name" =~ "${matchstore}" ]]; then
    echo "    $name %r0, %r2"
    echo "    e$name %r0, %r1, %r2"
  fi

  echo ""
done
