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

    GPIOHandle.Pin = SCK_PIN;
    HAL_GPIO_Init(SCK_PORT, &GPIOHandle);

    GPIOHandle.Pin = MOSI_PIN;
    HAL_GPIO_Init(MOSI_PORT, &GPIOHandle);

    hspi1.Instance = SPI_INSTANCE;
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
    HAL_GPIO_DeInit(SCK_PORT, SCK_PIN);
    HAL_GPIO_DeInit(MOSI_PORT, MOSI_PIN);

    if (HAL_SPI_DeInit(&hspi1) != HAL_OK) {
        return 1;
    }
    
    return 0;
}

uint8_t spi_write(uint8_t *buf, uint16_t len) {    
    if (len > 0) {
        if (HAL_SPI_Transmit(&hspi1, buf, len, HAL_MAX_DELAY) != HAL_OK) {
            return 1;
        }
    }
    return 0;
}

uint8_t spi_write_address16(uint8_t addr, uint8_t *buf, uint16_t len) {
    uint8_t buffer[2];
    uint8_t res;

    /* transmit the addr  */
    buffer[0] = (addr >> 8) & 0xFF;
    buffer[1] = addr & 0xFF;
    res = HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer, 2, 1000);
    if (res != 0) {
        return 1;
    }

    /* if len > 0 */
    if (len > 0) {
        /* transmit the buffer */
        res = HAL_SPI_Transmit(&hspi1, buf, len, 1000);
        if (res != 0) {           
            return 1;
        }
    }

    return 0; // OK
}

uint8_t spi_read(uint8_t *buf, uint16_t len) {
    uint8_t res;

    if (len > 0)
    {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&hspi1, buf, len, 1000);
        if (res != 0) {  
            return 1;
        }
    }

    return 0;
}

uint8_t spi_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len) {
    uint8_t buffer;
    uint8_t res;

    buffer = addr;
    res = HAL_SPI_Transmit(&hspi1, (uint8_t *)&buffer, 1, 1000);
    if (res != HAL_OK){       
        return 1;
    }

    if (len > 0) {
        res = HAL_SPI_Receive(&hspi1, buf, len, HAL_MAX_DELAY != 0);
        if (res != 0) {
            return 1;
        }
    }

    return 0;
}

// uint8_t spi_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len) {
//     bsp_gpio_write(ACCEL_CS_PORT, ACCEL_CS_PIN, GPIO_PIN_RESET);
    
// }
