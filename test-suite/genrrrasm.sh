
for i in "add_reg16 10101 00" "sub_reg16 10101 01" "and_reg16 10101 10" \
"or_reg16 10101 11" "smul_reg16 10110 00" "sdiv_reg16 10110 01" \
"umul_reg16 10110 10" "udiv_reg16 10110 11" "st_reg16 10111 00" \
"ld_reg16 10111 01" "xor_reg16 10111 10"; do

  set $i
  name="`echo $1 | sed -e \"s/_reg//g\"`"

  matchload="ld"
  matchstore="st"

  if [[ "$name" =~ "${matchload}" ]]; then
    echo "    $name [%r0 + %r1], %r2"
  fi

  if [[ "$name" =~ "${matchstore}" ]]; then
    echo "    $name %r2, [%r0 + %r1]"
  fi

  if [[ ! "$name" =~ "${matchload}" ]] && [[ ! "$name" =~ "${matchstore}" ]]; then
    echo "    $name %r0, %r1, %r2"
  fi

done
