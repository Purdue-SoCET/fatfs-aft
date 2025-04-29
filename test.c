#include "source/ff.h"
#include <stdio.h>
#include "source/spi_sd.h"
#include "source/sdio.h"
#include <stdlib.h>
#include <string.h>

int main() {
	volatile unsigned int* GPIO_1_DDR = (unsigned int*) 0x80000104;
	volatile unsigned int* GPIO_1_DATA = (unsigned int*) 0x80000100;
    *GPIO_1_DDR |= 3<<4;
    int rtv;

    printf("disk init returned %d\n", SD_disk_initialize());
    char* buffer = malloc(512 * 8);

    for(int j = 512*8 - 1; j >= 0; j--) buffer[j] = (uint8_t) 0xff;
    *GPIO_1_DATA |= 1 << 4;
    rtv = SD_disk_write(buffer, 0, 8);
    *GPIO_1_DATA &= ~(1<<4);
    printf("disk write 1 returned %d\n", rtv);

    // printf("SD_disk_write 1 returns\n");

    // for(int j = 0; j < 512*8; j++) buffer[j] = 0xFF;
    // *GPIO_1_DATA |= 1 << 4;
    // rtv = SD_disk_write(buffer, 8, 8);
    // *GPIO_1_DATA &= ~(1<<4);
    // printf("disk write 2 returned %d\n", rtv);
    // // printf("SD_disk_write 2 returns\n");

    // for(int j = 0; j < 512*8; j++) buffer[j] = 0;
    // *GPIO_1_DATA |= 1 << 4;
    // rtv = SD_disk_write(buffer, 16, 8);
    // *GPIO_1_DATA &= ~(1<<4);
    // printf("disk write 3 returned %d\n", rtv);

    // printf("SD_disk_write 3 returns\n");

    memset(buffer, 0, 512 * 8);

    *GPIO_1_DATA |= 1 << 5;
    rtv = SD_disk_read(buffer, 0, 8);
    volatile int h = 5;
    h++;
    *GPIO_1_DATA &= ~(1<<5);
    printf("disk read 1 returned %d\n", rtv);

    buffer[512] = 0;
    int i = 0;
    while(i < 512) {
	    if(i % 4 == 0) printf(" ");
        if(i % 16 == 0) printf("\n%3d: ", i);
        printf("%02x", buffer[i]);
        i++;
    }
    
    // memset(buffer, 0, 512 * 8);

    // *GPIO_1_DATA |= 1 << 4;
    // rtv = SD_disk_read(buffer, 8, 8);
    // *GPIO_1_DATA &= ~(1<<4);
    // printf("disk read 2 returned %d\n", rtv);

    // buffer[512] = 0;
    // i = 0;
    // while(i < 512) {
	//     if(i % 4 == 0) printf(" ");
    //     if(i % 16 == 0) printf("\n%3d: ", i);
    //     printf("%02x", buffer[i]);
    //     i++;
    // }

    // memset(buffer, 0xFF, 512 * 8);

    // *GPIO_1_DATA |= 1 << 4;
    // rtv = SD_disk_read(buffer, 16, 8);
    // *GPIO_1_DATA &= ~(1<<4);
    // printf("disk read 3 returned %d\n", rtv);

    // buffer[512] = 0;
    // i = 0;
    // while(i < 512) {
	//     if(i % 4 == 0) printf(" ");
    //     if(i % 16 == 0) printf("\n%3d: ", i);
    //     printf("%02x", buffer[i]);
    //     i++;
    // }
    //test

    printf("test.c completed!\n");
    return 0;
}
