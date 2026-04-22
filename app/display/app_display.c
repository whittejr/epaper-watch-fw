#include "app_display.h"
#include "rtc.h"
#include "icons.h"
#include <string.h>
#include <stdio.h>

uint8_t app_display_init(void) {
    return display_init();
}

uint8_t app_display_clear(void) {
    return display_clear();
}

uint8_t app_display_draw_text(uint8_t x, uint8_t y, const char *text, uint8_t color) {
    return display_write(x, y, text, color);
}

uint8_t app_display_draw_text_v(uint16_t x, uint16_t y, const char *text, const FONT_INFO *font, uint8_t color) {
    return display_write_v(x, y, text, font, color);
}

uint8_t app_display_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
    return display_draw_rect(x, y, w, h, color);
}

uint16_t app_display_get_string_width(const char *text) {
    return strlen(text) * 7; // Font_7x10 width
}

extern uint16_t gfx_get_string_width_v(const char *str, const FONT_INFO *font);
uint16_t app_display_get_string_width_v(const char *text, const FONT_INFO *font) {
    return gfx_get_string_width_v(text, font);
}

uint8_t app_display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color) {
    return display_draw_bitmap(x, y, bitmap, w, h, color);
}

uint8_t app_display_write_raw(const uint8_t *buffer) {
    return display_write_raw(buffer);
}

uint8_t app_display_update(display_update_mode_t mode) {
    return display_update(mode);
}

void app_display_draw_text_aligned(uint16_t x, uint16_t y, uint8_t align_center, const char *text, uint8_t color) {
    uint16_t final_x = x;
    if (align_center) {
        uint16_t text_width = app_display_get_string_width(text); 
        final_x = x - (text_width / 2);
    }
    app_display_draw_text(final_x, y, text, color);
}

void app_display_draw_text_aligned_v(uint16_t x, uint16_t y, uint8_t align_center, const char *text, const FONT_INFO *font, uint8_t color) {
    uint16_t final_x = x;
    if (align_center) {
        uint16_t text_width = app_display_get_string_width_v(text, font); 
        final_x = x - (text_width / 2);
    }
    app_display_draw_text_v(final_x, y, text, font, color);
}

uint8_t app_display_draw_battery(uint8_t x, uint8_t y, uint8_t level) {
    // Outer shell (white on black background of status bar)
    app_display_draw_rect(x, y, 14, 7, 1);
    app_display_draw_rect(x + 14, y + 2, 1, 3, 1); // Tip
    
    // Fill (black on white shell)
    uint8_t fill_w = (level * 12) / 100;
    if (fill_w > 12) fill_w = 12;
    
    app_display_draw_rect(x + 1, y + 1, fill_w, 5, 0);
    
    return 0;
}

void app_display_draw_status_bar(void) {
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    char time_buf[8];

    // Background tarja preta
    app_display_draw_rect(0, 0, 128, 11, 0);

    // Time (left)
    rtc_get_time(&sTime, &sDate);
    snprintf(time_buf, sizeof(time_buf), "%02d:%02d", sTime.Hours, sTime.Minutes);
    app_display_draw_text(2, 1, time_buf, 1); // White text
    
    
    // Battery (right)
    app_display_draw_battery(110, 2, 85); // Mock 85%
}
