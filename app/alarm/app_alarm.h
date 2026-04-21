/**
 * @file app_alarm.h
 * @brief Core logic for managing multiple software medication alarms.
 */

#ifndef APP_ALARM_H
#define APP_ALARM_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_ALARMS 5

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    bool active;
} alarm_t;

void app_alarm_init(void);
bool app_alarm_add(uint8_t hours, uint8_t minutes);
void app_alarm_remove(uint8_t index);
alarm_t* app_alarm_get_list(void);
bool app_alarm_tick(uint8_t current_hour, uint8_t current_minute);

#endif // APP_ALARM_H
