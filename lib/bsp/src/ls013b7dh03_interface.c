/**
* @file    ls013b7dh03_interface.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-01-27
*/

#include "ls013b7dh03_interface.h"
#include "board_config.h"
#include "spi.h"
#include "gpio.h"

uint8_t ls013b7dh03_interface_spi_init(void) {
    return 0;
}

uint8_t ls013b7dh03_interface_spi_deinit(void) {
    return 0;
}

uint8_t ls013b7dh03_interface_spi_write(uint8_t *buf, uint16_t len) {
    return spi_write(buf, len);     
}

uint8_t ls013b7dh03_interface_spi_write_refresh(uint8_t *buf, uint16_t len) {
    return spi_write(buf, len);
}

void ls013b7dh03_interface_cs_control(uint8_t state) {
    bsp_gpio_write(MIP_CS_PORT, MIP_CS_PIN, state);
}

uint8_t ls013b7dh03_interface_gpio_init(void) {
    return 0;
}

uint8_t ls013b7dh03_interface_gpio_deinit(void) {
    return 0;
}

void ls013b7dh03_interface_delay_ms(uint32_t ms) {
    bsp_delay_ms(ms);
}