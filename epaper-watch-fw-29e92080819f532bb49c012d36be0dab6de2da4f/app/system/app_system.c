/**
* @file    app_system.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-04-07
*/

#include <stdio.h>
#include "board_config.h"
#include "app_system.h"
#include "app_display.h"
#include "clock.h"
#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include "ui_manager.h"
#include "button.h"

extern const AppScreen_t Screen_Watchface; // A tela inicial

extern volatile uint8_t bsp_btn_exti_flag;

static button_t button;

uint8_t app_system_init() {
    HAL_Init();
    clock_config();
    gpio_init();
    uart_init();
    spi_init();
    
    app_display_init();

    button_init(&button);
    UI_Manager_Init(&Screen_Watchface);

    return 0;
}

uint8_t app_system_loop() {
   uint32_t last_1sec_tick = HAL_GetTick();

    while (1) {
        uint32_t current_tick = HAL_GetTick();

        if ((current_tick - last_1sec_tick) >= 1000) {
            last_1sec_tick = current_tick;
            

            UI_Manager_ProcessEvent(EVENT_TICK_1SEC); 
        }

        bool exti_trigger = false;
        if (bsp_btn_exti_flag != 0) {
            exti_trigger = true;
            bsp_btn_exti_flag = 0; // Limpa a global na hora que detectou
        }

        bool pin_is_active = (bsp_gpio_read(BUTTON_PORT, BUTTON_PIN) == 0);
        
        button_event_t btn_evt = button_process_interrupt_driven(&button, pin_is_active, &exti_trigger);

        if (btn_evt == BUTTON_EVENT_SHORT_CLICK) 
            UI_Manager_ProcessEvent(EVENT_BTN_NEXT);
        
        else if (btn_evt == BUTTON_EVENT_LONG_CLICK)

            UI_Manager_ProcessEvent(EVENT_BTN_SELECT);
        
    }
    return 0;
}
