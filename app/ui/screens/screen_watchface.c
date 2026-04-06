/**
* @file    screen_watchface.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-04-05
*/

#include "ui_manager.h"
#include "app_display.h"
#include "ui_layout.h"
#include <stdio.h>

extern const AppScreen_t Screen_Menu; 

static uint32_t segundos = 0;

static void Watchface_Draw(display_update_mode_t mode) {
    char buffer[32];
    
    app_display_clear(); 
    
    // Título fixo centralizado usando o layout
    app_display_draw_text_aligned(LAYOUT_STATUS_TITLE_X, LAYOUT_STATUS_Y, 1, "WATCHFACE");
    
    // Mensagem de boas vindas
    app_display_draw_text(LAYOUT_WF_MSG_X, LAYOUT_WF_MSG_Y, "Bem Vindo, Alessandro");

    // Dado dinâmico (exemplo comentado)
    // snprintf(buffer, sizeof(buffer), "Tempo: %u", segundos);
    // app_display_draw_text_aligned(LAYOUT_STATUS_TITLE_X, 80, 1, buffer);
    
    app_display_update(mode);
}

static void Watchface_OnEnter(void) {
    Watchface_Draw(SSD1681_UPDATE_FAST);
}

static void Watchface_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_1SEC) {
        segundos++;
        Watchface_Draw(SSD1681_UPDATE_PARTIAL); 
    }
    else if (event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(&Screen_Menu);
    }
}

const AppScreen_t Screen_Watchface = {
    .name = "Watchface",
    .on_enter = Watchface_OnEnter,
    .on_event = Watchface_OnEvent,
    .on_exit = NULL
};
