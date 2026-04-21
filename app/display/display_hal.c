#include "display_hal.h"
#include "ls013b7dh03_basic.h"
#include "gfx.h"
#include <string.h>

static uint8_t fb_ram[LS013B7DH03_BUFFER_SIZE] = {0};
static gfx_context_t gfx;

uint8_t display_init(void) {
    if (ls013b7dh03_basic_init(fb_ram) != 0) return 1;
    ls013b7dh03_basic_clear();
    gfx_init(&gfx, fb_ram, 128, 128, 1);
    gfx_clear(&gfx, 1);
    return 0;
}

uint8_t display_clear(void) {
    gfx_clear(&gfx, 1);
    return 0;
}

uint8_t display_write(uint8_t x, uint8_t y, const char *text) {
    gfx_write_string(&gfx, x, y, text, Font_7x10, 0);
    return 0;
}

uint8_t display_draw_pixel(uint8_t x, uint8_t y) {
    gfx_draw_pixel(&gfx, x, y, 0);
    return 0;
}

uint8_t display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h) {
    gfx_draw_bitmap(&gfx, x, y, bitmap, w, h, 0);
    return 0;
}

uint8_t display_update(display_update_mode_t mode) {
    if (ls013b7dh03_basic_refresh() != 0) return 1;
    return 0;
}
