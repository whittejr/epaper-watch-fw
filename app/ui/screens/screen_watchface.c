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
#include "font_digital_disco.h"
#include "app_accel.h"
#include "app_oximeter.h"
#include "rtc.h"
#include <stdio.h>
#include <stddef.h>

extern const AppScreen_t Screen_Menu; 

static void Watchface_Draw(display_update_mode_t mode) {
    char buffer[16];
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate; 
    
    rtc_get_time(&sTime, &sDate);

    app_display_clear(); 
    app_display_draw_status_bar();
    
    // BIG TIME (Digital Disco)
    snprintf(buffer, sizeof(buffer), "%02d:%02d", sTime.Hours, sTime.Minutes);
    app_display_draw_text_aligned_v(64, 30, 1, buffer, &digitalDisco_16ptFontInfo, 0);

    // SECONDS (Small)
    snprintf(buffer, sizeof(buffer), ":%02d", sTime.Seconds);
    app_display_draw_text(95, 45, buffer, 0);

    // DATE
    snprintf(buffer, sizeof(buffer), "%02d/%02d", sDate.Date, sDate.Month);
    app_display_draw_text_aligned(64, 60, 1, buffer, 0);

    // DASHBOARD DIVIDER
    app_display_draw_rect(10, 80, 108, 1, 0);

    // HEALTH METRICS
    // Heart Rate
    uint8_t hr = app_oximeter_get_hr();
    if (hr > 0) snprintf(buffer, sizeof(buffer), "HR: %d", hr);
    else snprintf(buffer, sizeof(buffer), "HR: --");
    app_display_draw_text(15, 95, buffer, 0);

    // Steps
    uint32_t steps = app_accel_get_steps();
    snprintf(buffer, sizeof(buffer), "STP: %lu", steps);
    app_display_draw_text(75, 95, buffer, 0);

    // Footer Hint
    app_display_draw_text_aligned(64, 115, 1, "[ MENU ]", 0);
    
    app_display_update(mode);
}

static void Watchface_OnEnter(void) {
    Watchface_Draw(DISPLAY_UPDATE_NORMAL);
}

static void Watchface_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_1SEC) {
        Watchface_Draw(DISPLAY_UPDATE_PARTIAL); 
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

