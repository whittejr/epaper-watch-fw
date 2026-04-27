/**
 * @file    ssd1681_interface.c
 * @author  Alessandro Davi
 * @brief   none
 * @version 0.1.0
 * @date    2025-11-03
 */

#include "ssd1681_interface.h"
#include "board_config.h"
#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

uint8_t ssd1681_interface_spi_init(void) {
    return 0;
}

uint8_t ssd1681_interface_spi_deinit(void) {
    return 0;
}

uint8_t ssd1681_interface_spi_write_cmd(uint8_t *buf, uint16_t len) {
    uint8_t res;
    
    bsp_gpio_write(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_RESET);
    res = spi_write(buf, len);
    bsp_gpio_write(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_SET);

    return res;
}

uint8_t ssd1681_interface_spi_read_cmd(uint8_t *buf, uint16_t len) {
    // uint8_t res;
    // res = spi_read(buf, len);
    // return res;

    return 0;
}

void ssd1681_interface_delay_ms(uint32_t ms) {
    bsp_delay_ms(ms);
}

uint8_t ssd1681_interface_spi_cmd_data_gpio_init(void) {
    return 0; 
}

uint8_t ssd1681_interface_spi_cmd_data_gpio_deinit(void) {
    return 0;
}

uint8_t ssd1681_interface_spi_cmd_data_gpio_write(uint8_t value) {
    bsp_gpio_write(EPD_DC_PORT, EPD_DC_PIN, value);
    return 0;
}

uint8_t ssd1681_interface_reset_gpio_init(void) {
    return 0;
}

uint8_t ssd1681_interface_reset_gpio_deinit(void) {
    return 0;
}

uint8_t ssd1681_interface_reset_gpio_write(uint8_t value) {
    bsp_gpio_write(EPD_RST_PORT, EPD_RST_PIN, value);
    return 0;
}

uint8_t ssd1681_interface_busy_gpio_init(void) {
    return 0;
}

uint8_t ssd1681_interface_busy_gpio_deinit(void) {
    return 0;
}

uint8_t ssd1681_interface_busy_gpio_read(uint8_t *value) {
    if (value == NULL) {
        return 1;
    }
    *value = bsp_gpio_read(EPD_BSY_PORT, EPD_BSY_PIN);
    return 0;
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
