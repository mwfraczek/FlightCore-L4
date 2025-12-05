#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void spi3_init(void);
void spi3_transmit(uint8_t *data, uint8_t size); 
void spi3_receive(uint8_t *data, uint8_t size);

#endif
