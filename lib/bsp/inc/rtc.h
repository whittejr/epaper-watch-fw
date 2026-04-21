#ifndef RTC_H
#define RTC_H

#include "stm32wbxx_hal.h"

uint8_t rtc_init(void);
void rtc_get_time(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate);
void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds);

#endif // RTC_H
