#include "spi.h"
#include "board_config.h"
#include "gpio.h"

static SPI_HandleTypeDef hspi1;

uint8_t spi_init(void) {
    GPIO_InitTypeDef GPIOHandle = {0};

    GPIOHandle.Mode = GPIO_MODE_AF_PP;
    GPIOHandle.Pull = GPIO_NOPULL;
    GPIOHandle.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIOHandle.Alternate = GPIO_AF5_SPI1; 

    GPIOHandle.Pin = EPD_SCK_PIN;
    HAL_GPIO_Init(EPD_SCK_PORT, &GPIOHandle);

    GPIOHandle.Pin = EPD_MOSI_PIN;
    HAL_GPIO_Init(EPD_MOSI_PORT, &GPIOHandle);

    hspi1.Instance = EPD_SPI_INSTANCE;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    hspi1.Init.CRCPolynomial = 7;

    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        return 1;
    }

    return 0;
}

uint8_t spi_deinit(void) {
    HAL_GPIO_DeInit(EPD_SCK_PORT, EPD_SCK_PIN);
    HAL_GPIO_DeInit(EPD_MOSI_PORT, EPD_MOSI_PIN);

    if (HAL_SPI_DeInit(&hspi1) != HAL_OK) {
        return 1;
    }

    return 0;
}

uint8_t spi_write(uint8_t *buf, uint16_t len) {
    bsp_gpio_write(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_RESET);
    if (len > 0) {
        if (HAL_SPI_Transmit(&hspi1, buf, len, HAL_MAX_DELAY) != HAL_OK) {
            return 1;
        }
    }

    bsp_gpio_write(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_SET);
    return 0;
}

uint8_t spi_read(uint8_t *buf, uint16_t len) {
    if (len > 0) {
        if (HAL_SPI_Receive(&hspi1, buf, len, HAL_MAX_DELAY) != HAL_OK) {
            return 1;
        }
    }
    return 0;
}
