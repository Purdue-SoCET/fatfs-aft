#include "source/ff.h"
#include <stdio.h>
#include "source/spi_sd.h"

int main() {
    // for(volatile int i = 0; i < 50000000; i++);
    //spi_gpio_init();
    //sd_cmd(0xAA, 0xFF00FF00, 0xAA);
    printf("A");
    SD_disk_initialize();
    printf("A");
    //test
    
    return 0;
}
