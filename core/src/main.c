/**
 * @file    main.c
 * @brief
 * @version 1.0.0
 * @author  Alessandro Davi
 * @date    2025-12-31
 */

#include "app_oximeter.h"
#include "app_system.h"
#include "ls013b7dh03.h"
#include "display_hal.h"
#include "stm32wbxx_hal.h"
#include <stdio.h>

void errorHandler(void);

int main(void) {
    if (app_system_init() != 0) errorHandler();

    // DESCOMENTE O WHILE E COLOQUE A LÓGICA DENTRO DELE
    while (1) {
        
        // 1. Verifica se o Timer pediu troca de VCOM
        // if (g_vcom_toggle_request) {
        //     ls013b7dh03_vcom_control(&sharp_handle); 
        //     g_vcom_toggle_request = 0; // Baixa a bandeira
        // }

        // 2. Roda o resto do sistema
        // app_system_loop();
    }
}

void errorHandler(void) {
    while (1);
}

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//     if (htim->Instance == TIM16)
//     {
//         g_vcom_toggle_request = 1;
//     }
// }
