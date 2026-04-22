/**
 * @file ls013b7dh03_interface.c
 * @brief  ls013b7dh03 interface implementation for STM32
 * @version 1.0.0
 * @author Alessandro Davi
 * @date 2026-04-21
 */

#include "ls013b7dh03_interface.h"
#include "spi.h"
#include "gpio.h"
#include "delay.h"

uint8_t ls013b7dh03_interface_spi_init(void) {
    // Peripheral init is handled by the BSP
    return 0;
}

uint8_t ls013b7dh03_interface_spi_deinit(void) {
    return spi_deinit();
}

uint8_t ls013b7dh03_interface_spi_write(uint8_t *buf, uint16_t len) {
    return spi_write(buf, len);
}

void ls013b7dh03_interface_cs_control(uint8_t state) {
    if (state) spi_lock();
    gpio_cs_control(state);
    if (!state) spi_unlock();
}

void ls013b7dh03_interface_delay_ms(uint32_t ms) {
    bsp_delay_ms(ms);
}
