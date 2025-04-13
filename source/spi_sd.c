//#include <riscv.h>
#include "pal.h"
#include "spi_sd.h"

void spi_gpio_init(void) {
	volatile unsigned int* GPIO_0_DDR = 0x80000004;
	volatile unsigned int* GPIO_0_PER = 0x80000012;
	*GPIO_0_DDR &= ~(Pin1);
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

	// *GPIO_0_DATA ^= Pin3;
	for(int i = 17; i > 1; i--){
		if((i & 1)) { // negedge
			new_val = (new_val & ~(1)) | ((msg>>7) &1); // load bit from msg into new_val
			msg <<= 1;
			new_val &= ~Pin2;
		}
		else { // posedge
			not_used = (new_val & ~(1)) | ((msg>>7) &1);
			not_used >>= 1;
			new_val |= Pin2;
		}
		*GPIO_0_DATA = new_val;		
	}

	// *GPIO_0_DATA ^= Pin3;
	return;
}

char sd_rcv_byte(void) {
	char new_val = Pin0;
	// new_val &= ~Pin3;
	volatile unsigned int* GPIO_0_DATA = 0x80000000;

	// Wait for a start bit from the SD card, on MISO (Pin1)
	uint8_t found_start_bit = 0;
	uint8_t clk_high = 0;
	int attempts = 0;
	while (!found_start_bit && (attempts < 200)) {
		if(clk_high) { // currently on pos, setting neg
			new_val ^= Pin2;
			*GPIO_0_DATA = new_val; // negedge
			clk_high = 0;
		}
		else { // is neg, setting pos
			new_val ^= Pin2;
			*GPIO_0_DATA = new_val; // posedge
			if(!(*GPIO_0_DATA & Pin1)) {
				found_start_bit = 1;
			}
			clk_high = 1;
		}
		attempts++;
	}
	if(attempts == 200) return 0xFF;

	// Sending one last negative edge
	new_val ^= Pin2;
	*GPIO_0_DATA = new_val;

	char read_data = 0;
	// Read in 7 bits
	for(int i = 6; i >= 0; i--) {
		new_val ^= Pin2;
		*GPIO_0_DATA = new_val; // posedge
		read_data |= (*GPIO_0_DATA & Pin1) >> 1;
		read_data <<= 1;
		
		new_val ^= Pin2;
		*GPIO_0_DATA = new_val; // negedge
	}

	return read_data >> 1;
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
	
	*GPIO_0_DATA &= ~Pin3;
	spi_send_byte(index);
	spi_send_byte(msg_1);	
	spi_send_byte(msg_2);
	spi_send_byte(msg_3);
	spi_send_byte(msg_4);
	spi_send_byte(crc);
	rtv = sd_rcv_byte();
	// TODO: Call SD Receive Byte
	*GPIO_0_DATA |= Pin3;
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
