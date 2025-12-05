//  STM32L476RG Peripheral Setup and Enable Functions

#include "stm32l476xx.h"
#include "stm32l476.h"
#include "peripherals.h"

void rcc_enable(void) {
	RCC_AHB2ENR  |= (7 << 0);  // Enable GPIOA & GPIOB & GPIOC clocks
	RCC_APB1ENR1 |= (1 << 21); // Enable I2C1 clock
	RCC_CCIPR    |= (1 << 12); // Select SYSCLK (HSI16) as I2C1 source clock
	RCC_APB1ENR1 |= (1 << 0);  // Enable TIM2 clock
	RCC_APB1ENR1 |= (1 << 17); // Enable USART2 clock
	RCC_CCIPR    |= (1 << 2);  // Select SYSCLK as USART2 clock	
	RCC_APB1ENR1 |= (1 << 18); // Enable USART3 clock
	RCC_CCIPR    |= (1 << 4);  // Select SYSCLK as USART3 clock
	RCC_APB1ENR1 |= (1 << 15); // Enable SPI3 clock
	RCC_APB2ENR  |= (1 << 0);  // Enable SYSCFG clock
}

void gpio_config(void) {
	// Set Nucleo User LD2 LED
	GPIOA_MODER &= ~(3 << 10); 	// Clear PA5 bits
	GPIOA_MODER |=  (1 << 10); 	// Set PA5 to output mode

	// Set PB8/PB9 as alternate function pins for I2C use case
	GPIOB_MODER &= ~(0xF << 16);    // Clear PB8/PB9 bits
	GPIOB_MODER |=  (0xA << 16);    // Set PB8/PB9 to AF mode
	GPIOB_AFRH  &=  ~((0xF << 4) | (0xF << 0)); // Clear AF bits
	GPIOB_AFRH  |=  (4 << 4) | (0x4 << 0);      // Set pins to AF4 (I2C selection)  
	GPIOB_OTYPER |=  (3 << 8);    // Set PB8/PB9 to open-drain
	GPIOB_PUPDR  &= ~(0xF << 16);   // Set NO pull up/downs (use external resistors)

	// USED PREVIOUSLY AS DEBUG PIN
	// Set GPIO debug pin (Nucleo D10 - PB6)
	// GPIOB_OSPEEDR |= (2 << 12);   // High speed
	// GPIOB_MODER &= ~(3 << 12);    // Clear PB6 bit
	// GPIOB_MODER |=  (1 << 12);    // Set PB6 to output mode
	
	// Set PA2 (TX) as AF7 for UART2 use case (serial console)
	GPIOA_MODER &= ~(3 << 4);	// Clear PA2 bits
	GPIOA_MODER |=  (2 << 4);	// Set PA2 to AF mode
	GPIOA_AFRL &= ~(0xF << 8);	// Clear AF bits
	GPIOA_AFRL |=  (7 << 8);	// Set PA2 to AF7 (USART2)
	GPIOA_OTYPER &= ~(1 << 2);   	// Set PA2 to push-pull (default state)
	GPIOA_PUPDR  &= ~(0xF << 4); 	// Set NO pull-up/downs  

	// Set PB11 (RX) as AF7 for UART3 use case (GPS module)
        GPIOB_MODER &= ~(3 << 22);	// Clear PB11 bits
        GPIOB_MODER |=  (2 << 22); 	// Set PB11 to AF mode
        GPIOB_AFRH &= ~(0xF << 12); 	// Clear AF bits
        GPIOB_AFRH |=  (7 << 12); 	// Set PB11 to AF7 (USART3)
        GPIOB_OTYPER &= ~(1 << 11); 	// Set PB11 to push-pull (default state)
        GPIOB_PUPDR  &= ~(3 << 22); 	// Clear
	GPIOB_PUPDR  |=  (1 << 22); 	// Set pull ups

	// Set PB10 as input for BMP390 data-ready interrupt
	GPIOB_MODER &= ~(3 << 20);   	// Set PB6 to input mode
	GPIOB_PUPDR &= ~(3 << 20);   	// Set NO pull-up/downs

	// Setup PC10,11,12 for SPI3 (BMI088)
	GPIOC_MODER &= ~(0x3F << 20);   // Clear PC bits
	GPIOC_MODER |=  (0x2A << 20);   // Set PC bits to AF mode
	GPIOC_AFRH  &= ~(0xFFF << 8);   // Clear PC AF bits
	GPIOC_AFRH  |=  (0x666 << 8);   // Set PC bits to AF6     
	GPIOC_OSPEEDR &= ~(0x3F << 20); // Clear PC bits
	GPIOC_OSPEEDR |= (0x2A << 20);  // Set PC bits to high speed

	// Set PC6 & PC7 as input for BMI088 interrupts
        GPIOC_MODER &= ~(0xF << 12);    // Set PC6 & PC7 to input mode
        GPIOC_PUPDR &= ~(0xF << 12);    // Set NO pull-up/downs

	// Set PC8, PC9 as BMI088 accel, gyro chip select pins
	GPIOC_MODER &= ~(0xF << 16); // Clear bits
	GPIOC_MODER |=  (0x5 << 16); // Set PC8, PC8 as output pins
}

void syscfg_enable(void) {
	// PB10 - Pressure/Temp Interrupt
	SYSCFG_EXTICR3 &= ~(0xF << 8);
	SYSCFG_EXTICR3 |=  (0x1 << 8);
	// PC6 - Accel Interrupt
	SYSCFG_EXTICR2 &= ~(0xF << 8);
	SYSCFG_EXTICR2 |=  (0x2 << 8);
	// PC7 - Gyro Interrupt
	SYSCFG_EXTICR2 &= ~(0xF << 12);
        SYSCFG_EXTICR2 |=  (0x2 << 12);
}

void exti_enable(void) {
	// Setup PB10 EXTI 
	EXTI_IMR1  |=  (1 << 10);   // Unmask interrupt
	EXTI_RTSR1 |=  (1 << 10);   // Trigger on rising edge
	EXTI_FTSR1 &= ~(1 << 10);   // Disable falling edge
	// PC6 EXTI
	EXTI_IMR1  |=  (1 << 6); 
	EXTI_RTSR1 |=  (1 << 6);
	// PC7 EXTI
	EXTI_IMR1  |=  (1 << 7);
        EXTI_RTSR1 |=  (1 << 7);
}

void tim2_enable(void) {
	TIM2_SMCR &= ~(7 << 0);  // Disable slave mode controller 
	TIM2_PSC = 15; 		 // Define prescaler (CK_CNT = fCK_PSC/(PSC+1) = 16MHz/(15 + 1) = 1 MHz) 
	TIM2_ARR = 999; 	 // Define auto-reload register for 1000 ticks (1000/1Mhz = 1ms)
	TIM2_CNT = 0; 		 // Define counter to start at zero (upcounting mode)
	TIM2_EGR |= (1 << 0);  	 // UG bit synchronizes settings to timer operation
	TIM2_DIER |= (1 << 0);   // Enable update interrupt
	TIM2_CR1 |= (1 << 0); 	 // Enable TIM2 counter 
}
