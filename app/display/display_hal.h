/**
 * @file display_hal.h
 * @brief  Hardware Abstraction Layer for the LS013B7DH03 Display
 */

#ifndef DISPLAY_HAL_H
#define DISPLAY_HAL_H

#include <stdint.h>

typedef enum {
    DISPLAY_UPDATE_NORMAL = 0,
    DISPLAY_UPDATE_PARTIAL
} display_update_mode_t;

typedef struct {
    uint8_t width;      /*!< Character width in pixels */
    uint16_t offset;    /*!< Offset into bitmap array */
} FONT_CHAR_INFO;

typedef struct {
    uint8_t height;               /*!< Character height in pixels */
    char start_char;              /*!< Start character */
    char end_char;                /*!< End character */
    uint8_t space_width;          /*!< Width of space character in pixels */
    const FONT_CHAR_INFO *descriptors; /*!< Pointer to character descriptors */
    const uint8_t *bitmaps;       /*!< Pointer to bitmap array */
} FONT_INFO;

uint8_t display_init(void);
uint8_t display_clear(void);
uint8_t display_update(display_update_mode_t mode);
uint8_t display_write(uint8_t x, uint8_t y, const char *text, uint8_t color);
uint8_t display_write_v(uint16_t x, uint16_t y, const char *text, const FONT_INFO *font, uint8_t color);
uint8_t display_draw_pixel(uint8_t x, uint8_t y);
uint8_t display_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
uint8_t display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);
uint8_t display_write_raw(const uint8_t *buffer);

#endif // DISPLAY_HAL_H
