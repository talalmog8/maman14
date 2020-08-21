.entry LIST
.entry LIST
.entry A
.extern W
.extern W

MAIN: add r3, LIST
LOOP: prn #48
lea W, r6
inc r6
mov r3, K
mov L4, L5
inc L5
inc L4
sub r1, r4
bne END
cmp K, #-6
bne &END
                dec W
            .entry MAIN
    rts
; comment
jmp &LOOP
add L3, L3
END: stop
STR: .string "abcd"
A: .string "abcdefght"
B: .data 1,2,3,4,5,6,7,8
LIST: .data 6, -9
.data -100
K: .data 31
.extern L3
.extern L4
.extern L5