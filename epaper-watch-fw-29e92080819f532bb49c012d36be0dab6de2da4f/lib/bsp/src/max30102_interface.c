/**
* @file    max30102_interface.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-02-23
*/

#include "max30102_interface.h"
#include "board_config.h"
#include "i2c.h"
#include "gpio.h"
#include "uart.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

extern volatile uint8_t g_oximeter_data_ready; /* oximeter_hal.c variable*/

uint8_t max30102_interface_i2c_init(void) {
    // O I2C é inicializado no BSP (app_system_init -> i2c_init)
    return 0;
}

uint8_t max30102_interface_i2c_deinit(void) {
    // O driver não desliga o barramento I2C compartilhado
    return 0;
}

uint8_t max30102_interface_i2c_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len) {
    // Repassa a chamada para o BSP genérico do I2C
    return i2c_read(addr, reg, buf, len);
}

uint8_t max30102_interface_i2c_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len) {
    // Repassa a chamada para o BSP genérico do I2C
    return i2c_write(addr, reg, buf, len);
}

void max30102_interface_delay_ms(uint32_t ms) {
    bsp_delay_ms(ms);
}

void max30102_interface_debug_print(const char *const fmt, ...) {
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

void max30102_interface_receive_callback(uint8_t type) {
    switch (type) {
    case MAX30102_INTERRUPT_STATUS_PPG_RDY:        
        break;
    case MAX30102_INTERRUPT_STATUS_FIFO_FULL:
        g_oximeter_data_ready = 1;
        break;
    default:
        break;
    }
}
