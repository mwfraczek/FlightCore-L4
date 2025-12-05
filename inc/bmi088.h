#ifndef BMI088_H
#define BMI088_H

#include <stdint.h>

extern volatile uint8_t bmi_accel_data_ready;
extern volatile uint8_t bmi_gyro_data_ready;

void bmi088_setup(void);
void bmi088_accel_transmit(uint8_t reg, uint8_t val); 
void bmi088_gyro_transmit(uint8_t reg, uint8_t val);

#endif
