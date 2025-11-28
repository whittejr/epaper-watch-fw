/**
 * @file    stm32wb55_driver_ssd1681_interface.c
 * @author  Alessandro Davi
 * @brief   none
 * @version 0.1.0
 * @date    2025-11-03
 */

#include "stm32wb55_driver_ssd1681_interface.h"

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t ssd1681_interface_spi_init(void) {

    /* RST | DC | BUSY Initialization  */
    epd_cmd_data_gpio_init();

    if (spi_init() != 0)
        return 1;

    return 0;
}

uint8_t ssd1681_interface_spi_deinit(void) {
    return spi_deinit();
}

uint8_t ssd1681_interface_spi_write_cmd(uint8_t *buf, uint16_t len) {
    return spi_write_cmd(buf, len);
}

uint8_t ssd1681_interface_spi_read_cmd(uint8_t *buf, uint16_t len) {
    return spi_read_cmd(buf, len);
}

void ssd1681_interface_delay_ms(uint32_t ms) {
    delay_ms(ms);
}

uint8_t ssd1681_interface_spi_cmd_data_gpio_init(void) {
    return epd_cmd_data_gpio_init();
}

uint8_t ssd1681_interface_spi_cmd_data_gpio_deinit(void) {
    return epd_cmd_data_gpio_deinit();
}

uint8_t ssd1681_interface_spi_cmd_data_gpio_write(uint8_t value) {
    return epd_cmd_data_gpio_write(value);
}

uint8_t ssd1681_interface_reset_gpio_init(void) {
    return epd_rst_gpio_init();
}

uint8_t ssd1681_interface_reset_gpio_deinit(void) {
    return epd_rst_gpio_deinit();
}

uint8_t ssd1681_interface_reset_gpio_write(uint8_t value) {
    return epd_rst_gpio_write(value);
}

uint8_t ssd1681_interface_busy_gpio_init(void) {
    return epd_busy_gpio_init();
}

uint8_t ssd1681_interface_busy_gpio_deinit(void) {
    return epd_busy_gpio_deinit();
}

uint8_t ssd1681_interface_busy_gpio_read(uint8_t *value) {
    return busy_gpio_read(value);
}
