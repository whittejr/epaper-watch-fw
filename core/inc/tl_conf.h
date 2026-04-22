/**
  ******************************************************************************
  * @file    tl_conf.h
  * @author  MCD Application Team
  * @brief   Transport Layer configuration file.
  ******************************************************************************
  */

#ifndef TL_CONF_H
#define TL_CONF_H

#include "stm32_wpan_common.h"

/* Total number of events in the pool */
#define TL_EVENT_FRAME_SIZE            (255 + 3)
#define TL_MAX_EVENT_ID                (10)

/* Configuration of the Mailbox memory areas */
#define TL_BLE_EVENT_POOL_SIZE         (TL_EVENT_FRAME_SIZE * TL_MAX_EVENT_ID)
#define TL_SYSTEM_EVENT_POOL_SIZE      (TL_EVENT_FRAME_SIZE * TL_MAX_EVENT_ID)

/* Memory for the Transport Layer */
typedef struct {
  uint8_t p_BleSpareEvtBuffer[TL_EVENT_FRAME_SIZE];
  uint8_t p_SystemSpareEvtBuffer[TL_EVENT_FRAME_SIZE];
  uint8_t p_AsynchEvtPool[TL_BLE_EVENT_POOL_SIZE];
  uint8_t p_SystemPool[TL_SYSTEM_EVENT_POOL_SIZE];
} TL_Pool_t;

#endif /* TL_CONF_H */
