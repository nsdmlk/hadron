bits 32

MBALIGN     equ 1<<0
MEMINFO     equ 1<<1
FLAGS       equ MBALIGN | MEMINFO
MAGIC       equ 0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .text
global start
start:
    mov esp, stack_top
    push ebx
    push eax
    extern kernel_main
    call kernel_main
.hang:
    hlt
    jmp .hang

section .bss
align 4
stack_bottom:
    resb 4096
stack_top: