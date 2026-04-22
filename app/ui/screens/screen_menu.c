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

extern const AppScreen_t Screen_Config;

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
    {"CONFIG", icon_config_16x16, &Screen_Config},
    {"RELOGIO", icon_clock_16x16, &Screen_AdjustTime},
    {"VOLTAR", icon_back_16x16, &Screen_Watchface}
};
#define MENU_COUNT 8
#define VISIBLE_ITEMS 4

static uint8_t cursor_menu = 0;
static uint8_t scroll_offset = 0;

static void Menu_Draw(display_update_mode_t mode) {
    app_display_clear();
    app_display_draw_status_bar();

    // TITLE
    app_display_draw_text_aligned(64, 20, 1, "MENU", 0);
    app_display_draw_rect(40, 31, 48, 1, 0);

    // LIST ITEMS (With Scrolling)
    for (int i = 0; i < VISIBLE_ITEMS; i++) {
        uint8_t index = scroll_offset + i;
        if (index >= MENU_COUNT) break;

        uint8_t y = LAYOUT_MENU_START_Y + (i * LAYOUT_MENU_SPACING);
        
        if (index == cursor_menu) {
            // Reverse Video Selection
            app_display_draw_rect(10, y - 2, 108, 14, 0);
            app_display_draw_bitmap(LAYOUT_MENU_ICON_X, y + 1, menu_items[index].icon, 16, 16, 1);
            app_display_draw_text(LAYOUT_MENU_TEXT_X, y, menu_items[index].text, 1);
        } else {
            app_display_draw_bitmap(LAYOUT_MENU_ICON_X, y + 1, menu_items[index].icon, 16, 16, 0);
            app_display_draw_text(LAYOUT_MENU_TEXT_X, y, menu_items[index].text, 0);
        }
    }
    
    // Scroll Indicator (small arrows or line)
    if (scroll_offset > 0) app_display_draw_text(115, 35, "^", 0);
    if (scroll_offset + VISIBLE_ITEMS < MENU_COUNT) app_display_draw_text(115, 110, "v", 0);

    app_display_update(mode);
}

static void Menu_OnEnter(void) {
    cursor_menu = 0;
    scroll_offset = 0;
    Menu_Draw(DISPLAY_UPDATE_NORMAL);
}

static void Menu_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_1SEC) {
        // Just refresh status bar time if needed, but usually not required for menu
    }
    else if (event == EVENT_BTN_NEXT) {
        cursor_menu++;
        if (cursor_menu >= MENU_COUNT) {
            cursor_menu = 0;
            scroll_offset = 0;
        }

        // Adjust scroll offset
        if (cursor_menu >= scroll_offset + VISIBLE_ITEMS) {
            scroll_offset = cursor_menu - VISIBLE_ITEMS + 1;
        }
        
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
