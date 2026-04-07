/**
 * @file accel_hal.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-04-07
 */

#ifndef ACCEL_HAL_H
#define ACCEL_HAL_H

#include "adxl362.h"
#include <stdint.h>


/**
 * @brief adxl362 fifo example default definition
 */
#define ADXL362_FIFO_DEFAULT_FIFO_TEMPERATURE           ADXL362_BOOL_FALSE                   /**<disable fifo temperature */
#define ADXL362_FIFO_DEFAULT_FIFO_MODE                  ADXL362_FIFO_MODE_STREAM             /**< stream mode */
#define ADXL362_FIFO_DEFAULT_FIFO_SAMPLE                128                                  /**< 128 */
#define ADXL362_FIFO_DEFAULT_INTERRUPT_PIN_LEVEL        ADXL362_INTERRUPT_PIN_LOW            /**< activity low */
#define ADXL362_FIFO_DEFAULT_BANDWIDTH_ODR              ADXL362_BANDWIDTH_ODR_DIV_2          /**< bandwidth odr 1/2 */
#define ADXL362_FIFO_DEFAULT_NOISE_MODE                 ADXL362_NOISE_MODE_NORMAL            /**< normal noise mode */
#define ADXL362_FIFO_DEFAULT_ODR                        ADXL362_ODR_12P5HZ                   /**< 12.5Hz */
#define ADXL362_FIFO_DEFAULT_RANGE                      ADXL362_RANGE_2G                     /**< 2g */

/*
*   FUNCTIONS
*/

/**
 * @brief     fifo example init
 * @param[in] *callback pointer to a callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t accel_init(void (*callback)(adxl362_frame_t *frame, uint16_t frame_len));

/**
 * @brief  fifo example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t accel_fifo_deinit(void);

/**
 * @brief  fifo irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t accel_fifo_irq_handler(void);

#endif // ACCEL_HAL_H
