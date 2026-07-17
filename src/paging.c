#include "paging.h"

static unsigned int page_directory[PAGE_DIR_SIZE] __attribute__((aligned(PAGE_SIZE)));
static unsigned int first_page_table[PAGE_TABLE_SIZE] __attribute__((aligned(PAGE_SIZE)));

void paging_init(void) {
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_WRITABLE;
    }
    
    page_directory[0] = ((unsigned int)first_page_table) | PAGE_PRESENT | PAGE_WRITABLE;
    
    for (int i = 1; i < PAGE_DIR_SIZE; i++) {
        page_directory[i] = 0;
    }

    asm volatile("mov %0, %%cr3" : : "r"((unsigned int)page_directory));
    
    unsigned int cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    cr0 |= 0x00010000;  
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}