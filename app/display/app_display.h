/**
 * @file app_display.h
 * @brief High-level display application interface
 */

#ifndef APP_DISPLAY_H
#define APP_DISPLAY_H

#include "display_hal.h"
#include <stdint.h>

uint8_t app_display_init(void);
uint8_t app_display_clear(void);
uint8_t app_display_draw_text(uint8_t x, uint8_t y, const char *text, uint8_t color);
uint8_t app_display_draw_text_v(uint16_t x, uint16_t y, const char *text, const FONT_INFO *font, uint8_t color);
uint8_t app_display_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void app_display_draw_text_aligned(uint16_t x, uint16_t y, uint8_t align_center, const char *text, uint8_t color);
void app_display_draw_text_aligned_v(uint16_t x, uint16_t y, uint8_t align_center, const char *text, const FONT_INFO *font, uint8_t color);
uint16_t app_display_get_string_width(const char *text);
uint16_t app_display_get_string_width_v(const char *text, const FONT_INFO *font);
uint8_t app_display_draw_battery(uint8_t x, uint8_t y, uint8_t level);
void app_display_draw_status_bar(void);
uint8_t app_display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);
uint8_t app_display_write_raw(const uint8_t *buffer);
uint8_t app_display_update(display_update_mode_t mode);

#endif // APP_DISPLAY_H
