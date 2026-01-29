/**
* @file    ls013b7dh03_interface.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-01-27
*/

#include <stdint.h>
#include "ls013b7dh03_interface.h"
#include "spi.h"
#include "gpio.h"

uint8_t ls013b7dh03_interface_spi_init(void) {
    return mip_display_spi_init();
}

uint8_t ls013b7dh03_interface_spi_deinit(void) {
    return mip_display_spi_deinit();
}

uint8_t ls013b7dh03_interface_spi_write_cmd(uint8_t *buf, uint16_t len) {
    return mip_display_spi_write(buf, len);     
}

uint8_t ls013b7dh03_interface_spi_write_refresh(uint8_t *buf, uint16_t len) {
    return mip_display_spi_write_refresh(buf, len);
}

void ls013b7dh03_interface_cs_control(uint8_t state) {
    ls013b7dh03_gpio_cs_control(state);
}


uint8_t ls013b7dh03_interface_gpio_init(void) {
    return ls013b7dh03_gpio_init();
}

uint8_t ls013b7dh03_interface_gpio_deinit(void) {
    return 0;
}

void ls013b7dh03_interface_delay_ms(uint32_t ms) {

}
