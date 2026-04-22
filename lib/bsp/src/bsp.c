#include "bsp.h"
#include "stm32wbxx_hal.h"
#include "i2c.h"

uint8_t bsp_init(void) {
    HAL_Init();
    clock_config();
    
    /* Enable HSEM Clock and Interrupt */
    __HAL_RCC_HSEM_CLK_ENABLE();
    HAL_NVIC_SetPriority(HSEM_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(HSEM_IRQn);

    gpio_init();
    uart_init();
    spi_init();
    i2c_init(); // Inicializa o barramento do Oximetro
    rtc_init();

    return 0;
}
