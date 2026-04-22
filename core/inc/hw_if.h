/**
  ******************************************************************************
  * @file    hw_if.h
  * @author  MCD Application Team
  * @brief   Hardware Interface configuration file.
  ******************************************************************************
  */

#ifndef HW_IF_H
#define HW_IF_H

#include "stm32wbxx_hal.h"
#include "stm32_wpan_common.h"

/* Common System Headers for WPAN */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* IPCC Configuration */
#define CFG_IPCC_BT_EVT_R_IT              IPCC_IRQ_CHANNEL_1
#define CFG_IPCC_BT_CMD_W_IT              IPCC_IRQ_CHANNEL_1
#define CFG_IPCC_SYSTEM_EVT_R_IT          IPCC_IRQ_CHANNEL_2
#define CFG_IPCC_SYSTEM_CMD_W_IT          IPCC_IRQ_CHANNEL_2

#endif /* HW_IF_H */
