/**
* @file    display_hal.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-01-30
*/
#include <string.h>
#include "display_hal.h"
#include "ssd1681.h"
#include "ssd1681_interface.h"

ssd1681_handle_t gs_handle;
uint8_t gs_lut[153] = {
    0x80, 0x48, 0x40, 0x0, 0x0, 0x0,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x40, 0x48, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,  0x0,  0x0,  0x80, 0x48, 0x40, 0x0, 0x0, 0x0, 0x0,
    0x0,  0x0,  0x0,  0x0, 0x0, 0x40, 0x48, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x0, 0xA, 0x0,
    0x0,  0x0,  0x0,  0x0, 0x0, 0x8,  0x1,  0x0,  0x8, 0x1, 0x0, 0x2, 0xA,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0,
    0x0,  0x0,  0x0,  0x0, 0x0, 0x0,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x0,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0,
    0x0,  0x0,  0x0,  0x0, 0x0, 0x0,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x0,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x0,  0x0, 0x0,
};


/* static functions */
static uint8_t ssd1681_setup(ssd1681_handle_t *handle) {
    uint8_t res;

    /* set the default driver output */
        res = ssd1681_set_driver_output(handle, SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_MUX, SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_GD, SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_SM,
                                        SSD1681_BASIC_DEFAULT_DRIVER_OUTPUT_TB);
        if (res != 0)
            ssd1681_interface_debug_print("ssd1681: set the driver output failed.\n");;

        res = ssd1681_set_data_entry_mode(handle, SSD1681_BASIC_DEFAULT_ADDRESS_MODE, SSD1681_BASIC_DEFAULT_ADDRESS_DIRECTION);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set data entry mode failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default temperature sensor */
        res = ssd1681_set_temperature_sensor(handle, SSD1681_BASIC_DEFAULT_TEMPERATURE_SENSOR);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set temperature sensor failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default border waveform */
        res = ssd1681_set_border_waveform(handle, SSD1681_BASIC_DEFAULT_VBD, SSD1681_BASIC_DEFAULT_VBD_FIX_LEVEL, SSD1681_BASIC_DEFAULT_GS_TRANSITION, SSD1681_BASIC_DEFAULT_VBD_TRANSITION);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set border waveform failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default x range */
        res = ssd1681_set_ram_x(handle, SSD1681_BASIC_DEFAULT_RAM_X_START, SSD1681_BASIC_DEFAULT_RAM_X_END);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set ram x failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default y range */
        res = ssd1681_set_ram_y(handle, SSD1681_BASIC_DEFAULT_RAM_Y_START, SSD1681_BASIC_DEFAULT_RAM_Y_END);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set ram y failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        res = ssd1681_set_ram_x_address_counter(handle, SSD1681_BASIC_DEFAULT_RAM_X_ADDRESS_COUNTER);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set ram x address counter failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default ram y address counter */
        res = ssd1681_set_ram_y_address_counter(handle, SSD1681_BASIC_DEFAULT_RAM_Y_ADDRESS_COUNTER);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set ram y address counter failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default display update control mode */
        res = ssd1681_set_display_update_control(handle, SSD1681_BASIC_DEFAULT_DISPLAY_CONTROL_RED, SSD1681_BASIC_DEFAULT_DISPLAY_CONTROL_BLACK);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set display update control failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default sequence */
        res = ssd1681_set_display_sequence(handle, SSD1681_BASIC_DEFAULT_DISPLAY_SEQUENCE);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set display sequence failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default vcom sense duration */
        res = ssd1681_set_vcom_sense_duration(handle, SSD1681_BASIC_DEFAULT_VCOM_SENSE_DURATION);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set vcom sense duration failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* enter the vcom sense */
        res = ssd1681_set_enter_vcom_sense(handle);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set enter vcom sense failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the vcom control reg */
        res = ssd1681_set_vcom_control_reg(handle);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set vcom control reg failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the lut register */
        // res = ssd1681_set_lut_register(handle, gs_lut, 153);
        // if (res != 0) {
        //     ssd1681_interface_debug_print("ssd1681: set lut register failed.\n");
        //     (void)ssd1681_deinit(&gs_handle);

        //     return 1;
        // }

        /* set the default end option */
        res = ssd1681_set_end_option(handle, SSD1681_BASIC_DEFAULT_END_OPT);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set end option failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default gate driving voltage */
        res = ssd1681_set_gate_driving_voltage(handle, SSD1681_BASIC_DEFAULT_GATE_DRIVING_VOLTAGE);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set gate driving voltage failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default source driving voltage */
        res = ssd1681_set_source_driving_voltage(handle, SSD1681_BASIC_DEFAULT_VSH1, SSD1681_BASIC_DEFAULT_VSH2, SSD1681_BASIC_DEFAULT_VSL);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set source driving voltage failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default vcom */
        res = ssd1681_set_vcom_register(handle, SSD1681_VCOM_NEGATIVE_0P8);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set vcom register failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default auto write red ram */
        res = ssd1681_set_auto_write_red_ram(handle, SSD1681_BASIC_DEFAULT_AUTO_RED_STEP_HEIGHT, SSD1681_BASIC_DEFAULT_AUTO_RED_STEP_WIDTH);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set auto write red ram failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default auto write black ram */
        res = ssd1681_set_auto_write_black_ram(handle, SSD1681_BASIC_DEFAULT_AUTO_BLACK_STEP_HEIGHT, SSD1681_BASIC_DEFAULT_AUTO_BLACK_STEP_WIDTH);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set auto write black ram failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default hv ready detection */
        res = ssd1681_set_hv_ready_detection(handle, SSD1681_BASIC_DEFAULT_COOL_DOWN_DURATION, SSD1681_BASIC_DEFAULT_COOL_DOWN_LOOP_NUM);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set hv ready detection failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default vci detection */
        res = ssd1681_set_vci_detection(handle, SSD1681_BASIC_DEFAULT_VCI);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set vci detection failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default booster soft start param */
        res = ssd1681_set_booster_soft_start(handle, (uint8_t)(SSD1681_BASIC_DEFAULT_PHASE1), (uint8_t)(SSD1681_BASIC_DEFAULT_PHASE2), (uint8_t)(SSD1681_BASIC_DEFAULT_PHASE3),
                                            (uint8_t)(SSD1681_BASIC_DEFAULT_DURATION));
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set booster soft start failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* set the default mode */
        res = ssd1681_set_mode(handle, SSD1681_BASIC_DEFAULT_MODE);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: set mode failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* clear the black */
        res = ssd1681_clear(handle, SSD1681_COLOR_BLACK);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: clear failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

        /* clear the red */
        res = ssd1681_clear(&gs_handle, SSD1681_COLOR_RED);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: clear failed.\n");
            (void)ssd1681_deinit(&gs_handle);

            return 1;
        }

    return 0;
}

