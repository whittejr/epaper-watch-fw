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
        bsp_delay_ms(500); // Dá um tempo pro usuário admirar a tela inicial
        
        // -------------------------------------------------------------
        // FASE 1: Navegando no Menu Principal
        // O usuário aperta NEXT 4 vezes para chegar em "Configurações"
        // -------------------------------------------------------------
        for (int i = 0; i < 4; i++) {
            bsp_delay_ms(500);
            UI_Manager_ProcessEvent(EVENT_BTN_NEXT); // Faz Partial Update
        }
        
        // -------------------------------------------------------------
        // FASE 2: Entrando em Configurações
        // -------------------------------------------------------------
        UI_Manager_ProcessEvent(EVENT_BTN_SELECT);
        bsp_delay_ms(1000);
        // Aqui o E-paper vai piscar, pois a Screen_MainMenu chamará a Screen_Config 
        // e a função Config_OnEnter pede um DISPLAY_UPDATE_FULL.

        
        // -------------------------------------------------------------
        // FASE 3: Navegando nas Configurações
        // O usuário aperta NEXT 2 vezes para descer até o botão "Voltar"
        // -------------------------------------------------------------
        for (int i = 0; i < 2; i++) {
            UI_Manager_ProcessEvent(EVENT_BTN_NEXT); // Faz Partial Update
            bsp_delay_ms(1500);
        }
        
        // -------------------------------------------------------------
        // FASE 4: Saindo das Configurações e Voltando pro Menu
        // -------------------------------------------------------------
        bsp_delay_ms(1000);
        UI_Manager_ProcessEvent(EVENT_BTN_SELECT);
        // O E-paper pisca a tela inteira de novo, voltando pro Menu Principal!
        
        // Fim do loop. Ele vai voltar a descer o menu principal...
    }
    return 0;
}