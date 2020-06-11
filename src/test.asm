
        LOAD32 [res]
        ; LOAD16 sq
        CALL sq
        CALLUSER
.l:     LOAD32 1
        SUB32
        ; DUP32
        ; STORE32 [res]
        JLT .l
        
        HLT

one:    DB 0x04030001
res:    DB 0x00000002

sq:     DIG32 2

        DUP32
        MUL32

        DUP32 2
        POP32
        RET

