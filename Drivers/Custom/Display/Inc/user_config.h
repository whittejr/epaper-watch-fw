/*
 * user_cfg.h
 *
 *  Created on: Jul 7, 2025
 *      Author: alessandro.davi
 */
#ifndef INC_USER_CONFIG_H_
#define INC_USER_CONFIG_H_

#include "stm32wbxx_hal.h"

#define EPD_W 200
#define EPD_H 200
/*
* EPD_SPI_CONFIG
*/
extern SPI_HandleTypeDef hspi1;
#define EPD_SPI_INSTANCE SPI1 
#define EPD_SDA_PORT    GPIOA
#define EPD_SDA_PIN     GPIO_PIN_7
#define EPD_SCL_PORT    GPIOA
#define EPD_SCL_PIN     GPIO_PIN_1
/*
* EPD_GPIO_CONFIG
*/
#define EPD_RST_PORT    GPIOB
#define EPD_RST_PIN     GPIO_PIN_6
#define EPD_DC_PORT     GPIOB
#define EPD_DC_PIN      GPIO_PIN_7
#define EPD_CS_PORT     GPIOA
#define EPD_CS_PIN      GPIO_PIN_4
#define EPD_BSY_PORT    GPIOB 
#define EPD_BSY_PIN     GPIO_PIN_9

extern uint8_t framebuffer[(EPD_W / 8) * EPD_H];


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
* SPI_CFG
*/
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} spi_pin_t;

typedef struct {
    spi_pin_t sda; 
    spi_pin_t scl; 
} spi_pins_t;

typedef struct {
    spi_pins_t pins;
    SPI_HandleTypeDef spi_handle;
} spi_config_t;


extern epd_user_config_t epd;
extern spi_config_t spi_display;

#endif /* INC_USER_CONFIG_H_ */
