#include "idt.h"
#include "pic.h"
#include "gdt.h"
#include "keyboard.h"
#include "memory.h"

unsigned int total_memory_mb = 0;

void kernel_main(unsigned int magic, unsigned int* multiboot_info) {
    char* video = (char*) 0xB8000;

    gdt_load();
    pic_init();
    idt_init();
    keyboard_init();
    
    if (multiboot_info[0] & 1) {
        unsigned int mem_lower = multiboot_info[1];
        unsigned int mem_upper = multiboot_info[2];
        total_memory_mb = (mem_lower + mem_upper) / 1024;
    }
    
    asm volatile("sti");
    
    while (1) {}
}