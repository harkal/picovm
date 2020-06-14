
        LOAD32 [res]
        CALL sqr
        CALLUSER
la:     LOAD32 1
        SUB32
        
        JLT la
        
        HLT

one:    DB 0x04030001
res:    DB 0x00000002

sqr:    LOAD32 [sfp + 2]

        DUP32
        MUL32

        STORE32 [sfp + 2]
        RET

