/**
 * @file display_hal.h
 * @brief  Hardware Abstraction Layer for the LS013B7DH03 Display
 * @version 1.0.0
 * @author Alessandro Davi
 * @date 2026-04-21
 */

#ifndef DISPLAY_HAL_H
#define DISPLAY_HAL_H

#include <stdint.h>

/**
 * @brief Generic display update modes
 */
typedef enum {
    DISPLAY_UPDATE_NORMAL = 0,
    DISPLAY_UPDATE_PARTIAL
} display_update_mode_t;

typedef struct {
    uint8_t width;      
    uint16_t offset;    
} FONT_CHAR_INFO;

typedef struct {
    uint8_t height;      
    char start_char;     
    char end_char;       
    uint8_t space_width; 
    const FONT_CHAR_INFO* char_info; 
    const uint8_t* data;             
} FONT_INFO;

uint8_t display_init(void);
uint8_t display_clear(void);
uint8_t display_update(display_update_mode_t mode);
uint8_t display_write(uint8_t x, uint8_t y, const char *text);
uint8_t display_draw_pixel(uint8_t x, uint8_t y);
uint8_t display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h);

#endif // DISPLAY_HAL_H
