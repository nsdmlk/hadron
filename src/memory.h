#ifndef MEMORY_H
#define MEMORY_H

void* malloc(unsigned int size);
void free(void* ptr);
void memory_init(void);

#endif