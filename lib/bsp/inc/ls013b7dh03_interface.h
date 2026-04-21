/**
 * @file ls013b7dh03_interface.h
 * @brief  ls013b7dh03 interface template
 * @version 1.0.0
 * @author Alessandro Davi
 * @date 2026-04-21
 */

#ifndef LS013B7DH03_INTERFACE_H
#define LS013B7DH03_INTERFACE_H

#include "ls013b7dh03.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief  Initialize hardware SPI
 * @return 0: success, 1: error
 */
uint8_t ls013b7dh03_interface_spi_init(void);

/**
 * @brief  Deinitialize hardware SPI
 * @return 0: success, 1: error
 */
uint8_t ls013b7dh03_interface_spi_deinit(void);

/**
 * @brief  Write data via SPI
 * @param  buf pointer to data
 * @param  len data length
 * @return 0: success, 1: error
 */
uint8_t ls013b7dh03_interface_spi_write(uint8_t *buf, uint16_t len);

/**
 * @brief  Control the Chip Select pin
 * @param  state 1: High, 0: Low
 */
void ls013b7dh03_interface_cs_control(uint8_t state);

/**
 * @brief  Delay in milliseconds
 * @param  ms time to wait
 */
void ls013b7dh03_interface_delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif
