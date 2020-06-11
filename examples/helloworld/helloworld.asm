loop:       LOAD16 message      ; load the message address
            LOAD16 [idx]        ; load the index
            LOAD                ; using the loaded address and index load the char
            JEQ finish          ; if zero the string terminated 

            CALLUSER            ; print the character

            POP                 ; pop the character

            LOAD [idx]          ; Increase the index
            LOAD 1              ;
            ADD                 ;
            STORE [idx]         ;

            JMP loop            ; repeat

finish:
            HLT

idx:        DB 0x0000

message:    DB "Hello World!"
            DB 0x00