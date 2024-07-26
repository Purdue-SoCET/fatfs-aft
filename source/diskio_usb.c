#include "diskio_usb.h"

DSTATUS USB_disk_status() {
    return -1;
}
DSTATUS USB_disk_initialize() {
    return -1;
}
DRESULT USB_disk_read(BYTE *buff, LBA_t sector, UINT count) {
    return RES_ERROR;
}
DRESULT USB_disk_write(const BYTE *buff, LBA_t sector, UINT count) {
    return RES_ERROR;
}
