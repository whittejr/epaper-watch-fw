/**
 * @file driver_ssd1681_basic.h
 * @brief  none
 * @version 0.1
 * @author Alessandro Davi
 * @date 2025-12-26
 */

#ifndef DRIVER_SSD1681_BASIC_H
#define DRIVER_SSD1681_BASIC_H

#include "driver_ssd1681_interface.h"

/**
 * @brief ssd1681 basic example default definition
 */
#define SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_MUX      0xC7                                                                                                           /**< 200 - 1*/
#define SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_GD       SSD1681_BOOL_FALSE                                                                                             /**< disable gd */
#define SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_SM       SSD1681_BOOL_FALSE                                                                                             /**< disable sm */
#define SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_TB       SSD1681_BOOL_FALSE                                                                                             /**< disable tb */
#define SSD1681_BASIC_DEFAULT_ADDRESS_MODE           SSD1681_ADDRESS_MODE_Y_DECREMENT_X_INCREMENT                                                                   /**< y decrement and x increment */
#define SSD1681_BASIC_DEFAULT_ADDRESS_DIRECTION      SSD1681_ADDRESS_DIRECTION_UPDATED_X                                                                            /**< update x */
#define SSD1681_BASIC_DEFAULT_TEMPERATURE_SENSOR     SSD1681_TEMPERATURE_SENSOR_INTERNAL                                                                            /**< internal sensor */
#define SSD1681_BASIC_DEFAULT_VBD                    SSD1681_VBD_GS_TRANSITION                                                                                      /**< vbd gs transition */
#define SSD1681_BASIC_DEFAULT_VBD_FIX_LEVEL          SSD1681_VBD_FIX_LEVEL_VSS                                                                                      /**< vbd fix level vss */
#define SSD1681_BASIC_DEFAULT_GS_TRANSITION          SSD1681_GS_TRANSITION_FOLLOW_LUT                                                                               /**< gs transition follow lut */
#define SSD1681_BASIC_DEFAULT_VBD_TRANSITION         SSD1681_VBD_TRANSITION_LUT1                                                                                    /**< vbd transition lut1 */
#define SSD1681_BASIC_DEFAULT_RAM_X_START            0x00                                                                                                           /**< 0 */
#define SSD1681_BASIC_DEFAULT_RAM_X_END              0x18                                                                                                           /**< 200 / 8 */
#define SSD1681_BASIC_DEFAULT_RAM_Y_START            0xC7                                                                                                           /**< 200 - 1 */
#define SSD1681_BASIC_DEFAULT_RAM_Y_END              0x00                                                                                                           /**< 0 */
#define SSD1681_BASIC_DEFAULT_RAM_X_ADDRESS_COUNTER  0x00                                                                                                           /**< 0 */
#define SSD1681_BASIC_DEFAULT_RAM_Y_ADDRESS_COUNTER  0xC7                                                                                                           /**< 200 - 1 */
#define SSD1681_BASIC_DEFAULT_DISPLAY_CONTROL_RED    SSD1681_DISPLAY_CONTROL_NORMAL                                                                                 /**< normal mode */
#define SSD1681_BASIC_DEFAULT_DISPLAY_CONTROL_BLACK  SSD1681_DISPLAY_CONTROL_NORMAL                                                                                 /**< normal mode */
#define SSD1681_BASIC_DEFAULT_DISPLAY_SEQUENCE       SSD1681_DISPLAY_SEQUENCE_0                                                                                     /**< sequence0 */
#define SSD1681_BASIC_DEFAULT_VCOM_SENSE_DURATION    0x00                                                                                                           /**< 0x00 */
#define SSD1681_BASIC_DEFAULT_END_OPT                SSD1681_END_OPT_NORMAL                                                                                         /**< normal mode */
#define SSD1681_BASIC_DEFAULT_GATE_DRIVING_VOLTAGE   SSD1681_GATE_DRIVING_VOLTAGE_20V                                                                               /**< 20V */
#define SSD1681_BASIC_DEFAULT_VSH1                   SSD1681_VSH_15V                                                                                                /**< 15V */
#define SSD1681_BASIC_DEFAULT_VSH2                   SSD1681_VSH_5V                                                                                                 /**< 5V */
#define SSD1681_BASIC_DEFAULT_VSL                    SSD1681_VSL_NEGATIVE_15                                                                                        /**< -15V */
#define SSD1681_BASIC_DEFAULT_VCOM                   SSD1681_VCOM_NEGATIVE_0P8                                                                                      /**< -0.8V */
#define SSD1681_BASIC_DEFAULT_AUTO_RED_STEP_HEIGHT   SSD1681_STEP_HEIGHT_200                                                                                        /**< 200 */
#define SSD1681_BASIC_DEFAULT_AUTO_RED_STEP_WIDTH    SSD1681_STEP_WIDTH_200                                                                                         /**< 200 */
#define SSD1681_BASIC_DEFAULT_AUTO_BLACK_STEP_HEIGHT SSD1681_STEP_HEIGHT_200                                                                                        /**< 200 */
#define SSD1681_BASIC_DEFAULT_AUTO_BLACK_STEP_WIDTH  SSD1681_STEP_WIDTH_200                                                                                         /**< 200 */
#define SSD1681_BASIC_DEFAULT_COOL_DOWN_DURATION     0x00                                                                                                           /**< 0x00 */
#define SSD1681_BASIC_DEFAULT_COOL_DOWN_LOOP_NUM     0x00                                                                                                           /**< 0x00 */
#define SSD1681_BASIC_DEFAULT_VCI                    SSD1681_VCI_LEVEL_2P3V                                                                                         /**< 2.3V */
#define SSD1681_BASIC_DEFAULT_PHASE1                 (SSD1681_DRIVING_STRENGTH_1 | SSD1681_MIN_OFF_TIME_8P4)                                                        /**< strength1 && mini off 8.4 */
#define SSD1681_BASIC_DEFAULT_PHASE2                 (SSD1681_DRIVING_STRENGTH_2 | SSD1681_MIN_OFF_TIME_9P8)                                                        /**< strength2 && mini off 9.8 */
#define SSD1681_BASIC_DEFAULT_PHASE3                 (SSD1681_DRIVING_STRENGTH_2 | SSD1681_MIN_OFF_TIME_3P9)                                                        /**< strength2 && mini off 3.9 */
#define SSD1681_BASIC_DEFAULT_DURATION               ((SSD1681_PHASE_DURATION_10MS << 4) | (SSD1681_PHASE_DURATION_40MS << 2) | (SSD1681_PHASE_DURATION_40MS << 0)) /**< 10ms && 40ms && 40ms */
#define SSD1681_BASIC_DEFAULT_MODE                   SSD1681_MODE_NORMAL                                                                                            /**< normal mode */

uint8_t ssd1681_basic_init(void);

void ssd1681_menu_test(void);

#endif // DRIVER_SSD1681_BASIC_H