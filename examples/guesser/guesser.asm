;
;  Guess the number example in picoVM assembly
;
            LOAD16 question
            CALL print_string
            POP16

            LOAD16 user_input
            CALL read_input
            POP16

            LOAD 0                  ; return value holder
            LOAD16 user_input
            CALL string_to_int
            POP16

            LOAD 2
            CALLUSER                ; get random number
            LOAD 3
            MOD
            LOAD 1
            ADD

            HLT

            SUB
            JEQ success

            LOAD16 fail_text
            CALL print_string
            POP16

            HLT

success:
            LOAD16 success_text
            CALL print_string
            POP16

            HLT

;
; Prints a null terminated string
;
print_string:       
            LOAD16 0x0          ; Reserve and init index to 0
.loop:      LOAD16 [SFP + 2]    ; Load message address
            LOAD16 [SFP - 4]    ; Load message index
            LOAD                ; Fetch and load the char
            JEQ .finish         ; If zero the string terminated 

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
            LOAD16 0            ; Reserve and init local index to 0
.loop:      LOAD 1
            CALLUSER
            
            DUP
            LOAD 0x0a           ; end of line check
            SUB
            POP
            JEQ .finish

            LOAD16 [SFP + 2]    ; save the read character
            LOAD16 [SFP - 4]    ;
            DIG 4               ;
            STORE               ;
            POP                 ;
            
            LOAD 1              ; Increase the index
            ADD

            JMP .loop

.finish:    POP                 ; Pop last read value

            LOAD16 [SFP + 2]    ; add null termination
            LOAD16 [SFP - 4]    ;
            LOAD 0              ;
            STORE               ;
            
            POP16               ; Pop local index
            RET

string_to_int:
            LOAD   0            ; Reserve the integer
            LOAD16 0            ; Reserve and init index to 0
.loop:      LOAD16 [SFP + 2]    ; Load message address
            LOAD16 [SFP - 5]    ; Load message index
            LOAD                ; Fetch and load the char
            JEQ .finish         ; If zero the string terminated 

            LOAD [SFP - 3]
            LOAD 10
            MUL
            DIG 1
            LOAD 0x30
            SUB
            ADD
            STORE [SFP - 3]

            POP

            LOAD16 1              ; Increase the index
            ADD16                 ;

            JMP .loop           ; repeat

.finish:    POP
            POP16
            
            STORE [SFP + 4]      ; Load the return value

            RET

question:   DB "Guess a number from 1 to 10 : "
            DB 0x00

success_text:   DB 0x0a
                DB "Correct! You are very lucky!"
                DB 0x00

fail_text:      DB 0x0a
                DB "I guess you will have to win an other time... :("
                DB 0x00

user_input: RESB 0x16

dummy:      DB "12"
            DB 0x0