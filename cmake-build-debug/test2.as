    MAIN: add r3, LIST
LOOP: prn #48
lea STR, r6
inc r6
mov r3,K
sub r1, r4
; tal
bne END
cmp K, #-6
bne &END
dec K
clr K
not r4

jmp &LOOP
END: stop
STR: .string "abcd"
red r1
prn r4

LIST: .data 6, -9, 900
TAL: .data 45,45,45,45,45,45,45,45,45,45,45,45,45,45,45
    .data -100
K: .data 31
        .extern L3
.entry MAIN
