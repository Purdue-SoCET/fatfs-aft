#include "source/ff.h"
#include <stdio.h>
#include "source/spi_sd.h"
#include "source/sdio.h"
#include <stdlib.h>
#include <string.h>

void read_test();
void format_write_test();

int main() {
    // two tests for basic file operations
    // format_write_test();
    read_test();
}

void format_write_test() {
    FATFS fs;           /* Filesystem object */
    FIL fil;            /* File object */
    FRESULT res;        /* API result code */
    UINT bw;            /* Bytes written */
    BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */

    /* Format the default drive with default parameters */
    res = f_mkfs("", 0, work, sizeof work);
    if (res) {
        printf("f_mkfs failed with %d\n", res);
    }

    /* Give a work area to the default drive */
    res = f_mount(&fs, "", 0);
    if(res) {
        printf("f_mount failed with %d\n", res);
    }

    /* Create a file as new */
    res = f_open(&fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
    if (res) {
        printf("f_open failed with %d\n", res);
    }

    /* Write a message */
    f_write(&fil, "Hello, World!\r\n", 15, &bw);
    if (bw != 15) {
        printf("f_write failed, writing %d bytes\n", bw);
    }

    /* Close the file */
    f_close(&fil);

    /* Unregister work area */
    f_mount(0, "", 0);
    printf("completed format_write_test\n");
}

/* Read a text file and display it */
void read_test() {
    FATFS FatFs;   /* Work area (filesystem object) for logical drive */
    FIL fil;        /* File object */
    char line[100]; /* Line buffer */
    FRESULT fr;     /* FatFs return code */

    /* Give a work area to the default drive */
    fr = f_mount(&FatFs, "", 0);
    if(fr) {
        printf("f_mount failed with code %d\n", fr);
    }

    /* Open a text file */
    fr = f_open(&fil, "hello.txt", FA_READ);
    if (fr) {
        printf("f_open failed with code %d\n", fr);
    }

    /* Read every line and display it */
    while (f_gets(line, sizeof line, &fil)) {
        printf(line);
    }

    /* Close the file */
    f_close(&fil);

    f_mount(0, "", 0);
    printf("completed read_test\n");
}