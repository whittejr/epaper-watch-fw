#include "it.h"
#include "stm32wbxx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

extern void xPortSysTickHandler(void);

void SysTick_Handler(void) {
    HAL_IncTick();
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xPortSysTickHandler();
    }
}

void EXTI9_5_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9); // Button is on PIN 9
}

void HSEM_IRQHandler(void) {
    HAL_HSEM_IRQHandler();
}
