/**
 * @file screen_adjust_time.c
 * @brief UI for setting the real-time clock (RTC) time.
 */

#include "ui_manager.h"
#include "app_display.h"
#include "ui_layout.h"
#include "bsp.h"
#include <stdio.h>
#include <stddef.h>

extern const AppScreen_t Screen_Menu;

typedef enum {
    ADJUST_HOURS,
    ADJUST_MINUTES
} AdjustState_t;

static uint8_t adj_hours = 0;
static uint8_t adj_minutes = 0;
static AdjustState_t adj_state = ADJUST_HOURS;

static void AdjustTime_Draw(void) {
    char buffer[32];
    app_display_clear();
    
    app_display_draw_text_aligned(64, 20, 1, "AJUSTAR HORA", 0);

    // Hours
    snprintf(buffer, sizeof(buffer), "%02d", adj_hours);
    app_display_draw_text_aligned(64 - 15, 60, 1, buffer, 0);
    if (adj_state == ADJUST_HOURS) app_display_draw_text_aligned(64 - 15, 75, 1, "^^", 0);

    app_display_draw_text_aligned(64, 60, 1, ":", 0);

    // Minutes
    snprintf(buffer, sizeof(buffer), "%02d", adj_minutes);
    app_display_draw_text_aligned(64 + 15, 60, 1, buffer, 0);
    if (adj_state == ADJUST_MINUTES) app_display_draw_text_aligned(64 + 15, 75, 1, "^^", 0);

    app_display_draw_text_aligned(64, 100, 1, (adj_state == ADJUST_HOURS) ? "SET HORAS" : "SET MINUTOS", 0);
    
    app_display_update(DISPLAY_UPDATE_NORMAL);
}

static void AdjustTime_OnEnter(void) {
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    rtc_get_time(&sTime, &sDate);
    
    adj_hours = sTime.Hours;
    adj_minutes = sTime.Minutes;
    adj_state = ADJUST_HOURS;
    AdjustTime_Draw();
}

static void AdjustTime_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_NEXT) {
        if (adj_state == ADJUST_HOURS) adj_hours = (adj_hours + 1) % 24;
        else adj_minutes = (adj_minutes + 1) % 60;
        AdjustTime_Draw();
    } else if (event == EVENT_BTN_SELECT) {
        if (adj_state == ADJUST_HOURS) {
            adj_state = ADJUST_MINUTES;
            AdjustTime_Draw();
        } else {
            rtc_set_time(adj_hours, adj_minutes, 0);
            UI_Manager_SwitchScreen(&Screen_Menu);
        }
    }
}

const AppScreen_t Screen_AdjustTime = {
    .name = "Ajustar Hora",
    .on_enter = AdjustTime_OnEnter,
    .on_event = AdjustTime_OnEvent,
    .on_exit = NULL
};
