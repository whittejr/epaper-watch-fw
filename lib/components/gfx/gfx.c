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


    uint8_t actual_color = ctx->inverted ? !color : color;
    

    uint32_t buffer_size = (ctx->width * ctx->height) / 8;

    if (actual_color) {
        memset(ctx->buffer, 0xFF, buffer_size); 
    } else {
        memset(ctx->buffer, 0x00, buffer_size);
    }
}

void gfx_draw_pixel(gfx_context_t *ctx, uint16_t x, uint16_t y, uint8_t color) {
    if (ctx == NULL || ctx->buffer == NULL) return;
    
    if (x >= ctx->width || y >= ctx->height) return;

    uint16_t byte_index = (y * (ctx->width / 8)) + (x / 8);
    uint8_t bit_mask = 1 << (7 - (x % 8)); // Assumindo MSB-first dentro do byte

    uint8_t actual_color = ctx->inverted ? !color : color;

    if (actual_color) {
        ctx->buffer[byte_index] |= bit_mask;
    } else {
        ctx->buffer[byte_index] &= ~bit_mask;
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