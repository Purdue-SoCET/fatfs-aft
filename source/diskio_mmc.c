#include "diskio_mmc.h"

DSTATUS MMC_disk_status() {
    return -1;
}
DSTATUS MMC_disk_initialize() {
    return -1;
}
DRESULT MMC_disk_read(BYTE *buff, LBA_t sector, UINT count) {
    return RES_ERROR;
}
DRESULT MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count) {
    return RES_ERROR;
}
