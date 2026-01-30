/**
 * @file display_hal.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-01-30
 */

#ifndef DISPLAY_HAL_H
#define DISPLAY_HAL_H

#include <stdint.h>

uint8_t display_init();

uint8_t display_clear();

uint8_t display_update();

uint8_t display_write();


#endif // DISPLAY_HAL_H