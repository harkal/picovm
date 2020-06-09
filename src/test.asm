        LOAD32 [res]
.l:     LOAD32 [one]
        SUB32
        DUP32
        STORE32 [res]
        JLT .l
        
        HLT

one:    DB 0x00000001
res:    DB 0x00000005

