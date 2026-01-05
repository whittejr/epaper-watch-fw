/**
 * @file    gpio.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-11-05
 */

#include "gpio.h"
#include "ssd1681.h"
// #include "stm32wbxx_hal_gpio.h"
#include "resources.h"
#include "stm32wb55xx.h"
#include <stm32wbxx_hal.h>

extern epd_user_config_t epd;

// uint8_t gpio_write(bsp_pin_t *pin, uint8_t state) {
//     HAL_GPIO_WritePin(pin->port, pin->pin, state);

//     return 0;
// }

uint8_t epd_cmd_data_gpio_init(void) {
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

uint8_t epd_cmd_data_gpio_write(uint8_t value) {
    if (value == SSD1681_CMD)
        HAL_GPIO_WritePin(epd.pins.dc.port, epd.pins.dc.pin, GPIO_PIN_RESET);

    else if (value == SSD1681_DATA)
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
 * @brief      epd_cmd_data_deinit
 * @return     status code
 *            - 0 sucess
 * @note
 */
uint8_t epd_cmd_data_gpio_deinit(void) {
    HAL_GPIO_DeInit(epd.pins.dc.port, epd.pins.dc.pin);

    return 0;
}

uint8_t epd_rst_gpio_init(void) {
    GPIO_InitTypeDef GPIOHandle = {0};

    GPIOHandle.Pin = epd.pins.rst.pin;
    GPIOHandle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOHandle.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(epd.pins.rst.port, &GPIOHandle);
    HAL_GPIO_WritePin(epd.pins.rst.port, epd.pins.rst.pin, GPIO_PIN_SET);

    return 0;
}

uint8_t epd_rst_gpio_deinit(void) {

    HAL_GPIO_DeInit(epd.pins.rst.port, epd.pins.rst.pin);

    return 0;
}

uint8_t epd_rst_gpio_write(uint8_t value) {
    if (value != 0)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

    return 0;
}

uint8_t epd_busy_gpio_init(void) {

    GPIO_InitTypeDef GPIOHandle = {0};

    //--- Pino de entrada: BUSY ---
    GPIOHandle.Pin = epd.pins.busy.pin;
    GPIOHandle.Mode = GPIO_MODE_INPUT;
    GPIOHandle.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(epd.pins.busy.port, &GPIOHandle);

    return 0;
}

uint8_t epd_busy_gpio_deinit(void) {
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

    HAL_GPIO_Init(GPIOA, &GPIOHandle);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    return 0;
}
