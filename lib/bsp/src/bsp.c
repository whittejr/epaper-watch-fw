#include "bsp.h"
#include "stm32wbxx_hal.h"

IPCC_HandleTypeDef hipcc;

static void MX_IPCC_Init(void) {
    hipcc.Instance = IPCC;
    if (HAL_IPCC_Init(&hipcc) != HAL_OK) {
        // Error_Handler
    }
    
    /* Enable IPCC Interrupts */
    HAL_NVIC_SetPriority(IPCC_C1_RX_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(IPCC_C1_RX_IRQn);
    HAL_NVIC_SetPriority(IPCC_C1_TX_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(IPCC_C1_TX_IRQn);
}

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
    rtc_init();
    
    /* Initialize Inter-Processor Communication */
    MX_IPCC_Init();

    return 0;
}
