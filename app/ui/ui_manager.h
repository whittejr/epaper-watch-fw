/**
 * @file ui_manager.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-02-27
 */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <stdint.h>

typedef enum {
    EVENT_NONE = 0,
    EVENT_BTN_NEXT,   
    EVENT_BTN_SELECT, 
    EVENT_BTN_BACK,   
    EVENT_TICK_1SEC,  
    EVENT_SENSOR_READY,
    EVENT_ALARM_TRIGGERED
} UI_Event_t;


typedef struct {
    const char *name;              
    void (*on_enter)(void);        
    void (*on_event)(UI_Event_t e);
    void (*on_exit)(void);         
} AppScreen_t;

void UI_Manager_Init(const AppScreen_t *initial_screen);
void UI_Manager_SwitchScreen(const AppScreen_t *new_screen);
void UI_Manager_ProcessEvent(UI_Event_t event);

#endif // UI_MANAGER_H
