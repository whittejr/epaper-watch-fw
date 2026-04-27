/**
 * @file uart.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-02-20
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>

uint8_t uart_init(void);
uint8_t uart_deinit(void);
void uart_write(uint8_t *buf, uint16_t len);

#endif // UART_H
