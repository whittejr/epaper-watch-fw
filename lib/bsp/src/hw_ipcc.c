/**
  ******************************************************************************
  * @file    hw_ipcc.c
  * @author  Gemini CLI
  * @brief   Hardware IPCC bridge functions for STM32_WPAN
  ******************************************************************************
  */

#include "stm32wbxx_hal.h"
#include "tl.h"

extern IPCC_HandleTypeDef hipcc;

/* Forward declarations of TL ISR functions defined in app_ble.c */
extern void TL_BLE_Isr(void);
extern void TL_SYS_Isr(void);

/* 
 * HW IPCC Bridge functions expected by STM32_WPAN TL 
 */
void HW_IPCC_BLE_Init(void) {
    HAL_IPCC_ActivateNotification(&hipcc, IPCC_CHANNEL_1, IPCC_CHANNEL_DIR_RX, (void*)TL_BLE_Isr);
}

void HW_IPCC_BLE_SendCmd(void) {
    HAL_IPCC_NotifyCPU(&hipcc, IPCC_CHANNEL_1, IPCC_CHANNEL_DIR_TX);
}

void HW_IPCC_SYS_Init(void) {
    HAL_IPCC_ActivateNotification(&hipcc, IPCC_CHANNEL_2, IPCC_CHANNEL_DIR_RX, (void*)TL_SYS_Isr);
}

void HW_IPCC_SYS_SendCmd(void) {
    HAL_IPCC_NotifyCPU(&hipcc, IPCC_CHANNEL_2, IPCC_CHANNEL_DIR_TX);
}

void HW_IPCC_MM_SendFreeBuf(void (*cb)(void)) {
    HAL_IPCC_NotifyCPU(&hipcc, IPCC_CHANNEL_4, IPCC_CHANNEL_DIR_TX);
}

void HW_IPCC_TRACES_Init(void) {
    HAL_IPCC_ActivateNotification(&hipcc, IPCC_CHANNEL_3, IPCC_CHANNEL_DIR_RX, (void*)TL_SYS_Isr);
}
