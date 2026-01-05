/**
 * @file    i2c.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-12-24
 */

#include "i2c.h"
#include "resources.h"

extern i2c_config_t i2c_config;

/*
 * @brief      i2c bus init
 * @return     status code
 *            - 0 success
 * @note
 */
uint8_t i2c_init(void) {
    GPIO_InitTypeDef GPIOHandle;

    GPIOHandle.Pin = i2c_config.sda.pin | i2c_config.scl.pin;
    GPIOHandle.Mode = GPIO_MODE_AF_OD;
    GPIOHandle.Alternate = GPIO_AF4_I2C1;
    GPIOHandle.Pull = GPIO_PULLUP;
    GPIOHandle.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIOHandle);

    if (HAL_I2C_Init(&i2c_config.i2c_handle) != 0)
        return 1;

    // dummy read
    uint8_t dummy_data;
    HAL_I2C_Mem_Read(&i2c_config.i2c_handle, I2C_SLAVE_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, &dummy_data, 1, 100);

    // Lê o Registrador 0x01 (Interrupt Status 2) - Só por garantia
    HAL_I2C_Mem_Read(&i2c_config.i2c_handle, I2C_SLAVE_ADDR, 0x01, I2C_MEMADD_SIZE_8BIT, &dummy_data, 1, 100);
    return 0;
}

/*
 * @brief      i2c bus deinit
 * @return     status code
 *            - 0 success
 * @note
 */
uint8_t i2c_deinit(void) {
    HAL_GPIO_DeInit(GPIOA, i2c_config.sda.pin | i2c_config.scl.pin);

    return 0;
}

/*
 * @brief      i2c bus read
 * @param[in]  addr i2c device write address
 * @param[in]  reg i2c register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *            - 0 success
 *            - 1 read failed
 * @note      addr = device_address_7bits << 1
 */
uint8_t i2c_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len) {
    uint8_t status;
    status = HAL_I2C_Mem_Read(&i2c_config.i2c_handle, addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, HAL_MAX_DELAY);

    if (status != 0)
        return 1;

    return 0;
}

/**
 * @brief      i2c bus read command
 * @param[in]  addr i2c device write address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       addr = device_address_7bits << 1
 */
uint8_t i2c_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len) {
    uint8_t status;
    status = HAL_I2C_Master_Receive(&i2c_config.i2c_handle, addr, buf, len, HAL_MAX_DELAY);

    if (status != 0)
        return 1;

    return 0;
}

/**
 * @brief     i2c bus write
 * @param[in] addr i2c device write address
 * @param[in] reg i2c register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      addr = device_address_7bits << 1
 */
uint8_t i2c_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len) {
    uint8_t status;
    status = HAL_I2C_Mem_Write(&i2c_config.i2c_handle, addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, HAL_MAX_DELAY);

    if (status != 0)
        return 1;

    return 0;
}

/**
 * @brief     i2c bus write command
 * @param[in] addr i2c device write address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      addr = device_address_7bits << 1
 */
uint8_t i2c_write_cmd(uint8_t addr, uint8_t *buf, uint16_t len) {
    uint8_t status;
    status = HAL_I2C_Master_Transmit(&i2c_config.i2c_handle, addr, buf, len, HAL_MAX_DELAY);

    if (status != 0)
        return 1;

    return 0;
}