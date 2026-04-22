/**
* @file    screen_menu.c
* @brief   Main menu with Card Style (1 App per screen)
*/

#include "ui_manager.h"
#include "app_display.h"
#include "ui_layout.h"
#include "icons.h" 
#include <stddef.h>

extern const AppScreen_t Screen_Watchface; 
extern const AppScreen_t Screen_AdjustTime;
extern const AppScreen_t Screen_Alarms;
extern const AppScreen_t Screen_Oximeter;
extern const AppScreen_t Screen_Accel;
extern const AppScreen_t Screen_Games;
extern const AppScreen_t Screen_BLE;

typedef struct {
    const char *text;
    const uint8_t *icon;
    const AppScreen_t *screen;
} MenuItem_t;

static const MenuItem_t menu_items[] = {
    {"CONEXAO", icon_bt_on_8x8, &Screen_BLE},
    {"OXIMETRO", icon_oximeter_16x16, &Screen_Oximeter},
    {"ATIVIDADE", icon_accel_16x16, &Screen_Accel},
    {"JOGOS", icon_data_16x16, &Screen_Games},
    {"ALARMES", icon_data_16x16, &Screen_Alarms},       
    {"RELOGIO", icon_clock_16x16, &Screen_AdjustTime},
    {"VOLTAR", icon_back_16x16, &Screen_Watchface}
};
#define MENU_COUNT 7

static uint8_t cursor_menu = 0;

static void Menu_Draw(display_update_mode_t mode) {
    app_display_clear();
    app_display_draw_status_bar();

    // CARD FRAME
    app_display_draw_rect(10, 20, 108, 90, 0); // Border
    app_display_draw_rect(12, 22, 104, 86, 1); // Inner clear

    // ICON (Centered in card)
    // Scale 16x16 to look bigger or just center it
    app_display_draw_bitmap(64 - 8, 45, menu_items[cursor_menu].icon, 16, 16, 0);

    // APP NAME
    app_display_draw_text_aligned(64, 80, 1, menu_items[cursor_menu].text, 0);

    // PAGINATION DOTS
    for (int i = 0; i < MENU_COUNT; i++) {
        uint8_t dot_x = 64 - (MENU_COUNT * 4) + (i * 8);
        if (i == cursor_menu) {
            app_display_draw_rect(dot_x, 115, 4, 4, 0); // Selected dot
        } else {
            // Unselected dot (empty square)
            app_display_draw_rect(dot_x, 115, 4, 1, 0);
            app_display_draw_rect(dot_x, 118, 4, 1, 0);
            app_display_draw_rect(dot_x, 115, 1, 4, 0);
            app_display_draw_rect(dot_x + 3, 115, 1, 4, 0);
        }
    }
    
    app_display_update(mode);
}

static void Menu_OnEnter(void) {
    cursor_menu = 0;
    Menu_Draw(DISPLAY_UPDATE_NORMAL);
}

static void Menu_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_NEXT) {
        cursor_menu++;
        if (cursor_menu >= MENU_COUNT) cursor_menu = 0;
        Menu_Draw(DISPLAY_UPDATE_PARTIAL); 
    }
    else if (event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(menu_items[cursor_menu].screen);
    }
}

const AppScreen_t Screen_Menu = {
    .name = "Menu Principal",
    .on_enter = Menu_OnEnter,
    .on_event = Menu_OnEvent,
    .on_exit = NULL
};
