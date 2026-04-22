/**
* @file    app_accel.c
* @brief   Accelerometer application with Pedometer and Sleep Monitoring
*/

#include "app_accel.h"
#include "accel_hal.h"
#include "ui_manager.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <math.h>
#include <string.h>

extern QueueHandle_t xSystemEventQueue;

/* Constants */
#define STEP_THRESHOLD 1.25f  // Gs
#define STEP_DEBOUNCE_MS 300
#define MOTION_THRESHOLD 0.30f // Variance threshold for motion
#define TILT_THRESHOLD 0.7f
#define SLEEP_MOTION_THRESHOLD 0.05f

/* Global State */
static uint32_t total_steps = 0;
static bool moving = false;
static float last_g[3] = {0, 0, 1.0f};
static uint32_t inactivity_ticks = 0;
TaskHandle_t xAccelTaskHandle = NULL;

uint8_t app_accel_init(void) {
    total_steps = 0;
    moving = false;
    inactivity_ticks = 0;
    return accel_init();
}

uint8_t app_accel_read(float g[3]) {
    uint8_t res = accel_basic_read(g);
    if (res == 0) {
        memcpy(last_g, g, sizeof(last_g));
    }
    return res;
}

void app_accel_get_last_g(float g[3]) {
    memcpy(g, last_g, sizeof(last_g));
}

uint32_t app_accel_get_steps(void) {
    return total_steps;
}

void app_accel_reset_steps(void) {
    total_steps = 0;
}

bool app_accel_is_moving(void) {
    return moving;
}

uint32_t app_accel_get_inactivity_mins(void) {
    // 80ms per tick -> 12.5 ticks per second -> 750 ticks per minute
    return inactivity_ticks / 750;
}

void vAccelTask(void *pvParameters) {
    float g[3];
    float mag;
    bool above_threshold = false;
    bool wrist_up = false;
    TickType_t last_step_time = 0;

    while (1) {
        if (app_accel_read(g) == 0) {
            // Calculate Magnitude
            mag = sqrtf(g[0]*g[0] + g[1]*g[1] + g[2]*g[2]);

            // Simple Pedometer Logic
            if (!above_threshold && mag > STEP_THRESHOLD) {
                TickType_t now = xTaskGetTickCount();
                if ((now - last_step_time) > pdMS_TO_TICKS(STEP_DEBOUNCE_MS)) {
                    total_steps++;
                    last_step_time = now;
                    above_threshold = true;
                }
            } else if (above_threshold && mag < (STEP_THRESHOLD - 0.1f)) {
                above_threshold = false;
            }

            // Tilt-to-Wake Heuristic (Wrist Up)
            if (!wrist_up && g[1] > TILT_THRESHOLD) {
                wrist_up = true;
                UI_Event_t wake_evt = EVENT_WAKE;
                xQueueSend(xSystemEventQueue, &wake_evt, 0);
            } else if (wrist_up && g[1] < (TILT_THRESHOLD - 0.2f)) {
                wrist_up = false;
            }

            // Simple Motion Detection
            float diff = fabsf(mag - 1.0f);
            moving = (diff > MOTION_THRESHOLD);

            // Inactivity Tracking (Sleep Monitor)
            if (diff < SLEEP_MOTION_THRESHOLD) {
                inactivity_ticks++;
            } else {
                inactivity_ticks = 0;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(80)); 
    }
}
