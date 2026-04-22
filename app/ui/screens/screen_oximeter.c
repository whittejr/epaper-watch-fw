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
    app_display_draw_text_aligned(64, 10, 1, "OXIMETRO", 0);
    
    // Heart Rate
    uint8_t hr = app_oximeter_get_hr();
    if (hr == 0) {
        snprintf(buffer, sizeof(buffer), "BPM: --");
    } else {
        snprintf(buffer, sizeof(buffer), "BPM: %d", hr);
    }
    app_display_draw_text_aligned(64, 50, 1, buffer, 0);

    // SpO2
    uint8_t spo2 = app_oximeter_get_spo2();
    if (spo2 == 0) {
        snprintf(buffer, sizeof(buffer), "SpO2: --%%");
    } else {
        snprintf(buffer, sizeof(buffer), "SpO2: %d%%", spo2);
    }
    app_display_draw_text_aligned(64, 80, 1, buffer, 0);

    // Dica
    app_display_draw_text_aligned(64, 110, 1, "LONGO: VOLTAR", 0);
    
    app_display_update(mode);
}

static void Oximeter_OnEnter(void) {
    app_oximeter_init();
    Oximeter_Draw(DISPLAY_UPDATE_NORMAL);
}

static void Oximeter_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_1SEC) {
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
