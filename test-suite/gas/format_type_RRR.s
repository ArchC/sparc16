
    add16 %i0, %i1, %i2
    sub16 %i0, %i1, %i2
    and16 %i0, %i1, %i2
    or16 %i0, %i1, %i2
    st16 %i2, [%i0 + %i1]
    ld16 [%i0 + %i1], %i2

    std16 %i2, [%i0+%i1]
    ldd16 [%i0+%i1], %i2
