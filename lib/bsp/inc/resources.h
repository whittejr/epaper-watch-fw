
/**
 * @file resources.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2025-12-31
 */

#ifndef RESOURCES_H
#define RESOURCES_H

#include "stm32wbxx_hal.h"

#define EPD_W 200
#define EPD_H 200
/*
 * EPD_SPI_CONFIG
 */
extern SPI_HandleTypeDef hspi1;
#define EPD_SPI_INSTANCE SPI1
#define EPD_MOSI_PORT    GPIOA
#define EPD_MOSI_PIN     GPIO_PIN_7
#define EPD_SCK_PORT     GPIOA
#define EPD_SCK_PIN      GPIO_PIN_1
/*
 * EPD_GPIO_CONFIG
 */
#define EPD_RST_PORT GPIOB
#define EPD_RST_PIN  GPIO_PIN_6
#define EPD_DC_PORT  GPIOB
#define EPD_DC_PIN   GPIO_PIN_7
#define EPD_CS_PORT  GPIOA
#define EPD_CS_PIN   GPIO_PIN_4
#define EPD_BSY_PORT GPIOB
#define EPD_BSY_PIN  GPIO_PIN_9

/*
*   MIP_CONFIG
*/
// MIP GPIO CONFIG
#define MIP_SPI_INSTANCE      SPI1
#define MIP_MOSI_PORT         GPIOA
#define MIP_MOSI_PIN          GPIO_PIN_7
#define MIP_SCK_PORT          GPIOA
#define MIP_SCK_PIN           GPIO_PIN_1
#define MIP_CS_PORT           GPIOA
#define MIP_CS_PIN            GPIO_PIN_4
#define MIP_DISP_PORT         GPIOA
#define MIP_DISP_PIN          GPIO_PIN_9
#define MIP_EXTCOMIN_PORT     GPIOA
#define MIP_EXTCOMIN_PIN      GPIO_PIN_0 //PWM

/*
 * I2C_CONFIG
 */
#define I2C_SLAVE_ADDR 0x57
#define I2C_INSTANCE   I2C1
#define I2C_SCL_PORT   GPIOA
#define I2C_SCL_PIN    GPIO_PIN_9
#define I2C_SDA_PORT   GPIOA
#define I2C_SDA_PIN    GPIO_PIN_10

extern uint8_t framebuffer[(EPD_W / 8) * EPD_H];

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} bsp_pin_t;

/*
 * EPD_CFG
 */
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} epd_pin_t;

typedef struct {
    epd_pin_t rst;
    epd_pin_t dc;
    epd_pin_t cs;
    epd_pin_t busy;
} epd_pins_t;

typedef struct {
    epd_pins_t pins;

    uint16_t width;
    uint16_t height;
    uint8_t *framebuffer;

    // Lógica de pintura
    uint16_t rotate;
    uint16_t color;
    uint16_t width_memory;
    uint16_t height_memory;
    uint16_t width_byte;
    uint16_t height_byte;
} epd_user_config_t;

/*
 * MAX30102_CONFIG
 */
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} max_pin_t;

typedef struct {
    max_pin_t irq;
} max_pins_t;

/*
 * SPI_CFG
 */
typedef struct {
    bsp_pin_t mosi;
    bsp_pin_t sck;
    SPI_HandleTypeDef spi_handle;
} spi_config_t;

/*
 * I2C_CFG
 */
typedef struct {
    bsp_pin_t sda;
    bsp_pin_t scl;
    I2C_HandleTypeDef i2c_handle;
} i2c_config_t;

extern epd_user_config_t epd;
extern i2c_config_t i2c_config;
extern spi_config_t spi_config;

#endif // RESOURCES_H
