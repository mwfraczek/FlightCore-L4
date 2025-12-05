// BMP390 Device Driver Library 

#include "stm32l476.h"
#include "i2c.h"
#include "peripherals.h"
#include "bmp390.h"
#include "utils.h"
#include "usart.h"

// Configuration Function 
int bmp390_setup(void) {
	uint8_t bmp_pwrctrl = 0x33; // Normal Mode - Temp/Pressure Enable
	uint8_t bmp_osr = 0x00;	    // No oversampling
	uint8_t bmp_odr = 0x08;	    // ~1 second sampling period
	uint8_t bmp_intctrl = 0x42; // Enable data ready interrupt - INT active high

	if (i2c1_write(BMP_SLAVE_ADDR, BMP_PWRCTRL_ADDR, &bmp_pwrctrl, 1, 1) != 0)
		return -1; 
        if (i2c1_write(BMP_SLAVE_ADDR, BMP_OSR_ADDR, &bmp_osr, 1, 1) != 0)
		return -1; 
        if (i2c1_write(BMP_SLAVE_ADDR, BMP_ODR_ADDR, &bmp_odr, 1, 1) != 0)
		return -1;
	if (i2c1_write(BMP_SLAVE_ADDR, BMP_INT_CTRL_ADDR, &bmp_intctrl, 1, 1) != 0)
                return -1;
	return 0;
}

// Sensor Status
int bmp390_status(uint8_t *bmp_status) {
	if (i2c1_write(BMP_SLAVE_ADDR, BMP_STATUS_ADDR, NULL, 0, 0) != 0)
		return -1; 
        if (i2c1_read(BMP_SLAVE_ADDR, bmp_status, 1, 1) != 0)
		return -1;
	return 0;
}

// Read Chip ID
int bmp390_chipid(uint8_t *bmp_chipid) {
        if (i2c1_write(BMP_SLAVE_ADDR, BMP_CHIPID_ADDR, NULL, 0, 0) != 0)
		return -1;
        if (i2c1_read(BMP_SLAVE_ADDR, bmp_chipid, 1, 1) != 0)
		return -1;
	return 0;
}

// Read Raw Pressure + Temperature Data
int bmp390_rawdata(bmp_raw_t *raw) {
	uint8_t buf[6];

	if (i2c1_write(BMP_SLAVE_ADDR, BMP_DATA_ADDR, NULL, 0, 0) != 0)
		return -1;
	if (i2c1_read(BMP_SLAVE_ADDR, buf, 6, 1) != 0)
		return -1;
	
	// store raw data in bmp390_raw_t struct
	raw->raw_pressure = 
		((uint32_t)(buf[2] << 16)) |
		((uint32_t)(buf[1] << 8))  |
		(uint32_t)(buf[0]);

	raw->raw_temperature = 
		((uint32_t)(buf[5] << 16)) |
		((uint32_t)(buf[4] << 8))  |
		(uint32_t)(buf[3]);

	return 0;
}

// Read Calibration Coefficients
int bmp390_coeffdata(bmp_coeff_t *coeff) {
	uint8_t buf[BMP_COEFF_LEN]; // temporary coeff data holding area

	if (i2c1_write(BMP_SLAVE_ADDR, BMP_COEFF_ADDR, NULL, 0, 0) != 0)
		return -1;
	if (i2c1_read(BMP_SLAVE_ADDR, buf, BMP_COEFF_LEN, 1) != 0)
		return -1;

	// store coefficients in bmp_coeff_t struct
	coeff->T1 = (uint16_t)((buf[1] << 8) | buf[0]);
	coeff->T2 = (uint16_t)((buf[3] << 8) | buf[2]);
	coeff->T3 = (int8_t)(buf[4]);
	coeff->P1 = (int16_t)((buf[6] << 8) | buf[5]);
	coeff->P2 = (int16_t)((buf[8] << 8) | buf[7]);
	coeff->P3 = (int8_t)(buf[9]);
	coeff->P4 = (int8_t)(buf[10]);
	coeff->P5 = (uint16_t)((buf[12] << 8) | buf[11]);
	coeff->P6 = (uint16_t)((buf[14] << 8) | buf[13]);
	coeff->P7 = (int8_t)(buf[15]);
	coeff->P8 = (int8_t)(buf[16]);
	coeff->P9 = (int16_t)((buf[18] << 8) | buf[17]);
	coeff->P10 = (int8_t)(buf[19]);
	coeff->P11 = (int8_t)(buf[20]);
	
	return 0;
}

