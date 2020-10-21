#include "isr.h"


.macro ISR_NO_ERROR nb
.align ISR_ALIGN
    pushl $0x0
    pushl %eax
    movl $\nb, %eax
    jmp isr_common
.endm

.macro ISR_ERROR nb
.align ISR_ALIGN
    pushl %eax
    movl $\nb, %eax
    jmp isr_common
.endm

isr_common:
    pushl %ebx

    pushw %es
    pushw %fs
    pushw %gs
    pushw %ds
    movl $0x10, %ebx
    movw %bx, %es
    movw %bx, %ds

    pushl %ecx
    pushl %edx
    pushl %edi
    pushl %esi

    pushl %eax

    pushl %esp
    call *interrupt_handler(, %eax, 4)
    addl $8, %esp
    popl %esi
    popl %edi
    popl %edx

    popw %ds
    popw %gs
    popw %fs
    popw %es

    popl %ebx
    popl %eax
    addl $4, %esp
    iret


.align ISR_ALIGN
.global isr_vector

isr_vector:
ISR_NO_ERROR 0
ISR_NO_ERROR 1
ISR_NO_ERROR 2
ISR_NO_ERROR 3
ISR_NO_ERROR 4
ISR_NO_ERROR 5
ISR_NO_ERROR 6
ISR_NO_ERROR 7
ISR_ERROR 8
ISR_NO_ERROR 9
ISR_ERROR 10
ISR_ERROR 11
ISR_ERROR 12
ISR_ERROR 13
ISR_ERROR 14
ISR_ERROR 15
ISR_NO_ERROR 16
ISR_ERROR 17
