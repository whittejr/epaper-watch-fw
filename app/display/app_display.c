/**
 * @file    app_display.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-12-26
 */

#include <stdio.h>
#include "app_display.h"
#include "display_hal.h"
#include "stm32wbxx_hal.h"



#define BMP_DEFINE
#include "bmp.h"

ssd1681_handle_t gs_handle;

extern uint8_t gs_lut[];
extern uint32_t red_buffer[32];
extern uint32_t ir_buffer[32];

// uint8_t app_display_init(void) {

//     uint8_t res;
//     /* link functions */
//     DRIVER_SSD1681_LINK_INIT(&gs_handle, ssd1681_handle_t);
//     DRIVER_SSD1681_LINK_SPI_INIT(&gs_handle, ssd1681_interface_spi_init);
//     DRIVER_SSD1681_LINK_SPI_DEINIT(&gs_handle, ssd1681_interface_spi_deinit);
//     DRIVER_SSD1681_LINK_SPI_WRITE_CMD(&gs_handle, ssd1681_interface_spi_write_cmd);
//     DRIVER_SSD1681_LINK_SPI_READ_CMD(&gs_handle, ssd1681_interface_spi_read_cmd);
//     DRIVER_SSD1681_LINK_SPI_CMD_DATA_GPIO_INIT(&gs_handle, ssd1681_interface_spi_cmd_data_gpio_init);
//     DRIVER_SSD1681_LINK_SPI_CMD_DATA_GPIO_DEINIT(&gs_handle, ssd1681_interface_spi_cmd_data_gpio_deinit);
//     DRIVER_SSD1681_LINK_SPI_CMD_DATA_GPIO_WRITE(&gs_handle, ssd1681_interface_spi_cmd_data_gpio_write);
//     DRIVER_SSD1681_LINK_RESET_GPIO_INIT(&gs_handle, ssd1681_interface_reset_gpio_init);
//     DRIVER_SSD1681_LINK_RESET_GPIO_DEINIT(&gs_handle, ssd1681_interface_reset_gpio_deinit);
//     DRIVER_SSD1681_LINK_RESET_GPIO_WRITE(&gs_handle, ssd1681_interface_reset_gpio_write);
//     DRIVER_SSD1681_LINK_BUSY_GPIO_INIT(&gs_handle, ssd1681_interface_busy_gpio_init);
//     DRIVER_SSD1681_LINK_BUSY_GPIO_DEINIT(&gs_handle, ssd1681_interface_busy_gpio_deinit);
//     DRIVER_SSD1681_LINK_BUSY_GPIO_READ(&gs_handle, ssd1681_interface_busy_gpio_read);
//     DRIVER_SSD1681_LINK_DELAY_MS(&gs_handle, ssd1681_interface_delay_ms);
//     // DRIVER_SSD1681_LINK_DEBUG_PRINT(&gs_handle, ssd1681_interface_debug_print);

//     res = ssd1681_init(&gs_handle);
//     if (res != 0)
//         return 1;

//     res = ssd1681_software_reset(&gs_handle);
//     if (res != 0)
//         return 1;

//     /* set the default driver output */
//     res = ssd1681_set_driver_output(&gs_handle, SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_MUX, SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_GD, SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_SM,
//                                     SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_TB);
//     if (res != 0)
//         return 1;

//     res = ssd1681_set_data_entry_mode(&gs_handle, SSD1681_BASIC_DEFAULT_ADDRESS_MODE, SSD1681_BASIC_DEFAULT_ADDRESS_DIRECTION);
//     if (res != 0)
//         return 1;

//     /* set the default temperature sensor */
//     res = ssd1681_set_temperature_sensor(&gs_handle, SSD1681_BASIC_DEFAULT_TEMPERATURE_SENSOR);
//     if (res != 0)
//         return 1;

//     /* set the default border waveform */
//     res = ssd1681_set_border_waveform(&gs_handle, SSD1681_BASIC_DEFAULT_VBD, SSD1681_BASIC_DEFAULT_VBD_FIX_LEVEL, SSD1681_BASIC_DEFAULT_GS_TRANSITION, SSD1681_BASIC_DEFAULT_VBD_TRANSITION);
//     if (res != 0)
//         return 1;

//     /* set the default x range */
//     res = ssd1681_set_ram_x(&gs_handle, SSD1681_BASIC_DEFAULT_RAM_X_START, SSD1681_BASIC_DEFAULT_RAM_X_END);
//     if (res != 0)
//         return 1;

//     /* set the default y range */
//     res = ssd1681_set_ram_y(&gs_handle, SSD1681_BASIC_DEFAULT_RAM_Y_START, SSD1681_BASIC_DEFAULT_RAM_Y_END);
//     if (res != 0)
//         return 1;

//     res = ssd1681_set_ram_x_address_counter(&gs_handle, SSD1681_BASIC_DEFAULT_RAM_X_ADDRESS_COUNTER);
//     if (res != 0)
//         return 1;

//     /* set the default ram y address counter */
//     res = ssd1681_set_ram_y_address_counter(&gs_handle, SSD1681_BASIC_DEFAULT_RAM_Y_ADDRESS_COUNTER);
//     if (res != 0)
//         return 1;

