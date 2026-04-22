/**
 * @file screen_alarms.c
 * @brief UI for managing medication alarms.
 */

#include "ui_manager.h"
#include "app_display.h"
#include "ui_layout.h"
#include "app_alarm.h"
#include <stdio.h>

extern const AppScreen_t Screen_Menu;

typedef enum {
    STATE_LIST,
    STATE_ADD_HOURS,
    STATE_ADD_MINUTES
} AlarmState_t;

static uint8_t cursor_alarm = 0;
static AlarmState_t alarm_state = STATE_LIST;
static uint8_t new_hours = 8;
static uint8_t new_minutes = 0;

static void Alarms_Draw(void) {
    app_display_clear();
    char buffer[32];

    if (alarm_state == STATE_LIST) {
        app_display_draw_text_aligned(LAYOUT_STATUS_TITLE_X, 20, 1, "ALARMES", 0);
        
        alarm_t* list = app_alarm_get_list();
        for (int i = 0; i < MAX_ALARMS; i++) {
            uint16_t y = 40 + (i * 12);
            if (i == cursor_alarm) {
                app_display_draw_text(10, y, ">", 0);
            }
            
            if (list[i].active) {
                snprintf(buffer, sizeof(buffer), "AL%d: %02d:%02d", i+1, list[i].hours, list[i].minutes);
            } else {
                snprintf(buffer, sizeof(buffer), "AL%d: ---", i+1);
            }
            app_display_draw_text(25, y, buffer, 0);
        }
        
        uint16_t footer_y = 110;
        if (cursor_alarm == MAX_ALARMS) app_display_draw_text(10, footer_y, ">", 0);
        app_display_draw_text(25, footer_y, "ADICIONAR NOVO", 0);
        
        if (cursor_alarm == MAX_ALARMS + 1) app_display_draw_text(10, footer_y + 12, ">", 0);
        app_display_draw_text(25, footer_y + 12, "VOLTAR", 0);

    } else {
        app_display_draw_text_aligned(LAYOUT_STATUS_TITLE_X, 20, 1, "NOVO ALARME", 0);
        
        snprintf(buffer, sizeof(buffer), "%02d:%02d", new_hours, new_minutes);
        app_display_draw_text_aligned(64, 60, 1, buffer, 0);
        
        if (alarm_state == STATE_ADD_HOURS) {
            app_display_draw_text_aligned(64, 80, 1, "^^   ", 0);
        } else {
            app_display_draw_text_aligned(64, 80, 1, "   ^^", 0);
        }
        
        app_display_draw_text_aligned(64, 100, 1, "LONG CLICK CONFIRMA", 0);
    }
    
    app_display_update(DISPLAY_UPDATE_NORMAL);
}

static void Alarms_OnEnter(void) {
    cursor_alarm = 0;
    alarm_state = STATE_LIST;
    Alarms_Draw();
}

static void Alarms_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_NEXT) {
        if (alarm_state == STATE_LIST) {
            cursor_alarm++;
            if (cursor_alarm > MAX_ALARMS + 1) cursor_alarm = 0;
        } else if (alarm_state == STATE_ADD_HOURS) {
            new_hours = (new_hours + 1) % 24;
        } else {
            new_minutes = (new_minutes + 1) % 60;
        }
        Alarms_Draw();
    } else if (event == EVENT_BTN_SELECT) {
        if (alarm_state == STATE_LIST) {
            if (cursor_alarm < MAX_ALARMS) {
                app_alarm_remove(cursor_alarm);
                Alarms_Draw();
            } else if (cursor_alarm == MAX_ALARMS) {
                alarm_state = STATE_ADD_HOURS;
                new_hours = 8;
                new_minutes = 0;
                Alarms_Draw();
            } else {
                UI_Manager_SwitchScreen(&Screen_Menu);
            }
        } else if (alarm_state == STATE_ADD_HOURS) {
            alarm_state = STATE_ADD_MINUTES;
            Alarms_Draw();
        } else {
            app_alarm_add(new_hours, new_minutes);
            alarm_state = STATE_LIST;
            Alarms_Draw();
        }
    }
}

const AppScreen_t Screen_Alarms = {
    .name = "Alarmes",
    .on_enter = Alarms_OnEnter,
    .on_event = Alarms_OnEvent,
    .on_exit = NULL
};
