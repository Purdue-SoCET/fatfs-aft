#ifndef DISKIO_MMC_H
#define DISKIO_MMC_H

#include "diskio.h"
#include "ff.h"

DSTATUS MMC_disk_status();
DSTATUS MMC_disk_initialize();
DRESULT MMC_disk_read(BYTE *buff, LBA_t sector, UINT count);
DRESULT MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count);

#endif
