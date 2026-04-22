/**
* @file    screen_ble.c
* @brief   Bluetooth management and diagnostic screen
*/

#include "ui_manager.h"
#include "app_display.h"
#include <stdio.h>

extern const AppScreen_t Screen_Menu;

static void BLE_Draw(void) {
    app_display_clear();
    app_display_draw_status_bar();
    app_display_draw_text_aligned(64, 20, 1, "BLUETOOTH", 0);
    app_display_draw_text_aligned(64, 60, 1, "Desativado", 0);
    app_display_draw_text_aligned(64, 115, 1, "LONGO: VOLTAR", 0);
    app_display_update(DISPLAY_UPDATE_NORMAL);
}

static void BLE_OnEnter(void) {
    BLE_Draw();
}

static void BLE_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(&Screen_Menu);
    }
}

const AppScreen_t Screen_BLE = {
    .name = "Bluetooth",
    .on_enter = BLE_OnEnter,
    .on_event = BLE_OnEvent,
    .on_exit = NULL
};
