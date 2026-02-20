/**
* @file    uart.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-02-20
*/
#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_def.h"

UART_HandleTypeDef hlpuart1;

uint8_t uart_init(void) {
    hlpuart1.Instance = LPUART1;
    hlpuart1.Init.BaudRate = 115200;
    hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
    hlpuart1.Init.StopBits = UART_STOPBITS_1;
    hlpuart1.Init.Parity = UART_PARITY_NONE;
    hlpuart1.Init.Mode = UART_MODE_TX_RX;
    hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
    if (HAL_UART_Init(&hlpuart1) != 0) return 1;

    return 0;
}

uint8_t uart_deinit(void) {
    if (HAL_UART_DeInit(&hlpuart1) != 0) return 1;
    return 0;  
}

void uart_write(uint8_t *buf, uint16_t len) {
    HAL_UART_Transmit(&hlpuart1, buf, len, HAL_MAX_DELAY);
}
