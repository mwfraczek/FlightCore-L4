#ifndef USART_H 
#define USART_H 

#include <stddef.h> // Handles 'NULL'
#include <stdint.h>

void usart2_init(void);
void usart3_init(void);
void usart2_transmit(uint8_t data);
void usart2_transmitint(int value);
void usart2_transmitstr(const char *s);
void usart2_transmitfloat(float value, int decimals);
void usart3_receive(char buf[100]);

#endif
