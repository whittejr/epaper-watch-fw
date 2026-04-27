/**
* @file    button.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-04-07
*/

#include "button.h"
#include "stm32wbxx_hal.h"

void button_init(button_t *btn) {
    btn->is_pressed = 0;
    btn->last_raw_state = 0;
    btn->last_debounce_time = 0;
    btn->press_start_time = 0;
    btn->long_press_handled = 0;
}

button_event_t button_process_interrupt_driven(button_t *btn, bool raw_pin_state, bool *exti_flag) {
    button_event_t event = BUTTON_EVENT_NONE;
    uint32_t current_tick_ms = HAL_GetTick();

    // 1. Se a interrupção avisou que houve mudança de estado físico
    if (*exti_flag) {
        *exti_flag = 0; // "Limpa/Consome" a flag para não processar de novo
        btn->last_debounce_time = current_tick_ms; // Inicia o timer do debounce
    }
    if ((current_tick_ms - btn->last_debounce_time) >= BUTTON_DEBOUNCE_MS) {
        
        if (raw_pin_state != btn->is_pressed) {
            btn->is_pressed = raw_pin_state;

            // O botão acabou de ser PRESSIONADO
            if (btn->is_pressed) {
                btn->press_start_time = current_tick_ms; 
                btn->long_press_handled = 0;         
            } 
            // O botão acabou de ser SOLTO
            else {

                if (!btn->long_press_handled) {
                    event = BUTTON_EVENT_SHORT_CLICK;
                }
            }
        }
    }

    if (btn->is_pressed && !btn->long_press_handled) {
        if ((current_tick_ms - btn->press_start_time) >= BUTTON_LONG_PRESS_MS) {
            event = BUTTON_EVENT_LONG_CLICK;
            btn->long_press_handled = 1;
        }
    }

    return event;
}