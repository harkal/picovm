;
;  Guess the number example in picoVM assembly
;
            ; LOAD16 question
            ; CALL print_string
            ; POP16

            ; LOAD16 user_input
            ; CALL read_input
            ; POP16

            LOAD16 dummy
            CALL string_to_int
            ;POP16

            HLT

;
; Prints a null terminated string
;
print_string:       
            LOAD16 0            ; Reserve and init index to 0
.loop:      DIG16 4             ; Load message address
            DIG16 2             ; Load message index
            LOAD                ; Fetch and load the char
            JEQ .finish          ; If zero the string terminated 

            LOAD 0              ; System func putc
            CALLUSER            ; print the character

            POP16               ; pop the character the func id   

            LOAD 1              ; Increase the index
            ADD                 ;

            JMP .loop           ; repeat

.finish:    POP
            POP16
            RET

;
; Gets user input until change of line
;
read_input: 
            LOAD16 0            ; Reserve and init index to 0
.loop:      LOAD 1
            CALLUSER
            
            DUP
            LOAD 0x0a           ; end of line
            SUB
            POP
            JEQ .finish

            DIG16 5
            DIG16 3
            DIG 4
            STORE
            POP
            
            LOAD 1
            ADD
            JMP .loop

.finish:    DIG16 5             ; add null termination
            DIG16 3
            LOAD 0
            STORE

            POP
            POP16
            RET

string_to_int:
            LOAD 0              ; Reserve the integer
            LOAD16 0            ; Reserve and init index to 0
.loop:      DIG16 5             ; Load message address
            DIG16 2             ; Load message index
            LOAD                ; Fetch and load the char
            JEQ .finish          ; If zero the string terminated 

            DIG 3
            LOAD 10
            MUL
            DUP 4
            POP16

            LOAD 1              ; Increase the index
            ADD                 ;

            JMP .loop           ; repeat

.finish:    POP
            POP16
            POP16
            RET

question:   DB "Guess a number from 0 to 9 : "
            DB 0x00

user_input: RESB 0x16

dummy:      DB "12"
            DB 0x0