#include "gpio.h"
#include "board_config.h"
#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_lptim.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "ui_manager.h"

extern QueueHandle_t xSystemEventQueue;

uint8_t gpio_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    static LPTIM_HandleTypeDef hlptim1 = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_LPTIM1_CLK_ENABLE();

    // Button Init
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    // Display Pins
    HAL_GPIO_WritePin(DISP_CS_PORT, DISP_CS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DISP_ON_PORT, DISP_ON_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = DISP_CS_PIN | DISP_ON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Accel CS
    HAL_GPIO_WritePin(ACCEL_CS_PORT, ACCEL_CS_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = ACCEL_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ACCEL_CS_PORT, &GPIO_InitStruct);

    // SPI1 Pins (SCK, MISO, MOSI)
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1; 
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(DISP_ON_PORT, DISP_ON_PIN, GPIO_PIN_SET);

    // PWM/LPTIM1 for EXTCOM
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

    if (HAL_LPTIM_Init(&hlptim1) == HAL_OK) {
        HAL_LPTIM_PWM_Start(&hlptim1, 16666, 8333);
    }

    return 0;
}

void gpio_cs_control(uint8_t state) {
    HAL_GPIO_WritePin(DISP_CS_PORT, DISP_CS_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == BUTTON_PIN) {
        extern volatile uint8_t bsp_btn_exti_flag;
        bsp_btn_exti_flag = 1;
    }
}
