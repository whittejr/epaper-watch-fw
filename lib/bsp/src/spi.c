#include "spi.h"
#include "resources.h"
#include "stm32wbxx_hal_gpio.h"

/**
 * @brief spi var definition
 */
extern epd_user_config_t epd;
extern spi_config_t spi_config;

static uint8_t a_spi_cs_init(void) {
    GPIO_InitTypeDef GPIOHandle = {0};

    GPIOHandle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOHandle.Pull = GPIO_NOPULL;
    GPIOHandle.Speed = GPIO_SPEED_FREQ_LOW;

    GPIOHandle.Pin = epd.pins.cs.pin;
    HAL_GPIO_Init(epd.pins.cs.port, &GPIOHandle);
    HAL_GPIO_WritePin(epd.pins.cs.port, epd.pins.cs.pin, GPIO_PIN_SET);

    return 0;
}

uint8_t spi_init(void) {
    GPIO_InitTypeDef GPIOHandle = {0};

    //--- Pinos de SPI: SCK e MOSI ---
    GPIOHandle.Mode = GPIO_MODE_AF_PP; // Alternate Function Push-Pull
    GPIOHandle.Pull = GPIO_NOPULL;
    GPIOHandle.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIOHandle.Alternate = GPIO_AF5_SPI1; // Confirme o AF correto no datasheet
                                          // (SPI1=AF5, SPI2=AF5 geralmente)

    // SCK
    GPIOHandle.Pin = spi_config.sck.pin;
    HAL_GPIO_Init(spi_config.sck.port, &GPIOHandle);

    // MOSI (SDA)
    GPIOHandle.Pin = spi_config.mosi.pin;
    HAL_GPIO_Init(spi_config.mosi.port, &GPIOHandle);

    if (HAL_SPI_Init(&spi_config.spi_handle) != HAL_OK) {
        return 1; // Erro
    }

    return a_spi_cs_init();
}

uint8_t spi_deinit(void) {
    HAL_GPIO_DeInit(epd.pins.cs.port, epd.pins.cs.pin);

    if (HAL_SPI_DeInit(&spi_config.spi_handle) != HAL_OK)
        return 1;

    return 0;
}

uint8_t spi_write_cmd(uint8_t *buf, uint16_t len) {
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(epd.pins.cs.port, epd.pins.cs.pin, GPIO_PIN_RESET);

    if (len > 0) {
        res = HAL_SPI_Transmit(&spi_config.spi_handle, buf, len, HAL_MAX_DELAY);
        if (res != HAL_OK) {
            /* set cs high */
            HAL_GPIO_WritePin(epd.pins.cs.port, epd.pins.cs.pin, GPIO_PIN_SET);

            return 1;
        }
    }
    /* set cs high */
    HAL_GPIO_WritePin(epd.pins.cs.port, epd.pins.cs.pin, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief      spi bus read command
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_cmd(uint8_t *buf, uint16_t len) {
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(epd.pins.cs.port, epd.pins.cs.pin, GPIO_PIN_RESET);

    /* if len > 0 */
    if (len > 0) {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&spi_config.spi_handle, buf, len, 1000);
        if (res != HAL_OK) {
            /* set cs high */
            HAL_GPIO_WritePin(epd.pins.cs.port, epd.pins.cs.pin, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(epd.pins.cs.port, epd.pins.cs.pin, GPIO_PIN_SET);

    return 0;
}


/*
*   MIP DISPLAY
*/
static uint8_t a_mip_spi_cs_init(void) {
    GPIO_InitTypeDef GPIOHandle = {0};

    GPIOHandle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOHandle.Pull = GPIO_NOPULL;
    GPIOHandle.Speed = GPIO_SPEED_FREQ_LOW;

    GPIOHandle.Pin = epd.pins.cs.pin;
    HAL_GPIO_Init(epd.pins.cs.port, &GPIOHandle);
    HAL_GPIO_WritePin(epd.pins.cs.port, epd.pins.cs.pin, GPIO_PIN_RESET);

    return 0;
}


uint8_t mip_display_spi_init() {
    GPIO_InitTypeDef GPIOHandle = {0};

    //--- Pinos de SPI: SCK e MOSI ---
    GPIOHandle.Mode = GPIO_MODE_AF_PP; // Alternate Function Push-Pull
    GPIOHandle.Pull = GPIO_NOPULL;
    GPIOHandle.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIOHandle.Alternate = GPIO_AF5_SPI1; // Confirme o AF correto no datasheet
                                          // (SPI1=AF5, SPI2=AF5 geralmente)

    // SCK
    GPIOHandle.Pin = spi_config.sck.pin;
    HAL_GPIO_Init(spi_config.sck.port, &GPIOHandle);

    // MOSI (SDA)
    GPIOHandle.Pin = spi_config.mosi.pin;
    HAL_GPIO_Init(spi_config.mosi.port, &GPIOHandle);

    if (HAL_SPI_Init(&spi_config.spi_handle) != HAL_OK) {
        return 1; // Erro
    }

    return a_mip_spi_cs_init();
}

uint8_t mip_display_spi_deinit() {
    HAL_GPIO_DeInit(epd.pins.cs.port, epd.pins.cs.pin);

    if (HAL_SPI_DeInit(&spi_config.spi_handle) != HAL_OK)
        return 1;

    return 0;
}

uint8_t mip_display_spi_write(uint8_t *buf, uint16_t len) {
    if (HAL_SPI_Transmit(&spi_config.spi_handle, buf, len, HAL_MAX_DELAY) != 0)
        return 1;
}

uint8_t mip_display_spi_write_refresh(uint8_t *buf, uint16_t len) {
    if (HAL_SPI_Transmit(&spi_config.spi_handle, buf, len, HAL_MAX_DELAY) != 0)
        return 1; 
    
    return 0;
}
