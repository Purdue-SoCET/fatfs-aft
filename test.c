#include "source/ff.h"
#include <stdio.h>
#include "source/spi_sd.h"
#include "source/sdio.h"
#include <stdlib.h>
#include <string.h>

int main() {
    printf("disk init returned %d\n", SD_disk_initialize());
    char* buffer = malloc(513);

    for(int j = 0; j < 512; j++) buffer[j] = (uint8_t) j;
    SD_disk_write(buffer, 0, 1);
    printf("SD_disk_write 1 returns\n");

    for(int j = 0; j < 512; j++) buffer[j] = 0xFF;
    SD_disk_write(buffer, 1, 1);
    printf("SD_disk_write 2 returns\n");

    for(int j = 0; j < 512; j++) buffer[j] = 0;
    SD_disk_write(buffer, 2, 1);
    printf("SD_disk_write 3 returns\n");

    memset(buffer, 0, 513);
    printf("disk read returned %d\n", SD_disk_read(buffer, 0, 1));
    buffer[512] = 0;
    int i = 0;
    while(i < 512) {
	    if(i % 4 == 0) printf(" ");
        if(i % 16 == 0) printf("\n%3d: ", i);
        printf("%02x", buffer[i]);
        i++;
    }
    
    memset(buffer, 0, 513);
    printf("disk read returned %d\n", SD_disk_read(buffer, 1, 1));
    buffer[512] = 0;
    i = 0;
    while(i < 512) {
	    if(i % 4 == 0) printf(" ");
        if(i % 16 == 0) printf("\n%3d: ", i);
        printf("%02x", buffer[i]);
        i++;
    }

    memset(buffer, 0xFF, 513);
    printf("disk read returned %d\n", SD_disk_read(buffer, 2, 1));
    buffer[512] = 0;
    i = 0;
    while(i < 512) {
	    if(i % 4 == 0) printf(" ");
        if(i % 16 == 0) printf("\n%3d: ", i);
        printf("%02x", buffer[i]);
        i++;
    }
    //test

    return 0;
}
