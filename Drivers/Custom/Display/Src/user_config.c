/**
 * @file    epd_user_cfg.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-11-03
 */

#include "user_config.h"

// Framebuffer global
uint8_t framebuffer[(EPD_W / 8) * EPD_H];

SPI_HandleTypeDef hspi1;

// Instância do display
epd_user_config_t epd = {.pins = {.rst = {EPD_RST_PORT, EPD_RST_PIN},
                                  .dc = {EPD_DC_PORT, EPD_DC_PIN},
                                  .cs = {EPD_CS_PORT, EPD_CS_PIN},
                                  .busy = {EPD_BSY_PORT, EPD_BSY_PIN}},

                         .width = EPD_W,
                         .height = EPD_H,
                         .framebuffer = framebuffer,

                         //  .rotate = EPD_ROTATE_0,
                         //  .color = EPD_COLOR_WHITE,
                         .width_memory = EPD_W,
                         .height_memory = EPD_H,
                         .width_byte = (EPD_W % 8 == 0) ? (EPD_W / 8) : (EPD_W / 8 + 1),
                         .height_byte = EPD_H};

spi_config_t spi_display = {.pins = {.sda = {EPD_SDA_PORT, EPD_SDA_PIN}, .scl = {EPD_SCL_PORT, EPD_SCL_PIN}},

                            .spi_handle = {.Instance = EPD_SPI_INSTANCE,
                                           .Init.DataSize = SPI_DATASIZE_8BIT,
                                           .Init.Direction = SPI_DIRECTION_2LINES,
                                           .Init.Mode = SPI_MODE_MASTER,
                                           .Init.FirstBit = SPI_FIRSTBIT_MSB,
                                           .Init.CLKPhase = SPI_PHASE_2EDGE,
                                           .Init.CLKPolarity = SPI_POLARITY_HIGH,
                                           .Init.NSS = SPI_NSS_SOFT,
                                           .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32,
                                           .Init.TIMode = SPI_TIMODE_DISABLE,
                                           .Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED,
                                           .Init.CRCPolynomial = 7}};
