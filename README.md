# FatFs for the AFT0x7+ w/ SD card
## Introduction
This is a port of the File allocation table File System (FatFs) to the AFT0x7+. Contained in this README are relevant requirements to run FatFs on the AFT. If you have questions about FatFs itself, read about [FatFs](http://elm-chan.org/fsw/ff/).
### General Requirements
It is presumed you have taken (or are taking) ECE 36200, and in particular understand SPI channels. You don't necessarily need to understand SPI to work with FatFs, but it will help massively with debugging if issues arise.

> [!NOTE]
> FatFs will only work on the FPGA, not software simulation. This is because there must be a physical backing storage with which FatFs communicates, and we can't simulate an SD card.
### Software Requirements
This project requires standard library functions from the "string" and "stdlib" libraries. To implement these on the AFT, you will need picolibc set up. Refer to software instructions if you encounter issues here.
### Hardware requirements
The driver implementation for the AFT uses a bit-banged SPI channel over GPIO pins on GPIO0. Due to specifics of the implementation, all of GPIO0 is reserved. Only pins 0-3 are actually used, and the mappings are listed.
|MOSI|MISO|SCLK|CS|
|-|-|-|-|
|Pin0|Pin1|Pin2|Pin3|

With these pin mappings, you will be able to connect the SD card to the AFT through an SD card reader with a SPI channel. You likely have an SD card reader from the 362 labs. Remember, the MISO pin must be pulled up. You may wish to review the SD specific [FatFs info](http://elm-chan.org/docs/mmc/mmc_e.html).
<br>
When working with FatFs, all work must be done off of an FPGA simulating the AFT. Please review the FPGA docs if you do not know how to work with the FPGA and Quartus. The GPIO pins on the AFT will output a 3.3V signal, so use the 3.3V/GND pair to power your SD circuit. Review the pin mappings for GPIO0 pins 0-3 [here](https://www.terasic.com.tw/attachment/archive/502/DE2_115_User_manual.pdf).

## Working with FatFs
### Testing your Implementation
In order to test your code, you can run "FatFs_test.c". This will run a set of 24 tests 10 times to confirm your SD card can communicate with the AFT correctly.
> [!WARNING]
> This test will erase all the contents of your SD card and unformat it.
### General Flow
In order to use a file system, it must exist and be mounted. FatFs comes with functions to do both of these things. Below is an example of code you can generally to mount a filesystem to the SD card. This must be done each time you attempt to use the SD card.

```C
    FATFS FatFs;   // Work area (filesystem object) for logical drive
    FIL fil;        /* File object */
    char line[100]; /* Line buffer */
    FRESULT fr;     /* FatFs return code */

    /* Give a work area to the default drive */
    fr = f_mount(&FatFs, "", 0);
    if(fr == FR_NO_FILESYSTEM){ //
        printf("Attempting to make file system\n");
        BYTE work[FF_MAX_SS]; 
        fr = f_mfks("", 0, work, sizeof work);
        if(fr == FR_OK) {
            printf("File system generated");
            fr = f_mount(&FatFs, "", 0);
            //would be good to check fr
            break;
        }
        else printf("mkfs failed\n");
        //error handle as you wish
    }
    ...your file code here
```
Here, we try to mount the filesystem (assuming it exists) and FatF can tell us if it doesn't. If so, we can try to make a file system. Note that it takes quite a long time (minutes) to complete mkfs. If mount succeeds, you can then use any function supported by us and FatFs. 
## Code Support
There are many different file functions built into FatFs. If you successfully mounted the file system, all of these should work. The exception to this is timestamp functionality, which is not currently implemented. The AFT (as of writing this) does not have a real time clock. Therefore, timestamps for files written to/created by the AFT are not reliable. It would be possible to fill in a dummy time in ''get_fattime()'' but it is not currently implemented.
## Example Project
You can view an example project in the MS-DOS repo. This can connect to your laptop through a serial to USB converter, allowing for commands to be send to the AFT through your own device's keyboard. Making this project work on your own FPGA setup of the AFTx07 is a good sanity check that the SD card is responding.
# Copyright Notice
## FatFs

This is a copy of FatFs, a "generic FAT file system module for small embedded
systems", by ChaN. See http://elm-chan.org/fsw/ff/00index_e.html.

Please submit bug reports to the http://elm-chan.org/fsw/ff/bd/[FatFs
User Forum], not to this repo.

## License

Copyright (C) 2022, ChaN, all right reserved.

FatFs module is an open source software. Redistribution and use of FatFs in
source and binary forms, with or without modification, are permitted provided
that the following condition is met:

1. Redistributions of source code must retain the above copyright notice,
   this condition and the following disclaimer.

This software is provided by the copyright holder and contributors "AS IS"
and any warranties related to this software are DISCLAIMED.
The copyright owner or contributors be NOT LIABLE for any damages caused
by use of this software.