uint8_t display_init() {extern uint8_t gs_lut[];
    uint8_t res;
        /* link functions */
        DRIVER_SSD1681_LINK_INIT(&gs_handle, ssd1681_handle_t);
        DRIVER_SSD1681_LINK_SPI_INIT(&gs_handle, ssd1681_interface_spi_init);
        DRIVER_SSD1681_LINK_SPI_DEINIT(&gs_handle, ssd1681_interface_spi_deinit);
        DRIVER_SSD1681_LINK_SPI_WRITE_CMD(&gs_handle, ssd1681_interface_spi_write_cmd);
        DRIVER_SSD1681_LINK_SPI_READ_CMD(&gs_handle, ssd1681_interface_spi_read_cmd);
        DRIVER_SSD1681_LINK_SPI_CMD_DATA_GPIO_INIT(&gs_handle, ssd1681_interface_spi_cmd_data_gpio_init);
        DRIVER_SSD1681_LINK_SPI_CMD_DATA_GPIO_DEINIT(&gs_handle, ssd1681_interface_spi_cmd_data_gpio_deinit);
        DRIVER_SSD1681_LINK_SPI_CMD_DATA_GPIO_WRITE(&gs_handle, ssd1681_interface_spi_cmd_data_gpio_write);
        DRIVER_SSD1681_LINK_RESET_GPIO_INIT(&gs_handle, ssd1681_interface_reset_gpio_init);
        DRIVER_SSD1681_LINK_RESET_GPIO_DEINIT(&gs_handle, ssd1681_interface_reset_gpio_deinit);
        DRIVER_SSD1681_LINK_RESET_GPIO_WRITE(&gs_handle, ssd1681_interface_reset_gpio_write);
        DRIVER_SSD1681_LINK_BUSY_GPIO_INIT(&gs_handle, ssd1681_interface_busy_gpio_init);
        DRIVER_SSD1681_LINK_BUSY_GPIO_DEINIT(&gs_handle, ssd1681_interface_busy_gpio_deinit);
        DRIVER_SSD1681_LINK_BUSY_GPIO_READ(&gs_handle, ssd1681_interface_busy_gpio_read);
        DRIVER_SSD1681_LINK_DELAY_MS(&gs_handle, ssd1681_interface_delay_ms);
        DRIVER_SSD1681_LINK_DEBUG_PRINT(&gs_handle, ssd1681_interface_debug_print);

        res = ssd1681_init(&gs_handle);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: init failed.\n");;
            return 1;
        }

        res = ssd1681_software_reset(&gs_handle);
        if (res != 0) {
            ssd1681_interface_debug_print("ssd1681: software reset failed.\n");
            return 1;
        }
        
        res = ssd1681_setup(&gs_handle); 
        if(res != 0) {
            ssd1681_interface_debug_print("ssd1681: setup failed\n");
            return 1;
        }
    return 0;
}

