#include "memory.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define HEAP_START 0x200000
#define HEAP_SIZE  0x100000

typedef struct block {
    unsigned int size;
    int free;
    struct block* next;
} block_t;

static void* heap_current = (void*) HEAP_START;
static int heap_initialized = 0;

void memory_init(void) {
    heap_current = (void*) HEAP_START;
    heap_initialized = 1;
    block_t* first = (block_t*) HEAP_START;
    first -> size = 0;
    first -> free = 0;
    first -> next = NULL;
    heap_current = (void*) (HEAP_START + sizeof(block_t));
}

void* malloc(unsigned int size) {
    if (!heap_initialized) memory_init();

    block_t* current = (block_t*) HEAP_START;
    while (current != NULL) {
        if (current->free && current->size >= size) {
            current->free = 0;
            return (void*)(current + 1);
        }
        current = current->next;
    }

    block_t* new_block = (block_t*) heap_current;
    new_block->size = size;
    new_block->free = 0;
    new_block->next = NULL;
    
    if ((block_t*) HEAP_START != new_block) {
        block_t* last = (block_t*) HEAP_START;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_block;
    }
    
    heap_current += sizeof(block_t) + size;
    return (void*)(new_block + 1);
}

void free(void* ptr) {
    if (!ptr) return;
    block_t* block = (block_t*) ptr - 1;
    block->free = 1;
}