# the commented instructions are so because they simply don't 
# make sense, but they have an opcode reserved for them on 
# sparc16_isa.ac

    addx_reg16 %r0, %r2
    eaddx_reg16 %r0, %r1, %r2

    subx_reg16 %r0, %r2
    esubx_reg16 %r0, %r1, %r2

    btst_reg16 %r0, %r2
    #ebtst_reg16 %r0, %r1, %r2

    clrw16 [%r0 + %r2]
    #eclrw16 [%r0 + %r1 + %r2]

    clrb16 [%r0 + %r2]
    #eclrb16 [%r0 + %r1 + %r2]

    clrh16 [%r0 + %r2]
    #eclrh16 [%r0 + %r1 + %r2]

    cmp_reg16 %r0, %r2
    #ecmp_reg16 %r0, %r1, %r2

    stb_reg16 %r2, [%r0]
    estb_reg16 %r2, [%r0+%r1]

    sth_reg16 %r2, [%r0]
    esth_reg16 %r2, [%r0+%r1]

    std_reg16 %r2, [%r0]
    estd_reg16 %r2, [%r0+%r1]

    lduh_reg16 [%r0], %r2
    elduh_reg16 [%r0+%r1], %r2

    ldub_reg16 [%r0], %r2
    eldub_reg16 [%r0+%r1], %r2

    ldd_reg16 [%r0], %r2
    eldd_reg16 [%r0+%r1], %r2

    ldsb_reg16 [%r0], %r2
    eldsb_reg16 [%r0+%r1], %r2

    ldsh_reg16 [%r0], %r2
    eldsh_reg16 [%r0+%r1], %r2

    sra_reg16 %r0, %r2
    esra_reg16 %r0, %r1, %r2

    srl_reg16 %r0, %r2
    esrl_reg16 %r0, %r1, %r2

    sll_reg16 %r0, %r2
    esll_reg16 %r0, %r1, %r2

    orn_reg16 %r0, %r2
    eorn_reg16 %r0, %r1, %r2

    xnor_reg16 %r0, %r2
    exnor_reg16 %r0, %r1, %r2

    andn_reg16 %r0, %r2
    eandn_reg16 %r0, %r1, %r2

    neg_reg16 %r0, %r2
    eneg_reg16 %r0, %r1, %r2

    restore16 %r0, %r2
    erestore16 %r0, %r1, %r2
