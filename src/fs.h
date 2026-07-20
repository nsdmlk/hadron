#ifndef FS_H
#define FS_H

#define FS_MAGIC 0x4844524E  // "HDRN" в ASCII
#define MAX_FILES 16
#define FILE_NAME_LEN 24
#define SECTOR_SIZE 512

typedef struct {
    unsigned int magic;
    unsigned int total_sectors;
    unsigned int file_count;
} superblock_t;

typedef struct {
    char name[FILE_NAME_LEN];
    unsigned int size;
    unsigned int first_sector;
} file_entry_t;

void fs_init(void);
void fs_create(const char* name);
void fs_list(void);
void fs_write(const char* name, const char* data, unsigned int size);
void fs_read(const char* name, char* buffer);
file_entry_t* fs_get_table(void);

#endif