/**
 * @file driver_ls013b7dh03.c
 * @brief  ls013b7dh03 display driver core implementation
 * @version 1.0.0
 * @author Alessandro Davi
 * @date 2026-04-21
 */

#include "ls013b7dh03.h"

#define BIT_WRITECMD (0x01)     // 0x80 reversed
#define BIT_VCOM     (0x02)     // 0x40 reversed
#define BIT_CLEAR    (0x04)     // 0x20 reversed

/**
 * @brief  Reverse byte bits for Sharp LCD (LSB-First)
 */
static uint8_t a_ls013b7dh03_reverse_byte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

uint8_t ls013b7dh03_init(ls013b7dh03_handle_t *handle) {
    if (handle == NULL) return 1;
    if (handle->spi_write == NULL || handle->cs_control == NULL) return 1;

    handle->vcom = 0;
    handle->inited = 1;
    return 0;
}

uint8_t ls013b7dh03_deinit(ls013b7dh03_handle_t *handle) {
    if (handle == NULL) return 1;
    handle->inited = 0;
    return 0;
}

uint8_t ls013b7dh03_clear(ls013b7dh03_handle_t *handle) {
    if (handle == NULL || !handle->inited) return 1;

    uint8_t buf[2] = {a_ls013b7dh03_reverse_byte(BIT_CLEAR | handle->vcom), 0x00};

    handle->cs_control(1);
    uint8_t res = handle->spi_write(buf, 2);
    handle->cs_control(0);

    return res;
}

uint8_t ls013b7dh03_refresh(ls013b7dh03_handle_t *handle) {
    if (handle == NULL || !handle->inited || handle->buffer == NULL) return 1;

    uint8_t linebuf[18];
    uint8_t dummy = 0x00;

    // VCOM is handled by hardware (EXTCOMIN pin via LPTIM1)
    // keep bit 0 in the command byte.
    handle->vcom = 0; 
    uint8_t cmd = a_ls013b7dh03_reverse_byte(BIT_WRITECMD | handle->vcom);

    handle->cs_control(1);
    
    if (handle->spi_write(&cmd, 1) != 0) {
        handle->cs_control(0);
        return 1;
    }

    for (uint8_t line = 1; line <= LS013B7DH03_HEIGHT; line++) {
        linebuf[0] = a_ls013b7dh03_reverse_byte(line);
        memcpy(&linebuf[1], &handle->buffer[16 * (line - 1)], 16);
        linebuf[17] = 0x00; 

        if (handle->spi_write(linebuf, 18) != 0) {
            handle->cs_control(0);
            return 1;
        }
    }

    handle->spi_write(&dummy, 1);
    handle->cs_control(0);

    return 0;
}

uint8_t ls013b7dh03_refresh_partial(ls013b7dh03_handle_t *handle, uint8_t start_line, uint8_t num_lines) {
    if (handle == NULL || !handle->inited || handle->buffer == NULL) return 1;
    if (start_line < 1 || (start_line + num_lines - 1) > LS013B7DH03_HEIGHT) return 1;

    uint8_t linebuf[18];
    uint8_t dummy = 0x00;

    // VCOM is handled by hardware (EXTCOMIN pin via LPTIM1)
    handle->vcom = 0;
    uint8_t cmd = a_ls013b7dh03_reverse_byte(BIT_WRITECMD | handle->vcom);

    handle->cs_control(1);
    
    if (handle->spi_write(&cmd, 1) != 0) {
        handle->cs_control(0);
        return 1;
    }

    for (uint8_t i = 0; i < num_lines; i++) {
        uint8_t current_line = start_line + i;
        linebuf[0] = a_ls013b7dh03_reverse_byte(current_line);
        memcpy(&linebuf[1], &handle->buffer[16 * (current_line - 1)], 16);
        linebuf[17] = 0x00; 

        if (handle->spi_write(linebuf, 18) != 0) {
            handle->cs_control(0);
            return 1;
        }
    }

    handle->spi_write(&dummy, 1);
    handle->cs_control(0);

    return 0;
}
