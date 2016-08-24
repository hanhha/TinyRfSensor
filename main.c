/* Name: main.c
 * Author: HMTH(c) 
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "common_tasks.h"
#include "tinudht.h"
#include "nRF24L01.h"

#define DHT11_PIN PB3
#define SCK_PIN PB2
#define MOMI_PIN PB0

void InitRF_SPI (void) {
	//Set SCK, MISO, CSN and CE as outport
	//TODO: implement
	//Set MOSI as input, connects to nRF MISO
	//TODO: implement
	//CSN high to start with, nothing to be sent to the nRF yet!
	//TODO: implement
	//CE low to start with, nothing to send/receive yet
	//TODO: implement
}

uint8_t WriteByteSPI (uint8_t cData) {
}

int main(void)
{
	uint8_t ReadDHTsts;
	
	InitRF_SPI ();
	_delay_ms(100); // delay for dht11 to stabilizer

	while (1) {
		TinuDHT tinudht;
		ReadDHTsts = tinudht_read (&tinudht, DHT11_PIN);
		if (ReadDHTsts == 0) {
			// Transmit data via RF
		}
		_delay_ms(5000);
	}
	
    return 0;   /* never reached */
}
