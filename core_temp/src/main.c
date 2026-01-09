/**
 * @file    main.c
 * @brief
 * @version 1.0.0
 * @author  Alessandro Davi
 * @date    2025-12-31
 */

#include "app_system.h"
#include "stm32wbxx_hal.h"
#include <stdio.h>

void errorHandler(void);

int main(void) {
    if (app_system_init() != 0)
        errorHandler();

    app_system_loop();

    while (1) {
    }
}

void errorHandler(void) {
    while (1)
        ;
}
