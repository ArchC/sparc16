# the commented instructions are so because they simply don't 
# make sense, but they have an opcode reserved for them on 
# sparc16_isa.ac

    addx16 %r0, %r2
    eaddx16 %r0, %r1, %r2

    subx16 %r0, %r2
    esubx16 %r0, %r1, %r2

    btst16 %r0, %r2
    #ebtst16 %r0, %r1, %r2

    clrw [%r0 + %r2]
    #eclrw [%r0 + %r1 + %r2]

    clrb16 [%r0 + %r2]
    #eclrb16 [%r0 + %r1 + %r2]

    clrh16 [%r0 + %r2]
    #eclrh16 [%r0 + %r1 + %r2]

    cmp16 %r0, %r2
    #ecmp16 %r0, %r1, %r2

    stb16 %r2, [%r0]
    estb16 %r2, [%r0+%r1]

    sth16 %r2, [%r0]
    esth16 %r2, [%r0+%r1]

    std16 %r2, [%r0]
    estd16 %r2, [%r0+%r1]

    lduh16 [%r0], %r2
    elduh16 [%r0+%r1], %r2

    ldub16 [%r0], %r2
    eldub16 [%r0+%r1], %r2

    ldd16 [%r0], %r2
    eldd16 [%r0+%r1], %r2

    ldsb16 [%r0], %r2
    eldsb16 [%r0+%r1], %r2

    ldsh16 [%r0], %r2
    eldsh16 [%r0+%r1], %r2

    sra16 %r0, %r2
    esra16 %r0, %r1, %r2

    srl16 %r0, %r2
    esrl16 %r0, %r1, %r2

    sll16 %r0, %r2
    esll16 %r0, %r1, %r2

    orn16 %r0, %r2
    eorn16 %r0, %r1, %r2

    xnor16 %r0, %r2
    exnor16 %r0, %r1, %r2

    andn16 %r0, %r2
    eandn16 %r0, %r1, %r2

    neg16 %r0, %r2
    eneg16 %r0, %r1, %r2

    restore16 %r0, %r2
    erestore16 %r0, %r1, %r2

    rd16 %wim, %r0
    rd16 %y, %r0
    rd16 %psr, %r0
    rd16 %tbr, %r0

    wr16 %wim, %r0
    wr16 %y, %r0
    wr16 %psr, %r0
    wr16 %tbr, %r0

