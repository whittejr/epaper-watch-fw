#include "stm32wbxx_hal.h"
#include "spi.h"
#include "stm32wbxx_hal_spi.h"
#include "gpio.h"

SPI_HandleTypeDef hspi1;

uint8_t spi_init(void) {
    __HAL_RCC_SPI1_CLK_ENABLE();

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES; 
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;   
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;       
    hspi1.Init.NSS = SPI_NSS_SOFT;              
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; // ~1MHz
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;      
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;

    if (HAL_SPI_Init(&hspi1) != HAL_OK) return 1;

    return 0;
}

uint8_t spi_deinit(void) {
    if (HAL_SPI_DeInit(&hspi1) != HAL_OK) return 1;
    return 0;
}

uint8_t spi_write(uint8_t *buf, uint16_t len) {
    if (HAL_SPI_Transmit(&hspi1, buf, len, HAL_MAX_DELAY) != HAL_OK) return 1;
    return 0;
}

uint8_t spi_write_address16(uint16_t addr, uint8_t *buf, uint16_t len) {
    uint8_t buffer[2] = {(addr >> 8) & 0xFF, addr & 0xFF};
    if (HAL_SPI_Transmit(&hspi1, buffer, 2, 1000) != HAL_OK) return 1;
    if (len > 0) {
        if (HAL_SPI_Transmit(&hspi1, buf, len, 1000) != HAL_OK) return 1;
    }
    return 0;
}

uint8_t spi_read_cmd(uint16_t addr, uint8_t *buf, uint16_t len) {
    uint8_t buffer[2] = {(uint8_t)(addr >> 8), (uint8_t)(addr & 0xFF)};
    if (HAL_SPI_Transmit(&hspi1, buffer, 2, 1000) != HAL_OK) return 1;
    if (len > 0) {
        if (HAL_SPI_Receive(&hspi1, buf, len, HAL_MAX_DELAY) != HAL_OK) return 1;
    }
    return 0;
}
