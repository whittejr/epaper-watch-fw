/**
 * @file gpio.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2025-11-05
 */

#ifndef GPIO_H
#define GPIO_H
#include "user_config.h"
#include "ssd1681.h"
#include <stdint.h>

uint8_t epd_cmd_data_gpio_init(void);
uint8_t epd_cmd_data_gpio_write(uint8_t value);
uint8_t busy_gpio_read(uint8_t *value);
uint8_t epd_cmd_data_gpio_deinit(void);
uint8_t epd_rst_gpio_init(void);
uint8_t epd_rst_gpio_deinit(void);
uint8_t epd_rst_gpio_write(uint8_t value);
uint8_t epd_busy_gpio_init(void);
uint8_t epd_busy_gpio_deinit(void);

#endif // GPIO_H