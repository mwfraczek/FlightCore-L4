#ifndef BMP390_H
#define BMP390_H

extern volatile uint8_t bmp_data_ready;

// BMP390 Memory Address
#define BMP_CHIPID_ADDR    0x00
#define BMP_STATUS_ADDR    0x03
#define BMP_DATA_ADDR      0x04 // 0x04-0x09
#define BMP_INT_STATUS_ADDR 0x11  
#define BMP_INT_CTRL_ADDR  0x19  
#define BMP_PWRCTRL_ADDR   0x1B
#define BMP_OSR_ADDR       0x1C
#define BMP_ODR_ADDR       0x1D
#define BMP_CONFIG_ADDR    0x1F
#define BMP_COEFF_ADDR     0x31 // 0x31-0x45 (Calibration Block = 21 bytes)
#define BMP_SLAVE_ADDR     0x76

#define BMP_COEFF_LEN	   21

// Raw 24-bit Pressure and Temperature Values 
typedef struct {
	uint32_t raw_pressure;
	uint32_t raw_temperature;
} bmp_raw_t;

// Sensor Calibration Coefficients 
typedef struct {
	uint16_t T1;
	uint16_t T2;
	int8_t  T3;
	int16_t P1;
	int16_t P2;
	int8_t  P3;
	int8_t  P4;
	uint16_t P5;
	uint16_t P6;
	int8_t  P7;
	int8_t  P8;
	int16_t P9;
	int8_t  P10;
	int8_t  P11;
} bmp_coeff_t;

// Calibrated Temperature and Pressure Values
typedef struct { 
	float temperature_c;
	float pressure_pa;
} bmp_cal_t;

// Calibrated Coefficients 
typedef struct {
	float PAR_T1, PAR_T2, PAR_T3;
	float PAR_P1, PAR_P2, PAR_P3, PAR_P4, PAR_P5, PAR_P6, 
	      PAR_P7, PAR_P8, PAR_P9, PAR_P10, PAR_P11;
	float t_lin;
} bmp_par_t; 

// Function Headers
int bmp390_setup(void);
int bmp390_status(uint8_t *bmp_status);
int bmp390_chipid(uint8_t *bmp_chipid);
int bmp390_rawdata(bmp_raw_t *raw);
int bmp390_coeffdata(bmp_coeff_t *coeff);
void bmp390_coeffconvert(const bmp_coeff_t *c, bmp_par_t *p);
float bmp390_temp(uint32_t raw_temp, bmp_par_t *p);
float bmp390_pressure(uint32_t raw_press, bmp_par_t *p);
void print_bmp390(float temp, float press);

#endif
