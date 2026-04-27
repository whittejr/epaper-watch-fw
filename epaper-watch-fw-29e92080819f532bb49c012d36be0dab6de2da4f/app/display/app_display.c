/**
 * @file    app_display.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-12-26
 */

#include <stdio.h>
#include "app_display.h"
#include "display_hal.h"
#include "stm32wbxx_hal.h"

uint8_t app_display_init(void) {
    display_init();

    return 0;
}

uint8_t app_display_clear(void) {
    display_clear();

    return 0;
}

uint8_t app_display_draw_text(uint8_t x, uint8_t y, const char *text) {
    display_write(x, y, text);

    return 0;
}

uint8_t app_display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h) {
    display_draw_bitmap(x, y, bitmap, w, h);
    return 0;
}

uint8_t app_display_update(display_update_mode_t mode) {
    display_update(mode);
    
    return 0;
}


uint8_t app_display_draw_pixel(uint8_t x, uint8_t y) {
    return display_draw_pixel(x, y);
}

void app_display_draw_text_aligned(uint16_t x, uint16_t y, uint8_t align_center, const char *text) {
    uint16_t final_x = x;
    
    if (align_center) {
        // Multiplica a quantidade de letras por 8 (que é a largura de cada letra na FONT_16)
        uint16_t text_width = strlen(text) * 8; 
        
        // Puxa o X para trás pela metade do tamanho da palavra
        final_x = x - (text_width / 2);
    }
    
    app_display_draw_text(final_x, y, text);
}
