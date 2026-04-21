#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm32wbxx_hal.h"

/* DISPLAY DIMENSIONS */
#define DISP_W 128
#define DISP_H 128

/* USER DISPLAY PINS (LS013B7DH03) */
#define DISP_CS_PORT      GPIOB
#define DISP_CS_PIN       GPIO_PIN_0
#define DISP_ON_PORT      GPIOB
#define DISP_ON_PIN       GPIO_PIN_1
#define DISP_EXTCOM_PORT  GPIOB
#define DISP_EXTCOM_PIN   GPIO_PIN_2

/* Compatibility aliases for display HAL */
#define MIP_CS_PORT       DISP_CS_PORT
#define MIP_CS_PIN        DISP_CS_PIN
#define MIP_DISP_PORT     DISP_ON_PORT
#define MIP_DISP_PIN      DISP_ON_PIN

/* SPI1 PINS (Standard for WB55) */
#define SCK_PORT     GPIOA
#define SCK_PIN      GPIO_PIN_5
#define MOSI_PORT    GPIOA
#define MOSI_PIN     GPIO_PIN_7

/* I2C BUS (Restored for compilation) */
#define I2C_INSTANCE      I2C1
#define I2C_SDA_PORT      GPIOA
#define I2C_SDA_PIN       GPIO_PIN_10
#define I2C_SCL_PORT      GPIOA
#define I2C_SCL_PIN       GPIO_PIN_9

/* BUTTONS & ACCEL (Restored for compilation) */
#define BUTTON_PORT       GPIOB
#define BUTTON_PIN        GPIO_PIN_9
#define ACCEL_CS_PORT     GPIOA
#define ACCEL_CS_PIN      GPIO_PIN_1

#endif // BOARD_CONFIG_H