//     /* set the default display update control mode */
//     res = ssd1681_set_display_update_control(&gs_handle, SSD1681_BASIC_DEFAULT_DISPLAY_CONTROL_RED, SSD1681_BASIC_DEFAULT_DISPLAY_CONTROL_BLACK);
//     if (res != 0)
//         return 1;

//     /* set the default sequence */
//     res = ssd1681_set_display_sequence(&gs_handle, SSD1681_BASIC_DEFAULT_DISPLAY_SEQUENCE);
//     if (res != 0)
//         return 1;

//     /* set the default vcom sense duration */
//     res = ssd1681_set_vcom_sense_duration(&gs_handle, SSD1681_BASIC_DEFAULT_VCOM_SENSE_DURATION);
//     if (res != 0)
//         return 1;

//     /* enter the vcom sense */
//     res = ssd1681_set_enter_vcom_sense(&gs_handle);
//     if (res != 0)
//         return 1;

//     /* set the vcom control reg */
//     res = ssd1681_set_vcom_control_reg(&gs_handle);
//     if (res != 0)
//         return 1;

//     /* set the lut register */
//     res = ssd1681_set_lut_register(&gs_handle, gs_lut, 153);
//     if (res != 0)
//         return 1;

//     /* set the default end option */
//     res = ssd1681_set_end_option(&gs_handle, SSD1681_BASIC_DEFAULT_END_OPT);
//     if (res != 0)
//         return 1;

//     /* set the default gate driving voltage */
//     res = ssd1681_set_gate_driving_voltage(&gs_handle, SSD1681_BASIC_DEFAULT_GATE_DRIVING_VOLTAGE);
//     if (res != 0)
//         return 1;

//     /* set the default source driving voltage */
//     res = ssd1681_set_source_driving_voltage(&gs_handle, SSD1681_BASIC_DEFAULT_VSH1, SSD1681_BASIC_DEFAULT_VSH2, SSD1681_BASIC_DEFAULT_VSL);
//     if (res != 0)
//         return 1;

//     /* set the default vcom */
//     res = ssd1681_set_vcom_register(&gs_handle, SSD1681_VCOM_NEGATIVE_0P8);
//     if (res != 0)
//         return 1;

//     /* set the default auto write red ram */
//     res = ssd1681_set_auto_write_red_ram(&gs_handle, SSD1681_BASIC_DEFAULT_AUTO_RED_STEP_HEIGHT, SSD1681_BASIC_DEFAULT_AUTO_RED_STEP_WIDTH);
//     if (res != 0)
//         return 1;

//     /* set the default auto write black ram */
//     res = ssd1681_set_auto_write_black_ram(&gs_handle, SSD1681_BASIC_DEFAULT_AUTO_BLACK_STEP_HEIGHT, SSD1681_BASIC_DEFAULT_AUTO_BLACK_STEP_WIDTH);
//     if (res != 0)
//         return 1;

//     /* set the default hv ready detection */
//     res = ssd1681_set_hv_ready_detection(&gs_handle, SSD1681_BASIC_DEFAULT_COOL_DOWN_DURATION, SSD1681_BASIC_DEFAULT_COOL_DOWN_LOOP_NUM);
//     if (res != 0)
//         return 1;

//     /* set the default vci detection */
//     res = ssd1681_set_vci_detection(&gs_handle, SSD1681_BASIC_DEFAULT_VCI);
//     if (res != 0)
//         return 1;

//     /* set the default booster soft start param */
//     res = ssd1681_set_booster_soft_start(&gs_handle, (uint8_t)(SSD1681_BASIC_DEFAULT_PHASE1), (uint8_t)(SSD1681_BASIC_DEFAULT_PHASE2), (uint8_t)(SSD1681_BASIC_DEFAULT_PHASE3),
//                                          (uint8_t)(SSD1681_BASIC_DEFAULT_DURATION));
//     if (res != 0)
//         return 1;

//     /* set the default mode */
//     res = ssd1681_set_mode(&gs_handle, SSD1681_BASIC_DEFAULT_MODE);
//     if (res != 0)
//         return 1;

//     /* clear the black */
//     res = ssd1681_gram_clear(&gs_handle, SSD1681_COLOR_BW);
//     if (res != 0)
//         return 1;

//     return 0;
// }

uint8_t app_display_menu_test(void) {
    uint8_t full_white[5000];
    for (int i = 0; i < 5000; i++)
        full_white[i] = 0xFF;

    ssd1681_gram_draw_bitmap(&gs_handle, 0, 0, 199, 199, full_white);
    
    char str[20];
    // sprintf(str, "%d", red_buffer[0]);
    str[0] = 20;
    
    ssd1681_gram_write_string(&gs_handle, SSD1681_COLOR_BW, 0, 0, &str[0], 1, 1, SSD1681_FONT_24);
    ssd1681_gram_update(&gs_handle, SSD1681_UPDATE_TYPE_FULL);
    // ssd1681_gram_write_string(ssd1681_handle_t *handle, ssd1681_color_t color, uint8_t x, uint8_t y, char *str, uint16_t len, uint8_t data, ssd1681_font_t font)

    ssd1681_set_mode(&gs_handle, SSD1681_MODE_DEEP_SLEEP_1);
    return 0;
}


/*
* MIP
*/
uint8_t app_display_init(void) {
    // display_init();
    
    // display_split();
    // display_update();
    // display_clear();
    

    tesst_code();
    
    

    // display_test();
    return 0;
}
