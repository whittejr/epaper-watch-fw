/**
* @file    display_hal.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-01-30
*/
#include <string.h>
#include "display_hal.h"
#include "ls013b7dh03.h"
#include "ls013b7dh03_interface.h"


static ls013b7dh03_handle_t sharp_handle;
static uint8_t sharp_video_buffer[128 * 128 / 8];

uint8_t display_init() {
    uint8_t res;

    DRIVER_LS013B7DH03_LINK_INIT(&sharp_handle, ls013b7dh03_handle_t);
    DRIVER_LS013B7DH03_LINK_SPI_INIT(&sharp_handle, ls013b7dh03_interface_spi_init);
    DRIVER_LS013B7DH03_LINK_SPI_DEINIT(&sharp_handle, ls013b7dh03_interface_spi_deinit);
    DRIVER_LS013B7DH03_LINK_SPI_WRITE(&sharp_handle, ls013b7dh03_interface_spi_write);
    DRIVER_LS013B7DH03_LINK_SPI_WRITE_REFRESH(&sharp_handle, ls013b7dh03_interface_spi_write_refresh);
    DRIVER_LS013B7DH03_LINK_CS_CONTROL(&sharp_handle, ls013b7dh03_interface_cs_control);
    DRIVER_LS013B7DH03_LINK_GPIO_INIT(&sharp_handle, ls013b7dh03_interface_gpio_init);
    DRIVER_LS013B7DH03_LINK_DELAY_MS(&sharp_handle, ls013b7dh03_interface_delay_ms);
    

    sharp_handle.framebuffer = sharp_video_buffer;
    sharp_handle.width = 128;
    sharp_handle.height = 128;

    res = ls013b7dh03_init(&sharp_handle);
    if (res != 0) return 1;   
    
    return 0;
}

uint8_t display_clear() {
    uint8_t res;

    res = ls013b7dh03_clear(&sharp_handle);
    if (res != 0) return 1;

    return 0;
}

uint8_t display_write() {
    
    return 0;
}

uint8_t display_update() {
    ls013b7dh03_refresh(&sharp_handle);

    return 0;
}

uint8_t display_test() {

    DRIVER_LS013B7DH03_LINK_INIT(&sharp_handle, ls013b7dh03_handle_t);
    DRIVER_LS013B7DH03_LINK_SPI_INIT(&sharp_handle, ls013b7dh03_interface_spi_init);
    DRIVER_LS013B7DH03_LINK_SPI_DEINIT(&sharp_handle, ls013b7dh03_interface_spi_deinit);
    DRIVER_LS013B7DH03_LINK_SPI_WRITE(&sharp_handle, ls013b7dh03_interface_spi_write);
    DRIVER_LS013B7DH03_LINK_SPI_WRITE_REFRESH(&sharp_handle, ls013b7dh03_interface_spi_write_refresh);
    DRIVER_LS013B7DH03_LINK_CS_CONTROL(&sharp_handle, ls013b7dh03_interface_cs_control);
    DRIVER_LS013B7DH03_LINK_GPIO_INIT(&sharp_handle, ls013b7dh03_interface_gpio_init);
    DRIVER_LS013B7DH03_LINK_DELAY_MS(&sharp_handle, ls013b7dh03_interface_delay_ms);
    

    sharp_handle.framebuffer = sharp_video_buffer;
    sharp_handle.width = 128;
    sharp_handle.height = 128;

    ls013b7dh03_test(&sharp_handle);
}

uint8_t display_split() {
    ls013b7dh03_split_horizontal(&sharp_handle);
}