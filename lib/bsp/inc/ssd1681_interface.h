#ifndef DRIVER_SSD1681_INTERFACE_H
#define DRIVER_SSD1681_INTERFACE_H

#include <stdint.h>

uint8_t ssd1681_interface_spi_init(void);

uint8_t ssd1681_interface_spi_deinit(void);

uint8_t ssd1681_interface_spi_write_cmd(uint8_t *buf, uint16_t len);

uint8_t ssd1681_interface_spi_read_cmd(uint8_t *buf, uint16_t len);

uint8_t ssd1681_interface_spi_cmd_data_gpio_init(void);

uint8_t ssd1681_interface_spi_cmd_data_gpio_deinit(void);

uint8_t ssd1681_interface_spi_cmd_data_gpio_write(uint8_t value);

uint8_t ssd1681_interface_reset_gpio_init(void);

uint8_t ssd1681_interface_reset_gpio_deinit(void);

uint8_t ssd1681_interface_reset_gpio_write(uint8_t value);

uint8_t ssd1681_interface_busy_gpio_init(void);

uint8_t ssd1681_interface_busy_gpio_deinit(void);

uint8_t ssd1681_interface_busy_gpio_read(uint8_t *value);

void ssd1681_interface_delay_ms(uint32_t ms);

void ssd1681_interface_debug_print(const char *const fmt, ...);

#endif // DRIVER_SSD1681_INTERFACE_H