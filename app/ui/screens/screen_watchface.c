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
#include "app_state.h"
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
    
    // BRANDING (Seiko Homage)
    app_display_draw_text_aligned(64, 25, 1, "SHM-5", 0);
    app_display_draw_rect(50, 36, 28, 1, 0);

    // MAIN TIME (Digital, but clean)
    uint8_t hours = sTime.Hours;
    if (!g_watch_settings.time_format_24h) {
        if (hours > 12) hours -= 12;
        if (hours == 0) hours = 12;
    }
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, sTime.Minutes);
    app_display_draw_text_aligned_v(64, 45, 1, buffer, &digitalDisco_16ptFontInfo, 0);

    // DATE WINDOW (Seiko style at 3 o'clock position)
    // Box
    app_display_draw_rect(90, 52, 32, 18, 0);
    app_display_draw_rect(91, 53, 30, 16, 1); // inner clear
    // Date text
    const char* days[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    // Note: sDate.WeekDay should be 0-6 if RTC is configured correctly
    snprintf(buffer, sizeof(buffer), "%s %d", days[sDate.WeekDay % 7], sDate.Date);
    app_display_draw_text(93, 56, buffer, 0);

    // "AUTOMATIC" / "21 JEWELS" Style text
    app_display_draw_text_aligned(64, LAYOUT_SEIKO_BRAND_Y, 1, "AUTOMATIC", 0);
    app_display_draw_text_aligned(64, LAYOUT_SEIKO_BRAND_Y + 12, 1, "WATER RESIST", 0);

    // DASHBOARD DIVIDER (Thin hairlines)
    app_display_draw_rect(10, 108, 108, 1, 0);

    // SMALL METRICS (Footer)
    uint8_t hr = app_oximeter_get_hr();
    uint32_t steps = app_accel_get_steps();
    snprintf(buffer, sizeof(buffer), "HR:%d  STP:%lu", hr, steps);
    app_display_draw_text_aligned(64, 112, 1, buffer, 0);
    
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

