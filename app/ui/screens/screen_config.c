/**
* @file    screen_config.c
* @brief   Advanced Watch Settings (Seiko Style with Scrolling)
*/

#include "ui_manager.h"
#include "app_display.h"
#include "ui_layout.h"
#include "icons.h"
#include "app_state.h"
#include <stddef.h>
#include <stdio.h>

extern const AppScreen_t Screen_Menu; 

typedef enum {
    CFG_FORMAT,
    CFG_BRIGHTNESS,
    CFG_WRIST_WAKE,
    CFG_VIBRATION,
    CFG_THEME,
    CFG_LANGUAGE,
    CFG_BACK,
    CFG_COUNT
} ConfigItem_t;

#define CFG_VISIBLE_ITEMS 4

static uint8_t cursor_cfg = 0;
static uint8_t cfg_scroll_offset = 0;

static const char *brightness_names[] = {"BAIXO", "MEDIO", "ALTO"};

static void Config_Draw(display_update_mode_t mode) {
    app_display_clear();
    app_display_draw_status_bar();
    
    // TITLE
    app_display_draw_text_aligned(64, 20, 1, "CONFIGURACOES", 0);
    app_display_draw_rect(30, 31, 68, 1, 0);

    for (int i = 0; i < CFG_VISIBLE_ITEMS; i++) {
        uint8_t index = cfg_scroll_offset + i;
        if (index >= CFG_COUNT) break;

        uint8_t y = LAYOUT_MENU_START_Y + (i * LAYOUT_MENU_SPACING);
        char buf[32];

        switch(index) {
            case CFG_FORMAT:
                snprintf(buf, sizeof(buf), "FORMATO: %s", g_watch_settings.time_format_24h ? "24H" : "12H");
                break;
            case CFG_BRIGHTNESS:
                snprintf(buf, sizeof(buf), "BRILHO: %s", brightness_names[g_watch_settings.brightness]);
                break;
            case CFG_WRIST_WAKE:
                snprintf(buf, sizeof(buf), "PULSO: %s", g_watch_settings.wrist_wake_enabled ? "LIGADO" : "DESLIG");
                break;
            case CFG_VIBRATION:
                snprintf(buf, sizeof(buf), "VIBRAR: %s", g_watch_settings.vibration_enabled ? "SIM" : "NAO");
                break;
            case CFG_THEME:
                snprintf(buf, sizeof(buf), "TEMA: %s", g_watch_settings.dark_theme ? "DARK" : "CLARO");
                break;
            case CFG_LANGUAGE:
                snprintf(buf, sizeof(buf), "IDIO: %s", g_watch_settings.language_pt ? "PT-BR" : "EN-US");
                break;
            case CFG_BACK:
                snprintf(buf, sizeof(buf), "VOLTAR");
                break;
        }

        if (index == cursor_cfg) {
            app_display_draw_rect(10, y - 2, 108, 14, 0);
            app_display_draw_text(15, y, buf, 1);
        } else {
            app_display_draw_text(15, y, buf, 0);
        }
    }
    
    // Scroll Indicators
    if (cfg_scroll_offset > 0) app_display_draw_text(115, 35, "^", 0);
    if (cfg_scroll_offset + CFG_VISIBLE_ITEMS < CFG_COUNT) app_display_draw_text(115, 110, "v", 0);

    app_display_update(mode);
}

static void Config_OnEnter(void) {
    cursor_cfg = 0;
    cfg_scroll_offset = 0;
    Config_Draw(DISPLAY_UPDATE_NORMAL); 
}

static void Config_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_NEXT) {
        cursor_cfg++;
        if (cursor_cfg >= CFG_COUNT) {
            cursor_cfg = 0;
            cfg_scroll_offset = 0;
        }

        if (cursor_cfg >= cfg_scroll_offset + CFG_VISIBLE_ITEMS) {
            cfg_scroll_offset = cursor_cfg - CFG_VISIBLE_ITEMS + 1;
        }
        
        Config_Draw(DISPLAY_UPDATE_PARTIAL); 
    }
    else if (event == EVENT_BTN_SELECT) {
        switch(cursor_cfg) {
            case CFG_FORMAT: g_watch_settings.time_format_24h = !g_watch_settings.time_format_24h; break;
            case CFG_BRIGHTNESS: 
                g_watch_settings.brightness++; 
                if (g_watch_settings.brightness > BRIGHTNESS_HIGH) g_watch_settings.brightness = BRIGHTNESS_LOW; 
                break;
            case CFG_WRIST_WAKE: g_watch_settings.wrist_wake_enabled = !g_watch_settings.wrist_wake_enabled; break;
            case CFG_VIBRATION: g_watch_settings.vibration_enabled = !g_watch_settings.vibration_enabled; break;
            case CFG_THEME: g_watch_settings.dark_theme = !g_watch_settings.dark_theme; break;
            case CFG_LANGUAGE: g_watch_settings.language_pt = !g_watch_settings.language_pt; break;
            case CFG_BACK:
                UI_Manager_SwitchScreen(&Screen_Menu);
                return;
        }
        Config_Draw(DISPLAY_UPDATE_PARTIAL);
    }
}

const AppScreen_t Screen_Config = {
    .name = "Configuracoes",
    .on_enter = Config_OnEnter,
    .on_event = Config_OnEvent,
    .on_exit = NULL
};
