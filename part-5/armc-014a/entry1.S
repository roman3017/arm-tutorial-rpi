.equ NEXTADDR, 0x10000

.global _start
_start:
        mov sp, #0x80000
        ldr r0, =NEXTADDR
        bl bootmain
        ldr r5, =NEXTADDR
        bx r5

.global PUT32
PUT32:
        str r1,[r0]
        bx lr

.global GET32
GET32:
        ldr r0,[r0]
        bx lr
