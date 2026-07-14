#include "idt.h"
#include "pic.h"
#include "gdt.h"
#include "keyboard.h"

void kernel_main(void) {
    char* video = (char*) 0xB8000;

    gdt_load();
    pic_init();
    idt_init();
    keyboard_init();
    asm volatile("sti");
    
    while (1) {}
}