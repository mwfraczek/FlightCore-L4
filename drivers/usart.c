// USART Driver Library for STM32L476RG applications

#include "stm32l476.h"
#include "i2c.h"
#include "peripherals.h"
#include "bmp390.h"
#include "utils.h"

// Setup USART2 peripheral
void usart2_init(void) { 
	USART2_CR1 &= ~(1 << 12); // M0 word length
	USART2_CR1 &= ~(1 << 28); // M1 word length (8-bit word length)
	USART2_CR2 &= ~(3 << 12); // STOP bits (n = 0)
	USART2_BRR  = 0x0683;     // Baud rate = 9600 (fCK / baud) 0x0683
	USART2_CR1 |=  (1 << 3);  // Transmitter enable (TE)
	USART2_CR1 |=  (1 << 0);  // USART enable (UE)
}

// Setup USART3 periperhal
void usart3_init(void) { 
        USART3_CR1 &= ~(1 << 12); // M0 word length
        USART3_CR1 &= ~(1 << 28); // M1 word length (8-bit word length)
        USART3_CR2 &= ~(3 << 12); // STOP bits (n = 0)
        USART3_BRR  = 0x0683;     // Baud rate = 9600 (fCK / baud) 0x0683
	USART3_CR1 |=  (1 << 5);  // Enable RXNE interrupt
	USART3_CR1 |=  (1 << 2);  // Receiver enable (RE)
        USART3_CR1 |=  (1 << 0);  // USART enable (UE)
}

// General TX Function
void usart2_transmit(uint8_t data) {
	while (!(USART2_ISR & (1 << 7))); 
	USART2_TDR = data;
}

// Transmit integer data (helper)
void usart2_transmitint(int value) {
	char buf[12];  
	size_t i = 0;

	// Handle '0' values
	if (value == 0) {
		usart2_transmit('0');
		return;
	}

	// Handle negative values
	if (value < 0) {
		usart2_transmit('-');
		value = -value;
	}

	// Convert digits to characters in reverse
	while (value > 0 && i < sizeof(buf) - 1) {
		buf[i++] = (value % 10) + '0';
		value /= 10;
	}

	// Send digits in correct order
	while (i--) {
		usart2_transmit(buf[i]);
	}
}


// Transmit string data (helper)
void usart2_transmitstr(const char *s) {
    while (*s) {
        usart2_transmit(*s++);
    }
}

// Transmit float data (helper)
void usart2_transmitfloat(float value, int decimals) {
	if (value < 0) {
		usart2_transmit('-');
		value = -value;
	}
	int whole = (int)value;
	float frac_part = value - (float)whole;

	// Scale fractional part
	for (int i = 0; i < decimals; i++) {
		frac_part *= 10.0f;
	}
	int frac = (int)frac_part;

	usart2_transmitint(whole);
	usart2_transmit('.');

	// Ensure leading zeros in fractional part
	int div = 1;
	for (int i = 1; i < decimals; i++)
		div *= 10;
	while (frac < div) {
		usart2_transmit('0');
		div /= 10;
	}
	usart2_transmitint(frac);
}
