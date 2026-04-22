/**
 * @file app_accel.h
 * @brief  Accelerometer application interface
 */

#ifndef APP_ACCEL_H
#define APP_ACCEL_H

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief Initializes the accelerometer application
 * @return status code (0 for success)
 */
uint8_t app_accel_init(void);

/**
 * @brief Reads real-time acceleration in Gs
 * @param g Array of 3 floats [X, Y, Z]
 * @return status code
 */
uint8_t app_accel_read(float g[3]);

/**
 * @brief Gets total steps counted
 * @return Step count
 */
uint32_t app_accel_get_steps(void);

/**
 * @brief Resets step counter
 */
void app_accel_reset_steps(void);

/**
 * @brief Checks if significant motion is detected
 * @return true if moving
 */
bool app_accel_is_moving(void);

/**
 * @brief Gets minutes of inactivity (potential sleep)
 * @return Inactivity minutes
 */
uint32_t app_accel_get_inactivity_mins(void);

/**
 * @brief Accelerometer processing task
 */
void vAccelTask(void *pvParameters);

extern TaskHandle_t xAccelTaskHandle;

#endif // APP_ACCEL_H
