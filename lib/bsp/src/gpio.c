/**
 * @file    gpio.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-11-05
 */

#include "gpio.h"
#include "board_config.h"

static TIM_HandleTypeDef htim17;

uint8_t gpio_init(void) {
    GPIO_InitTypeDef GPIOHandle = {0};

    /* LPUART1 */
    GPIOHandle.Mode = GPIO_MODE_AF_PP;
    GPIOHandle.Alternate = GPIO_AF8_LPUART1;
    GPIOHandle.Pin = GPIO_PIN_2;
    GPIOHandle.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIOHandle.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIOHandle);

    /* SSD1681 */
    GPIOHandle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOHandle.Pull = GPIO_PULLDOWN;
    GPIOHandle.Speed = GPIO_SPEED_FREQ_LOW;
    GPIOHandle.Pin = EPD_RST_PIN;
    HAL_GPIO_Init(EPD_RST_PORT, &GPIOHandle);
    HAL_GPIO_WritePin(EPD_RST_PORT, EPD_RST_PIN, GPIO_PIN_SET);

    GPIOHandle.Pull = GPIO_NOPULL;
    GPIOHandle.Pin = EPD_DC_PIN;
    HAL_GPIO_Init(EPD_DC_PORT, &GPIOHandle);

    GPIOHandle.Pin = EPD_CS_PIN;
    HAL_GPIO_Init(EPD_CS_PORT, &GPIOHandle);
    HAL_GPIO_WritePin(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_SET);

    GPIOHandle.Mode = GPIO_MODE_INPUT;
    GPIOHandle.Pull = GPIO_NOPULL;
    GPIOHandle.Pin = EPD_BSY_PIN;
    HAL_GPIO_Init(EPD_BSY_PORT, &GPIOHandle);


    /* LS013B7DH03 DISP PIN, TIMER AND PWM GPIO INIT */
    // GPIOHandle.Mode = GPIO_MODE_OUTPUT_PP;
    // GPIOHandle.Pull = GPIO_PULLDOWN;
    // GPIOHandle.Speed = GPIO_SPEED_FREQ_MEDIUM;
    // GPIOHandle.Pin = MIP_DISP_PIN;
    // HAL_GPIO_Init(MIP_DISP_PORT, &GPIOHandle);

    // htim17.Instance = TIM17;
    // htim17.Init.Prescaler = 2;
    // htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
    // htim17.Init.Period = 33332;
    // htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    // htim17.Init.RepetitionCounter = 0;
    // htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    // if (HAL_TIM_Base_Init(&htim17) != HAL_OK) return 1;
    // if (HAL_TIM_PWM_Init(&htim17) != HAL_OK) return 1;

    // TIM_OC_InitTypeDef sConfigOC = {0};
    // sConfigOC.OCMode = TIM_OCMODE_PWM1;
    // sConfigOC.Pulse = 16667;
    // sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    // sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    // sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    // sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    // sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    // if (HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) return 1;

    // GPIOHandle.Pin = GPIO_PIN_9;
    // GPIOHandle.Mode = GPIO_MODE_AF_PP;
    // GPIOHandle.Pull = GPIO_NOPULL;
    // GPIOHandle.Speed = GPIO_SPEED_FREQ_MEDIUM;
    // GPIOHandle.Alternate = GPIO_AF14_TIM17;
    // HAL_GPIO_Init(GPIOB, &GPIOHandle);

    /* MAX30102 INT PIN */
    GPIOHandle.Pin = GPIO_PIN_4; 
    GPIOHandle.Mode = GPIO_MODE_IT_FALLING;
    GPIOHandle.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIOHandle);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    return 0;
}

void bsp_gpio_write(GPIO_TypeDef *port, uint16_t pin, uint8_t state) {
    if (state) {
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    }
}

uint8_t bsp_gpio_read(GPIO_TypeDef *port, uint16_t pin) {
    return HAL_GPIO_ReadPin(port, pin);
}

void bsp_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}