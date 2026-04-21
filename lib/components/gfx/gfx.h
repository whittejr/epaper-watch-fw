/**
 * @file gfx.h
 * @brief  Graphics library for monochrome displays
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-04-21
 */

#ifndef GFX_H
#define GFX_H

#include "shmfonts.h"
#include <stdint.h>

typedef struct {
    uint8_t *buffer;      // Frame buffer pointer
    uint16_t width;
    uint16_t height;
    uint8_t inverted;     // 1 for Sharp (1=white)
} gfx_context_t;

void gfx_init(gfx_context_t *ctx, uint8_t *buffer, uint16_t w, uint16_t h, uint8_t inverted);
void gfx_clear(gfx_context_t *ctx, uint8_t color);
void gfx_draw_pixel(gfx_context_t *ctx, uint16_t x, uint16_t y, uint8_t color);
void gfx_draw_bitmap(gfx_context_t *ctx, uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint8_t color);
void gfx_write_char(gfx_context_t *ctx, uint16_t x, uint16_t y, char ch, FontDef font, uint8_t color);
void gfx_write_string(gfx_context_t *ctx, uint16_t x, uint16_t y, const char *str, FontDef font, uint8_t color);

#endif // GFX_H
