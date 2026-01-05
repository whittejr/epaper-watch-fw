    // stm32wbxx_it.c
    #include "max30102.h"
    #include "stm32wbxx_hal.h"
    extern TIM_HandleTypeDef htim16;
    extern max30102_handle_t max_handle;

    void SysTick_Handler(void) {
        HAL_IncTick();
    }

    void EXTI4_IRQHandler(void) {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);

        max30102_irq_handler(&max_handle);
    }