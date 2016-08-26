/* Name: main.c
 * Author: HMTH(c) 
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "common_tasks.h"
#include "tinudht.h"
#include "nRF24L01.h"

#define SPI_PORT PORTB
#define DHT11_PIN PB3
#define SPI_SCK PB2
#define SPI_MOMI PB0
#define SPI_DDR DDRB
#define SPI_PIN PINB

void spi_enable() {
	SETBIT(SPI_DDR,SPI_SCK); // output mode for SPI_SCK pin
}

uint8_t spi_rf_transceive_byte (uint8_t cData) {
	uint8_t rdData, bits = 8;
	
	do {
		rdData <<= 1;
		
		if (ISONE(SPI_PIN,SPI_MOMI)) rdData++;
		
		SETBIT(SPI_DDR,SPI_MOMI); // output mode for MOMI
		if (ISONE(cData,0x80)) SETBIT(SPI_PORT,SPI_MOMI);
		SPI_PIN = (1 << SPI_SCK);
	
		CLRBIT(SPI_DDR,SPI_MOMI); // input mode for MOMI
		SPI_PIN = (1 << SPI_SCK);    // toggle SCK

		CLRBIT(SPI_PORT,SPI_MOMI);
		cData <<= 1;
		
	} while (--bits);
	
	return rdData;
}

void spi_rf_trans_byte (uint8_t cData) {
	uint8_t bits = 8;
	SETBIT(SPI_DDR<SPI_MOMI); // output mode for MOMI
	
	do {
		if (ISONE(cData,0x80)) SPI_PIN = (1 << SPI_MOMI);
		SPI_PIN = (1 << SPI_SCK);
		SPI_PIN = (1 << SPI_SCK); // toggle;
		CLRBIT(SPI_PORT,SPI_MOMI);
		cData <<= 1;
	} while (--bits);
	
	CLRBIT(SPI_DDR,SPI_MOMI); // input mode for MOMI
}

uint8_t spi_rf_recv_byte () {
	uint8_t pinstate, rdData, bits = 8;
	
	do {
		rdData <<= 1;
		SPI_PIN = (1 << SPI_SCK);
		pinstate = SPI_PIN;
		SPI_PIN = (1 << SPI_SCK);
		if (ISONE(pinstate,0x80)) rdData++;
	} while (--bits);
	
	return rdData;
}

ISR (WDT_vect) {
	TinuDHT tinudht;
	uint8_t ReadDHTsts;
	
	ReadDHTsts = tinudht_read (&tinudht, DHT11_PIN);
	
	if (ReadDHTsts == 0) {
		spi_rf_trans_byte (0xAA); // remark value, after this received data are humidity and temperature
		spi_rf_trans_byte (tinudht.humidity);
		spi_rf_trans_byte (tinudht.temperature);
	}
}

int main(void) {
	_delay_ms(100); // delay for dht11 to stabilizer
	// Disable ADC 
	CLRBIT(ADCSRA,ADEN);
	
	spi_enable();
	
	// Enable watchdong timer interrupt
	// Set timer
	WDTCR |= (0 << WDP3) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);// set timer interval to 2 s
	// Set interrupt mode
	SETBIT(WDTCR,WDTIE);
	sei();
	
	// Go to sleep mode
	set_sleep_mode (SLEEP_MODE_PWR_DOWN);
	while (1) {
		sleep_mode();
	}
	
	return 0; //* never reached */
}