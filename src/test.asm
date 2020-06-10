
        LOAD16 [one]
        LOAD16 [one]
        LOAD32 [one]
        STORE32
        ;CONVF
.l:     LOAD [one]
        SUB
        ; DUP32
        ; STORE32 [res]
        JLT .l
        
        HLT

one:    DB 0x04030001
res:    DB 0x00000000

