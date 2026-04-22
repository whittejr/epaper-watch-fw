/**
  ******************************************************************************
  * @file    tl_dbg_conf.h
  * @author  MCD Application Team
  * @brief   Debug configuration file for Transport Layer.
  ******************************************************************************
  */

#ifndef TL_DBG_CONF_H
#define TL_DBG_CONF_H

/* Disable all WPAN debug macros to satisfy linker */
#define TL_MM_DBG_MSG(...)
#define TL_HCI_CMD_DBG_MSG(...)
#define TL_HCI_CMD_DBG_BUF(...)
#define TL_HCI_CMD_DBG_RAW(...)
#define TL_HCI_EVT_DBG_MSG(...)
#define TL_HCI_EVT_DBG_BUF(...)
#define TL_HCI_EVT_DBG_RAW(...)
#define TL_SHCI_CMD_DBG_MSG(...)
#define TL_SHCI_CMD_DBG_BUF(...)
#define TL_SHCI_CMD_DBG_RAW(...)
#define TL_SHCI_EVT_DBG_MSG(...)
#define TL_SHCI_EVT_DBG_BUF(...)
#define TL_SHCI_EVT_DBG_RAW(...)

#endif /* TL_DBG_CONF_H */
