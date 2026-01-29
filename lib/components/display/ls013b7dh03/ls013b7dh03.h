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
    uint8_t (*spi_write_cmd)(uint8_t *buf, uint16_t len);
    uint8_t (*spi_write_refresh)(uint8_t *buf, uint16_t len);    
    uint8_t (*gpio_init)(void);
    void (*cs_control)(uint8_t state);
    void (*delay_ms)(uint32_t ms);    
} ls013b7dh03_handle_t;


#define LCD_HEIGHT		    128
#define LCD_WIDTH           128
#define LCD_BUFFER_SIZE     (LCD_HEIGHT * LCD_HEIGHT / 8)

#define SHARPMEM_BIT_WRITECMD (0x01)     // 0x80 in LSB format
#define SHARPMEM_BIT_VCOM 	  (0x02)     // 0x40 in LSB format
#define SHARPMEM_BIT_CLEAR    (0x04)    // 0x20 in LSB format

// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} LCD_COLOR;

// Struct to store display info
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} LCD_128x128_t;

/*
* LINKERS
*/
#define DRIVER_LS013B7DH03_LINK_INIT(HANDLE, STRUCTURE)                            \
    memset(HANDLE, 0, sizeof(STRUCTURE))

#define DRIVER_LS013B7DH03_LINK_SPI_INIT(HANDLE, FUC) (HANDLE)->spi_init = FUC

#define DRIVER_LS013B7DH03_LINK_SPI_DEINIT(HANDLE, FUC) (HANDLE)->spi_deinit = FUC

#define DRIVER_LS013B7DH03_LINK_GPIO_INIT(HANDLE, FUC)                        \
    (HANDLE)->gpio_init = FUC

#define DRIVER_LS013B7DH03_LINK_SPI_WRITE_CMD(HANDLE, FUC)                     \
    (HANDLE)->spi_write_cmd = FUC

#define DRIVER_LS013B7DH03_LINK_SPI_WRITE_REFRESH(HANDLE, FUC)  (HANDLE)->spi_write_refresh = FUC

#define DRIVER_LS013B7DH03_LINK_CS_CONTROL(HANDLE, FUC)        (HANDLE)->cs_control = FUC
    
#define DRIVER_LS013B7DH03_LINK_DELAY_MS(HANDLE, FUC) (HANDLE)->delay_ms = FUC




uint8_t ls013b7dh03_init(ls013b7dh03_handle_t *handle);
uint8_t ls013b7dh03_clear(ls013b7dh03_handle_t *handle);
uint8_t ls013b7dh03_refresh(ls013b7dh03_handle_t *handle);
void ls013b7dh03_drawPixel(uint8_t x, uint8_t y, LCD_COLOR color);
char ls013b7dh03_writeChar(char ch, FontDef Font, LCD_COLOR color);
void ls013b7dh03_test(ls013b7dh03_handle_t *handle);

#endif // LS013B7DH03_H
