/**
 * @file    app_display.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-12-26
 */

#include <stdio.h>
#include "app_display.h"
#include "display_hal.h"
#include "stm32wbxx_hal.h"

uint8_t app_display_init(void) {
    display_init();

    return 0;
}

uint8_t app_display_clear(void) {

    return 0;
}

uint8_t app_display_draw_text(void) {
    display_write();

    return 0;
}

uint8_t app_display_update(void) {
    display_update();
    
    return 0;
}


