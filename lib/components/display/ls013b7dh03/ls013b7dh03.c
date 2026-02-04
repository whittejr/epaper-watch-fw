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
    if (handle->spi_init() != 0) return 1;
    if (handle->gpio_init() != 0) return 1;

    if (handle->width == 0) handle->width = LS013B7DH03_WIDTH;
    if (handle->height == 0) handle->height = LS013B7DH03_HEIGHT;
    
    // allow display to stabilize before initialize communication (30us)
    handle->delay_ms(1);

	return 0;
}


uint8_t ls013b7dh03_clear(ls013b7dh03_handle_t *handle) {    
    if (handle == NULL) return 1;

    memset(handle->framebuffer, 0x00, LS013B7DH03_BUFFER_SIZE);

    /* 
    * CLEAR command without VCOM bit.
    * VCOM is provided externally via EXTCOMIN pin.
    */
    uint8_t cmd[2] = { reverse_byte(SHARPMEM_BIT_CLEAR) , 0x00};

    handle->cs_control(LS_CS_SELECT);
    handle->spi_write(cmd, 2);
    handle->cs_control(LS_CS_DESELECT);

    return 0;
}

void ls013b7dh03_drawPixel(ls013b7dh03_handle_t *handle, uint8_t x, uint8_t y, ls_color_t color) {
    if (handle == NULL || handle->framebuffer == NULL) return;
    if ((x < 0) || (x >= handle->width) || (y < 0) || (y >= handle->height)) return;

    // Find the right byte: (line * 16) + (column / 8)
    uint16_t byte_idx = (y * (handle->width / 8)) + (x / 8);

    // Bit 0 = left pixel, Bit 7 = right pixel (inside the byte)
    uint8_t bit_mask = 1 << (x % 8);

    if (color == LS_COLOR_WHITE) {
        handle->framebuffer[byte_idx] |= bit_mask;  // Set bit (Branco)
    } else {
        handle->framebuffer[byte_idx] &= ~bit_mask; // Clear bit (Preto)
    }
}

void ls013b7dh03_split_horizontal(ls013b7dh03_handle_t *handle) {
    // Percorre todas as linhas (Y)
    for (uint8_t y = 0; y < handle->height; y++) {
        
        // Define a cor baseada na linha atual
        // Se estiver na metade de cima (< 64), é PRETO. Senão, BRANCO.
        ls_color_t cor_atual = (y < (handle->height / 2)) ? LS_COLOR_BLACK : LS_COLOR_WHITE;

        // Percorre todas as colunas (X) daquela linha
        for (uint8_t x = 0; x < handle->width; x++) {
            ls013b7dh03_drawPixel(handle, x, y, cor_atual);
        }
    }

    // IMPORTANTE: Só atualiza a tela DEPOIS de pintar tudo na memória
    ls013b7dh03_refresh(handle);
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

    uint8_t cmd = reverse_byte(SHARPMEM_BIT_WRITECMD);
    uint8_t dummy = 0x00;

    handle->cs_control(LS_CS_SELECT);
    handle->spi_write(&cmd, 1);

    for (uint8_t line = 1; line <= handle->height; line++) {
        uint8_t line_addr = reverse_byte(line); 
        handle->spi_write(&line_addr, 1);

        uint8_t *line_ptr = &handle->framebuffer[(line - 1) * (handle->width / 8)];
        handle->spi_write(line_ptr, (handle->width / 8));

        handle->spi_write(&dummy, 1);
    }

    handle->spi_write(&dummy, 1);

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

