/**
* @file    ls013b7dh03.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-01-27
*/

// #include "shm_graphics.h"
#include "ls013b7dh03.h"
#include "display_hal.h"
#include "resources.h"
#include "shmfonts.h"
#include <string.h>

/*
*   FUNCTIONS
*/
static uint8_t reverse_byte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

uint8_t ls013b7dh03_init(ls013b7dh03_handle_t *handle) {
	/* verify */
	if (handle->spi_init == NULL) return 1;
	if (handle->spi_deinit == NULL) return 1;
	if (handle->spi_write == NULL) return 1;
	if (handle->gpio_init == NULL) return 1;

	/* init */
    // if (handle->gpio_init() != 0) return 1;
    // handle->delay_ms(5);
    // if (handle->spi_init() != 0) return 1;
    
    
    if (handle->width == 0) handle->width = LS013B7DH03_WIDTH;
    if (handle->height == 0) handle->height = LS013B7DH03_HEIGHT;
    
    // allow display to stabilize before initialize communication (30us)
    handle->delay_ms(1);

    // ls013b7dh03_clear(handle);
    // ls013b7dh03_clear(handle);

	return 0;
}


uint8_t ls013b7dh03_clear(ls013b7dh03_handle_t *handle) {    
    if (handle == NULL) return 1;

    /* 
    * CLEAR command without VCOM bit.
    * VCOM is provided externally via EXTCOMIN pin.
    */
    /* ATT -> testing EXTMODE = L */
    uint8_t cmd = SHARPMEM_BIT_CLEAR | 0x40;
    uint8_t dummy = 0x00;

    handle->cs_control(LS_CS_SELECT);
    handle->delay_ms(1);

    handle->spi_write(&cmd, 1);
    handle->spi_write(&dummy, 1);

    handle->delay_ms(1);
    handle->cs_control(LS_CS_DESELECT);

    for (int i = 0; i < 2048; i++) {
    if (i < 1024) {
        // Primeira metade (Linhas 0 a 63) -> Branco
        handle->framebuffer[i] = 0x00; 
    } else {
        // Segunda metade (Linhas 64 a 127) -> Preto
        handle->framebuffer[i] = 0xFF; 
    }
}
    // memset(handle->framebuffer, 0x00, LS013B7DH03_BUFFER_SIZE);

    return 0;
}

void ls013b7dh03_drawPixel(ls013b7dh03_handle_t *handle, uint8_t x, uint8_t y, ls_color_t color) {
    if (handle == NULL || handle->framebuffer == NULL) return;
    if ((x < 0) || (x >= handle->width) || (y < 0) || (y >= handle->height)) return;

    // Find the right byte: (line * 16) + (column / 8)
    uint16_t byte_idx = (y * (handle->width / 8)) + (x / 8);

    uint8_t bit_mask = 0x80 >> (x & 7);

    if (color == LS_COLOR_WHITE) 
        handle->framebuffer[byte_idx] |= bit_mask;
    else 
        handle->framebuffer[byte_idx] &= ~bit_mask;
}

/*
* @brief       
* @param[in]   
* @return      
*            - 
*            - 
* @note       
*/
uint8_t ls013b7dh03_refresh(ls013b7dh03_handle_t *handle) {
    if (handle == NULL) return 1;

    // handle->vcom_state = (handle->vcom_state == SHARPMEM_BIT_VCOM) ? 0x00 : SHARPMEM_BIT_VCOM;

    uint8_t cmd = SHARPMEM_BIT_WRITECMD | 0x40;
    uint8_t dummy = 0x00;

    uint8_t line_buffer[32]; // array to invert the 16 bytes before sending via SPI

    handle->cs_control(LS_CS_SELECT);
    handle->delay_ms(1);
    handle->spi_write(&cmd, 1);

    uint8_t bytes_per_line = handle->width / 8;
    if (bytes_per_line > sizeof(line_buffer)) return 1; // verify bytes_per_line is bigger than the line_buffer

    for (uint8_t line = 1; line <= handle->height; line++) {
        uint8_t line_addr = reverse_byte(line); 
        handle->spi_write(&line_addr, 1); // sends line addr

        
        uint8_t *src_ptr = &handle->framebuffer[(line - 1) * bytes_per_line];
        for (uint8_t i = 0; i < bytes_per_line; i++)
            line_buffer[i] = reverse_byte(src_ptr[i]);
        
        handle->spi_write(line_buffer, bytes_per_line); // inverted data
        handle->spi_write(&dummy, 1);
    }
    handle->spi_write(&dummy, 1);
    handle->delay_ms(1);

    handle->cs_control(LS_CS_DESELECT);

    return 0;
}

void ls013b7dh03_test(ls013b7dh03_handle_t *handle)
{
    if (handle == NULL) return;

    // 1. Init
    ls013b7dh03_init(handle);

    // --------------------------------------------------
    // TESTE 1: TELA DIVIDIDA (ESQUERDA PRETA / DIREITA BRANCA)
    // --------------------------------------------------
    
    // Limpa buffer para Branco
    ls013b7dh03_clear(handle);

    for (uint8_t y = 0; y < handle->height; y++) {
        for (uint8_t x = 0; x < handle->width; x++) {
            if (x < 64) {
                ls013b7dh03_drawPixel(handle, x, y, LS_COLOR_BLACK);
            } else {
                ls013b7dh03_drawPixel(handle, x, y, LS_COLOR_WHITE);
            }
        }
    }
    ls013b7dh03_refresh(handle);

    if(handle->delay_ms) handle->delay_ms(2000);
    
    ls013b7dh03_clear(handle);

    for (uint8_t i = 0; i < 128; i++) {
        ls013b7dh03_drawPixel(handle, i, 0, LS_COLOR_BLACK);
        ls013b7dh03_drawPixel(handle, i, 127, LS_COLOR_BLACK);
        
        ls013b7dh03_drawPixel(handle, 0, i, LS_COLOR_BLACK);
        ls013b7dh03_drawPixel(handle, 127, i, LS_COLOR_BLACK);

 
        ls013b7dh03_drawPixel(handle, i, i, LS_COLOR_BLACK);
    }
    
    ls013b7dh03_refresh(handle);
}


void ls013b7dh03_test1(ls013b7dh03_handle_t *handle) {
    ls013b7dh03_init(handle);

    ls013b7dh03_clear(handle);

    // ls013b7dh03_drawPixel(handle, 10, 10, LS_COLOR_BLACK);

    ls013b7dh03_refresh(handle);
}

void ls013b7dh03_vcom_control(ls013b7dh03_handle_t *handle) {
    if (handle == NULL) return;

    // 1. Toggle do VCOM (O MESMO do refresh)
    handle->vcom_state = (handle->vcom_state == SHARPMEM_BIT_VCOM) ? 0x00 : SHARPMEM_BIT_VCOM;

    // 2. Comando: Apenas VCOM (sem bit de escrita)
    // 0x00 (No-Op) | VCOM
    uint8_t cmd = reverse_byte(0x00 | handle->vcom_state);
    
    uint8_t dummy = 0x00;

    // 3. Envio rápido
    handle->cs_control(LS_CS_SELECT);
    handle->delay_ms(1); // ideal: delay_us(6)
    
    handle->spi_write(&cmd, 1);
    handle->spi_write(&dummy, 1); // Trailer 8-bit
    
    handle->delay_ms(1); // ideal: delay_us(2)
    handle->cs_control(LS_CS_DESELECT);
}