// Coefficient Conversion  
void bmp390_coeffconvert(const bmp_coeff_t *c, bmp_par_t *p) {
	p->PAR_T1 = (float)c->T1 * (float)(1u << 8);
	p->PAR_T2 = (float)c->T2 / (float)(1u << 30);
	p->PAR_T3 = (float)c->T3 / (float)((uint64_t)1 << 48);
	p->PAR_P1 = ((float)c->P1 - (float)(1u << 14)) / (float)(1u << 20);
	p->PAR_P2 = ((float)c->P2 - (float)(1u << 14)) / (float)(1u << 29);
	p->PAR_P3 = (float)c->P3 / (float)((uint64_t)1 << 32);
	p->PAR_P4 = (float)c->P4 / (float)((uint64_t)1 << 37);
	p->PAR_P5 = (float)c->P5 * (float)(1u << 3);
	p->PAR_P6 = (float)c->P6 / (float)(1u << 6);
	p->PAR_P7 = (float)c->P7 / (float)(1u << 8);
	p->PAR_P8 = (float)c->P8 / (float)(1u << 15);
	p->PAR_P9 = (float)c->P9 / (float)((uint64_t)1 << 48);
	p->PAR_P10 = (float)c->P10 / (float)((uint64_t)1 << 48);
	p->PAR_P11 = (float)c->P11 * 2.710505431e-20f; // P11 / (2^65), stored as float constant 
}

// Temperature Conversion (Bosch Equation)
float bmp390_temp(uint32_t raw_temp, bmp_par_t *p) {
	float partial_data1; 
	float partial_data2;

	partial_data1 = (float)raw_temp - p->PAR_T1;
	partial_data2 = (float)(partial_data1 * p->PAR_T2);

	p->t_lin = partial_data2 + (partial_data1 * partial_data1) * p->PAR_T3;

	return p->t_lin;
}

// Pressure Conversion (Bosch Equation)
float bmp390_pressure(uint32_t raw_press, bmp_par_t *p) {
	float comp_press;
	float partial_data1; 
        float partial_data2;
	float partial_data3; 
        float partial_data4;
	float partial_out1; 
        float partial_out2;

	partial_data1 = p->PAR_P6 * p->t_lin;
	partial_data2 = p->PAR_P7 * (p->t_lin * p->t_lin);
	partial_data3 = p->PAR_P8 * (p->t_lin * p->t_lin * p->t_lin);
	partial_out1 = p->PAR_P5 + partial_data1 + partial_data2 + partial_data3;

	partial_data1 = p->PAR_P2 * p->t_lin;
        partial_data2 = p->PAR_P3 * (p->t_lin * p->t_lin);
        partial_data3 = p->PAR_P4 * (p->t_lin * p->t_lin * p->t_lin);       
        partial_out2 = (float)raw_press * (p->PAR_P1 + partial_data1 + partial_data2 + partial_data3);

	partial_data1 = (float)raw_press * (float)raw_press;
        partial_data2 = p->PAR_P9 + p->PAR_P10 * p->t_lin;
        partial_data3 = partial_data1 * partial_data2;
	partial_data4 = partial_data3 + ((float)raw_press * (float)raw_press * (float)raw_press) * p->PAR_P11;
	comp_press = partial_out1 + partial_out2 + partial_data4;

	return comp_press;
}

// Output BMP390 Temperature & Pressure Readings
void print_bmp390(float temp, float press) {
	usart2_transmitstr("Temp (C): ");
	usart2_transmitfloat(temp, 2);
	usart2_transmitstr("\r\n");

	usart2_transmitstr("Pressure (Pa): ");
	usart2_transmitfloat(press, 2);
	usart2_transmitstr("\r\n");
}

// BMP390 Data Ready External Interrupt
volatile uint8_t bmp_data_ready = 0;
void EXTI15_10_IRQHandler(void) {
	if (EXTI_PR1 & (1 << 10)) {   // Check if interrupt flag is raised
		EXTI_PR1 = (1 << 10); // Clear interrupt flag bit
		bmp_data_ready = 1;  // Data is ready signal
	}
}
