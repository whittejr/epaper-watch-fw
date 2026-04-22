#include "it.h"
#include "stm32wbxx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

extern void xPortSysTickHandler(void);
extern IPCC_HandleTypeDef hipcc;

void SysTick_Handler(void) {
    HAL_IncTick();
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xPortSysTickHandler();
    }
}

void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9); // Button is on PIN 9
}

void IPCC_C1_RX_IRQHandler(void) {
    HAL_IPCC_RX_IRQHandler(&hipcc);
}

void IPCC_C1_TX_IRQHandler(void) {
    HAL_IPCC_TX_IRQHandler(&hipcc);
}

void HSEM_IRQHandler(void) {
    HAL_HSEM_IRQHandler();
}
