/**
* @file    app_system.c
* @brief   Main application orchestrator
*/

#include "app_system.h"
#include "app_display.h"
#include "app_accel.h"
#include "app_alarm.h"
#include "bsp.h"
#include "board_config.h"
#include "button.h"
#include "ui_manager.h"

extern const AppScreen_t Screen_Watchface; 
extern const AppScreen_t Screen_AlarmRinging;
extern volatile uint8_t bsp_btn_exti_flag;

static button_t button;

uint8_t app_system_init(void) {
    bsp_init();

    app_display_init();
    app_accel_init();
    app_alarm_init();
    button_init(&button);

    UI_Manager_Init(&Screen_Watchface);

    return 0;
}

uint8_t app_system_loop(void) {
    uint32_t last_1sec_tick = HAL_GetTick();

    while (1) {
        uint32_t current_tick = HAL_GetTick();

        // 1-Second Periodic Tasks
        if ((current_tick - last_1sec_tick) >= 1000) {
            last_1sec_tick = current_tick;
            
            RTC_TimeTypeDef sTime;
            RTC_DateTypeDef sDate;
            rtc_get_time(&sTime, &sDate);
            
            if (app_alarm_tick(sTime.Hours, sTime.Minutes)) {
                UI_Manager_SwitchScreen(&Screen_AlarmRinging);
            }

            UI_Manager_ProcessEvent(EVENT_TICK_1SEC); 
            app_display_update(DISPLAY_UPDATE_NORMAL);
        }

        // Button Processing
        bool exti_trigger = false;
        if (bsp_btn_exti_flag) {
            exti_trigger = true;
            bsp_btn_exti_flag = 0;
        }

        bool pin_is_active = (bsp_gpio_read(BUTTON_PORT, BUTTON_PIN) == 0);
        button_event_t btn_evt = button_process_interrupt_driven(&button, pin_is_active, &exti_trigger);

        if (btn_evt == BUTTON_EVENT_SHORT_CLICK) {
            UI_Manager_ProcessEvent(EVENT_BTN_NEXT);
            app_display_update(DISPLAY_UPDATE_NORMAL);
        }
        else if (btn_evt == BUTTON_EVENT_LONG_CLICK) {
            UI_Manager_ProcessEvent(EVENT_BTN_SELECT);
            app_display_update(DISPLAY_UPDATE_NORMAL);
        }
    }
}
