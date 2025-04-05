//#include <riscv.h>
#include "pal.h"
#include "spi_sd.h"

void spi_gpio_init(void) {
	volatile unsigned int* GPIO_0_DDR = 0x80000004;
	volatile unsigned int* GPIO_0_PER = 0x80000012;
	*GPIO_0_DDR |= (Pin0) | (Pin2) | (Pin3);
}

/*
 * spi_send_byte
 * parameter: char msg, the packet to send, 8 bits long
 * this function emulates a spi on gpio0 using pins 0-3
 * pin 0 is mosi, pin 1 is miso, pin2 is sclk, pin3 is SS/CS
 * rtv: 
 *
 * */
void spi_send_byte(char msg){
	char new_val = 0;
	volatile unsigned int* GPIO_0_DATA = 0x80000000;
	volatile char not_used;

	*GPIO_0_DATA ^= Pin3;
	for(int i = 16; i > 0; i--){
		if((i & 1)) new_val = (new_val & ~(1)) | ((msg >> (i>>1)) &1); // load bit from msg into new_val
		else not_used = (new_val & ~(1)) | ((msg >> (i>>1)) &1);

		new_val ^= 1 << 2; //
		new_val |= 1 <<3; 
		*GPIO_0_DATA = new_val;		
	}
	*GPIO_0_DATA ^= Pin3;
	return;
}

char sd_rcv_byte(void){
	// TODO: Complete
	for(int i = 7; i >= 0; i--){
		
		for(int j = 0; i < 100; j++){
		
		}
	}

	return 0xFF;
}

char sd_cmd(char index, int msg, char crc){
	// first byte: 01 index
	// 2-5 bytes: msg
	// 6 byte: crc
	
	char new_msg = 0;
	
	index = 0b01000000 | index;
	char msg_1 = (msg & 0xFF000000) >> 24;
	char msg_2 = (msg & 0xFF0000) >> 16;
	char msg_3 = (msg & 0xFF00) >> 8;
	char msg_4 = (msg & 0xFF);
	crc = (crc <<1) | 1;
	char rtv = 0x00;
	volatile unsigned int* GPIO_0_DATA = 0x80000000;
	
	spi_send_byte(index);
	spi_send_byte(msg_1);	
	spi_send_byte(msg_2);
	spi_send_byte(msg_3);
	spi_send_byte(msg_4);
	spi_send_byte(crc);
	
	// TODO: Call SD Receive Byte
	return rtv;	
}

int sd_rcv_r3(void){
	int rtv = 0;
	rtv = sd_rcv_byte();
	rtv = (rtv << 8) | sd_rcv_byte();
	rtv = (rtv << 8) | sd_rcv_byte();
	rtv = (rtv << 8) | sd_rcv_byte();
	return rtv;
}