/**
 * @file app_state.h
 * @brief Centralized application state and settings for the watch.
 */

#ifndef APP_STATE_H
#define APP_STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    BRIGHTNESS_LOW = 0,
    BRIGHTNESS_MID,
    BRIGHTNESS_HIGH
} brightness_level_t;

typedef struct {
    bool time_format_24h;
    brightness_level_t brightness;
    bool wrist_wake_enabled;
    bool vibration_enabled;
    bool dark_theme;
    bool language_pt;
} watch_settings_t;

// Global settings instance (initialized in app_system.c)
extern watch_settings_t g_watch_settings;

#endif // APP_STATE_H
