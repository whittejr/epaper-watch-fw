#include "it.h"
#include "stm32wbxx_hal.h"

void SysTick_Handler(void) {
    HAL_IncTick();
}

void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
