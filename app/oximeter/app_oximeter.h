/**
 * @file app_oximeter.h
 * @brief  Oximeter application interface
 * @version 0.2
 * @date 2026-04-21
 */

#ifndef APP_OXIMETER_H
#define APP_OXIMETER_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief Initializes the oximeter application
 * @return status code (0 for success)
 */
uint8_t app_oximeter_init(void);

/**
 * @brief Updates the oximeter with a new sample
 * @param red Raw RED led sample
 * @param ir Raw IR led sample
 * @return 1 if new HR/SpO2 result is calculated, 0 otherwise
 */
uint8_t app_oximeter_update(uint32_t red, uint32_t ir);

/**
 * @brief Gets the last calculated heart rate
 * @return BPM
 */
uint8_t app_oximeter_get_hr(void);

/**
 * @brief Gets the last calculated SpO2 percentage
 * @return SpO2 %
 */
uint8_t app_oximeter_get_spo2(void);

/**
 * @brief Gets the current collection progress (0 to 100)
 * @return Percentage
 */
uint16_t app_oximeter_get_progress(void);

/**
 * @brief Triggers calculation manually (if needed)
 */
void app_oximeter_calculate(void);

/**
 * @brief Oximeter background task
 */
void vOximeterTask(void *pvParameters);

extern TaskHandle_t xOximeterTaskHandle;

#endif // APP_OXIMETER_H
