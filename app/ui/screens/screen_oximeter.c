/**
* @file    screen_oximeter.c
* @brief   Oximeter display screen
* @version 0.1.0
* @author  Gemini CLI
* @date    2026-04-21
*/

#include "ui_manager.h"
#include "app_display.h"
#include "ui_layout.h"
#include "app_oximeter.h"
#include <stdio.h>
#include <stddef.h>

extern const AppScreen_t Screen_Menu; 

static void Oximeter_Draw(display_update_mode_t mode) {
    char buffer[32];
    
    app_display_clear(); 
    
    // Título
    app_display_draw_text_aligned(64, 5, 1, "OXIMETRO", 0);
    app_display_draw_rect(10, 16, 108, 1, 0);

    // Heart Rate
    uint8_t hr = app_oximeter_get_hr();
    if (hr == 0) {
        snprintf(buffer, sizeof(buffer), "BPM: --");
    } else {
        snprintf(buffer, sizeof(buffer), "BPM: %d", hr);
    }
    app_display_draw_text_aligned(64, 45, 1, buffer, 0);

    // SpO2
    uint8_t spo2 = app_oximeter_get_spo2();
    if (spo2 == 0) {
        snprintf(buffer, sizeof(buffer), "SpO2: --%%");
    } else {
        snprintf(buffer, sizeof(buffer), "SpO2: %d%%", spo2);
    }
    app_display_draw_text_aligned(64, 70, 1, buffer, 0);

    // Barra de Progresso (Movida para o fundo para não sobrepor)
    uint16_t progress = app_oximeter_get_progress();
    if (progress < 100) {
        app_display_draw_text_aligned(64, 85, 1, "CALIBRANDO...", 0);
        
        int bar_x = 24;
        int bar_y = 100;
        app_display_draw_rect(bar_x, bar_y, 80, 1, 0);         // Topo
        app_display_draw_rect(bar_x, bar_y + 8, 80, 1, 0);     // Fundo
        app_display_draw_rect(bar_x, bar_y, 1, 9, 0);          // Esquerda
        app_display_draw_rect(bar_x + 79, bar_y, 1, 9, 0);     // Direita
        
        int fill_w = (progress * 76) / 100;
        if (fill_w > 0) {
            app_display_draw_rect(bar_x + 2, bar_y + 2, fill_w, 5, 0);
        }
    } else {
        app_display_draw_text_aligned(64, 90, 1, "SINAL ESTAVEL", 0);
    }

    // Dica
    app_display_draw_text_aligned(64, 115, 1, "VOLTAR: CLIQUE LONGO", 0);
    
    app_display_update(mode);
}

static void Oximeter_OnEnter(void) {
    app_oximeter_init();
    Oximeter_Draw(DISPLAY_UPDATE_NORMAL);
}

static void Oximeter_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_100MS) {
        Oximeter_Draw(DISPLAY_UPDATE_PARTIAL); 
    }
    else if (event == EVENT_BTN_SELECT) {
        // Long click returns to menu
        UI_Manager_SwitchScreen(&Screen_Menu);
    }
}

const AppScreen_t Screen_Oximeter = {
    .name = "Oximetro",
    .on_enter = Oximeter_OnEnter,
    .on_event = Oximeter_OnEvent,
    .on_exit = NULL
};
