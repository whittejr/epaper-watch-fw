/**
 * @file app_display.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2025-12-26
 */

#ifndef DRIVER_SSD1681_BASIC_H
#define DRIVER_SSD1681_BASIC_H

#include "display_hal.h"
#include "stdint.h"

#include "ssd1681.h"

uint8_t app_display_init(void);
uint8_t app_display_clear(void);
uint8_t app_display_draw_text(uint8_t x, uint8_t y, const char *text);
uint8_t app_display_update(display_update_mode_t mode);

#endif // DRIVER_SSD1681_BASIC_H
