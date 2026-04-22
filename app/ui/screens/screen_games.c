/**
* @file    screen_games.c
* @brief   Games submenu
*/

#include "ui_manager.h"
#include "app_display.h"
#include "ui_layout.h"
#include <stddef.h>

extern const AppScreen_t Screen_Menu;
extern const AppScreen_t Screen_Snake;
extern const AppScreen_t Screen_Doom;
extern const AppScreen_t Screen_Flappy;
extern const AppScreen_t Screen_Parkour;

static const char *game_items[] = {
    "Snake",
    "Doom Lite",
    "Flappy Watch",
    "Parkour Watch",
    "Voltar"
};
#define GAMES_COUNT 5

static uint8_t cursor_games = 0;

static void Games_Draw(display_update_mode_t mode) {
    app_display_clear();
    app_display_draw_text_aligned(64, 10, 1, "JOGOS", 0);

    for (int i = 0; i < GAMES_COUNT; i++) {
        uint16_t pos_y = 40 + (i * 20);
        if (i == cursor_games) {
            app_display_draw_text(10, pos_y, ">", 0);
        }
        app_display_draw_text(30, pos_y, game_items[i], 0);
    }
    
    app_display_update(mode);
}

static void Games_OnEnter(void) {
    cursor_games = 0;
    Games_Draw(DISPLAY_UPDATE_NORMAL);
}

static void Games_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_NEXT) {
        cursor_games++;
        if (cursor_games >= GAMES_COUNT) cursor_games = 0;
        Games_Draw(DISPLAY_UPDATE_PARTIAL);
    }
    else if (event == EVENT_BTN_SELECT) {
        switch (cursor_games) {
            case 0: UI_Manager_SwitchScreen(&Screen_Snake); break;
            case 1: UI_Manager_SwitchScreen(&Screen_Doom); break;
            case 2: UI_Manager_SwitchScreen(&Screen_Flappy); break;
            case 3: UI_Manager_SwitchScreen(&Screen_Parkour); break;
            case 4: UI_Manager_SwitchScreen(&Screen_Menu); break;
        }
    }
}

const AppScreen_t Screen_Games = {
    .name = "Jogos",
    .on_enter = Games_OnEnter,
    .on_event = Games_OnEvent,
    .on_exit = NULL
};
