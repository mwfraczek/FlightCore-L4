// SPI Protocol Driver Library

#include "stm32l476.h"
#include "spi.h"
#include "peripherals.h"

void spi3_init(void) {
	SPI3_CR1 &= ~(1 << 6); // Disable SPI 
	SPI3_CR1 |=  (3 << 3); // Set Baud Rate = fPCLK(16) / 16 = 1MHz
	SPI3_CR1 |=  (0 << 1); // CPOL = 0 (idle state high)
	SPI3_CR1 |=  (0 << 0); // CPHA = 0 (sample data on leading clock edge)
	SPI3_CR1 |=  (1 << 2); // Set as master  
	SPI3_CR1 |=  (1 << 9); // SSM = 1 (software slave management)
	SPI3_CR1 |=  (1 << 8); // SSI = 1 (internal NSS high)
	SPI3_CR2 |=  (1 << 12); // FRXTH = 1 (RXNE set after 8-bit RX FIFO level, not 16-bit default)
	SPI3_CR1 |=  (1 << 6); // Enable SPI
} 

void spi3_transmit(uint8_t *data, uint8_t size) {
	uint8_t i = 0;
	volatile uint8_t dummy;

	while (i < size) {
		while (!(SPI3_SR & (1 << 1))); // Wait for empty Tx buffer
		SPI3_DR = data[i]; // Write data to data register
		while (!(SPI3_SR & (1 << 0))); // Wait for Rx buffer not empty
		dummy = SPI3_DR; // Drain simultaneously-received byte (keeps RX in sync with TX)
		i++;
	}
	while (SPI3_SR & (1 << 7)); // Wait for clear SPI bus
}

void spi3_receive(uint8_t *data, uint8_t size) { 
	while(size) {
		while (!(SPI3_SR & (1 << 1))); // Wait for empty Tx buffer
		SPI3_DR = 0; // Send dummy byte	
		while (!(SPI3_SR & (1 << 0))); // Wait for empty Rx buffer
		*data++ = SPI3_DR;
		size--; 
	}
	while (SPI3_SR & (1 << 7)); // Wait for clear SPI bus
}
