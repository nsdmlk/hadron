#include "fs.h"
#include "ata.h"

static superblock_t superblock;
static file_entry_t file_table[MAX_FILES];
static unsigned char sector_buffer[SECTOR_SIZE];

file_entry_t* fs_get_table(void) {
    ata_read(1, (unsigned char*) file_table);
    return file_table;
}

static void memcpy(void* dest, const void* src, int n) {
    char* d = (char*) dest;
    const char* s = (const char*) src;
    for (int i = 0; i < n; i++) d[i] = s[i];
}

void fs_init(void) {
    ata_read(0, sector_buffer);
    superblock = *(superblock_t*) sector_buffer;
    
    if (superblock.magic != FS_MAGIC) {
        superblock.magic = FS_MAGIC;
        superblock.total_sectors = 32768;
        superblock.file_count = 0;
        
        for (int i = 0; i < MAX_FILES; i++) {
            file_table[i].name[0] = '\0';
            file_table[i].size = 0;
            file_table[i].first_sector = 0;
        }
        
        *(superblock_t*) sector_buffer = superblock;
        ata_write(0, sector_buffer);
        
        memcpy(sector_buffer, file_table, sizeof(file_table));
        ata_write(1, sector_buffer);
    } else {
        ata_read(1, sector_buffer);
    }
}

void fs_create(const char* name) {
    if (superblock.file_count >= MAX_FILES) return;
    
    ata_read(1, (unsigned char*) file_table);
    
    int slot = -1;
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].name[0] == '\0') {
            slot = i;
            break;
        }
    }
    if (slot == -1) return;
    
    for (int i = 0; name[i] && i < FILE_NAME_LEN - 1; i++) {
        file_table[slot].name[i] = name[i];
    }
    file_table[slot].size = 0;
    file_table[slot].first_sector = 2 + slot * 128;
    
    superblock.file_count++;
    
    *(superblock_t*) sector_buffer = superblock;
    ata_write(0, sector_buffer);
    
    ata_write(1, (unsigned char*) file_table);
}

void fs_list(void) {
    ata_read(1, (unsigned char*) file_table);
}

void fs_write(const char* name, const char* data, unsigned int size) {
}

void fs_read(const char* name, char* buffer) {
}