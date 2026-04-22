#include "adxl362_interface.h"
#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include "board_config.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

extern SPI_HandleTypeDef hspi1;

uint8_t adxl362_interface_spi_init(void) {
    return 0;
}

uint8_t adxl362_interface_spi_deinit(void) {
    return 0;
}

uint8_t adxl362_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len) {
    uint8_t res;
    spi_lock();
    bsp_gpio_write(ACCEL_CS_PORT, ACCEL_CS_PIN, 0);
    res = spi_read_cmd(reg, buf, len);
    bsp_gpio_write(ACCEL_CS_PORT, ACCEL_CS_PIN, 1);
    spi_unlock();
    return res;
}

uint8_t adxl362_interface_spi_read_address16(uint16_t addr, uint8_t *buf, uint16_t len) {
    uint8_t res;
    spi_lock();
    bsp_gpio_write(ACCEL_CS_PORT, ACCEL_CS_PIN, 0);
    res = spi_read_cmd(addr, buf, len);
    bsp_gpio_write(ACCEL_CS_PORT, ACCEL_CS_PIN, 1);
    spi_unlock();
    return res;
}

uint8_t adxl362_interface_spi_write_address16(uint16_t addr, uint8_t *buf, uint16_t len) {
    uint8_t res;
    spi_lock();
    bsp_gpio_write(ACCEL_CS_PORT, ACCEL_CS_PIN, 0);
    res = spi_write_address16(addr, buf, len);
    bsp_gpio_write(ACCEL_CS_PORT, ACCEL_CS_PIN, 1);
    spi_unlock();
    return res;
}

void adxl362_interface_delay_ms(uint32_t ms) {
    bsp_delay_ms(ms);
}

void adxl362_interface_debug_print(const char *const fmt, ...) {
    // UART debug output if needed
}

void adxl362_interface_receive_callback(uint8_t type) {
    // Handle interrupts if used
}
