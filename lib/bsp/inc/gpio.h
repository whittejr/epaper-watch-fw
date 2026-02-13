/**
 * @file gpio.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2025-11-05
 */

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/*
 * Display
 */

//  SSD1681
uint8_t ssd1681_cmd_data_gpio_init(void);
uint8_t ssd1681_cmd_data_gpio_write(uint8_t value);
uint8_t busy_gpio_read(uint8_t *value);
uint8_t ssd1681_cmd_data_gpio_deinit(void);
uint8_t ssd1681_rst_gpio_init(void);
uint8_t ssd1681_rst_gpio_deinit(void);
uint8_t ssd1681_rst_gpio_write(uint8_t value);
uint8_t ssd1681_busy_gpio_init(void);
uint8_t ssd1681_busy_gpio_deinit(void);

//  LS013B7DH03
uint8_t ls013b7dh03_gpio_init(void);
uint8_t ls013b7dh03_extcomin_gpio_init(void);
uint8_t ls013b7dh03_gpio_cs_control(uint8_t state);
void ls013b7dh03_delay_ms(uint32_t ms);
uint8_t ls013b7dh03_vcom_tim(void);
/*
* Oximeter
*/

/*
 * MAX30102
 */
uint8_t max_irq_gpio_init(void);

#endif // GPIO_H
