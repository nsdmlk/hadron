#include "vga.h"

void vga_putchar(char c, int x, int y) {
    char* video = (char*) 0xB8000;
    int offset = (y * 80 + x) * 2;
    video[offset] = c;
    video[offset + 1] = 0x0F;
}