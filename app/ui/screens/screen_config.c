#include "ui_manager.h"
#include "app_display.h"

// Para poder voltar
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
    
    app_display_draw_text(30, 5, "CONFIGS");

    for (int i = 0; i < CONFIG_COUNT; i++) {
        uint16_t pos_y = 50 + (i * 40); // Mais espaçado, só tem 3 opções
        
        if (i == cursor_cfg) {
            app_display_draw_text(10, pos_y, ">");
        }
        app_display_draw_text(30, pos_y, config_items[i]);
    }
    
    app_display_update(mode);
}

static void Config_OnEnter(void) {
    cursor_cfg = 0;
    Config_Draw(SSD1681_UPDATE_FULL); // Pisca a tela inteira pra limpar o Menu Antigo
}

static void Config_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_NEXT) {
        cursor_cfg++;
        if (cursor_cfg >= CONFIG_COUNT) cursor_cfg = 0;
        
        Config_Draw(SSD1681_UPDATE_PARTIAL); // Move o cursor macio
    }
    else if (event == EVENT_BTN_SELECT) {
        // if (cursor_cfg == 2) { 
            // "Voltar" é o índice 2
            UI_Manager_SwitchScreen(&Screen_Menu);
        // }
    }
}

const AppScreen_t Screen_Config = {
    .name = "Configuracoes",
    .on_enter = Config_OnEnter,
    .on_event = Config_OnEvent,
    .on_exit = NULL
};