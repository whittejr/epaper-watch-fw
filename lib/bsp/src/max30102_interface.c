/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      driver_max30102_interface_template.c
 * @brief     driver max30102 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-11-13
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/11/13  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "max30102_interface.h"
#include "delay.h"
#include "i2c.h"
#include "max30102.h"

// static volatile uint8_t flag; /**< flag */
// static uint32_t raw_red[32];  /**< raw red buffer */
// static uint32_t raw_ir[32];   /**< raw ir buffer */

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
}

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
// void max30102_interface_receive_callback(uint8_t type) {
//     switch (type) {
//     case MAX30102_INTERRUPT_STATUS_FIFO_FULL: {
//         max30102_interface_debug_print("max30102: irq fifo full.\n");

//         break;
//     }
//     case MAX30102_INTERRUPT_STATUS_PPG_RDY: {
//         max30102_interface_debug_print("max30102: irq ppg rdy.\n");

//         break;
//     }
//     case MAX30102_INTERRUPT_STATUS_ALC_OVF: {
//         max30102_interface_debug_print("max30102: irq alc ovf.\n");

//         break;
//     }
//     case MAX30102_INTERRUPT_STATUS_PWR_RDY: {
//         max30102_interface_debug_print("max30102: irq pwr rdy.\n");

//         break;
//     }
//     case MAX30102_INTERRUPT_STATUS_DIE_TEMP_RDY: {
//         max30102_interface_debug_print("max30102: irq die temp rdy.\n");

//         break;
//     }
//     default: {
//         max30102_interface_debug_print("max30102: unknown code.\n");

//         break;
//     }
//     }
// }

/**
 * @brief     interface default config
 * @param[in] max30102_handle_t
 * @note      none
 */
// uint8_t max30102_interface_default_config(max30102_handle_t *handle) {
//     uint8_t res;
//     uint32_t timeout;
//     uint16_t raw;
//     float temp;
//     max30102_bool_t enable;
//     max30102_info_t info;

//     DRIVER_MAX30102_LINK_INIT(handle, max30102_handle_t);
//     DRIVER_MAX30102_LINK_I2C_INIT(handle, max30102_interface_i2c_init);
//     DRIVER_MAX30102_LINK_I2C_DEINIT(handle, max30102_interface_i2c_deinit);
//     DRIVER_MAX30102_LINK_I2C_READ(handle, max30102_interface_i2c_read);
//     DRIVER_MAX30102_LINK_I2C_WRITE(handle, max30102_interface_i2c_write);
//     DRIVER_MAX30102_LINK_DELAY_MS(handle, max30102_interface_delay_ms);
//     DRIVER_MAX30102_LINK_DEBUG_PRINT(handle, max30102_interface_debug_print);
//     DRIVER_MAX30102_LINK_RECEIVE_CALLBACK(handle, max30102_interface_receive_callback);

//     /* init the max30102 */
//     res = max30102_init(handle);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: init failed.\n");
//         return 1;
//     }
//     /* enable shutdown */
//     res = max30102_set_shutdown(handle, MAX30102_BOOL_TRUE);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set shutdown failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set fifo sample averaging */
//     res = max30102_set_fifo_sample_averaging(handle, MAX30102_SAMPLE_AVERAGING_8);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set fifo sample averaging failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set fifo roll */
//     res = max30102_set_fifo_roll(handle, MAX30102_BOOL_TRUE);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set fifo roll failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set fifo almost full */
//     res = max30102_set_fifo_almost_full(handle, 0xF);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set fifo almost full failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set mode */
//     res = max30102_set_mode(handle, MAX30102_MODE_SPO2);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set mode failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set spo2 adc range */
//     res = max30102_set_spo2_adc_range(handle, MAX30102_SPO2_ADC_RANGE_4096);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set spo2 adc range failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set spo2 sample rate */
//     res = max30102_set_spo2_sample_rate(handle, MAX30102_SPO2_SAMPLE_RATE_50_HZ);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set spo2 sample rate failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set adc resolution */
//     res = max30102_set_adc_resolution(handle, MAX30102_ADC_RESOLUTION_18_BIT);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set adc resolution failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set led red pulse amplitude */
//     res = max30102_set_led_red_pulse_amplitude(handle, 0x24);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set led red pulse amplitude failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set led ir pulse amplitude */
//     res = max30102_set_led_ir_pulse_amplitude(handle, 0x24);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set led ir pulse amplitude failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set slot */
//     res = max30102_set_slot(handle, MAX30102_SLOT_1, MAX30102_LED_RED);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set slot failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set slot */
//     res = max30102_set_slot(handle, MAX30102_SLOT_2, MAX30102_LED_IR);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set slot failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set slot */
//     res = max30102_set_slot(handle, MAX30102_SLOT_3, MAX30102_LED_NONE);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set slot failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set slot */
//     res = max30102_set_slot(handle, MAX30102_SLOT_4, MAX30102_LED_NONE);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set slot failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* disable die temperature */
//     res = max30102_set_die_temperature(handle, MAX30102_BOOL_FALSE);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set die temperature failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set interrupt */
//     res = max30102_set_interrupt(handle, MAX30102_INTERRUPT_FIFO_FULL_EN, MAX30102_BOOL_TRUE);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set interrupt failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set interrupt */
//     res = max30102_set_interrupt(handle, MAX30102_INTERRUPT_PPG_RDY_EN, MAX30102_BOOL_FALSE);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set interrupt failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set interrupt */
//     res = max30102_set_interrupt(handle, MAX30102_INTERRUPT_ALC_OVF_EN, MAX30102_BOOL_TRUE);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set interrupt failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* set interrupt */
//     res = max30102_set_interrupt(handle, MAX30102_INTERRUPT_DIE_TEMP_RDY_EN, MAX30102_BOOL_FALSE);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set interrupt failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     /* disable shutdown */
//     res = max30102_set_shutdown(handle, MAX30102_BOOL_FALSE);
//     if (res != 0) {
//         max30102_interface_debug_print("max30102: set shutdown failed.\n");
//         (void)max30102_deinit(handle);

//         return 1;
//     }

//     return 0;
// }
