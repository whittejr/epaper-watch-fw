/**
  ******************************************************************************
  * @file    app_conf.h
  * @author  MCD Application Team
  * @brief   Application configuration file for STM32WPAN Middleware.
  ******************************************************************************
  */

#ifndef APP_CONF_H
#define APP_CONF_H

#include "hw_if.h"

/* BLE Configuration */
#define CFG_DEV_NAME_BLE                     "E-Paper Watch"
#define CFG_BLE_MAX_CONN_EVENT_LENGTH        (0x280)
#define CFG_BLE_GAP_APPEARANCE               (0x00C0) // Watch Generic

/* GATT Server Capacity */
#define CFG_BLE_NUM_GATT_SERVICES            (8)
#define CFG_BLE_NUM_GATT_ATTRIBUTES          (64)
#define CFG_BLE_ATT_VALUE_ARRAY_SIZE         (1024)

/* BLE Stack Memory Pool */
#define CFG_BLE_MBLOCK_COUNT                 (128)
#define CFG_BLE_MBLOCK_SIZE                  (32)

/* System Configuration */
#define CFG_TL_BLE_EVENT_FRAME_SIZE          (255 + 3)
#define CFG_TL_BLE_MAX_EVENT_ID              (10)

/* OTP Definitions */
#define CFG_OTP_BASE_ADDRESS                 0x1FFF8000
#define CFG_OTP_END_ADRESS                   0x1FFF8400

/* Trace and Debug */
#define CFG_DEBUG_TRACE_UART                 (0) // Set to 1 if you want BLE logs on UART

#endif /* APP_CONF_H */
