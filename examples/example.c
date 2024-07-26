/*---------------------------------------------------------------------*/
/* Raw Read/Write Throughput Checker                                   */
/*---------------------------------------------------------------------*/

#include "diskio.h"
#include "ff.h"
#include "format.h"
#include "pal.h"
#include <stdio.h>

// TODO: move this to femtokernel
uint32_t get_mtime() {
    CLINTRegBlk *clint = (CLINTRegBlk *)CLINT_BASE;
    return clint->mtime;
}

int test_raw_speed(BYTE pdrv,   /* Physical drive number */
                   DWORD lba,   /* Start LBA for read/write test */
                   DWORD len,   /* Number of bytes to read/write (must be multiple of sz_buff) */
                   void *buff,  /* Read/write buffer */
                   UINT sz_buff /* Size of read/write buffer (must be multiple of FF_MAX_SS) */
) {
    WORD ss;
    DWORD ofs, tmr;

#if FF_MIN_SS != FF_MAX_SS
    if (disk_ioctl(pdrv, GET_SECTOR_SIZE, &ss) != RES_OK) {
        printf("\ndisk_ioctl() failed.\n");
        return 0;
    }
#else
    ss = FF_MAX_SS;
#endif

    print("Starting raw write test at sector %lu in %u bytes of data chunks...", lba, sz_buff);
    tmr = get_mtime();
    for (ofs = 0; ofs < len / ss; ofs += sz_buff / ss) {
        if (disk_write(pdrv, buff, lba + ofs, sz_buff / ss) != RES_OK) {
            print("\ndisk_write() failed.\n");
            return 0;
        }
    }
    if (disk_ioctl(pdrv, CTRL_SYNC, 0) != RES_OK) {
        print("\ndisk_ioctl() failed.\n");
        return 0;
    }
    tmr = get_mtime() - tmr;
    print("\n%lu bytes written and it took %lu timer ticks.\n", len, tmr);

    print("Starting raw read test at sector %lu in %u bytes of data chunks...", lba, sz_buff);
    tmr = get_mtime();
    for (ofs = 0; ofs < len / ss; ofs += sz_buff / ss) {
        if (disk_read(pdrv, buff, lba + ofs, sz_buff / ss) != RES_OK) {
            print("\ndisk_read() failed.\n");
            return 0;
        }
    }
    tmr = get_mtime() - tmr;
    print("\n%lu bytes read and it took %lu timer ticks.\n", len, tmr);

    print("Test completed.\n");
    return 1;
}

void print_error(FRESULT res) {
    switch (res) {
    case FR_DISK_ERR:
        print("FR_DISK_ERR\n");
        break;
    case FR_INT_ERR:
        print("FR_INT_ERR\n");
        break;
    case FR_NOT_READY:
        print("FR_NOT_READY\n");
        break;
    case FR_NO_FILE:
        print("FR_NO_FILE\n");
        break;
    case FR_NO_PATH:
        print("FR_NO_PATH\n");
        break;
    case FR_INVALID_NAME:
        print("FR_INVALID_NAME\n");
        break;
    case FR_DENIED:
        print("FR_DENIED\n");
        break;
    case FR_EXIST:
        print("FR_EXIST\n");
        break;
    case FR_INVALID_OBJECT:
        print("FR_INVALID_OBJECT\n");
        break;
    case FR_WRITE_PROTECTED:
        print("FR_WRITE_PROTECTED\n");
        break;
    case FR_INVALID_DRIVE:
        print("FR_INVALID_DRIVE\n");
        break;
    case FR_NOT_ENABLED:
        print("FR_NOT_ENABLED\n");
        break;
    case FR_NO_FILESYSTEM:
        print("FR_NO_FILESYSTEM\n");
        break;
    case FR_MKFS_ABORTED:
        print("FR_MKFS_ABORTED\n");
        break;
    case FR_TIMEOUT:
        print("FR_TIMEOUT\n");
        break;
    case FR_LOCKED:
        print("FR_LOCKED\n");
        break;
    case FR_NOT_ENOUGH_CORE:
        print("FR_NOT_ENOUGH_CORE\n");
        break;
    case FR_TOO_MANY_OPEN_FILES:
        print("FR_TOO_MANY_OPEN_FILES\n");
        break;
    case FR_INVALID_PARAMETER:
        print("FR_INVALID_PARAMETER\n");
        break;
    default: {
    }
    }
}

int main() {
    print("hello");
    FATFS fs;
    FRESULT res;
    if ((res = f_mount(&fs, "", 0)) != FR_OK) {
        print_error(res);
        return -1;
    };
    if ((res = f_mkdir("/stuff")) != FR_OK) {
        print_error(res);
        return -1;
    }
    // DIR dir;
    // if (f_opendir(&dir, "/") != FR_OK) {
    //     return -1;
    // }
    return 0;
}