uint8_t display_clear() {
    // ssd1681_gram_clear(&gs_handle, S aD1681_COLOR_BLACK);
    memset(gs_handle.black_gram, 0x00, sizeof(gs_handle.black_gram));

    return 0;
}

uint8_t display_write(uint8_t x, uint8_t y, const char *text) {
    uint16_t size = strlen(text);

    ssd1681_gram_write_string(&gs_handle, SSD1681_COLOR_BLACK, x, y, 
                            (char*)text, size, SSD1681_COLOR_BLACK, SSD1681_FONT_24);
    return 0;
}

uint8_t display_update(display_update_mode_t mode) {
    uint8_t res = 0;

    switch (mode) {
        case SSD1681_UPDATE_FULL:
            ssd1681_set_b_ram(&gs_handle);
            
            // 2. Copia a tela base para a RAM Vermelha e trava ela lá
            memcpy(gs_handle.red_gram, gs_handle.black_gram, sizeof(gs_handle.black_gram));
            ssd1681_set_r_ram(&gs_handle); 
            
            // 3. Pisca a tela inteira e fixa a imagem
            res = ssd1681_gram_update(&gs_handle, SSD1681_COLOR_BLACK, SSD1681_UPDATE_FULL);
            break;

        case SSD1681_UPDATE_PARTIAL:
            ssd1681_set_b_ram(&gs_handle);
            
            // 3. Manda atualizar. 
            res = ssd1681_gram_partial_update(&gs_handle); 
            
            // ATENÇÃO: Nunca copie a RAM preta para a vermelha aqui no partial!
            break;
            
        case SSD1681_UPDATE_FAST:
            ssd1681_set_b_ram(&gs_handle);
            
            // 2. Manda a tela atualizar fisicamente
            res = ssd1681_gram_update(&gs_handle, SSD1681_COLOR_BLACK, SSD1681_UPDATE_FAST);
            
            // 3. AGORA SIM! A tela já atualizou.
            // Copiamos a imagem nova para a RAM Vermelha (passado), 
            // para que a PRÓXIMA transição não borre (evita ghosting).
            memcpy(gs_handle.red_gram, gs_handle.black_gram, sizeof(gs_handle.black_gram));
            ssd1681_set_r_ram(&gs_handle); 
            
            break;

        default:
            gs_handle.debug_print("Modo de update invalido!\n");
            return 1;
    }

    if (res != 0) gs_handle.debug_print("failed to update");
    return res;
}
