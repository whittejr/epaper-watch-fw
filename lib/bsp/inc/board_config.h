/**
 * @file board_config.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-02-26
 */

#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm32wbxx_hal.h"

/* DISPLAY DIMENSIONS */
#define EPD_W 200
#define EPD_H 200

/* SPI BUS CONFIGURATION (SPI1) */
#define EPD_SPI_INSTANCE SPI1
#define EPD_MOSI_PORT    GPIOA
#define EPD_MOSI_PIN     GPIO_PIN_7
#define EPD_SCK_PORT     GPIOA
#define EPD_SCK_PIN      GPIO_PIN_1

/* E-PAPER CONTROL PINS */
#define EPD_BSY_PORT     GPIOA
#define EPD_BSY_PIN      GPIO_PIN_5
// #define EPD_RST_PORT     GPIOB
// #define EPD_RST_PIN      GPIO_PIN_6
// #define EPD_DC_PORT      GPIOB
// #define EPD_DC_PIN       GPIO_PIN_7
// #define EPD_CS_PORT      GPIOA
// #define EPD_CS_PIN       GPIO_PIN_4
#define EPD_RST_PORT     GPIOA
#define EPD_RST_PIN      GPIO_PIN_6
#define EPD_DC_PORT      GPIOA
#define EPD_DC_PIN       GPIO_PIN_8
#define EPD_CS_PORT      GPIOA
#define EPD_CS_PIN       GPIO_PIN_9


/* MIP (MEMORY IN PIXEL) LCD PINS */
#define MIP_SPI_INSTANCE  SPI1
#define MIP_MOSI_PORT     GPIOA
#define MIP_MOSI_PIN      GPIO_PIN_7
#define MIP_SCK_PORT      GPIOA
#define MIP_SCK_PIN       GPIO_PIN_1
#define MIP_CS_PORT       GPIOA
#define MIP_CS_PIN        GPIO_PIN_4
#define MIP_DISP_PORT     GPIOA
#define MIP_DISP_PIN      GPIO_PIN_9
#define MIP_EXTCOMIN_PORT GPIOA
#define MIP_EXTCOMIN_PIN  GPIO_PIN_0 // PWM

/* I2C BUS CONFIGURATION (I2C1) */
#define I2C_SLAVE_ADDR    0x57
#define I2C_INSTANCE      I2C1
#define I2C_SCL_PORT      GPIOA
#define I2C_SCL_PIN       GPIO_PIN_9
#define I2C_SDA_PORT      GPIOA
#define I2C_SDA_PIN       GPIO_PIN_10

#endif // BOARD_CONFIG_H
