#include "diskio_ram.h"
#include "ffconf.h"

volatile unsigned int *RAM_DISK_ADDR = (volatile unsigned int *)0xD0000000;

// MMIO disk already initializes the disk
DSTATUS RAM_disk_status() {
    return 0;
}

// MMIO disk already initializes the filesystem during initial
DSTATUS RAM_disk_initialize() {
    return 0;
}

DRESULT RAM_disk_read(BYTE *buff, LBA_t sector, UINT count) {
    for (unsigned int i = 0; i < FF_MAX_SS * count; i += 4) {
        unsigned int word = RAM_DISK_ADDR[((sector * FF_MAX_SS) + i) / 4];
        buff[i + 3] = word & 0xFF;
        buff[i + 2] = (word >> 8) & 0xFF;
        buff[i + 1] = (word >> 16) & 0xFF;
        buff[i] = (word >> 24) & 0xFF;
    }
    return RES_OK;
}

DRESULT RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count) {
    for (unsigned int i = 0; i < (FF_MAX_SS * count); i += 4) {
        unsigned int word =
            (buff[i + 3] << 24) | (buff[i + 2] << 16) | (buff[i + 1] << 8) | buff[i];
        RAM_DISK_ADDR[((sector * FF_MAX_SS) + i) / 4] = word;
    }
    return RES_OK;
}
