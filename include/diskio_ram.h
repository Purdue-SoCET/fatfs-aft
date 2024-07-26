#ifndef DISKIO_RAM_H
#define DISKIO_RAM_H

#include "ff.h"
#include "diskio.h"

DSTATUS RAM_disk_status();
DSTATUS RAM_disk_initialize();
DRESULT RAM_disk_read(BYTE *buff, LBA_t sector, UINT count);
DRESULT RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count);

#endif
