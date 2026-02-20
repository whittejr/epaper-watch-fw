/**
 * @file oximeter_hal.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-02-20
 */

#ifndef OXIMETER_HAL_H
#define OXIMETER_HAL_H

#include "stdint.h"

/**
 * @brief max30102 fifo example default definition
 */
#define MAX30102_FIFO_DEFAULT_SAMPLE_AVERAGING          MAX30102_SAMPLE_AVERAGING_8      /**< sample 8 */
#define MAX30102_FIFO_DEFAULT_FIFO_ROLL                 MAX30102_BOOL_TRUE               /**< enable */
#define MAX30102_FIFO_DEFAULT_FIFO_ALMOST_FULL          0xF                              /**< 0xF */
#define MAX30102_FIFO_DEFAULT_MODE                      MAX30102_MODE_SPO2               /**< spo2 mode */
#define MAX30102_FIFO_DEFAULT_SPO2_ADC_RANGE            MAX30102_SPO2_ADC_RANGE_4096     /**< adc range 4096 */
#define MAX30102_FIFO_DEFAULT_SPO2_SAMPLE_RATE          MAX30102_SPO2_SAMPLE_RATE_100_HZ /**< 100 Hz */
#define MAX30102_FIFO_DEFAULT_ADC_RESOLUTION            MAX30102_ADC_RESOLUTION_18_BIT   /**< 18 bits */
#define MAX30102_FIFO_DEFAULT_LED_RED_PULSE_AMPLITUDE   0x7F                             /**< 0x7F */
#define MAX30102_FIFO_DEFAULT_LED_IR_PULSE_AMPLITUDE    0x7F                             /**< 0x7F */
#define MAX30102_FIFO_DEFAULT_SLOT1                     MAX30102_LED_RED                 /**< led red */
#define MAX30102_FIFO_DEFAULT_SLOT2                     MAX30102_LED_IR                  /**< led ir */
#define MAX30102_FIFO_DEFAULT_SLOT3                     MAX30102_LED_NONE                /**< led none */
#define MAX30102_FIFO_DEFAULT_SLOT4                     MAX30102_LED_NONE                /**< led none */
#define MAX30102_FIFO_DEFAULT_DIE_TEMPERATURE           MAX30102_BOOL_FALSE              /**< disable */
#define MAX30102_FIFO_DEFAULT_INTERRUPT_PPG_RDY_EN      MAX30102_BOOL_FALSE              /**< disable */
#define MAX30102_FIFO_DEFAULT_INTERRUPT_ALC_OVF_EN      MAX30102_BOOL_TRUE               /**< enable */
#define MAX30102_FIFO_DEFAULT_INTERRUPT_DIE_TEMP_RDY_EN MAX30102_BOOL_TRUE               /**< enable */


uint8_t oximeter_init(void);

#endif // OXIMETER_HAL_H
