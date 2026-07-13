bits 32
global isr0, isr32, isr33, isr100

isr0:
    mov byte [0xB8000 + 6], 'D'
    iretd

isr32:
    pushad
    mov al, 0x20
    out 0x20, al
    popad
    iretd

isr33:
    pushad
    in al, 0x60         
    mov byte [0xB8000 + 8], al  
    mov al, 0x20
    out 0x20, al
    popad
    iretd

isr100:
    push eax
    mov ax, 0x10
    mov ds, ax
    mov byte [0xB8000 + 4], '!'
    pop eax
    iretd