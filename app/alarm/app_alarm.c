/**
 * @file app_alarm.c
 * @brief Core logic for managing multiple software medication alarms.
 */

#include "app_alarm.h"
#include <string.h>

static alarm_t alarms[MAX_ALARMS];
static uint8_t last_tick_minute = 0xFF; // Avoid multiple triggers in the same minute

void app_alarm_init(void) {
    memset(alarms, 0, sizeof(alarms));
    for (int i = 0; i < MAX_ALARMS; i++) {
        alarms[i].active = false;
    }
}

bool app_alarm_add(uint8_t hours, uint8_t minutes) {
    for (int i = 0; i < MAX_ALARMS; i++) {
        if (!alarms[i].active) {
            alarms[i].hours = hours;
            alarms[i].minutes = minutes;
            alarms[i].active = true;
            return true;
        }
    }
    return false; // No space left
}

void app_alarm_remove(uint8_t index) {
    if (index < MAX_ALARMS) {
        alarms[index].active = false;
    }
}

alarm_t* app_alarm_get_list(void) {
    return alarms;
}

bool app_alarm_tick(uint8_t current_hour, uint8_t current_minute) {
    if (current_minute == last_tick_minute) {
        return false;
    }
    
    last_tick_minute = current_minute;
    
    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarms[i].active && 
            alarms[i].hours == current_hour && 
            alarms[i].minutes == current_minute) {
            return true; // Alarm triggered
        }
    }
    
    return false;
}
