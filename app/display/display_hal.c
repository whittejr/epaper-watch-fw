#include "display_hal.h"
#include "ls013b7dh03_basic.h"
#include "gfx.h"
#include "app_state.h"
#include <stdbool.h>
#include <string.h>

static uint8_t fb_ram[LS013B7DH03_BUFFER_SIZE] = {0};
static gfx_context_t gfx;

// No Sharp Memory LCD: 1 = Branco (Limpo), 0 = Preto (Pintado)
#define COLOR_WHITE 1
#define COLOR_BLACK 0

uint8_t display_init(void) {
    if (ls013b7dh03_basic_init(fb_ram) != 0) return 1;
    ls013b7dh03_basic_clear();
    gfx_init(&gfx, fb_ram, 128, 128, 0);
    display_clear();
    return 0;
}

uint8_t display_clear(void) {
    // Se for DARK, o fundo deve ser PRETO (0)
    // Se for LIGHT, o fundo deve ser BRANCO (1)
    uint8_t bg_color = g_watch_settings.dark_theme ? COLOR_BLACK : COLOR_WHITE;
    gfx_clear(&gfx, bg_color);
    return 0;
}

uint8_t display_set_theme(bool dark) {
    g_watch_settings.dark_theme = dark;
    display_clear();
    return 0;
}

uint8_t display_write(uint8_t x, uint8_t y, const char *text, uint8_t color) {
    // Inverte a cor do texto baseada no tema
    // Se o usuario pede cor 0 (normal), no Light vira Preto (0), no Dark vira Branco (1)
    uint8_t actual_color = g_watch_settings.dark_theme ? !color : color;
    gfx_write_string(&gfx, x, y, text, Font_7x10, actual_color);
    return 0;
}

uint8_t display_write_v(uint16_t x, uint16_t y, const char *text, const FONT_INFO *font, uint8_t color) {
    uint8_t actual_color = g_watch_settings.dark_theme ? !color : color;
    gfx_write_string_v(&gfx, x, y, text, font, actual_color);
    return 0;
}

uint8_t display_draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
    uint8_t actual_color = g_watch_settings.dark_theme ? !color : color;
    gfx_draw_pixel(&gfx, x, y, actual_color);
    return 0;
}

uint8_t display_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
    uint8_t actual_color = g_watch_settings.dark_theme ? !color : color;
    gfx_draw_rect(&gfx, x, y, w, h, actual_color);
    return 0;
}

uint8_t display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color) {
    uint8_t actual_color = g_watch_settings.dark_theme ? !color : color;
    gfx_draw_bitmap(&gfx, x, y, bitmap, w, h, actual_color);
    return 0;
}

uint8_t display_write_raw(const uint8_t *buffer) {
    return ls013b7dh03_basic_write_raw_frame(buffer);
}

uint8_t display_update(display_update_mode_t mode) {
    if (ls013b7dh03_basic_refresh() != 0) return 1;
    return 0;
}
