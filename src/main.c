#include "stm32l476xx.h"
#include "stm32l476.h"
#include "peripherals.h"
#include "utils.h"
#include "i2c.h"
#include "bmp390.h"
#include "usart.h"
#include "gps.h"
#include "spi.h"
#include "bmi088.h"

int main(void) {
	// Variable Declarations
	uint8_t bmpstatus;
	volatile float temperature_c;
	volatile float pressure_pa;
	char gpsdata[100];
	bmp_coeff_t cal_raw; 	// raw coeffs
	bmp_par_t cal;		// converted coeffs
	bmp_raw_t raw;		// raw readings
	bmi_raw_t accel_raw;	// BMI088 raw accel reading
	bmi_raw_t gyro_raw;	// BMI088 raw gyro reading
	uint8_t accel_chipid;	// BMI088 accel chip ID (expect 0x1E / 30)
	uint8_t gyro_chipid;	// BMI088 gyro chip ID (expect 0x0F / 15)
	uint32_t chipid_last_tick = 0;

	// Setup STM32 Peripherals 
	rcc_enable();
	gpio_config();
	tim2_enable();
	i2c1_init();
	usart2_init();
	usart3_init();
	syscfg_enable();
	exti_enable();
	spi3_init();

	// Interrupt Setup
	NVIC_SetPriority(TIM2_IRQn, 4);
	NVIC_EnableIRQ(TIM2_IRQn);  	// timer-base LED interrupt  
	NVIC_SetPriority(USART3_IRQn, 3);
	NVIC_EnableIRQ(USART3_IRQn);	// UART stream GPS interrupt
	NVIC_SetPriority(EXTI15_10_IRQn, 2);
	NVIC_EnableIRQ(EXTI15_10_IRQn);	// BMP390 external interrupt
	NVIC_SetPriority(EXTI9_5_IRQn, 1);
        NVIC_EnableIRQ(EXTI9_5_IRQn); // BMI088 external interrupt

	// Wait For Ready Sensor 
	if (bmp390_setup() != 0) led_blink_fast();
	do {
		if (bmp390_status(&bmpstatus) != 0)
			led_blink_fast(); 
	} while (bmpstatus != 0x70);

	// BMI088 Setup
	bmi088_setup();

	// Read and Convert Sensor Calibration Coefficients
	bmp390_coeffdata(&cal_raw);
        bmp390_coeffconvert(&cal_raw, &cal);
	
	// Output Temp, Pressure, GPS Values via UART
	while (1) {
		// Re-print BMI088 chip IDs every ~2s (expect 30 = 0x1E, 15 = 0x0F)
		if (tick_ms - chipid_last_tick >= 2000) {
			chipid_last_tick = tick_ms;
			bmi088_accel_chipid(&accel_chipid);
			bmi088_gyro_chipid(&gyro_chipid);
			usart2_transmitstr("Accel ChipID: ");
			usart2_transmitint(accel_chipid);
			usart2_transmitstr("\r\n");
			usart2_transmitstr("Gyro ChipID: ");
			usart2_transmitint(gyro_chipid);
			usart2_transmitstr("\r\n");
		}
		// Pull raw data, convert, output
		if (bmp_data_ready) {
			bmp_data_ready = 0;
			bmp390_rawdata(&raw);
			temperature_c = bmp390_temp(raw.raw_temperature, &cal); 
			pressure_pa = bmp390_pressure(raw.raw_pressure, &cal);
			print_bmp390(temperature_c, pressure_pa);
		}
		// Print only 'GPRMC' NMEA sentences
		if (gps_parse(gpsdata)) {
			if (strncmp(gpsdata, "$GPRMC", 6) == 0) {
				print_gps(gpsdata);
			}
		}
		// Pull raw BMI088 data, output
		if (bmi_accel_data_ready) {
			bmi_accel_data_ready = 0;
			bmi088_accel_data(&accel_raw);
			print_bmi088(&accel_raw, &gyro_raw);
		}
		if (bmi_gyro_data_ready) {
			bmi_gyro_data_ready = 0;
			bmi088_gyro_data(&gyro_raw);
			print_bmi088(&accel_raw, &gyro_raw);
		}
	}
}
