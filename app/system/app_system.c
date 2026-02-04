/**
 * @file    app_system.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2026-01-07
 */

#include "app_system.h"
#include "app_display.h"
#include "app_oximeter.h"
#include "bmp.h"
#include "clock.h"
#include "stm32wbxx_hal.h"
#include <stdint.h>
#include "ls013b7dh03_interface.h"

extern ssd1681_handle_t gs_handle;
extern uint8_t gs_lut[];

uint8_t app_system_init() {
    HAL_Init();
    if (clock_init() != 0)
        return 1;
    if (app_display_init() != 0)
        return 1;
    // if (app_oximeter_init() != 0)
    //     return 1;

    return 0;
}

uint8_t app_system_loop() {
    
    // oximeter_proccess();

    if (app_display_menu_test() != 0)
        return 1;
    
    return 0;
}

// GPIO_InitTypeDef led;
// led.Mode = GPIO_MODE_OUTPUT_PP;
// led.Pin = GPIO_PIN_4;
// led.Speed = GPIO_SPEED_FREQ_HIGH;
// HAL_GPIO_Init(GPIOE, &led);
