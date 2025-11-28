// stm32wbxx_it.c
#include "stm32wbxx_hal.h"
extern TIM_HandleTypeDef htim16;

void SysTick_Handler(void) {
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */

    /* USER CODE BEGIN SysTick_IRQn 1 */
    HAL_IncTick();
    /* USER CODE END SysTick_IRQn 1 */
}