/**
 * @file i2c.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2025-12-24
 */

#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/*
 * @brief      i2c bus init
 * @return     status code
 *            - 0 success
 * @note
 */
uint8_t i2c_init(void);

/*
 * @brief      i2c bus deinit
 * @return     status code
 *            - 0 success
 * @note
 */
uint8_t i2c_deinit(void);

/*
 * @brief      i2c bus read
 * @param[in]  addr iic device write address
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *            - 0 success
 *            - 1 read failed
 * @note      addr = device_address_7bits << 1
 */
uint8_t i2c_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      i2c bus read command
 * @param[in]  addr iic device write address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       addr = device_address_7bits << 1
 */
uint8_t i2c_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief     i2c bus write
 * @param[in] addr iic device write address
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      addr = device_address_7bits << 1
 */
uint8_t i2c_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief     i2c bus write command
 * @param[in] addr iic device write address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      addr = device_address_7bits << 1
 */
uint8_t i2c_write_cmd(uint8_t addr, uint8_t *buf, uint16_t len);

#endif // I2C_H