/**
* @file    app_accel.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-04-07
*/

#include "accel_hal.h"
#include <stdint.h>

uint8_t app_accel_init(void) {

    return accel_init();
}

uint8_t app_accel_deinit(void) {

    return app_accel_deinit();
}

uint8_t app_accel_read(float g[3]) {
    
    /* read data */
    accel_basic_read(g);
    
    return 0;
}
