global _main

section .data
    msg: db "Hello, Hadron", 10
    len: equ $ - msg

section .text
_main:
    mov rax, 0x02000004
    mov rdi, 1
    lea rsi, [rel msg]      ; RIP-относительный адрес
    mov rdx, len
    syscall

    mov rax, 0x02000001
    xor rdi, rdi
    syscall