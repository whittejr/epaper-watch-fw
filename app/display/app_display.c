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



#define BMP_DEFINE
#include "bmp.h"


extern uint32_t red_buffer[32];
extern uint32_t ir_buffer[32];

/*
* MIP
*/
uint8_t app_display_init(void) {
    display_init();
    
    // display_split();
    // display_update();
    // display_clear();  
    
    // display_test();
    return 0;
}
