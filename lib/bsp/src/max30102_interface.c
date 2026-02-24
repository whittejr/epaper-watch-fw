/**
* @file    max30102_interface.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-02-23
*/

#include "max30102_interface.h"
#include "delay.h"
#include "i2c.h"
#include <stdarg.h>
#include "uart.h"

/**
 * @brief  interface i2c bus init
 * @return status code
 *         - 0 success
 *         - 1 i2c init failed
 * @note   none
 */
uint8_t max30102_interface_i2c_init(void) {
    return i2c_init();
}

/**
 * @brief  interface i2c bus deinit
 * @return status code
 *         - 0 success
 *         - 1 i2c deinit failed
 * @note   none
 */
uint8_t max30102_interface_i2c_deinit(void) {
    return i2c_deinit();
}

/**
 * @brief      interface i2c bus read
 * @param[in]  addr i2c device write address
 * @param[in]  reg i2c register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max30102_interface_i2c_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len) {
    return i2c_read(addr, reg, buf, len);
}

/**
 * @brief     interface i2c bus write
 * @param[in] addr i2c device write address
 * @param[in] reg i2c register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t max30102_interface_i2c_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len) {
    return i2c_write(addr, reg, buf, len);
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void max30102_interface_delay_ms(uint32_t ms) {
    delay_ms(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void max30102_interface_debug_print(const char *const fmt, ...) {
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)uart_write((uint8_t *)str, len);
}
