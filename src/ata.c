#include "ata.h"

#define ATA_DATA    0x1F0
#define ATA_ERROR   0x1F1
#define ATA_SECCOUNT 0x1F2
#define ATA_LBA_LOW 0x1F3
#define ATA_LBA_MID 0x1F4
#define ATA_LBA_HIGH 0x1F5
#define ATA_DRIVE   0x1F6
#define ATA_COMMAND 0x1F7
#define ATA_STATUS  0x1F7

static inline void outb(unsigned short port, unsigned char data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char data;
    asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

void ata_init(void) {
    for (int i = 0; i < 4; i++) inb(ATA_STATUS);
}

void ata_read(unsigned int lba, unsigned char* buffer) {
    while (inb(ATA_STATUS) & 0x80);
    while (!(inb(ATA_STATUS) & 0x40));
    
    outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_SECCOUNT, 1);
    outb(ATA_LBA_LOW, lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_COMMAND, 0x20);
    
    while (!(inb(ATA_STATUS) & 0x08));
    
    for (int i = 0; i < 256; i++) {
        unsigned short word;
        asm volatile(
            "movw $0x1F0, %%dx\n"
            "inw %%dx, %%ax\n"
            : "=a"(word)
            :
            : "dx"
        );
        buffer[i * 2] = word & 0xFF;
        buffer[i * 2 + 1] = (word >> 8) & 0xFF;
    }
    
    while (inb(ATA_STATUS) & 0x80);
}

void ata_write(unsigned int lba, unsigned char* buffer) {
    while (inb(ATA_STATUS) & 0x80);
    while (!(inb(ATA_STATUS) & 0x40));
    
    outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_SECCOUNT, 1);
    outb(ATA_LBA_LOW, lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_COMMAND, 0x30);
    
    while (!(inb(ATA_STATUS) & 0x08));
    
    for (int i = 0; i < 256; i++) {
        unsigned short word = (buffer[i * 2 + 1] << 8) | buffer[i * 2];
        asm volatile(
            "movw $0x1F0, %%dx\n"
            "outw %%ax, %%dx\n"
            : 
            : "a"(word)
            : "dx"
        );
    }
    
    while (inb(ATA_STATUS) & 0x80);
    outb(ATA_COMMAND, 0xE7);
    while (inb(ATA_STATUS) & 0x80);
}