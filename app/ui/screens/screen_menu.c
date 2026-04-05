// /**
// * @file    screen_menu.c
// * @brief   none
// * @version 0.1.0
// * @author  Alessandro Davi
// * @date    2026-02-27
// */

// #include "ui_manager.h"
// #include "app_display.h"

// extern const AppScreen_t Screen_Watchface;
// extern const AppScreen_t Screen_Oximeter;


// static const char *menu_items[] = {
//     "1. Medir Oximetro",
//     "2. Configurar",
//     "3. Sobre"
// };
// #define MENU_COUNT 3

// static uint8_t cursor_index = 0;

// static void Menu_Draw(void) {
//     // app_display_clear(WHITE);
//     // app_display_draw_text(10, 5, "--- MENU ---");

//     for (int i = 0; i < MENU_COUNT; i++) {
//         uint16_t y_pos = 30 + (i * 20); // Espaçamento de 20px
        
//         // Se for o item selecionado, desenha um ">" antes
//         if (i == cursor_index) {
//             // app_display_draw_text(5, y_pos, ">");
//         }
        
//         app_display_draw_text(20, y_pos, menu_items[i]);
//     }
//     app_display_update();
// }

// static void OnEnter(void) {
//     cursor_index = 0; // Sempre reseta o cursor para o topo
//     Menu_Draw();
// }

// static void OnEvent(UI_Event_t event) {
//     switch (event) {
//         case EVENT_BTN_NEXT:

//             cursor_index++;
//             if (cursor_index >= MENU_COUNT) cursor_index = 0;
//             Menu_Draw();
//             break;

//         case EVENT_BTN_SELECT:
            
//             if (cursor_index == 0) {
//                 UI_Manager_SwitchScreen(&Screen_Oximeter);
//             } 
//             else if (cursor_index == 1) {
//                 // UI_Manager_SwitchScreen(&Screen_Config);
//             }
//             break;

//         case EVENT_BTN_BACK:

//             UI_Manager_SwitchScreen(&Screen_Watchface);
//             break;
            
//         default:
//             break;
//     }
// }

// static void OnExit(void) {

// }

// const AppScreen_t Screen_MainMenu = {
//     .name = "Menu",
//     .on_enter = OnEnter,
//     .on_event = OnEvent,
//     .on_exit = OnExit
// };