/*
 * bmp.h
 *
 *  Created on: Apr 13, 2025
 *      Author: alere
 */

#ifndef INC_BMP_H_
#define INC_BMP_H_

#include <stdint.h>

#define epd_width 200
#define epd_height 200

/* ICONS */
#define icons_size 24
#define default_icon_start icons_size
#define select_icon_size_x 200
#define select_icon_size_y 61

#define icon_battery_pos_x	(epd_width - (icons_size * 2))
#define icon_battery_pos_y	(icons_size)

#define icon_accel_pos_x		default_icon_start
#define icon_accel_pos_y		(icons_size * 3)

#define icon_oxy_pos_x			default_icon_start
#define icon_oxy_pos_y			(icons_size * 5)

#define icon_meds_pos_x			default_icon_start
#define icon_meds_pos_y			(icons_size * 7)

#define icon_sel_background_pos_x (0)
#define icon_sel_background_pos_y (90)

/* LUTs */
extern uint8_t gs_lut[];
extern uint8_t gs_lut_partial[];

/* Imagens */
extern const uint8_t img[];
extern const uint8_t icon_meds[];
extern const uint8_t icon_accel[];
extern const uint8_t icon_oxi[];
extern const uint8_t icon_battery[];
extern const uint8_t *bitmap_icons[4];
extern const uint8_t scrollbar_background[];
extern const uint8_t item_sel_background[];


#endif /* INC_BMP_H_ */
