/**
* @file    ui_manager.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-02-27
*/

#include "ui_manager.h"
#include <stddef.h>

static const AppScreen_t *current_screen = NULL;

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

    if (current_screen->on_enter != NULL) {
        current_screen->on_enter();
    }
}

void UI_Manager_ProcessEvent(UI_Event_t event) {
    if (current_screen != NULL && current_screen->on_event != NULL) {
        

        current_screen->on_event(event);
    }
}
