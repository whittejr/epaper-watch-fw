/**
 * @file shmfonts.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2026-01-27
 */

#ifndef SHMFONTS_H
#define SHMFONTS_H

#include <stdint.h>

typedef struct {
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef;


extern FontDef Font_6x8;
extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;


#endif // SHMFONTS_H