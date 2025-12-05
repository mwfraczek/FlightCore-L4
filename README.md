# Overview:
This project contains embedded C firmware for the **STM32** Nucleo-L476RG, interfacing with a **BMP390 Pressure and Temperature sensor**, a **BMI088 IMU**, and a GT-U7 **GPS module**. All peripherals and devices are configured in a bare-metal environment directly from relevant Reference Manuals and Datasheets. The goal is to build a strong foundation in embedded systems while expanding the project's functionality for a model rocket flight controller application.


## Features: 🔧
- Bare-metal STM32L4 firmware. 
- Interrupt-driven data handling for non-blocking, real-time operation. 
- I2C driver for BMP390 pressure and temperature sensor.
- SPI driver for BMI088 accelerometer and gyroscope sensor.
- UART drivers for GPS input (USART3) and serial output (USART2).
- Floating-point conversion of BMP390 raw measurement data.
- Ring-buffer GPS parser for continuous NMEA sentence extraction.
- Python logging script for UART output capture and CSV generation. 


## Demonstration: 🎥
**Hardware & Data Stream Demo:** 
- https://youtu.be/9T74qPDPzVY


## Future Work: 🚧 
- Pull IMU data and apply sensor-fusion algorithm to produce practical orientation vector. 
- Parse NMEA sentences to extract time, longitude, latitude, and altitude.


## Hardware Requirements: 🧰
- **MCU:** Nucleo-L476RG (STM32L476RG).
- **Device:** BMP390 Breakout Board - GT-U7 GPS Module - BMI088 Shuttle Board
- **Configuration:** -To Be Updated-


## Build Instructions: 🛠️
- **Install** ARM GCC toolchain: `sudo apt install gcc-arm-none-eabi`
- **Clone** repo: `git clone https://github.com/mwfraczek/FlightCore-L4.git`
- **Build**: `make` 
- **Flash**: `st-flash --reset write bin/main.bin 0x08000000` or use STM32CubeIDE


## Directory Structure: 📂
.
├── bin/
├── build/
├── controls/
│   ├── chute.c
│   └── tvc.c
├── drivers/
│   ├── bmi088.c
│   ├── bmp390.c
│   ├── gps.c
│   ├── i2c.c
│   ├── peripherals.c
│   ├── spi.c
│   ├── usart.c
│   └── utils.c
├── inc/
│   ├── bmi088.h
│   ├── bmp390.h
│   ├── cmsis
│   │   ├── core/
│   │   └── device/
│   ├── gps.h
│   ├── i2c.h
│   ├── peripherals.h
│   ├── spi.h
│   ├── stm32l476.h
│   ├── usart.h
│   └── utils.h
├── linker.ld
├── log.py
├── logs/
├── Makefile
├── README.md
└── src/
    ├── main.c
    ├── startup.s
    └── system.c


## Contributions: 👤
- **Author:** Michael Fraczek 
- **Contact** mwfraczek@icloud.com
- Open to collaboration & feedback!
