/**
* @file    screen_menu.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-04-05
*/

#include "ui_manager.h"
#include "app_display.h"
#include "ui_layout.h"
#include "icons.h" 

extern const AppScreen_t Screen_Config;
extern const AppScreen_t Screen_Watchface; 

typedef struct {
    const char *text;
    const uint8_t *icon;
} MenuItem_t;

static const MenuItem_t menu_items[] = {
    {"Oxi", icon_oximeter_16x16},
    {"Accel", icon_accel_16x16},
    {"Ver Dados", icon_data_16x16},       
    {"Hora", icon_clock_16x16},
    {"Config", icon_config_16x16},
    {"Voltar", icon_back_16x16}
};
#define MENU_COUNT 6

static uint8_t cursor_menu = 0;

static void Menu_Draw(display_update_mode_t mode) {
    app_display_clear();
    
    // 1. Título Fixo no topo
    app_display_draw_text_aligned(LAYOUT_STATUS_TITLE_X, LAYOUT_STATUS_Y, 1, "MENU PRINCIPAL");

    // 2. Desenha a lista de opções obedecendo ao ui_layout.h
    for (int i = 0; i < MENU_COUNT; i++) {
        uint16_t pos_y = LAYOUT_MENU_START_Y + (i * LAYOUT_MENU_SPACING); 
        
        if (i == cursor_menu) {
            app_display_draw_text(LAYOUT_MENU_CURSOR_X, pos_y, ">");
        }
        
        if (menu_items[i].icon != NULL) {
            app_display_draw_bitmap(LAYOUT_MENU_ICON_X, pos_y, menu_items[i].icon, 16, 16);
        }

        app_display_draw_text(LAYOUT_MENU_TEXT_X, pos_y, menu_items[i].text); 
    }
    
    app_display_update(mode);
}

static void Menu_OnEnter(void) {
    cursor_menu = 0;
    Menu_Draw(SSD1681_UPDATE_FAST);
}

static void Menu_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_NEXT) {
        cursor_menu++;
        if (cursor_menu >= MENU_COUNT) cursor_menu = 0;
        
        Menu_Draw(SSD1681_UPDATE_PARTIAL); 
    }
    else if (event == EVENT_BTN_SELECT) {
        switch (cursor_menu) {
            case 4: 
                UI_Manager_SwitchScreen(&Screen_Config);
                break;
            case 5: 
                UI_Manager_SwitchScreen(&Screen_Watchface);
                break;
            // Adicione os outros cases futuramente
        }
    }
}

const AppScreen_t Screen_Menu = {
    .name = "Menu Principal",
    .on_enter = Menu_OnEnter,
    .on_event = Menu_OnEvent,
    .on_exit = NULL
};
