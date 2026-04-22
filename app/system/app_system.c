/**
* @file    app_system.c
* @brief   Main application orchestrator (FreeRTOS version)
*/

#include "app_system.h"
#include "app_display.h"
#include "app_accel.h"
#include "app_alarm.h"
#include "app_oximeter.h"
#include "bsp.h"
#include "board_config.h"
#include "button.h"
#include "ui_manager.h"
#include "assets/anim_frames.h"

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

extern const AppScreen_t Screen_Watchface; 
extern const AppScreen_t Screen_AlarmRinging;

/* Task Handles */
TaskHandle_t xUITaskHandle = NULL;
TaskHandle_t xAlarmTaskHandle = NULL;
TaskHandle_t xButtonTaskHandle = NULL;
extern TaskHandle_t xOximeterTaskHandle;
extern TaskHandle_t xAccelTaskHandle;

/* Queues */
QueueHandle_t xSystemEventQueue = NULL;

/* Global Flags (from ISR) */
volatile uint8_t bsp_btn_exti_flag = 0;

/* Task Functions */
void vUITask(void *pvParameters);
void vAlarmTask(void *pvParameters);
void vButtonTask(void *pvParameters);

uint8_t app_system_init(void) {
    bsp_init();

    /* Ajusta prioridade do SysTick para o FreeRTOS (deve ser a mais baixa) */
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

    app_display_init();

    /* Boot Animation (GIF) */
    for (int frame = 0; frame < ANIM_NUM_FRAMES; frame++) {
        app_display_write_raw(anim_frames[frame]);
        HAL_Delay(50); // 50ms (~20 fps)
    }

    app_accel_init();
    app_alarm_init();
    app_oximeter_init();

    UI_Manager_Init(&Screen_Watchface);

    /* Create Event Queue */
    xSystemEventQueue = xQueueCreate(10, sizeof(UI_Event_t));
    configASSERT(xSystemEventQueue != NULL);

    /* Create Tasks */
    BaseType_t xRet;
    xRet = xTaskCreate(vUITask, "UI_Task", 512, NULL, 3, &xUITaskHandle);
    configASSERT(xRet == pdPASS);

    xRet = xTaskCreate(vAlarmTask, "Alarm_Task", 256, NULL, 2, &xAlarmTaskHandle);
    configASSERT(xRet == pdPASS);

    xRet = xTaskCreate(vButtonTask, "Button_Task", 256, NULL, 4, &xButtonTaskHandle);
    configASSERT(xRet == pdPASS);

    xRet = xTaskCreate(vOximeterTask, "Oximeter_Task", 512, NULL, 2, &xOximeterTaskHandle);
    configASSERT(xRet == pdPASS);

    xRet = xTaskCreate(vAccelTask, "Accel_Task", 512, NULL, 2, &xAccelTaskHandle);
    configASSERT(xRet == pdPASS);

    return 0;
}
uint8_t app_system_loop(void) {
    /* Start Scheduler */
    vTaskStartScheduler();
    while (1);
    return 0;
}

void vUITask(void *pvParameters) {
    UI_Event_t event;
    while (1) {
        /* Fica dormindo até que um evento chegue na fila */
        if (xQueueReceive(xSystemEventQueue, &event, portMAX_DELAY) == pdPASS) {
            UI_Manager_ProcessEvent(event);
            app_display_update(DISPLAY_UPDATE_NORMAL);
        }
    }
}

void vAlarmTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(100);
    UI_Event_t tick_100ms = EVENT_TICK_100MS;
    UI_Event_t tick_1s = EVENT_TICK_1SEC;
    uint8_t count_1s = 0;

    while (1) {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        
        // Sempre envia o tick de 100ms
        xQueueSend(xSystemEventQueue, &tick_100ms, 0);

        count_1s++;
        if (count_1s >= 10) {
            count_1s = 0;
            
            RTC_TimeTypeDef sTime;
            RTC_DateTypeDef sDate;
            rtc_get_time(&sTime, &sDate);
            
            if (app_alarm_tick(sTime.Hours, sTime.Minutes)) {
                UI_Manager_SwitchScreen(&Screen_AlarmRinging);
            }

            // Notifica a UI_Task que um segundo passou
            xQueueSend(xSystemEventQueue, &tick_1s, 0);
        }
    }
}

void vButtonTask(void *pvParameters) {
    button_t btn;
    button_init(&btn);
    TickType_t last_short_click_time = 0;
    const TickType_t double_click_window = pdMS_TO_TICKS(350);

    while (1) {
        bool raw_state = (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_RESET);
        button_event_t b_evt = button_process_interrupt_driven(&btn, raw_state, (bool *)&bsp_btn_exti_flag);

        if (b_evt == BUTTON_EVENT_SHORT_CLICK) {
            TickType_t now = xTaskGetTickCount();
            if ((now - last_short_click_time) < double_click_window) {
                UI_Event_t ui_evt = EVENT_BTN_DOUBLE;
                xQueueSend(xSystemEventQueue, &ui_evt, 0);
                last_short_click_time = 0; // Reset
            } else {
                last_short_click_time = now;
                // We wait a bit to see if it's a single or start of double
                // For better responsiveness in menus, we send NEXT immediately,
                // but Doom will listen for DOUBLE.
                UI_Event_t ui_evt = EVENT_BTN_NEXT;
                xQueueSend(xSystemEventQueue, &ui_evt, 0);
            }
        } else if (b_evt == BUTTON_EVENT_LONG_CLICK) {
            UI_Event_t ui_evt = EVENT_BTN_SELECT;
            xQueueSend(xSystemEventQueue, &ui_evt, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
