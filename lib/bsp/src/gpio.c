/**
 * @file    gpio.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-11-05
 */

#include "gpio.h"
#include "board_config.h"
#include "stm32wb55xx.h"
#include "stm32wbxx_hal_gpio.h"

volatile uint8_t bsp_btn_exti_flag = 0;

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

    /* MAX30102 INT PIN */
    GPIOHandle.Pin = GPIO_PIN_4; 
    GPIOHandle.Mode = GPIO_MODE_IT_FALLING;
    GPIOHandle.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIOHandle);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    /* BUTTON INT PIN */
    GPIOHandle.Pin = BUTTON_PIN;
    GPIOHandle.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIOHandle.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIOHandle);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    return 0;
}

void bsp_gpio_write(GPIO_TypeDef *port, uint16_t pin, uint8_t state) {
    if (state)
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

uint8_t bsp_gpio_read(GPIO_TypeDef *port, uint16_t pin) {
    return HAL_GPIO_ReadPin(port, pin);
}

void bsp_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    
    // Roteia a interrupção: Foi o botão?
    if (GPIO_Pin == BUTTON_PIN) 
        bsp_btn_exti_flag = 1;
    
    
    // else if (GPIO_Pin == ADXL_INT_PIN) {
    //     bsp_adxl_exti_flag = true;
    // }
}
