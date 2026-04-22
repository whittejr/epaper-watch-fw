/**
* @file    screen_config.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-04-05
*/

#include "ui_manager.h"
#include "app_display.h"
#include "ui_layout.h"
#include "icons.h"
#include <stddef.h>

extern const AppScreen_t Screen_Menu; 

static const char *config_items[] = {
    "Display",
    "Bluetooth",
    "Voltar"
};
#define CONFIG_COUNT 3

static uint8_t cursor_cfg = 0;

static void Config_Draw(display_update_mode_t mode) {
    app_display_clear();
    
    // 1. Título Fixo
    app_display_draw_text_aligned(LAYOUT_STATUS_TITLE_X, LAYOUT_STATUS_Y, 1, "CONFIGURACOES", 0);
    
    // 2. Ícone estático de voltar (Demonstração do layout livre)
    app_display_draw_bitmap(LAYOUT_STATUS_BACK_X, LAYOUT_STATUS_Y, icon_back_16x16, 16, 16, 0);

    // 3. Lista de Opções (Usando o mesmo espaçamento padrão do menu)
    for (int i = 0; i < CONFIG_COUNT; i++) {
        uint16_t pos_y = LAYOUT_MENU_START_Y + (i * LAYOUT_MENU_SPACING); 
        
        if (i == cursor_cfg) {
            app_display_draw_text(LAYOUT_MENU_CURSOR_X, pos_y, ">", 0);
        }
        // Aqui não temos array de ícones, então desenhamos apenas o texto
        app_display_draw_text(LAYOUT_MENU_TEXT_X, pos_y, config_items[i], 0);
    }
    
    app_display_update(mode);
}

static void Config_OnEnter(void) {
    cursor_cfg = 0;
    Config_Draw(DISPLAY_UPDATE_NORMAL); 
}

static void Config_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_NEXT) {
        cursor_cfg++;
        if (cursor_cfg >= CONFIG_COUNT) cursor_cfg = 0;
        
        Config_Draw(DISPLAY_UPDATE_PARTIAL); 
    }
    else if (event == EVENT_BTN_SELECT) {
        // Se for o "Voltar" (índice 2)
        if (cursor_cfg == 2) { 
            UI_Manager_SwitchScreen(&Screen_Menu);
        }
    }
}

const AppScreen_t Screen_Config = {
    .name = "Configuracoes",
    .on_enter = Config_OnEnter,
    .on_event = Config_OnEvent,
    .on_exit = NULL
};

