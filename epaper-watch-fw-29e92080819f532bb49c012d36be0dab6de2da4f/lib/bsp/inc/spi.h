#ifndef SPI_H
#define SPI_H

#include <stdint.h>

uint8_t spi_init(void);
uint8_t spi_deinit(void);
uint8_t spi_write(uint8_t *buf, uint16_t len);
uint8_t spi_read(uint8_t *buf, uint16_t len);
uint8_t spi_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len);
uint8_t spi_write_address16(uint8_t addr, uint8_t *buf, uint16_t len);

#endif // SPI_H
