/**
 * @file display_hal.h
 * @brief  Hardware Abstraction Layer for the LS013B7DH03 Display
 */

#ifndef DISPLAY_HAL_H
#define DISPLAY_HAL_H

#include <stdint.h>

/**
 * @brief Display update modes for the e-paper/MIP display.
 */
typedef enum {
    DISPLAY_UPDATE_NORMAL = 0,
    DISPLAY_UPDATE_PARTIAL
} display_update_mode_t;

/**
 * @brief Font structures for specialized bitmapped fonts.
 */
typedef struct {
    uint8_t width;
    uint16_t offset;
} FONT_CHAR_INFO;

typedef struct {
    uint8_t height;
    char start_char;
    char end_char;
    uint8_t space_width;
    const FONT_CHAR_INFO *descriptors;
    const uint8_t *bitmaps;
} FONT_INFO;

/* --- System Interface --- */
uint8_t display_init(void);
uint8_t display_clear(void);
uint8_t display_update(display_update_mode_t mode);

/* --- Basic Primitives --- */
uint8_t display_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
uint8_t display_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
uint8_t display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);

/* --- Text Primitives --- */
uint8_t display_write(uint8_t x, uint8_t y, const char *text, uint8_t color);
uint8_t display_write_v(uint16_t x, uint16_t y, const char *text, const FONT_INFO *font, uint8_t color);

/* --- Specialized Interface --- */
uint8_t display_write_raw(const uint8_t *buffer);

#endif // DISPLAY_HAL_H
