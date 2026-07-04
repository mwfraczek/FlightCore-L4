#ifndef BMI088_H
#define BMI088_H

#include <stdint.h>

extern volatile uint8_t bmi_accel_data_ready;
extern volatile uint8_t bmi_gyro_data_ready;

// BMI088 Register Addresses
#define ACC_CHIPID_ADDR  0x00
#define ACC_DATA_ADDR    0x12 // 0x12-0x17 (X/Y/Z, LSB first)
#define GYRO_CHIPID_ADDR 0x00
#define GYRO_DATA_ADDR   0x02 // 0x02-0x07 (X/Y/Z, LSB first)

// Expected Chip ID Values
#define ACC_CHIPID_VAL   0x1E
#define GYRO_CHIPID_VAL  0x0F

// Raw Accelerometer / Gyroscope Readings
typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} bmi_raw_t;

void bmi088_setup(void);
void bmi088_accel_transmit(uint8_t reg, uint8_t val);
void bmi088_gyro_transmit(uint8_t reg, uint8_t val);
void bmi088_accel_receive(uint8_t reg, uint8_t *data, uint8_t size);
void bmi088_gyro_receive(uint8_t reg, uint8_t *data, uint8_t size);
int bmi088_accel_chipid(uint8_t *chipid);
int bmi088_gyro_chipid(uint8_t *chipid);
void bmi088_accel_data(bmi_raw_t *raw);
void bmi088_gyro_data(bmi_raw_t *raw);
void print_bmi088(const bmi_raw_t *accel, const bmi_raw_t *gyro);

#endif
