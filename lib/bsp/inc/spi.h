/**
 * @file spi.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2025-11-04
 */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>

uint8_t spi_init(void);

uint8_t spi_deinit(void);

uint8_t spi_write_cmd(uint8_t *buf, uint16_t len);

uint8_t spi_read_cmd(uint8_t *buf, uint16_t len);

/*
*   SHARP_DISPLAY
*/
uint8_t mip_display_spi_init();
uint8_t mip_display_spi_deinit();
uint8_t mip_display_spi_write(uint8_t *buf, uint16_t len);
uint8_t mip_display_spi_write_refresh(uint8_t *buf, uint16_t len);



#endif // SPI_H
