.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager14handleInteruptEhj


.macro HandleException num
.global _ZN16InterruptManager19HandleException\num\()Ev
_ZN16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interuptnumber)
    jmp int_bottom
.endm

.macro HandleInteruptRequest num
.global _ZN16InterruptManager25HandleInteruptRequest\num\()Ev
_ZN16InterruptManager25HandleInteruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interuptnumber)
    jmp int_bottom
.endm

HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0A
HandleException 0x0B
HandleException 0x0C
HandleException 0x0D
HandleException 0x0E
HandleException 0x0F
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13

HandleInteruptRequest 0x00
HandleInteruptRequest 0x01
HandleInteruptRequest 0x02
HandleInteruptRequest 0x03
HandleInteruptRequest 0x04
HandleInteruptRequest 0x05
HandleInteruptRequest 0x06
HandleInteruptRequest 0x07
HandleInteruptRequest 0x08
HandleInteruptRequest 0x09
HandleInteruptRequest 0x0A
HandleInteruptRequest 0x0B
HandleInteruptRequest 0x0C
HandleInteruptRequest 0x0D
HandleInteruptRequest 0x0E
HandleInteruptRequest 0x0F
HandleInteruptRequest 0x31



int_bottom:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interuptnumber)
    call _ZN16InterruptManager14handleInteruptEhj

    add %esp, 6
    mov %eax, %esp # den stack wechseln

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    .global _ZN16InterruptManager21IgnoreInteruptRequestEv
    _ZN16InterruptManager21IgnoreInteruptRequestEv:

    iret

.data
    interuptnumber: .byte 0