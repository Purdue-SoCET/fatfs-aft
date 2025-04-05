#ifndef SDIO_DEFINED
#define SDIO_DEFINED

// FatFS required DISKIO functions
int SD_disk_status();
int SD_disk_initialize();
int SD_disk_read();
int SD_disk_write();

#endif