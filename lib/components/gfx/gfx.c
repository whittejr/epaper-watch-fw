/**
* @file    gfx.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-04-21
*/

#include "gfx.h"
#include <string.h>
#include <stddef.h>

void gfx_init(gfx_context_t *ctx, uint8_t *buffer, uint16_t w, uint16_t h, uint8_t inverted) {
    if (ctx == NULL || buffer == NULL) return;

    ctx->buffer = buffer;
    ctx->width = w;
    ctx->height = h;
    ctx->inverted = inverted;
}

void gfx_clear(gfx_context_t *ctx, uint8_t color) {
    if (ctx == NULL || ctx->buffer == NULL) return;

    // Se o bit for 1, preenche com 0xFF, se 0, com 0x00
    uint8_t fill_val = color ? 0xFF : 0x00;
    uint32_t buffer_size = (ctx->width * ctx->height) / 8;
    memset(ctx->buffer, fill_val, buffer_size);
}

void gfx_draw_pixel(gfx_context_t *ctx, uint16_t x, uint16_t y, uint8_t color) {
    if (x >= ctx->width || y >= ctx->height) return;

    uint32_t byte_index = (y * (ctx->width / 8)) + (x / 8);
    uint8_t bit_mask = 1 << (7 - (x % 8));

    if (color) {
        ctx->buffer[byte_index] |= bit_mask;
    } else {
        ctx->buffer[byte_index] &= ~bit_mask;
    }
}

void gfx_draw_rect(gfx_context_t *ctx, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color) {
    if (ctx == NULL) return;

    for (uint16_t i = 0; i < h; i++) {
        for (uint16_t j = 0; j < w; j++) {
            gfx_draw_pixel(ctx, x + j, y + i, color);
        }
    }
}

void gfx_write_char(gfx_context_t *ctx, uint16_t x, uint16_t y, char ch, FontDef font, uint8_t color) {
    if (ctx == NULL || ch < 32 || ch > 126) return;

    uint32_t b;
    for (uint16_t i = 0; i < font.FontHeight; i++) {
        b = font.data[(ch - 32) * font.FontHeight + i];
        
        for (uint16_t j = 0; j < font.FontWidth; j++) {
            if ((b << j) & 0x8000) {
                gfx_draw_pixel(ctx, x + j, y + i, color);
            } else {
                gfx_draw_pixel(ctx, x + j, y + i, !color);
            }
        }
    }
}

void gfx_write_string(gfx_context_t *ctx, uint16_t x, uint16_t y, const char *str, FontDef font, uint8_t color) {
    if (ctx == NULL || str == NULL) return;

    uint16_t current_x = x;

    while (*str) {
        if (current_x + font.FontWidth > ctx->width) {
            current_x = 0;
            y += font.FontHeight;
        }
        
        if (y + font.FontHeight > ctx->height) {
            break;
        }

        gfx_write_char(ctx, current_x, y, *str, font, color);
        
        current_x += font.FontWidth;
        str++;
    }
}

void gfx_draw_bitmap(gfx_context_t *ctx, uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint8_t color) {
    if (ctx == NULL || bitmap == NULL) return;

    int16_t byteWidth = (w + 7) / 8;
    uint8_t byte = 0;

    for (int16_t j = 0; j < h; j++) {
        for (int16_t i = 0; i < w; i++) {
            
            if (i & 7) {
                byte <<= 1;
            } else {
                byte = bitmap[j * byteWidth + i / 8];
            }
            
            if (byte & 0x80) {
                gfx_draw_pixel(ctx, x + i, y + j, color);
            }
        }
    }
}

void gfx_write_char_v(gfx_context_t *ctx, uint16_t x, uint16_t y, char ch, const FONT_INFO *font, uint8_t color) {
    if (ctx == NULL || font == NULL) return;

    if (ch == ' ') {
        for (uint16_t j = 0; j < font->height; j++) {
            for (uint16_t i = 0; i < font->space_width; i++) {
                gfx_draw_pixel(ctx, x + i, y + j, !color);
            }
        }
        return;
    }

    if (ch < font->start_char || ch > font->end_char) return;

    uint8_t char_index = ch - font->start_char;
    const FONT_CHAR_INFO *char_info = &font->descriptors[char_index];
    
    if (char_info->width == 0) return;

    // Clear background first
    for (uint16_t j = 0; j < font->height; j++) {
        for (uint16_t i = 0; i < char_info->width; i++) {
            gfx_draw_pixel(ctx, x + i, y + j, !color);
        }
    }

    gfx_draw_bitmap(ctx, x, y, &font->bitmaps[char_info->offset], char_info->width, font->height, color);
}

void gfx_write_string_v(gfx_context_t *ctx, uint16_t x, uint16_t y, const char *str, const FONT_INFO *font, uint8_t color) {
    if (ctx == NULL || str == NULL || font == NULL) return;

    uint16_t current_x = x;

    while (*str) {
        uint8_t w = font->space_width;
        if (*str != ' ') {
            if (*str >= font->start_char && *str <= font->end_char) {
                w = font->descriptors[*str - font->start_char].width;
            } else {
                w = 0;
            }
        }

        if (current_x + w > ctx->width) {
            current_x = 0;
            y += font->height;
        }
        
        if (y + font->height > ctx->height) {
            break;
        }

        if (w > 0) {
            gfx_write_char_v(ctx, current_x, y, *str, font, color);
            current_x += w;
        }
        str++;
    }
}

uint16_t gfx_get_string_width_v(const char *str, const FONT_INFO *font) {
    if (str == NULL || font == NULL) return 0;
    uint16_t width = 0;
    while (*str) {
        if (*str == ' ') {
            width += font->space_width;
        } else if (*str >= font->start_char && *str <= font->end_char) {
            width += font->descriptors[*str - font->start_char].width;
        }
        str++;
    }
    return width;
}
