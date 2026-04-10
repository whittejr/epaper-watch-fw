/**
 * @file app_accel.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-04-07
 */

#ifndef APP_ACCEL_H
#define APP_ACCEL_H

#include <stdint.h>

uint8_t app_accel_init(void);
uint8_t app_accel_read(float g[3]);

#endif // APP_ACCEL_H
