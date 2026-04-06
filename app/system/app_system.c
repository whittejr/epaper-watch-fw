#include "app_system.h"
#include "app_display.h"
#include "clock.h"
#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include "ui_manager.h"
#include <stdio.h>

extern const AppScreen_t Screen_Watchface; // A tela inicial

uint8_t app_system_init() {
    HAL_Init();
    clock_config();
    gpio_init();
    uart_init();
    spi_init();
    
    app_display_init();
    UI_Manager_Init(&Screen_Watchface);

    return 0;
}

uint8_t app_system_loop() {
    
    while (1) {
        bsp_delay_ms(500);
        
        // -------------------------------------------------------------
        // Entrando no Menu
        // -------------------------------------------------------------
        UI_Manager_ProcessEvent(EVENT_BTN_SELECT);
        bsp_delay_ms(1000);
        
        // -------------------------------------------------------------
        // Navegando no Menu
        // -------------------------------------------------------------
        for (int i = 0; i < 4; i++) {
            UI_Manager_ProcessEvent(EVENT_BTN_NEXT); // Faz Partial Update
            bsp_delay_ms(1500);
        }
        
        // -------------------------------------------------------------
        // Entrando nas configurações
        // -------------------------------------------------------------
        bsp_delay_ms(1000);
        UI_Manager_ProcessEvent(EVENT_BTN_SELECT);
    }
    return 0;
}
