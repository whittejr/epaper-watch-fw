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
#include "peripherals.h"
#include "resources.h"
#include "ls013b7dh03_interface.h"
#include "stm32wbxx_hal_gpio.h"

extern ssd1681_handle_t gs_handle;
extern uint8_t gs_lut[];

uint8_t app_system_init() {
    HAL_Init();
    if (clock_config() != 0)        return 1;
    if (hw_init() != 0)             return 1;
    if (app_display_init() != 0)    return 1;
    // if (app_oximeter_init() != 0)
    //     return 1;

    HAL_GPIO_WritePin(MIP_DISP_PORT, MIP_DISP_PIN, GPIO_PIN_RESET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(MIP_DISP_PORT, MIP_DISP_PIN, GPIO_PIN_SET);
    if (app_display_init() != 0)    return 1;
    return 0;
}

uint8_t app_system_loop() {
    
    // oximeter_proccess();

    if (app_display_menu_test() != 0)
        return 1;
    
    return 0;
}
