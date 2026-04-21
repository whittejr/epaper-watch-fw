#include "bsp.h"
#include "stm32wbxx_hal.h"

uint8_t bsp_init(void) {
    HAL_Init();
    clock_config();
    gpio_init();
    uart_init();
    spi_init();
    rtc_init();
    // i2c_init(); // Opcional por agora
    return 0;
}
