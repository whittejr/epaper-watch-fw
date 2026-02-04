/**
 * @file ls013b7dh03_interface.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-01-27
 */

#ifndef LS013B7DH03_INTERFACE_H
#define LS013B7DH03_INTERFACE_H

#include <stdint.h>

uint8_t ls013b7dh03_interface_spi_init(void);

uint8_t ls013b7dh03_interface_spi_deinit(void);

uint8_t ls013b7dh03_interface_spi_write(uint8_t *buf, uint16_t len);

uint8_t ls013b7dh03_interface_spi_write_refresh(uint8_t *buf, uint16_t len);

void ls013b7dh03_interface_cs_control(uint8_t state);

uint8_t ls013b7dh03_interface_gpio_init(void);

uint8_t ls013b7dh03_interface_gpio_deinit(void);

void ls013b7dh03_interface_delay_ms(uint32_t ms);

#endif // LS013B7DH03_INTERFACE_H
