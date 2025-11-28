/**
 * @file stm32wb55_driver_ssd1681_interface.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2025-11-06
 */

#ifndef STM32WB55_DRIVER_SSD1681_INTERFACE_H
#define STM32WB55_DRIVER_SSD1681_INTERFACE_H

#include <stdint.h>
#include "driver_ssd1681_interface.h"
#include "delay.h"
#include "gpio.h"
#include "spi.h"
#include "user_config.h"
#include <stm32wbxx_hal.h>

uint8_t ssd1681_interface_spi_init(void);

uint8_t ssd1681_interface_spi_deinit(void);

uint8_t ssd1681_interface_spi_write_cmd(uint8_t *buf, uint16_t len);

uint8_t ssd1681_interface_spi_read_cmd(uint8_t *buf, uint16_t len);

void ssd1681_interface_delay_ms(uint32_t ms);

uint8_t ssd1681_interface_spi_cmd_data_gpio_init(void);

uint8_t ssd1681_interface_spi_cmd_data_gpio_deinit(void);

uint8_t ssd1681_interface_spi_cmd_data_gpio_write(uint8_t value);

uint8_t ssd1681_interface_reset_gpio_init(void);

uint8_t ssd1681_interface_reset_gpio_deinit(void);

uint8_t ssd1681_interface_reset_gpio_write(uint8_t value);

uint8_t ssd1681_interface_busy_gpio_init(void);

uint8_t ssd1681_interface_busy_gpio_deinit(void);

uint8_t ssd1681_interface_busy_gpio_read(uint8_t *value);

#endif // STM32WB55_DRIVER_SSD1681_INTERFACE_H