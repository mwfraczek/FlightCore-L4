#ifndef STM32L476_H
#define STM32L476_H

#include <stdint.h>

#define RCC_AHB2ENR   (*(volatile uint32_t*)0x4002104C) // peripheral clock enable (GPIOA/B)
#define RCC_APB1ENR1  (*(volatile uint32_t*)0x40021058) // peripheral clock enable (I2C1)
#define RCC_CR        (*(volatile uint32_t*)0x40021000) // clock control 
#define RCC_CCIPR     (*(volatile uint32_t*)0x40021088) // peripheral independent clock source select
#define RCC_APB1RSTR1 (*(volatile uint32_t*)0x40021038) // clock reset 
#define RCC_CFGR      (*(volatile uint32_t*)0x40021008) // clock configuration
#define RCC_APB2ENR   (*(volatile uint32_t*)0x40021060) 

#define TIM2_CR1      (*(volatile uint32_t*)0x40000000) // TIM2 control register 
#define TIM2_SMCR     (*(volatile uint32_t*)0x40000008) // TIM2 slave mode control  
#define TIM2_DIER     (*(volatile uint32_t*)0x4000000C) // TIM2 DMA/Interrupt enable
#define TIM2_SR       (*(volatile uint32_t*)0x40000010) // TIM2 status register (flags)
#define TIM2_EGR      (*(volatile uint32_t*)0x40000014) // TIM2 event generation
#define TIM2_CNT      (*(volatile uint32_t*)0x40000024) // TIM2 slave mode control
#define TIM2_PSC      (*(volatile uint32_t*)0x40000028) // TIM2 prescaler
#define TIM2_ARR      (*(volatile uint32_t*)0x4000002C) // TIM2 auto-reload 

#define GPIOA_MODER   (*(volatile uint32_t*)0x48000000) // GPIOA mode
#define GPIOA_OTYPER  (*(volatile uint32_t*)0x48000004) // GPIOA Type - Open drain vs push/pull 
#define GPIOA_OSPEEDR (*(volatile uint32_t*)0x48000008) // GPIOA output speed 
#define GPIOA_ODR     (*(volatile uint32_t*)0x48000014) // GPIOA output data
#define GPIOA_AFRL    (*(volatile uint32_t*)0x48000020) // GPIOA alternate function LO
#define GPIOA_AFRH    (*(volatile uint32_t*)0x48000024) // GPIOA alt function register
#define GPIOA_PUPDR   (*(volatile uint32_t*)0x4800000C) // GPIOA pull up/down

#define GPIOB_MODER   (*(volatile uint32_t*)0x48000400) // GPIOB base address/mode
#define GPIOB_OTYPER  (*(volatile uint32_t*)0x48000404) // Type - Open drain vs push/pull
#define GPIOB_OSPEEDR (*(volatile uint32_t*)0x48000408) // GPIOB output speed register
#define GPIOB_IDR     (*(volatile uint32_t*)0x48000410) // GPIOB input data register
#define GPIOB_ODR     (*(volatile uint32_t*)0x48000414) // GPIOB output data register
#define GPIOB_BSRR    (*(volatile uint32_t*)0x48000418) // GPIOB bit set/reset register
#define GPIOB_PUPDR   (*(volatile uint32_t*)0x4800040C) // GPIOB pull up/down register
#define GPIOB_AFRH    (*(volatile uint32_t*)0x48000424) // GPIOB alt function register

#define GPIOC_MODER   (*(volatile uint32_t*)0x48000800) // GPIOC base address/mode
#define GPIOC_OTYPER  (*(volatile uint32_t*)0x48000804) // Type - Open drain vs push/pull
#define GPIOC_OSPEEDR (*(volatile uint32_t*)0x48000808) // GPIOC output speed register
#define GPIOC_IDR     (*(volatile uint32_t*)0x48000810) // GPIOC input data register
#define GPIOC_ODR     (*(volatile uint32_t*)0x48000814) // GPIOC output data register
#define GPIOC_BSRR    (*(volatile uint32_t*)0x48000818) // GPIOC bit set/reset register
#define GPIOC_PUPDR   (*(volatile uint32_t*)0x4800080C) // GPIOC pull up/down register
#define GPIOC_AFRH    (*(volatile uint32_t*)0x48000824) // GPIOC alt function register

#define I2C1_CR1      (*(volatile uint32_t*)0x40005400) // I2C1 configuration register
#define I2C1_CR2      (*(volatile uint32_t*)0x40005404) // I2C1 transaction/control register
#define I2C1_TIMINGR  (*(volatile uint32_t*)0x40005410) // I2C1 timing register
#define I2C1_ISR      (*(volatile uint32_t*)0x40005418) // I2C1 status register
#define I2C1_ICR      (*(volatile uint32_t*)0x4000541C) // I2C1 flag clearing register
#define I2C1_RXDR     (*(volatile uint32_t*)0x40005424) // I2C1 receive data register
#define I2C1_TXDR     (*(volatile uint32_t*)0x40005428) // I2C1 transmit data register

#define USART2_CR1    (*(volatile uint32_t*)0x40004400) // USART2 config register 
#define USART2_CR2    (*(volatile uint32_t*)0x40004404) // USART2 config register
#define USART2_BRR    (*(volatile uint32_t*)0x4000440C) // USART2 baud rate register
#define USART2_ISR    (*(volatile uint32_t*)0x4000441C) // USART2 status register
#define USART2_RDR    (*(volatile uint32_t*)0x40004424) // USART2 receive data register
#define USART2_TDR    (*(volatile uint32_t*)0x40004428) // USART2 transmit data register

#define USART3_CR1    (*(volatile uint32_t*)0x40004800) // USART3 config register
#define USART3_CR2    (*(volatile uint32_t*)0x40004804) // USART3 config register
#define USART3_BRR    (*(volatile uint32_t*)0x4000480C) // USART3 baud rate register
#define USART3_RQR    (*(volatile uint32_t*)0x40004818) // USART3 request register
#define USART3_ISR    (*(volatile uint32_t*)0x4000481C) // USART3 status register
#define USART3_ICR    (*(volatile uint32_t*)0x40004820) // USART3 flag clear register
#define USART3_RDR    (*(volatile uint32_t*)0x40004824) // USART3 receive data register
#define USART3_TDR    (*(volatile uint32_t*)0x40004828) // USART3 transmit data register

#define SYSCFG_EXTICR2 (*(volatile uint32_t*)0x4001000C) // SYSCFG external interrupt config [4-7] 
#define SYSCFG_EXTICR3 (*(volatile uint32_t*)0x40010010) // SYSCFG external interrupt config [8-11]

#define EXTI_IMR1     (*(volatile uint32_t*)0x40010400) // EXTI interrupt mask register 
#define EXTI_RTSR1    (*(volatile uint32_t*)0x40010408) // EXTI rising trigger selection register
#define EXTI_FTSR1    (*(volatile uint32_t*)0x4001040C) // EXTI falling trigger selection register 
#define EXTI_PR1      (*(volatile uint32_t*)0x40010414) // EXTI pending interrupt flag register 

#define SPI3_CR1      (*(volatile uint32_t*)0x40003C00) // SPI3 control register 1
#define SPI3_CR2      (*(volatile uint32_t*)0x40003C04) // SPI3 control register 2
#define SPI3_SR       (*(volatile uint32_t*)0x40003C08) // SPI3 status register
#define SPI3_DR       (*(volatile uint8_t*)0x40003C0C)  // SPI3 data register (8-bit access to match FRXTH/8-bit frame)

#endif
