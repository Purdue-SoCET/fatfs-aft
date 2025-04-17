#ifndef SDIO_DEFINED
#define SDIO_DEFINED

// FatFS required DISKIO functions
int SD_disk_status();
int SD_disk_initialize();
int SD_disk_read(unsigned char*, uint32_t, unsigned int);
int SD_disk_write();

#endif