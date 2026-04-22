/**
  ******************************************************************************
  * @file    ble_conf.h
  * @author  MCD Application Team
  * @brief   Configuration file for BLE Middleware.
  ******************************************************************************
  */

#ifndef BLE_CONF_H
#define BLE_CONF_H

#include "app_conf.h"

/* Max number of simultaneous connections */
#define BLE_CFG_CLT_MAX_NBR_CB                     (1)
#define BLE_CFG_SRV_MAX_NBR_CB                     (1)

#define BLE_CFG_SRV_MAX_NBR_ATT_PKT                (10)
#define BLE_CFG_CLT_MAX_NBR_ATT_PKT                (10)

#endif /* BLE_CONF_H */
