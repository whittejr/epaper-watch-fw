/**
 * @file    ssd1681_interface.c
 * @author  Alessandro Davi
 * @brief   none
 * @version 0.1.0
 * @date    2025-11-03
 */

#include "ssd1681_interface.h"
#include "delay.h"
#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include "string.h"
#include <stdarg.h>
#include <stdio.h>

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t ssd1681_interface_spi_init(void) {

    /* RST | DC | BUSY Initialization  */
    ssd1681_cmd_data_gpio_init();

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
    return ssd1681_cmd_data_gpio_init();
}

uint8_t ssd1681_interface_spi_cmd_data_gpio_deinit(void) {
    return ssd1681_cmd_data_gpio_deinit();
}

uint8_t ssd1681_interface_spi_cmd_data_gpio_write(uint8_t value) {
    return ssd1681_cmd_data_gpio_write(value);
}

uint8_t ssd1681_interface_reset_gpio_init(void) {
    return ssd1681_rst_gpio_init();
}

uint8_t ssd1681_interface_reset_gpio_deinit(void) {
    return ssd1681_rst_gpio_deinit();
}

uint8_t ssd1681_interface_reset_gpio_write(uint8_t value) {
    return ssd1681_rst_gpio_write(value);
}

uint8_t ssd1681_interface_busy_gpio_init(void) {
    return ssd1681_busy_gpio_init();
}

uint8_t ssd1681_interface_busy_gpio_deinit(void) {
    return ssd1681_busy_gpio_deinit();
}

uint8_t ssd1681_interface_busy_gpio_read(uint8_t *value) {
    return busy_gpio_read(value);
}

void ssd1681_interface_debug_print(const char *const fmt, ...) {
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)uart_write((uint8_t *)str, len);
}
