        LOAD [res]
.l:     LOAD [one]
        SUB
        JLT .l
        
        HLT

one:    DB 0x01
res:    DB 0x05

