/**
* @file    accel_hal.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-04-07
*/

#include <stdint.h>
#include "adxl362.h"
#include "adxl362_interface.h"
#include "accel_hal.h"

static adxl362_handle_t gs_handle;

uint8_t accel_init(void) {
    uint8_t res;
    
    /* link interface function */
    DRIVER_ADXL362_LINK_INIT(&gs_handle, adxl362_handle_t);
    DRIVER_ADXL362_LINK_SPI_INIT(&gs_handle, adxl362_interface_spi_init);
    DRIVER_ADXL362_LINK_SPI_DEINIT(&gs_handle, adxl362_interface_spi_deinit);
    DRIVER_ADXL362_LINK_SPI_READ(&gs_handle, adxl362_interface_spi_read);
    DRIVER_ADXL362_LINK_SPI_READ_ADDRESS16(&gs_handle, adxl362_interface_spi_read_address16);
    DRIVER_ADXL362_LINK_SPI_WRITE_ADDRESS16(&gs_handle, adxl362_interface_spi_write_address16);
    DRIVER_ADXL362_LINK_DELAY_MS(&gs_handle, adxl362_interface_delay_ms);
    DRIVER_ADXL362_LINK_DEBUG_PRINT(&gs_handle, adxl362_interface_debug_print);
    DRIVER_ADXL362_LINK_RECEIVE_CALLBACK(&gs_handle, adxl362_interface_receive_callback);
    
    /* init the adxl362 */
    res = adxl362_init(&gs_handle);
    if (res != 0) return 1;
    
    /* basic configuration */
    adxl362_set_fifo_mode(&gs_handle, ADXL362_FIFO_MODE_DISABLE);
    adxl362_set_bandwidth(&gs_handle, ADXL362_BASIC_DEFAULT_BANDWIDTH_ODR);
    adxl362_set_range(&gs_handle, ADXL362_RANGE_2G);
    adxl362_set_odr(&gs_handle, ADXL362_ODR_12P5HZ);
    
    /* start measurement */
    res = adxl362_set_mode(&gs_handle, ADXL362_MODE_MEASUREMENT);
    if (res != 0) return 1;
    
    return 0;
}

uint8_t accel_deinit(void) {
    if (adxl362_deinit(&gs_handle) != 0) return 1;
    return 0;
}

uint8_t accel_basic_read(float g[3])  {
    int8_t raw[3];
    // Usando 8msb por ser mais rápido e robusto para o UI
    if (adxl362_read_8msb(&gs_handle, raw, g) != 0) {
        return 1;
    }
    return 0;
}
