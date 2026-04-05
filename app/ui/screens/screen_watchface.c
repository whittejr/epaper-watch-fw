// /**
// * @file    screen_watchface.c
// * @brief   none
// * @version 0.1.0
// * @author  Alessandro Davi
// * @date    2026-02-27
// */

// #include "ui_manager.h"
// #include "app_display.h"
// #include "clock.h"
// #include <stdio.h>

// extern const AppScreen_t Screen_MainMenu;

// // --- LAYOUT ---
// #define TIME_X      40
// #define TIME_Y      80
// #define DATE_X      60
// #define DATE_Y      110

// static void Watchface_Draw(void) {
//     char time_str[16];
//     char date_str[16];
    

//     sprintf(time_str, "12:34:56");
//     sprintf(date_str, "25 FEV");


//     App_Display_Clear(WHITE);
    

//     App_Display_DrawText(TIME_X, TIME_Y, time_str);
    
//     App_Display_DrawText(DATE_X, DATE_Y, date_str);
    
//     App_Display_Update();
// }

// static void OnEnter(void) {
//     Watchface_Draw();
// }

// static void OnEvent(UI_Event_t event) {
//     switch (event) {
//         case EVENT_TICK_1SEC:

//             Watchface_Draw();
//             break;

//         case EVENT_BTN_SELECT:

//             UI_Manager_SwitchScreen(&Screen_MainMenu);
//             break;
            
//         default:
//             break;
//     }
// }

// static void OnExit(void) {

// }

// const AppScreen_t Screen_Watchface = {
//     .name = "Watchface",
//     .on_enter = OnEnter,
//     .on_event = OnEvent,
//     .on_exit = OnExit
// };