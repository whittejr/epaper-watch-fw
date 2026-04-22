/**
 * @file ls013b7dh03_basic.c
 * @brief  High-level basic example implementation
 * @version 1.0.0
 * @author Alessandro Davi
 * @date 2026-04-21
 */

#include "ls013b7dh03_basic.h"

static ls013b7dh03_handle_t gs_handle;

uint8_t ls013b7dh03_basic_init(const uint8_t *fb_pointer) {
    if (fb_pointer == NULL) return 1;

    // Link interface functions
    LS013B7DH03_LINK_INIT(&gs_handle, ls013b7dh03_handle_t);
    LS013B7DH03_LINK_SPI_WRITE(&gs_handle, ls013b7dh03_interface_spi_write);
    LS013B7DH03_LINK_CS_CONTROL(&gs_handle, ls013b7dh03_interface_cs_control);
    LS013B7DH03_LINK_DELAY_MS(&gs_handle, ls013b7dh03_interface_delay_ms);

    gs_handle.buffer = fb_pointer;

    return ls013b7dh03_init(&gs_handle);
}

uint8_t ls013b7dh03_basic_deinit(void) {
    return ls013b7dh03_deinit(&gs_handle);
}

uint8_t ls013b7dh03_basic_clear(void) {
    return ls013b7dh03_clear(&gs_handle);
}

uint8_t ls013b7dh03_basic_refresh(void) {
    return ls013b7dh03_refresh(&gs_handle);
}

uint8_t ls013b7dh03_basic_write_raw_frame(const uint8_t *frame) {
    return ls013b7dh03_write_raw_frame(&gs_handle, frame);
}
