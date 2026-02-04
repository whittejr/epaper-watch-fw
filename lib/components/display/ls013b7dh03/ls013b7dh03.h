/**
 * @file ls013b7dh03.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-01-27
 */

#ifndef LS013B7DH03_H
#define LS013B7DH03_H

#include <stdint.h>
#include "shmfonts.h"



/**
 * @brief ls013b7dh03 handle structure definition
 */
typedef struct ls013b7dh03_handle_t
{
    uint8_t (*spi_init)(void);                           
    uint8_t (*spi_deinit)(void);                         
    uint8_t (*spi_write)(uint8_t *buf, uint16_t len);
    uint8_t (*spi_write_refresh)(uint8_t *buf, uint16_t len);    
    uint8_t (*gpio_init)(void);
    void (*cs_control)(uint8_t state);
    void (*delay_ms)(uint32_t ms); 
    
    uint8_t *framebuffer;
    uint16_t width;
    uint16_t height;
} ls013b7dh03_handle_t;


#define LS013B7DH03_WIDTH       128
#define LS013B7DH03_HEIGHT      128
#define LS013B7DH03_BUFFER_SIZE (LS013B7DH03_WIDTH * LS013B7DH03_HEIGHT / 8)

#define SHARPMEM_BIT_WRITECMD (0x01)     // 0x80 in LSB format
#define SHARPMEM_BIT_VCOM 	  (0x02)     // 0x40 in LSB format
#define SHARPMEM_BIT_CLEAR    (0x04)    // 0x20 in LSB format

// Enumeration for screen colors
typedef enum {
    LS_COLOR_BLACK = 0x00, // Black color, no pixel
    LS_COLOR_WHITE = 0x01  // Pixel is set. Color depends on OLED
} ls_color_t;

typedef enum {
    LS_CS_DESELECT = 0,
    LS_CS_SELECT   = 1
} ls_cs_state;

/*
* LINKERS
*/
#define DRIVER_LS013B7DH03_LINK_INIT(HANDLE, STRUCTURE)             memset(HANDLE, 0, sizeof(STRUCTURE))

#define DRIVER_LS013B7DH03_LINK_SPI_INIT(HANDLE, FUC)               (HANDLE)->spi_init = FUC

#define DRIVER_LS013B7DH03_LINK_SPI_DEINIT(HANDLE, FUC)             (HANDLE)->spi_deinit = FUC

#define DRIVER_LS013B7DH03_LINK_GPIO_INIT(HANDLE, FUC)              (HANDLE)->gpio_init = FUC

#define DRIVER_LS013B7DH03_LINK_SPI_WRITE(HANDLE, FUC)              (HANDLE)->spi_write = FUC

#define DRIVER_LS013B7DH03_LINK_SPI_WRITE_REFRESH(HANDLE, FUC)      (HANDLE)->spi_write_refresh = FUC

#define DRIVER_LS013B7DH03_LINK_CS_CONTROL(HANDLE, FUC)             (HANDLE)->cs_control = FUC
    
#define DRIVER_LS013B7DH03_LINK_DELAY_MS(HANDLE, FUC)               (HANDLE)->delay_ms = FUC




uint8_t ls013b7dh03_init(ls013b7dh03_handle_t *handle);
uint8_t ls013b7dh03_clear(ls013b7dh03_handle_t *handle);
uint8_t ls013b7dh03_refresh(ls013b7dh03_handle_t *handle);
void ls013b7dh03_drawPixel(ls013b7dh03_handle_t *handle, uint8_t x, uint8_t y, ls_color_t color);
void ls013b7dh03_test(ls013b7dh03_handle_t *handle);
void ls013b7dh03_split_horizontal(ls013b7dh03_handle_t *handle);

#endif // LS013B7DH03_H
