/**
 * @file max30102_app.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2025-12-26
 */

#ifndef MAX30102_APP_H
#define MAX30102_APP_H

#include "max30102_interface.h"
#include <stdint.h>

/**
 * @brief  fifo example irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t max30102_fifo_irq_handler(void);

/**
 * @brief     fifo example init
 * @param[in] *fifo_receive_callback pointer to a fifo receive callback
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t app_oximeter_init(void);

/**
 * @brief  fifo example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max30102_fifo_deinit(void);

/**
 * @brief         read the data
 * @param[out]    *raw_red pointer to a read raw data buffer
 * @param[out]    *raw_ir pointer to a ir raw data buffer
 * @param[in,out] *len pointer to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 * @note          none
 */
uint8_t max30102_fifo_read(uint32_t *raw_red, uint32_t *raw_ir, uint8_t *len);

/**
 * @}
 */
 uint8_t max30102_event();
 uint8_t oximeter_proccess();


#endif // MAX30102_APP_H