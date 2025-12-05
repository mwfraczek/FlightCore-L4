// BMI088 IMU Driver Library

#include "stm32l476.h"
#include "spi.h"
#include "peripherals.h"
#include "utils.h"
#include "bmi088.h"

// IMU setup 
void bmi088_setup(void) {
	// accelerometer configuration
	uint8_t accel_on = 0x04; // accel on 
	uint8_t acc_pwrctrl_addr = 0x7D;
	uint8_t int1_config = 0x4; // active low, push-pull, output
	uint8_t int1_ioconf_addr = 0x53;
	uint8_t int1_drdy = 0x04; // map data ready interrupt to int1
	uint8_t int1_mapdata_addr = 0x58; 
	uint8_t acc_odr = 0x85; // 4-fold osr, 12.5Hz odr = 1Hz data rate 
	uint8_t acc_conf_addr = 0x40; 
	// gyroscope configuration
	uint8_t gyro_int_enable = 0x80; // explicitly enable gyro interrupt
	uint8_t gyro_intctrl_addr = 0x15;
	uint8_t int3_config = 0x0; // active low, push-pull
	uint8_t int3_ioconf_addr = 0x16;
	uint8_t int3_drdy = 0x00; // data ready int mapped to int3
	uint8_t int3_iomap_addr = 0x18; 
	uint8_t gyro_odr = 0x07; // 32Hz data rate
	uint8_t gyro_bandwidth_addr = 0x10; 

	// accel/gyro SPI setup
	GPIOC_PUPDR |= (1 << 16); // PC8 - CSB1 accel pullup
	GPIOC_PUPDR |= (1 << 18); // PC9 - CSB2 gyro pullup
	GPIOC_ODR   &= ~(1 << 8); // PC8 - CSB1 low
	GPIOC_ODR   |=  (1 << 8); // PC8 - CSB1 high
	delay_ms(1);
	bmi088_accel_transmit(acc_pwrctrl_addr, accel_on);
	delay_ms(1);
	
	// accel interrupt and data rate
	bmi088_accel_transmit(int1_ioconf_addr, int1_config); 
	bmi088_accel_transmit(int1_mapdata_addr, int1_drdy); 
	bmi088_accel_transmit(acc_conf_addr, acc_odr); 

	// gyro interrupt and data rate
        bmi088_gyro_transmit(gyro_intctrl_addr, gyro_int_enable);
	bmi088_gyro_transmit(int3_ioconf_addr, int3_config);
	bmi088_gyro_transmit(int3_iomap_addr, int3_drdy);
	bmi088_gyro_transmit(gyro_bandwidth_addr, gyro_odr); 
}

// Accel setup helper 
void bmi088_accel_transmit(uint8_t reg, uint8_t val) { 
	GPIOC_ODR   &= ~(1 << 8); // Accel - CSB1 low
	spi3_transmit(&reg, 1); 
	spi3_transmit(&val, 1); 
	GPIOC_ODR   |=  (1 << 8); // Accel - CSB1 high
}

// Gyro setup helper 
void bmi088_gyro_transmit(uint8_t reg, uint8_t val) {
        GPIOC_ODR   &= ~(1 << 9); // Gyro - CSB2 low
	spi3_transmit(&reg, 1);
        spi3_transmit(&val, 1);
	GPIOC_ODR   |=  (1 << 9); // Gyro - CSB2 high
}

// BMI088 Interrupts
volatile uint8_t bmi_accel_data_ready = 0;
volatile uint8_t bmi_gyro_data_ready = 0;
void EXTI9_5_IRQHandler(void) {
	// Accel on PC6 - EXTI6
	if (EXTI_PR1 & (1 << 6)) {   // Check if interrupt flag is raised
		EXTI_PR1 = (1 << 6); // Clear interrupt flag bit
		bmi_accel_data_ready = 1;  // Data is ready signal
	}
	// Gyro on PC7 - EXTI76
	if (EXTI_PR1 & (1 << 7)) {   // Check if interrupt flag is raised 
                EXTI_PR1 = (1 << 7); // Clear interrupt flag bit 
                bmi_gyro_data_ready = 1;  // Data is ready signal
        }
}
