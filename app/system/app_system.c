/**
 * @file    app_system.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2026-01-07
 */

#include "stdint.h"
#include "stm32wbxx_hal.h"
#include "app_system.h"
#include "app_display.h"
#include "app_oximeter.h"
#include "clock.h"
#include "gpio.h"
#include "i2c.h"
#include "uart.h"

uint8_t app_system_init() {
    HAL_Init();
    if (clock_config() != 0)        return 1;
    if (gpio_init() != 0)           return 1;
    if (i2c_init() != 0)            return 1;
    if (uart_init() != 0)           return 1;
    
    // if (app_display_init() != 0)    return 1;
    return 0;
}

uint8_t app_system_loop() {
    // oximeter_proccess();

    return 0;
}
