.l:     LOAD32 [res]
        LOAD32 [one]
        SUB
        STORE32 [res]
        LOAD32 [res]
        ;YIELD
        JLT .l
        
        HLT

one:    DB 0x00000001
res:    DB 0x00000005

