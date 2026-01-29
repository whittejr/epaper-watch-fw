/**
 * @file    clock.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2026-01-07
 */

#include "stm32wbxx_hal.h"

uint8_t clock_init() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_TIM17_CLK_ENABLE();

    return 0;
}