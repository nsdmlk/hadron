; gdt.asm — Global Descriptor Table
bits 32
global gdt_load

section .data
gdt_start:
    ; null descriptor (обязательно)
    dd 0
    dd 0

    ; code segment (0x08)
    dw 0xFFFF       ; limit low
    dw 0            ; base low
    db 0            ; base middle
    db 10011010b    ; access: present, ring 0, code, readable
    db 11001111b    ; granularity: 4K, 32-bit, limit high
    db 0            ; base high

    ; data segment (0x10)
    dw 0xFFFF       ; limit low
    dw 0            ; base low
    db 0            ; base middle
    db 10010010b    ; access: present, ring 0, data, writable
    db 11001111b    ; granularity
    db 0            ; base high

gdt_end:

gdt_desc:
    dw gdt_end - gdt_start - 1   ; limit
    dd gdt_start                  ; base

section .text
gdt_load:
    cli
    lgdt [gdt_desc]
    
    jmp 0x08:.reload_cs

.reload_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret