#include "idt.h"
#include "vga.h"

struct idt_entry {
    unsigned short base_low;
    unsigned short selector;
    unsigned char  zero;
    unsigned char  flags;
    unsigned short base_high;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr   idt_p;

extern void isr0();
extern void isr32();
extern void isr33();
extern void isr100();

void idt_set_entry(int num, unsigned int handler) {
    idt[num].base_low = handler & 0xFFFF;
    idt[num].base_high = (handler >> 16) & 0xFFFF;
    idt[num].selector = 0x08;
    idt[num].zero = 0;
    idt[num].flags = 0x8E;  // present, ring 0, 32-bit interrupt gate
}

void idt_init() {
    idt_p.limit = sizeof(idt) - 1;
    idt_p.base  = (unsigned int)&idt;

    for (int i = 0; i < 256; i++) {
        idt_set_entry(i, (unsigned int)isr0);
    }

    idt_set_entry(100, (unsigned int)isr100);

    idt_set_entry(32, (unsigned int)isr32); 
    idt_set_entry(33, (unsigned int)isr33); 

    asm volatile("lidt %0" : : "m"(idt_p));

    vga_putchar('O', 0, 0);
    vga_putchar('K', 1, 0);
}

