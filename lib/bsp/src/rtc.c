#include "rtc.h"

RTC_HandleTypeDef hrtc;

uint8_t rtc_init(void) {
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    if (HAL_RTC_Init(&hrtc) != HAL_OK) return 1;

    // Default: 21/04/2026 12:00:00
    rtc_set_time(12, 0, 0);

    RTC_DateTypeDef sDate = {0};
    sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
    sDate.Month = RTC_MONTH_APRIL;
    sDate.Date = 21;
    sDate.Year = 26;
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    return 0;
}

void rtc_get_time(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate) {
    HAL_RTC_GetTime(&hrtc, sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, sDate, RTC_FORMAT_BIN);
}

void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    RTC_TimeTypeDef sTime = {0};
    sTime.Hours = hours;
    sTime.Minutes = minutes;
    sTime.Seconds = seconds;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}
