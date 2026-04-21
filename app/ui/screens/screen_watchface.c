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
#include <stddef.h>

#include "bsp.h"

extern const AppScreen_t Screen_Menu; 

static void Watchface_Draw(display_update_mode_t mode) {
    char buffer[32];
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate; 
    
    rtc_get_time(&sTime, &sDate);

    app_display_clear(); 
    
    // Título centralizado
    app_display_draw_text_aligned(64, 20, 1, "SMARTWATCH WB55");
    
    // RELÓGIO
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
    app_display_draw_text_aligned(64, 60, 1, buffer);

    // Footer
    app_display_draw_text_aligned(64, 100, 1, "SISTEMA OK");
    
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

