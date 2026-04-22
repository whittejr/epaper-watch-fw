/**
 * @file screen_alarm_ringing.c
 * @brief UI for medication alarm notification.
 */

#include "ui_manager.h"
#include "app_display.h"
#include "ui_layout.h"
#include <stddef.h>

extern const AppScreen_t Screen_Watchface;

static void AlarmRinging_Draw(void) {
    app_display_clear();
    
    app_display_draw_text_aligned(64, 40, 1, "!!! ATENCAO !!!", 0);
    app_display_draw_text_aligned(64, 60, 1, "HORA DO REMEDIO", 0);
    app_display_draw_text_aligned(64, 100, 1, "CLIQUE P/ SAIR", 0);
    
    app_display_update(DISPLAY_UPDATE_NORMAL);
}

static void AlarmRinging_OnEnter(void) {
    AlarmRinging_Draw();
}

static void AlarmRinging_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_NEXT || event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(&Screen_Watchface);
    }
}

const AppScreen_t Screen_AlarmRinging = {
    .name = "Alarme Tocado",
    .on_enter = AlarmRinging_OnEnter,
    .on_event = AlarmRinging_OnEvent,
    .on_exit = NULL
};
