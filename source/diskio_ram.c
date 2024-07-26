#include "diskio_ram.h"
#include "ffconf.h"

volatile char *RAM_DISK_ADDR = (volatile char *)0xD0000000;

// MMIO disk already initializes the disk
DSTATUS RAM_disk_status() {
    return 0;
}

// MMIO disk already initializes the filesystem during initial
DSTATUS RAM_disk_initialize() {
    return 0;
}

DRESULT RAM_disk_read(BYTE *buff, LBA_t sector, UINT count) {
    for (unsigned int i = 0; i < (FF_MAX_SS * count); i++) {
        buff[i] = RAM_DISK_ADDR[(sector * FF_MAX_SS) + i];
    }
    return RES_OK;
}

DRESULT RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count) {
    for (unsigned int i = 0; i < (FF_MAX_SS * count); i++) {
        RAM_DISK_ADDR[(sector * FF_MAX_SS) + i] = buff[i];
    }
    return RES_OK;
}
