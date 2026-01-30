/**
 * @file    gpio.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-11-05
 */

#include "gpio.h"
#include "resources.h"
#include "stm32wbxx_hal_gpio.h"
#include "stm32wbxx_hal_tim.h"
#include <stm32wbxx_hal.h>

extern epd_user_config_t epd;
static TIM_HandleTypeDef htim17 = {0};

uint8_t ssd1681_cmd_data_gpio_init(void) {
    GPIO_InitTypeDef GPIOHandle = {0};

    //--- Pinos de saída: DC ---
    GPIOHandle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOHandle.Pull = GPIO_NOPULL;
    GPIOHandle.Speed = GPIO_SPEED_FREQ_LOW;

    // DC
    GPIOHandle.Pin = epd.pins.dc.pin;
    HAL_GPIO_Init(epd.pins.dc.port, &GPIOHandle);

    return 0;
}

uint8_t ssd1681_cmd_data_gpio_write(uint8_t value) {
    if (value == 0) // CMD
        HAL_GPIO_WritePin(epd.pins.dc.port, epd.pins.dc.pin, GPIO_PIN_RESET);

    else if (value == 1) // DATA
        HAL_GPIO_WritePin(epd.pins.dc.port, epd.pins.dc.pin, GPIO_PIN_SET);

    return 0;
}

uint8_t busy_gpio_read(uint8_t *value) {

    if (value == NULL)
        return 1;

    return *value = HAL_GPIO_ReadPin(epd.pins.busy.port, epd.pins.busy.pin);

    //  *value;
}

/*
 * @brief      ssd1681_cmd_data_deinit
 * @return     status code
 *            - 0 sucess
 * @note
 */
uint8_t ssd1681_cmd_data_gpio_deinit(void) {
    HAL_GPIO_DeInit(epd.pins.dc.port, epd.pins.dc.pin);

    return 0;
}

uint8_t ssd1681_rst_gpio_init(void) {
    GPIO_InitTypeDef GPIOHandle = {0};

    GPIOHandle.Pin = epd.pins.rst.pin;
    GPIOHandle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOHandle.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(epd.pins.rst.port, &GPIOHandle);
    HAL_GPIO_WritePin(epd.pins.rst.port, epd.pins.rst.pin, GPIO_PIN_SET);

    return 0;
}

uint8_t ssd1681_rst_gpio_deinit(void) {

    HAL_GPIO_DeInit(epd.pins.rst.port, epd.pins.rst.pin);

    return 0;
}

uint8_t ssd1681_rst_gpio_write(uint8_t value) {
    if (value != 0)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

    return 0;
}

uint8_t ssd1681_busy_gpio_init(void) {

    GPIO_InitTypeDef GPIOHandle = {0};

    //--- Pino de entrada: BUSY ---
    GPIOHandle.Pin = epd.pins.busy.pin;
    GPIOHandle.Mode = GPIO_MODE_INPUT;
    GPIOHandle.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(epd.pins.busy.port, &GPIOHandle);

    return 0;
}

uint8_t ssd1681_busy_gpio_deinit(void) {
    HAL_GPIO_DeInit(epd.pins.busy.port, epd.pins.busy.pin);

    return 0;
}

/*
 * MAX30102
 */
uint8_t max_irq_gpio_init(void) {
    GPIO_InitTypeDef GPIOHandle;

    GPIOHandle.Pin = GPIO_PIN_4;
    GPIOHandle.Mode = GPIO_MODE_IT_FALLING;
    GPIOHandle.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOB, &GPIOHandle);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    return 0;
}


/*
*   LS013B7DH03
*/
// EXTMODE E DISP PINS NÃO PRECISO PORQUE O EXTMODE JA ESTÁ HIGH (INVERSAO POR HARDWARE NO PINO EXTCOMIN)
// E DISP ESTÁ HIGH (DISPLAY ATIVO)
uint8_t ls013b7dh03_gpio_init(void) {
    if (ls013b7dh03_extcomin_gpio_init() != 0)
        return 1;

    return 0;
}

uint8_t ls013b7dh03_extcomin_gpio_init(void) {
    // GPIO CONFIG
    GPIO_InitTypeDef GPIO_Handle = {0};
    // PWM CONFIG
    TIM_OC_InitTypeDef sConfigOC = {0};

    htim17.Instance = TIM17;
    htim17.Init.Prescaler = 1;
    htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim17.Init.Period = 33332;
    htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim17.Init.RepetitionCounter = 0;
    htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim17) != 0)
        return 1;
    if (HAL_TIM_PWM_Init(&htim17) != 0)
        return 1;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 16666;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) != 0)
        return 1;

    GPIO_Handle.Pin = GPIO_PIN_9;
    GPIO_Handle.Mode = GPIO_MODE_AF_PP;
    GPIO_Handle.Pull = GPIO_NOPULL;
    GPIO_Handle.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_Handle.Alternate = GPIO_AF14_TIM17;
    HAL_GPIO_Init(GPIOB, &GPIO_Handle);

    HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);

    return 0;
}

uint8_t ls013b7dh03_gpio_cs_control(uint8_t state) {
    if (state)
        HAL_GPIO_WritePin(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_RESET);

    return 0;
}

void ls013b7dh03_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}
