#ifndef DISKIO_USB_H
#define DISKIO_USB_H

#include "diskio.h"
#include "ff.h"

DSTATUS USB_disk_status();
DSTATUS USB_disk_initialize();
DRESULT USB_disk_read(BYTE *buff, LBA_t sector, UINT count);
DRESULT USB_disk_write(const BYTE *buff, LBA_t sector, UINT count);

#endif
