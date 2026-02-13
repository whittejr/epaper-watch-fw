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
#include "ssd1681.h"
#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_gpio.h"
#include "stm32wbxx_hal_tim.h"


ls013b7dh03_handle_t sharp_handle;
static uint8_t sharp_video_buffer[128 * 128 / 8];
extern TIM_HandleTypeDef htim17;

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

    return 0;
}

uint8_t display_split() {
    ls013b7dh03_split_horizontal(&sharp_handle);

    return 0;
}

uint8_t tesst_code() {
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

    for (int i = 0; i < 2048; i++) {
        if (i < 1024) {
            // Primeira metade (Linhas 0 a 63) -> Branco
            sharp_handle.framebuffer[i] = 0x00; 
        } else {
            // Segunda metade (Linhas 64 a 127) -> Preto
            sharp_handle.framebuffer[i] = 0xFF; 
        }
    }

    
    // memset(sharp_handle.framebuffer, 0xFF, LS013B7DH03_BUFFER_SIZE);

    // 1. Inicializa o hardware
    if (ls013b7dh03_init(&sharp_handle) != 0) {
        return 1; // Erro
    }

    // HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
    
    // 2. Limpa a tela (Reset buffer e VCOM)
    // ls013b7dh03_clear(&sharp_handle); ls013b7dh03_clear(&sharp_handle); 
    // ls013b7dh03_clear(&sharp_handle);
    
    // ---------------------------------------------------------
    // TESTE: CAIXA COM 'X' (O Teste de Geometria)
    // ---------------------------------------------------------
    
    // Borda Superior e Inferior
    // for (int x = 0; x < 128; x++) {
    //     ls013b7dh03_drawPixel(&sharp_handle, x, 0, LS_COLOR_BLACK);   // Topo
    //     ls013b7dh03_drawPixel(&sharp_handle, x, 127, LS_COLOR_BLACK); // Fundo
    // }

    // // Borda Esquerda e Direita
    // for (int y = 0; y < 128; y++) {
    //     ls013b7dh03_drawPixel(&sharp_handle, 0, y, LS_COLOR_BLACK);   // Esq
    //     ls013b7dh03_drawPixel(&sharp_handle, 127, y, LS_COLOR_BLACK); // Dir
    // }

    // // Diagonal Principal (Esquerda-Topo -> Direita-Fundo)
    // for (int i = 0; i < 128; i++) {
    //     ls013b7dh03_drawPixel(&sharp_handle, i, i, LS_COLOR_BLACK);
    // }

    // // Diagonal Inversa (Direita-Topo -> Esquerda-Fundo)
    // for (int i = 0; i < 128; i++) {
    //     ls013b7dh03_drawPixel(&sharp_handle, 127 - i, i, LS_COLOR_BLACK);
    // }

    // // Ponto Central (Para referência visual)
    // ls013b7dh03_drawPixel(&sharp_handle, 63, 64, LS_COLOR_BLACK); // Buraco no meio
    // ls013b7dh03_drawPixel(&sharp_handle, 64, 64, LS_COLOR_BLACK); 

    // 3. Envia para o display (CRÍTICO: Sem isso nada aparece)
    ls013b7dh03_refresh(&sharp_handle);

    return 0;
}
