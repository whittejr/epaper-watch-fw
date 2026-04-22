/**
* @file    screen_accel.c
* @brief   Accelerometer data visualizer
*/

#include "ui_manager.h"
#include "app_display.h"
#include "app_accel.h"
#include <stdio.h>

extern const AppScreen_t Screen_Menu;

static void Accel_Draw(display_update_mode_t mode) {
    char buffer[32];
    float g[3];
    
    app_display_clear();
    app_display_draw_text_aligned(64, 10, 1, "ACELEROMETRO", 0);

    app_accel_read(g);
    
    snprintf(buffer, sizeof(buffer), "X: %.2f G", g[0]);
    app_display_draw_text(10, 40, buffer, 0);
    
    snprintf(buffer, sizeof(buffer), "Y: %.2f G", g[1]);
    app_display_draw_text(10, 60, buffer, 0);
    
    snprintf(buffer, sizeof(buffer), "Z: %.2f G", g[2]);
    app_display_draw_text(10, 80, buffer, 0);

    snprintf(buffer, sizeof(buffer), "Passos: %lu", app_accel_get_steps());
    app_display_draw_text(10, 110, buffer, 0);

    app_display_update(mode);
}

static void Accel_OnEnter(void) {
    Accel_Draw(DISPLAY_UPDATE_NORMAL);
}

static void Accel_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_1SEC) {
        Accel_Draw(DISPLAY_UPDATE_PARTIAL);
    } else if (event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(&Screen_Menu);
    }
}

const AppScreen_t Screen_Accel = {
    .name = "Accel",
    .on_enter = Accel_OnEnter,
    .on_event = Accel_OnEvent,
    .on_exit = NULL
};
