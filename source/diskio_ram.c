#include "diskio_ram.h"

DSTATUS RAM_disk_status() {
    return -1;
}
DSTATUS RAM_disk_initialize() {
    return -1;
}
DRESULT RAM_disk_read(BYTE *buff, LBA_t sector, UINT count) {
    return RES_ERROR;
}
DRESULT RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count) {
    return RES_ERROR;
}
