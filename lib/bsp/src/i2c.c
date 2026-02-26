/**

 * @file    i2c.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-12-24
 */

#include "stm32wbxx_hal.h"
#include "i2c.h"
#include "board_config.h"

static I2C_HandleTypeDef hi2c1;


/*
 * @brief      i2c bus init
 * @return     status code
 *            - 0 success
 * @note
 */
uint8_t i2c_init(void) {
    GPIO_InitTypeDef GPIOHandle = {0};

    GPIOHandle.Pin = I2C_SDA_PIN | I2C_SCL_PIN;
    GPIOHandle.Mode = GPIO_MODE_AF_OD;
    GPIOHandle.Alternate = GPIO_AF4_I2C1;
    GPIOHandle.Pull = GPIO_PULLUP;
    GPIOHandle.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_SDA_PORT, &GPIOHandle); 

    hi2c1.Instance = I2C_INSTANCE;
    hi2c1.Init.Timing = 400000;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

    if (HAL_I2C_Init(&hi2c1) != 0) {
        return 1;
    }

    return 0;
}

/*
 * @brief      i2c bus deinit
 * @return     status code
 *            - 0 success
 * @note
 */
uint8_t i2c_deinit(void) {
    HAL_GPIO_DeInit(I2C_SDA_PORT, I2C_SDA_PIN | I2C_SCL_PIN);
    HAL_I2C_DeInit(&hi2c1);
    
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
    if (HAL_I2C_Mem_Read(&hi2c1, addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, HAL_MAX_DELAY) != HAL_OK) {
        return 1;
    }
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
    if (HAL_I2C_Master_Receive(&hi2c1, addr, buf, len, HAL_MAX_DELAY) != HAL_OK) {
        return 1;
    }
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
    if (HAL_I2C_Mem_Write(&hi2c1, addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, HAL_MAX_DELAY) != HAL_OK) {
        return 1;
    }
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
    if (HAL_I2C_Master_Transmit(&hi2c1, addr, buf, len, HAL_MAX_DELAY) != HAL_OK) {
        return 1;
    }
    return 0;
}