/**
 * @file    resources.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-12-31
 */

#include "resources.h"
#include "stm32wbxx_hal_spi.h"

// Framebuffer global
uint8_t framebuffer[(EPD_W / 8) * EPD_H];

// Instância do display
epd_user_config_t epd = {.pins = {.rst = {EPD_RST_PORT, EPD_RST_PIN}, .dc = {EPD_DC_PORT, EPD_DC_PIN}, .cs = {EPD_CS_PORT, EPD_CS_PIN}, .busy = {EPD_BSY_PORT, EPD_BSY_PIN}},

                         .width = EPD_W,
                         .height = EPD_H,
                         .framebuffer = framebuffer,

                         //  .rotate = EPD_ROTATE_0,
                         //  .color = EPD_COLOR_WHITE,
                         .width_memory = EPD_W,
                         .height_memory = EPD_H,
                         .width_byte = (EPD_W % 8 == 0) ? (EPD_W / 8) : (EPD_W / 8 + 1),
                         .height_byte = EPD_H};

spi_config_t spi_config = {.mosi = {.port = EPD_MOSI_PORT, .pin = EPD_MOSI_PIN},
                           .sck = {.port = EPD_SCK_PORT, .pin = EPD_SCK_PIN},

                           .spi_handle = {.Instance = EPD_SPI_INSTANCE,
                                          .Init.DataSize = SPI_DATASIZE_8BIT,
                                          .Init.Direction = SPI_DIRECTION_2LINES,
                                          .Init.Mode = SPI_MODE_MASTER,
                                          .Init.FirstBit = SPI_FIRSTBIT_MSB,
                                          .Init.CLKPhase = SPI_PHASE_1EDGE,
                                          .Init.CLKPolarity = SPI_POLARITY_LOW,
                                          .Init.NSS = SPI_NSS_SOFT,
                                          .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256,
                                          .Init.TIMode = SPI_TIMODE_DISABLE,
                                          .Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED,
                                          .Init.CRCPolynomial = 7}};

i2c_config_t i2c_config = {.sda = {.port = I2C_SDA_PORT, .pin = I2C_SDA_PIN},
                           .scl = {.port = I2C_SCL_PORT, .pin = I2C_SCL_PIN},
                           .i2c_handle = {.Instance = I2C_INSTANCE,

                                          .Init.Timing = 400000,
                                          .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
                                          .Init.DualAddressMode = I2C_DUALADDRESS_DISABLED,
                                          .Init.NoStretchMode = I2C_NOSTRETCH_DISABLED,
                                          .Init.GeneralCallMode = I2C_GENERALCALL_DISABLED,
                                          .Init.OwnAddress1 = I2C_SLAVE_ADDR

                           }};
