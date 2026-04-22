/**
 * @file    app_oximeter.c
 * @brief   Oximeter application logic with CMSIS-DSP
 * @version 0.2.0
 * @author  Gemini CLI
 * @date    2026-04-21
 */

#include "app_oximeter.h"
#include "oximeter_hal.h"
#include "app_accel.h"
#include "arm_math.h"
#include <math.h>
#include <string.h>
#include <stdbool.h>

/* Constants for Calculation */
#define SAMPLE_RATE 100 // Hz
#define WINDOW_SIZE 500 // 5 seconds of data

/* CMSIS-DSP Biquad Filter (Bandpass 0.5Hz - 4Hz) @ 100Hz fs */
#define NUM_STAGES 2
static float32_t biquad_coeffs[5 * NUM_STAGES] = {
    /* Stage 1: b0, b1, b2, a1, a2 (sign inverted for a1, a2) */
    0.010432413f, 0.020864826f, 0.010432413f, 1.717844586f, -0.763488562f,
    /* Stage 2 */
    1.0f, -2.0f, 1.0f, 1.958530942f, -0.959707933f
};
static float32_t biquad_state_red[4 * NUM_STAGES];
static float32_t biquad_state_ir[4 * NUM_STAGES];
static arm_biquad_casd_df1_inst_f32 S_red, S_ir;

/* Buffers */
static float32_t red_f32[WINDOW_SIZE];
static float32_t ir_f32[WINDOW_SIZE];
static float32_t red_filtered[WINDOW_SIZE];
static float32_t ir_filtered[WINDOW_SIZE];
static uint16_t buffer_index = 0;

/* Results */
static uint8_t last_hr = 0;
static uint8_t last_spo2 = 0;

/* FreeRTOS Task Handle */
TaskHandle_t xOximeterTaskHandle = NULL;

uint8_t app_oximeter_init(void) {
    buffer_index = 0;
    last_hr = 0;
    last_spo2 = 0;

    /* Initialize Hardware */
    if (oximeter_init() != 0) {
        return 1;
    }

    /* Initialize Biquad Filters */
    arm_biquad_cascade_df1_init_f32(&S_red, NUM_STAGES, biquad_coeffs, biquad_state_red);
    arm_biquad_cascade_df1_init_f32(&S_ir, NUM_STAGES, biquad_coeffs, biquad_state_ir);

    return 0;
}

void app_oximeter_calculate(void) {
    if (buffer_index < WINDOW_SIZE) return;

    // SENSOR FUSION: Skip calculation if moving significantly
    if (app_accel_is_moving()) {
        last_hr = 0;
        last_spo2 = 0;
        buffer_index = 0;
        return;
    }

    float32_t mean_red, mean_ir;
    float32_t rms_red, rms_ir;

    // 1. DC Removal using CMSIS Mean
    arm_mean_f32(red_f32, WINDOW_SIZE, &mean_red);
    arm_mean_f32(ir_f32, WINDOW_SIZE, &mean_ir);

    for(int i=0; i<WINDOW_SIZE; i++) {
        red_f32[i] -= mean_red;
        ir_f32[i] -= mean_ir;
    }

    // 2. Bandpass Filtering using CMSIS Biquad
    arm_biquad_cascade_df1_f32(&S_red, red_f32, red_filtered, WINDOW_SIZE);
    arm_biquad_cascade_df1_f32(&S_ir, ir_f32, ir_filtered, WINDOW_SIZE);

    // 3. RMS Calculation using CMSIS
    arm_rms_f32(red_filtered, WINDOW_SIZE, &rms_red);
    arm_rms_f32(ir_filtered, WINDOW_SIZE, &rms_ir);

    // 4. Peak Detection for HR (on filtered IR)
    uint16_t peaks = 0;
    bool ascending = true;
    for(int i=1; i<WINDOW_SIZE; i++) {
        if (ascending && ir_filtered[i] < ir_filtered[i-1]) {
            if (ir_filtered[i-1] > 0.1f) { // Threshold to avoid noise peaks
                peaks++;
            }
            ascending = false;
        } else if (!ascending && ir_filtered[i] > ir_filtered[i-1]) {
            ascending = true;
        }
    }

    last_hr = (uint8_t)(peaks * 12); // peaks in 5s -> BPM
    if (last_hr < 40 || last_hr > 220) last_hr = 0;

    // 5. SpO2 Calculation (Ratio of Ratios)
    if (mean_red > 0 && mean_ir > 0 && rms_ir > 0) {
        float32_t R = (rms_red / mean_red) / (rms_ir / mean_ir);
        float32_t spo2_f = 110.0f - 25.0f * R;
        if (spo2_f > 100.0f) spo2_f = 100.0f;
        if (spo2_f < 70.0f) spo2_f = 0;
        last_spo2 = (uint8_t)spo2_f;
    } else {
        last_spo2 = 0;
    }

    buffer_index = 0;
}

uint8_t app_oximeter_update(uint32_t red, uint32_t ir) {
    red_f32[buffer_index] = (float32_t)red;
    ir_f32[buffer_index] = (float32_t)ir;

    buffer_index++;

    if (buffer_index >= WINDOW_SIZE) {
        app_oximeter_calculate();
        return 1;
    }
    return 0;
}

void vOximeterTask(void *pvParameters) {
    uint32_t red_samples[32];
    uint32_t ir_samples[32];
    uint8_t num_samples;

    while (1) {
        num_samples = 32;
        if (oximeter_read(red_samples, ir_samples, &num_samples) == 0) {
            for (uint8_t i = 0; i < num_samples; i++) {
                app_oximeter_update(red_samples[i], ir_samples[i]);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Poll every 100ms
    }
}

uint8_t app_oximeter_get_hr(void) { return last_hr; }
uint8_t app_oximeter_get_spo2(void) { return last_spo2; }
