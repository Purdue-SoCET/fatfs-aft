#include "source/ff.h"
#include <stdio.h>

int main() {
    volatile unsigned int *GPIO_DATA = 0x80000000;
    volatile unsigned int *GPIO_DIR  = 0x80000004;

    (*GPIO_DIR) = 0xFF;
    unsigned int state = ~(0);

    for(int k = 0; k < 100; k++) {
	//for (int i = 0; i < 0x800000; i++);
        (*GPIO_DATA) = state;
        state = ~state;
    }
    printf("TESTING");


    f_mount(0, 0, 0);
    return 0;
}
