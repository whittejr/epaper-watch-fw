/**
* @file    ui_manager.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-02-27
*/

#include "ui_manager.h"
#include <stddef.h>

#include "app_state.h"

extern const AppScreen_t Screen_Watchface;
extern const AppScreen_t Screen_Menu;
extern const AppScreen_t Screen_Oximeter;
extern const AppScreen_t Screen_Doodle;

static const AppScreen_t *current_screen = NULL;
static uint32_t inactivity_timer = 0;

void UI_Manager_Init(const AppScreen_t *initial_screen) {
    if (initial_screen != NULL) {
        UI_Manager_SwitchScreen(initial_screen);
    }
}

void UI_Manager_SwitchScreen(const AppScreen_t *new_screen) {
    if (new_screen == NULL || new_screen == current_screen) {
        return; 
    }

    if (current_screen != NULL && current_screen->on_exit != NULL) {
        current_screen->on_exit();
    }

    current_screen = new_screen;
    inactivity_timer = 0; // Zera inatividade ao mudar de tela

    if (current_screen->on_enter != NULL) {
        current_screen->on_enter();
    }
}

void UI_Manager_ProcessEvent(UI_Event_t event) {
    // Logica de Inatividade Global e Wake
    if (event == EVENT_TICK_1SEC) {
        inactivity_timer++;
        // Timeout de 10 segundos para voltar a watchface (se nao estiver nela, no Oximetro ou no Jogo)
        if (inactivity_timer >= 10 && 
            current_screen != &Screen_Watchface && 
            current_screen != &Screen_Oximeter && 
            current_screen != &Screen_Doodle) {
            UI_Manager_SwitchScreen(&Screen_Watchface);
            return; // Bloqueia outros eventos no mesmo ciclo
        }
    } else if (event == EVENT_WAKE) {
        // Tilt detectado pelo acelerômetro!
        // So funciona se estiver na Watchface E a configuracao estiver ligada
        if (current_screen == &Screen_Watchface && g_watch_settings.wrist_wake_enabled) {
            UI_Manager_SwitchScreen(&Screen_Menu);
        }
        inactivity_timer = 0;
    } else if (event != EVENT_TICK_100MS && event != EVENT_TICK_50MS && event != EVENT_NONE) {
        // Qualquer outro evento interativo zera o timeout
        inactivity_timer = 0;
    }

    if (current_screen != NULL && current_screen->on_event != NULL) {
        current_screen->on_event(event);
    }
}
