#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include "clock.h"
#include "gpio.h"
#include "spi.h"
#include "rtc.h"
#include "uart.h"
#include "i2c.h"

uint8_t bsp_init(void);

#endif // BSP_H
