#include "gpio.h"
#include "board_config.h"
#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_lptim.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "ui_manager.h"

extern QueueHandle_t xSystemEventQueue;

uint8_t gpio_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    static LPTIM_HandleTypeDef hlptim1 = {0};

    // Habilita Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_LPTIM1_CLK_ENABLE();

    // Button Init (PC13)
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    // CS, DISP as output
    HAL_GPIO_WritePin(DISP_CS_PORT, DISP_CS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DISP_ON_PORT, DISP_ON_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = DISP_CS_PIN | DISP_ON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // spi configuration
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1; 
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // enables display (disp = high)
    HAL_GPIO_WritePin(DISP_ON_PORT, DISP_ON_PIN, GPIO_PIN_SET);

    // pwm init
    GPIO_InitStruct.Pin = DISP_EXTCOM_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_LPTIM1; 
    HAL_GPIO_Init(DISP_EXTCOM_PORT, &GPIO_InitStruct);

    hlptim1.Instance = LPTIM1;
    hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC; 
    hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV4; 
    hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
    hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
    hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
    hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;

    if (HAL_LPTIM_Init(&hlptim1) != HAL_OK) {
        return 1;
    }

    // period = 16666, pulse (Duty 50%) = 8333
    if (HAL_LPTIM_PWM_Start(&hlptim1, 16666, 8333) != HAL_OK) {
        return 1;
    }

    return 0;
}

void gpio_cs_control(uint8_t state) {
    if (state)
        HAL_GPIO_WritePin(DISP_CS_PORT, DISP_CS_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(DISP_CS_PORT, DISP_CS_PIN, GPIO_PIN_RESET);
}

void bsp_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

void bsp_gpio_write(GPIO_TypeDef *port, uint16_t pin, uint8_t state) {
    HAL_GPIO_WritePin(port, pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

uint8_t bsp_gpio_read(GPIO_TypeDef *port, uint16_t pin) {
    return (uint8_t)HAL_GPIO_ReadPin(port, pin);
}

extern volatile uint8_t bsp_btn_exti_flag;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == BUTTON_PIN) {
        bsp_btn_exti_flag = 1;
    }
}
