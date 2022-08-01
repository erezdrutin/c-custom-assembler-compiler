macro m1
    inc r1
    mov A, r1
    mov A, r1
    mov A, r1
    mov A, r1
endmacro
macro m2
    inc r2
    mov B, r2
endmacro
m1
inc r3
mov E, r3
m2
inc r4
mov F, r4
m1
SIUUUU:
    m1
    inc r4, 3
    m2
label:
    m3