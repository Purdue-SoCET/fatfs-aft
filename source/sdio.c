#include "pal.h"
#include "sdio.h"
#include "spi_sd.h"

int SD_disk_status() {

};

int SD_disk_initialize() {
	//this is disk_initialize, one of the 3 required driver functions. 
	//That has a DSTATUS return value though, so change name later	
	volatile unsigned int* GPIO_0_DATA = 0x80000000;
	volatile char new_val = 0;
	char msg = 0xFF;
	volatile char not_used;
	int r3_resp;

	//init the sd card by sending 80 or so clock cycles
	*GPIO_0_DATA ^= Pin3;	
	for(int i = 512; i > 0; i--){
		if((i & 1))not_used = (new_val & ~(1)) | ((msg >> (i>>6)) &1); // load bit from msg into new_val
		else not_used = (new_val & ~(1)) | ((msg >> (i>>6)) &1);
		new_val ^= 1 << 2;
		*GPIO_0_DATA |= 0b1001;
	}
	*GPIO_0_DATA ^= Pin3;	
	char err_code = sd_cmd(0,0,0x95);
	if(err_code !=0x1) return err_code;
	err_code = sd_cmd(8,0x1AA,0x87);
	r3_resp = sd_rcv_r3();
	for(int i = 0; i< 1000; i ++){
		//send 55, then cmd 41, acmd 41 is 55+41
		//attempt 1000 times, if this loop did failed, the sd card did not init
		err_code = sd_cmd(55,0,0x1);
		if(err_code == 1){
			err_code = sd_cmd(41,0x40000000,0x1);
			if(err_code == 0) goto init_suc;		
		} 
	}
	return -1;
	init_suc:
	err_code = sd_cmd(58,0,1);
	r3_resp = sd_rcv_r3();	
	err_code = sd_cmd(16,0x200,1);
	return 1;
};
int SD_disk_read() {

};
int SD_disk_write() {
    
};