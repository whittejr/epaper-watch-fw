/**
* @file    app_display.c
* @brief   Implementation of High-level Seiko UI Toolkit
*/

#include "app_display.h"
#include "rtc.h"
#include <string.h>
#include <stdio.h>

uint8_t app_display_init(void) {
    return display_init();
}

uint16_t app_display_get_string_width(const char *text) {
    return strlen(text) * 7; // Default Font_7x10 width
}

extern uint16_t gfx_get_string_width_v(const char *str, const FONT_INFO *font);
uint16_t app_display_get_string_width_v(const char *text, const FONT_INFO *font) {
    return gfx_get_string_width_v(text, font);
}

void app_display_draw_text_aligned(uint16_t x, uint16_t y, uint8_t align_center, const char *text, uint8_t color) {
    uint16_t final_x = x;
    if (align_center) {
        uint16_t text_width = app_display_get_string_width(text); 
        final_x = x - (text_width / 2);
    }
    display_write(final_x, y, text, color);
}

void app_display_draw_text_aligned_v(uint16_t x, uint16_t y, uint8_t align_center, const char *text, const FONT_INFO *font, uint8_t color) {
    uint16_t final_x = x;
    if (align_center) {
        uint16_t text_width = app_display_get_string_width_v(text, font); 
        final_x = x - (text_width / 2);
    }
    display_write_v(final_x, y, text, font, color);
}

void app_display_draw_battery(uint8_t x, uint8_t y, uint8_t level) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%d%%", level);
    display_write(x, y, buf, 0);
}

void app_display_draw_status_bar(void) {
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    char time_buf[8];

    // Seiko-style top hairline
    display_draw_rect(0, 12, 128, 1, 0);

    // Time (left)
    rtc_get_time(&sTime, &sDate);
    snprintf(time_buf, sizeof(time_buf), "%02d:%02d", sTime.Hours, sTime.Minutes);
    display_write(4, 2, time_buf, 0);
    
    // Battery (right)
    app_display_draw_battery(102, 2, 85); 
}
