#include "idt.h"
#include "pic.h"
#include "gdt.h"

void kernel_main(void) {
    char* video = (char*) 0xB8000;

    gdt_load();
    pic_init();
    idt_init();
    asm volatile("sti");
    asm volatile("int $100");
    
    while (1) {}
}