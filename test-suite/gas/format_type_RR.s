# the commented instructions are so because they simply don't 
# make sense, but they have an opcode reserved for them on 
# sparc16_isa.ac

    addx16 %i0, %i2
    eaddx16 %i0, %i1, %i2

    subx16 %i0, %i2
    esubx16 %i0, %i1, %i2

    btst16 %i0, %i2

    clrw [%i0 + %i2]

    clrb16 [%i0 + %i2]

    clrh16 [%i0 + %i2]

    cmp16 %i0, %i2

    stb16 %i2, [%i0]
    estb16 %i2, [%i0+%i1]

    sth16 %i2, [%i0]
    esth16 %i2, [%i0+%i1]

    lduh16 [%i0], %i2
    elduh16 [%i0+%i1], %i2

    ldub16 [%i0], %i2
    eldub16 [%i0+%i1], %i2

    ldsb16 [%i0], %i2
    eldsb16 [%i0+%i1], %i2

    ldsh16 [%i0], %i2
    eldsh16 [%i0+%i1], %i2

    sra16 %i0, %i2
    esra16 %i0, %i1, %i2

    srl16 %i0, %i2
    esrl16 %i0, %i1, %i2

    sll16 %i0, %i2
    esll16 %i0, %i1, %i2

    orn16 %i0, %i2
    eorn16 %i0, %i1, %i2

    xnor16 %i0, %i2
    exnor16 %i0, %i1, %i2

    andn16 %i0, %i2
    eandn16 %i0, %i1, %i2

    neg16 %i0, %i2
    eneg16 %i0, %i1, %i2

    restore16 %i0, %i2
    erestore16 %i0, %i1, %i2

    smul16 %i0, %i1
    esmul16 %i0, %i1, %i2

    sdiv16 %i0, %i1
    esdiv16 %i0, %i1, %i2

    umul16 %i0, %i1
    eumul16 %i0, %i1, %i2

    udiv16 %i0, %i1
    eudiv16 %i0, %i1, %i2
    
    xor16 %i0, %i1
    exor16 %i0, %i1, %i2

    rd16 %wim, %i0
    rd16 %y, %i0
    rd16 %psr, %i0
    rd16 %tbr, %i0

    wr16 %wim, %i0
    wr16 %y, %i0
    wr16 %psr, %i0
    wr16 %tbr, %i0

    callr %i0
    jmpr %i0
    ret
    retl
    trestore
    nop
