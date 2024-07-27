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

FRESULT open_append(FIL *fp,         /* [OUT] File object to create */
                    const char *path /* [IN]  File name to be opened */
) {
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
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
    FATFS fs;
    FRESULT res;
    if ((res = f_mount(&fs, "", 0)) != FR_OK) {
        print_error(res);
        return -1;
    };
    DIR dir;
    if ((res = f_mkdir("/STUFF")) != FR_OK && res != FR_EXIST) {
        print_error(res);
        return -1;
    }
    FIL file;
    if ((res = open_append(&file, "/STUFF/LOGFILE.TXT")) != FR_OK) {
        print_error(res);
        return -1;
    }
    f_printf(&file, "May 6, 1869\n");
    if ((res = f_lseek(&file, 0)) != FR_OK) {
        print_error(res);
        return -1;
    };
    while (!f_eof(&file)) {
        char buf[32];
        UINT bytes_read;
        if ((res = f_read(&file, buf, 32, &bytes_read)) != FR_OK) {
            print_error(res);
            return -1;
        }
        print(buf);
    }
    f_close(&file);

    return 0;
}
