/**
 * @file ls013b7dh03.h
 * @brief  ls013b7dh03 display driver core
 * @version 1.0.0
 * @author Alessandro Davi
 * @date 2026-04-21
 */

#ifndef LS013B7DH03_H
#define LS013B7DH03_H

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

#define LS013B7DH03_WIDTH            128        
#define LS013B7DH03_HEIGHT           128        
#define LS013B7DH03_BUFFER_SIZE      ((LS013B7DH03_WIDTH * LS013B7DH03_HEIGHT) / 8)

/**
 * @brief ls013b7dh03 handle structure
 */
typedef struct ls013b7dh03_handle_s
{
    uint8_t (*spi_write)(uint8_t *buf, uint16_t len);              /**< SPI write function */
    void (*cs_control)(uint8_t state);                             /**< CS pin control function */
    void (*delay_ms)(uint32_t ms);                                 /**< Delay function */
    
    uint8_t inited;                                                /**< inited flag */
    uint8_t vcom;                                                  /**< internal vcom state */
    const uint8_t *buffer;                                         /**< pointer to external frame buffer */
} ls013b7dh03_handle_t;

/**
 * @brief ls013b7dh03 linkers
 */
#define LS013B7DH03_LINK_INIT(HANDLE, STRUCTURE)    memset(HANDLE, 0, sizeof(STRUCTURE))
#define LS013B7DH03_LINK_SPI_WRITE(HANDLE, FUC)     (HANDLE)->spi_write = FUC
#define LS013B7DH03_LINK_CS_CONTROL(HANDLE, FUC)    (HANDLE)->cs_control = FUC
#define LS013B7DH03_LINK_DELAY_MS(HANDLE, FUC)      (HANDLE)->delay_ms = FUC

/**
 * @brief  Initialize the driver
 * @param  handle pointer to ls013b7dh03 handle
 * @return 0: success, 1: error
 */
uint8_t ls013b7dh03_init(ls013b7dh03_handle_t *handle);

/**
 * @brief  Deinitialize the driver
 * @param  handle pointer to ls013b7dh03 handle
 * @return 0: success, 1: error
 */
uint8_t ls013b7dh03_deinit(ls013b7dh03_handle_t *handle);

/**
 * @brief  Clear the physical display via hardware command
 * @param  handle pointer to ls013b7dh03 handle
 * @return 0: success, 1: error
 */
uint8_t ls013b7dh03_clear(ls013b7dh03_handle_t *handle);

/**
 * @brief  Full display refresh from handle->buffer
 * @param  handle pointer to ls013b7dh03 handle
 * @return 0: success, 1: error
 */
uint8_t ls013b7dh03_refresh(ls013b7dh03_handle_t *handle);

/**
 * @brief  Write a raw frame buffer directly to display (bypass handle->buffer)
 * @param  handle pointer to ls013b7dh03 handle
 * @param  frame pointer to raw 2048 bytes frame
 * @return 0: success, 1: error
 */
uint8_t ls013b7dh03_write_raw_frame(ls013b7dh03_handle_t *handle, const uint8_t *frame);

/**
 * @brief  Partial display refresh from handle->buffer
 * @param  handle pointer to ls013b7dh03 handle
 * @param  start_line line index (1-128)
 * @param  num_lines number of lines to refresh
 * @return 0: success, 1: error
 */
uint8_t ls013b7dh03_refresh_partial(ls013b7dh03_handle_t *handle, uint8_t start_line, uint8_t num_lines);

#ifdef __cplusplus
}
#endif

#endif
