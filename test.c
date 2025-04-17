#include "source/ff.h"
#include <stdio.h>
#include "source/spi_sd.h"
#include "source/sdio.h"
#include <stdlib.h>

int main() {
    printf("disk init returned %d\n", SD_disk_initialize());
    char* buffer = malloc(513);
    printf("disk read returned %d\n", SD_disk_read(buffer, 0, 1));
    buffer[512] = 0;
    int i = 0;
    printf("\n0: ");
    while(i < 512) {
        printf("%2x", buffer[i]);
        if(i % 4 == 0) printf("\n%3d: ", i);
        i++;
    }
    
    //test

    return 0;
}
