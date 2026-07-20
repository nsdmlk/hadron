#ifndef ATA_H
#define ATA_H

void ata_read(unsigned int lba, unsigned char* buffer);
void ata_write(unsigned int lba, unsigned char* buffer);
void ata_init(void);

#endif