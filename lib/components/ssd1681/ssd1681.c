/*
 * ssd1681.c
 *
 *  Created on: Apr 13, 2025
 *      Author: alere
 */

#include "ssd1681.h"
#include "driver_ssd1681_font.h"


#define SSD1681_CMD_SW_RESET 0x12
/**
 * @brief chip information definition
 */
#define CHIP_NAME          "Solomon Systech SSD1681" /**< chip name */
#define MANUFACTURER_NAME  "Solomon Systech"         /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN 2.2f                      /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX 3.7f                      /**< chip max supply voltage */
#define MAX_CURRENT        500.00f                   /**< chip max current */
#define TEMPERATURE_MIN    -40.0f                    /**< chip min operating temperature */
#define TEMPERATURE_MAX    85.0f                     /**< chip max operating temperature */
#define DRIVER_VERSION     1000                      /**< driver version */

/**
 * @brief chip command data definition
 */
#define SSD1681_CMD  0 /**< command */
#define SSD1681_DATA 1 /**< data */

/**
 * @brief chip command definition
 */
#define SSD1681_CMD_DRIVER_OUTPUT_CONTROL                   0x01
#define SSD1681_CMD_GATE_DRIVING_VOLTAGE_CONTROL            0x03
#define SSD1681_CMD_SOURCE_DRIVING_VOLTAGE_CONTROL          0x04
#define SSD1681_CMD_INITIAL_CODE_SETTING_OTP_PROGRAM        0x08
#define SSD1681_CMD_WRITE_REGISTER_FOR_INITIAL_CODE_SETTING 0x09
#define SSD1681_CMD_READ_REGISTER_FOR_INITIAL_CODE_SETTING  0x0A
#define SSD1681_CMD_BOOSTER_SOFT_START_CONTROL              0x0C
#define SSD1681_CMD_DEEP_SLEEP_MODE                         0x10
#define SSD1681_CMD_DATA_ENTRY_MODE_SETTING                 0x11
#define SSD1681_CMD_SW_RESET                                0x12
#define SSD1681_CMD_HV_READY_DETECTION                      0x14
#define SSD1681_CMD_VCI_DETECTION                           0x15
#define SSD1681_CMD_TEMPERATURE_SENSOR_CONTROL              0x18
#define SSD1681_CMD_TEMPERATURE_SENSOR_CONTROL_WRITE        0x1A

#define SSD1681_CMD_TEMPERATURE_SENSOR_CONTROL_READ 0x1B

#define SSD1681_CMD_TEMPERATURE_SENSOR_CONTROL_WRITE_EXT 0x1C

#define SSD1681_CMD_MASTER_ACTIVATION                      0x20
#define SSD1681_CMD_DISPLAY_UPDATA_CONTROL_1               0x21
#define SSD1681_CMD_DISPLAY_UPDATA_CONTROL_2               0x22
#define SSD1681_CMD_WRITE_RAM_BW                           0x24
#define SSD1681_CMD_WRITE_RAM_RED                          0x26
#define SSD1681_CMD_READ_RAM                               0x27
#define SSD1681_CMD_VCOM_SENSE                             0x28
#define SSD1681_CMD_VCOM_SENSE_DURATION                    0x29
#define SSD1681_CMD_PROGRAM_VCOM_OTP                       0x2A
#define SSD1681_CMD_WRITE_REGISTER_FOR_VCOM_CONTROL        0x2B
#define SSD1681_CMD_WRITE_VCOM_REGISTER                    0x2C
#define SSD1681_CMD_OTP_REGISTER_READ_FOR_DISPLAY_OPTION   0x2D
#define SSD1681_CMD_USER_ID_READ                           0x2E
#define SSD1681_CMD_STATUS_BIT_READ                        0x2F
#define SSD1681_CMD_PROGRAM_WS_OTP                         0x30
#define SSD1681_CMD_LOAD_WS_OTP                            0x31
#define SSD1681_CMD_WRITE_LUT_REGISTER                     0x32
#define SSD1681_CMD_CRC_CALCULATION                        0x34
#define SSD1681_CMD_CRC_STATUS_READ                        0x35
#define SSD1681_CMD_PROGRAM_OTP_SELECTION                  0x36
#define SSD1681_CMD_WRITE_REGISTER_FOR_DISPLAY_OPTION      0x37
#define SSD1681_CMD_WRITE_REGISTER_FOR_USER_ID             0x38
#define SSD1681_CMD_OTP_PROGRAM_MODE                       0x39
#define SSD1681_CMD_BORDER_WAVEFORM_CONTROL                0x3C
#define SSD1681_CMD_END_OPTION                             0x3F
#define SSD1681_CMD_READ_RAM_OPTION                        0x41
#define SSD1681_CMD_SET_RAM_X                              0x44
#define SSD1681_CMD_SET_RAM_Y                              0x45
#define SSD1681_CMD_AUTO_WRITE_RED_RAM_FOR_REGULAR_PATTERN 0x46
#define SSD1681_CMD_AUTO_WRITE_BW_RAM_FOR_REGULAR_PATTERN  0x47
#define SSD1681_CMD_SET_RAM_X_ADDRESS_COUNTER              0x4E
#define SSD1681_CMD_SET_RAM_Y_ADDRESS_COUNTER              0x4F
#define SSD1681_CMD_SET_RAM_X_ADDRESS_WINDOW               0x44
#define SSD1681_CMD_SET_RAM_Y_ADDRESS_WINDOW               0x45
#define SSD1681_CMD_NOP                                    0x7F

/**
 * @brief     write multiple bytes
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] command set command
 * @param[in] *data pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ssd1681_multiple_write_byte(ssd1681_handle_t *handle, uint8_t command, uint8_t *data, uint16_t len) {
    uint8_t res;
    uint8_t value;
    uint16_t i;
    uint16_t timeout_counter = 0;
    value = 1;

    do {
        res = handle->busy_gpio_read(&value);
        // if (res != 0)
        //     return 1; // Erro lendo GPIO

        if (res == 1) {           // Se estiver ocupado
            handle->delay_ms(10); // Espere um pouco

            // Opcional: adicione um timeout MUITO longo para
            // detectar um hang completo (ex: 10 segundos)
            timeout_counter += 10;
            if (timeout_counter > 10000) {
                // handle->debug_print("SSD1681 BUSY Timeout (>10s)\n");
                return 1; // Erro de Timeout
            }
        }
    } while (value == 1);

    if (command != 0) {
        res = handle->spi_cmd_data_gpio_write(SSD1681_CMD);
        if (res != 0)
            return 1;

        res = handle->spi_write_cmd(&command, 1);
        if (res != 0)
            return 1;
    }

    if (len != 0) {
        res = handle->spi_cmd_data_gpio_write(SSD1681_DATA);
        if (res != 0)
            return 1;

        if (handle->spi_write_cmd(data, len) != 0)
            return 1;
    }

    return 0;
}

static void ssd1681_wait_busy(ssd1681_handle_t *handle, uint8_t *value) {
    // Assumindo que 1 (HIGH) = Ocupado. Verifique seu hardware.
    while (handle->busy_gpio_read(value) == 1) {

        handle->delay_ms(10); // Espera 10ms
    }
}

static uint8_t a_ssd1681_multiple_read_byte(ssd1681_handle_t *handle, uint8_t command, uint8_t *data, uint16_t len) {
    uint8_t res;
    uint8_t value;
    uint16_t i;

    value = 1;
    for (i = 0; i < SSD1681_BUSY_MAX_RETRY_TIMES; i++) {
        res = handle->busy_gpio_read(&value);
        if (res != 0) {
            return 1;
        }
        if (value == 1) {
            handle->delay_ms(SSD1681_BUSY_MAX_DELAY_MS);
        } else {
            break;
        }
    }
    if (value == 1) {
        return 1;
    }

    res = handle->spi_cmd_data_gpio_write(SSD1681_CMD);
    if (res != 0) {
        return 1;
    }
    res = handle->spi_write_cmd(&command, 1);
    if (res != 0) {
        return 1;
    }
    if (len != 0) {
        res = handle->spi_cmd_data_gpio_write(SSD1681_DATA);
        if (res != 0) {
            return 1;
        }
        for (i = 0; i < len; i++) {
            if (handle->spi_read_cmd(data + i, 1) != 0) {
                return 1;
            }
        }
    }

    return 0;
}

uint8_t ssd1681_init(ssd1681_handle_t *handle) {
    if (handle == NULL) {
        return 2;
    }
    // if (handle->debug_print == NULL) {
    //     return 3;
    // }
    if (handle->spi_init == NULL) {
        handle->debug_print("ssd1681: spi_init is null.\n");

        return 3;
    }
    if (handle->spi_deinit == NULL) {
        handle->debug_print("ssd1681: spi_deinit is null.\n");

        return 3;
    }
    if (handle->spi_write_cmd == NULL) {
        handle->debug_print("ssd1681: spi_write_cmd is null.\n");

        return 3;
    }
    if (handle->delay_ms == NULL) {
        handle->debug_print("ssd1681: delay_ms is null.\n");

        return 3;
    }
    if (handle->spi_cmd_data_gpio_init == NULL) {
        handle->debug_print("ssd1681: spi_cmd_data_gpio_init is null.\n");

        return 3;
    }
    if (handle->spi_cmd_data_gpio_deinit == NULL) {
        handle->debug_print("ssd1681: spi_cmd_data_gpio_deinit is null.\n");

        return 3;
    }
    if (handle->spi_cmd_data_gpio_write == NULL) {
        handle->debug_print("ssd1681: spi_cmd_data_gpio_write is null.\n");

        return 3;
    }
    if (handle->reset_gpio_init == NULL) {
        handle->debug_print("ssd1681: reset_gpio_init is null.\n");

        return 3;
    }
    if (handle->reset_gpio_deinit == NULL) {
        handle->debug_print("ssd1681: reset_gpio_deinit is null.\n");

        return 3;
    }
    if (handle->reset_gpio_write == NULL) {
        handle->debug_print("ssd1681: reset_gpio_write is null.\n");

        return 3;
    }
    if (handle->busy_gpio_init == NULL) {
        handle->debug_print("ssd1681: busy_gpio_init is null.\n");

        return 3;
    }
    if (handle->busy_gpio_deinit == NULL) {
        handle->debug_print("ssd1681: busy_gpio_deinit is null.\n");

        return 3;
    }
    if (handle->busy_gpio_read == NULL) {
        handle->debug_print("ssd1681: busy_gpio_read is null.\n");

        return 3;
    }

    if (handle->spi_cmd_data_gpio_init() != 0) {
        handle->debug_print("ssd1681: spi cmd data gpio init failed.\n");

        return 5;
    }
    if (handle->reset_gpio_init() != 0) {
        handle->debug_print("ssd1681: reset gpio init failed.\n");
        (void)handle->spi_cmd_data_gpio_deinit();

        return 5;
    }
    if (handle->busy_gpio_init() != 0) {
        handle->debug_print("ssd1681: busy gpio init failed.\n");
        (void)handle->spi_cmd_data_gpio_deinit();
        (void)handle->reset_gpio_deinit();

        return 5;
    }

    if (handle->reset_gpio_write(0) != 0) {
        handle->debug_print("ssd1681: reset gpio write failed.\n");
        (void)handle->spi_cmd_data_gpio_deinit();
        (void)handle->reset_gpio_deinit();
        (void)handle->busy_gpio_deinit();

        return 4;
    }
    handle->delay_ms(10);
    if (handle->reset_gpio_write(1) != 0) {
        handle->debug_print("ssd1681: reset gpio write failed.\n");
        (void)handle->spi_cmd_data_gpio_deinit();
        (void)handle->reset_gpio_deinit();
        (void)handle->busy_gpio_deinit();

        return 4;
    }
    handle->delay_ms(200);

    if (handle->spi_init() != 0) {
        handle->debug_print("ssd1681: spi init failed.\n");
        (void)handle->spi_cmd_data_gpio_deinit();
        (void)handle->reset_gpio_deinit();
        (void)handle->busy_gpio_deinit();

        return 1;
    }

    handle->inited = 1;

    return 0;
}

/**
 * @brief     software reset
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 software reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_software_reset(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL)
        return 2;

    if (handle->inited != 1)
        return 3;

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SW_RESET, NULL, 0);
    if (res != 0)
        return 1;

    return 0;
}

// uint8_t ssd1681_write_cmd(ssd1681_handle_t *handle, uint8_t *buf, uint8_t
// len) {
//     uint8_t res;
//     uint8_t i;

//     if (handle == NULL) {
//         return 2;
//     }
//     if (handle->inited != 1) {
//         return 3;
//     }

//     // res = handle->spi_cmd_data_gpio_write(SSD1681_CMD);
//     // if (res != 0) {
//     //     return 1;
//     // }

//     for (i = 0; i < len; i++) {
//         if (handle->spi_write_cmd(buf + i, 1) != 0) {
//             return 1;
//         }
//     }

//     return 0;
// }

/**
 * @brief     write the register data
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
// uint8_t ssd1681_write_data(ssd1681_handle_t *handle, uint8_t *buf,
//                            uint8_t len) {
//     uint8_t res;
//     uint8_t i;

//     if (handle == NULL) {
//         return 2;
//     }
//     if (handle->inited != 1) {
//         return 3;
//     }

//     res = handle->spi_cmd_data_gpio_write(SSD1681_DATA);
//     if (res != 0) {
//         return 1;
//     }
//     for (i = 0; i < len; i++) {
//         if (handle->spi_write_cmd(buf + i, 1) != 0) {
//             return 1;
//         }
//     }

//     return 0;
// }

uint8_t ssd1681_set_driver_output(ssd1681_handle_t *handle, uint16_t mux, ssd1681_bool_t gd, ssd1681_bool_t sm,
                                  ssd1681_bool_t tb) {
    uint8_t res;
    uint8_t b;
    uint8_t buf[3];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (mux > 0x1FF) {

        return 4;
    }

    buf[0] = (mux >> 0) & 0xFF;
    buf[1] = (mux >> 8) & 0x01;
    b = 0;
    if (gd != 0) {
        b |= 1 << 2;
    }
    if (sm != 0) {
        b |= 1 << 1;
    }
    if (tb != 0) {
        b |= 1 << 0;
    }
    buf[2] = b;

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_DRIVER_OUTPUT_CONTROL, (uint8_t *)buf, 3);
    if (res != 0) {

        return 1;
    }

    return 0;
}

/**
 * @brief     set the gate driving voltage
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] voltage gate driving voltage
 * @return    status code
 *            - 0 success
 *            - 1 set gate driving voltage failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_gate_driving_voltage(ssd1681_handle_t *handle, ssd1681_gate_driving_voltage_t voltage) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = voltage; /* set the voltage */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_GATE_DRIVING_VOLTAGE_CONTROL, (uint8_t *)buf,
                                        1); /* write byte */
    if (res != 0)                           /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the source driving voltage
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] vsh1 set vsh1
 * @param[in] vsh2 set vsh2
 * @param[in] vsl set vsl
 * @return    status code
 *            - 0 success
 *            - 1 set source driving voltage failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      vsh1 >= vsh2
 */
uint8_t ssd1681_set_source_driving_voltage(ssd1681_handle_t *handle, ssd1681_vsh_t vsh1, ssd1681_vsh_t vsh2,
                                           ssd1681_vsl_t vsl) {
    uint8_t res;
    uint8_t buf[3];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = vsh1; /* set vsh1 */
    buf[1] = vsh2; /* set vsh2 */
    buf[2] = vsl;  /* set vsl */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SOURCE_DRIVING_VOLTAGE_CONTROL, (uint8_t *)buf,
                                        3); /* write byte */
    if (res != 0)                           /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     initial code setting otp program
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 otp initial failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_otp_initial(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_INITIAL_CODE_SETTING_OTP_PROGRAM, NULL, 0); /* write byte */
    if (res != 0) /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     write register for initial code setting
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] *param pointer to a param buffer
 * @return    status code
 *            - 0 success
 *            - 1 otp initial write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_otp_initial_write(ssd1681_handle_t *handle, uint8_t param[4]) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_REGISTER_FOR_INITIAL_CODE_SETTING, param,
                                        4); /* write byte */
    if (res != 0)                           /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     read register for initial code setting
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 otp initial read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_otp_initial_read(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_read_byte(handle, SSD1681_CMD_READ_REGISTER_FOR_INITIAL_CODE_SETTING, NULL,
                                       0); /* read byte */
    if (res != 0)                          /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the booster soft start
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] phase1 phase 1
 * @param[in] phase2 phase 2
 * @param[in] phase3 phase 3
 * @param[in] duration phase duration
 * @return    status code
 *            - 0 success
 *            - 1 set booster soft start failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_booster_soft_start(ssd1681_handle_t *handle, uint8_t phase1, uint8_t phase2, uint8_t phase3,
                                       uint8_t duration) {
    uint8_t res;
    uint8_t buf[4];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = 0x80 | phase1;                                                                      /* set the phase1 */
    buf[1] = 0x80 | phase2;                                                                      /* set the phase2 */
    buf[2] = 0x80 | phase3;                                                                      /* set the phase3 */
    buf[3] = duration;                                                                           /* set the duration */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_BOOSTER_SOFT_START_CONTROL, buf, 4); /* write byte */
    if (res != 0)                                                                                /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the mode
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] mode set mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_mode(ssd1681_handle_t *handle, ssd1681_mode_t mode) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = mode;                                                                    /* set the mode */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_DEEP_SLEEP_MODE, buf, 1); /* write byte */
    if (res != 0)                                                                     /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the data entry mode
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] mode address mode
 * @param[in] direction address direction
 * @return    status code
 *            - 0 success
 *            - 1 set data entry mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_data_entry_mode(ssd1681_handle_t *handle, ssd1681_address_mode_t mode,
                                    ssd1681_address_direction_t direction) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = (uint8_t)((mode << 0) | (direction << 2));                                       /* set the config */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_DATA_ENTRY_MODE_SETTING, buf, 1); /* write byte */
    if (res != 0)                                                                             /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the hv ready detection
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] cool_down_duration cool down duration
 * @param[in] cool_down_loop_num cool down loop num
 * @return    status code
 *            - 0 success
 *            - 1 set hv ready detection failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 cool_down_duration is over 7
 *            - 5 cool_down_loop_num is over 7
 * @note      cool_down_duration <= 7
 *            cool_down_loop_num <= 7
 *            duration = 10ms x (cool_down_duration + 1)
 */
uint8_t ssd1681_set_hv_ready_detection(ssd1681_handle_t *handle, uint8_t cool_down_duration,
                                       uint8_t cool_down_loop_num) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }
    if (cool_down_duration > 0x7) /* check the cool_down_duration */
    {

        return 4; /* return error */
    }
    if (cool_down_loop_num > 0x7) /* check the cool_down_loop_num */
    {

        return 5; /* return error */
    }

    buf[0] = (cool_down_duration << 4) | (cool_down_loop_num << 0);                      /* set the config */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_HV_READY_DETECTION, buf, 1); /* write byte */
    if (res != 0)                                                                        /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the vci detection
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] level vci level
 * @return    status code
 *            - 0 success
 *            - 1 set vci detection failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_vci_detection(ssd1681_handle_t *handle, ssd1681_vci_level_t level) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = level;                                                                 /* set the level */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_VCI_DETECTION, buf, 1); /* write byte */
    if (res != 0)                                                                   /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the temperature sensor
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] sensor temperature sensor
 * @return    status code
 *            - 0 success
 *            - 1 set temperature sensor failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_temperature_sensor(ssd1681_handle_t *handle, ssd1681_temperature_sensor_t sensor) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = sensor; /* set the temperature sensor */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_TEMPERATURE_SENSOR_CONTROL, buf, 1); /* write byte */
    if (res != 0)                                                                                /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     write the temperature sensor
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] control set control
 * @return    status code
 *            - 0 success
 *            - 1 write temperature sensor failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 control is over 0xFFF
 * @note      none
 */
uint8_t ssd1681_write_temperature_sensor(ssd1681_handle_t *handle, uint16_t control) {
    uint8_t res;
    uint8_t buf[2];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }
    if (control > 0xFFF) /* check the control */
    {

        return 4; /* return error */
    }

    buf[0] = (control >> 4) & 0xFF;        /* set the temperature data */
    buf[1] = ((control >> 0) << 4) & 0xFF; /* set the temperature data */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_TEMPERATURE_SENSOR_CONTROL_WRITE, buf, 2); /* write byte */
    if (res != 0) /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief      read the temperature sensor
 * @param[in]  *handle pointer to an ssd1681 handle structure
 * @param[out] *control pointer to a control buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature sensor failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ssd1681_read_temperature_sensor(ssd1681_handle_t *handle, uint16_t *control) {
    uint8_t res;
    uint8_t buf[2];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_read_byte(handle, SSD1681_CMD_TEMPERATURE_SENSOR_CONTROL_READ, buf, 2); /* read byte */
    if (res != 0) /* check the result */
    {

        return 1; /* return error */
    }
    *control = (((uint16_t)buf[0]) << 4) | ((buf[1] >> 4) & 0xFF); /* set the control */

    return 0; /* success return 0 */
}

/**
 * @brief     write command to external temperature sensor
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] *param pointer to a param buffer
 * @return    status code
 *            - 0 success
 *            - 1 write temperature sensor ext failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_write_temperature_sensor_ext(ssd1681_handle_t *handle, uint8_t param[3]) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_TEMPERATURE_SENSOR_CONTROL_WRITE_EXT, param,
                                        3); /* write byte */
    if (res != 0)                           /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     master activate
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set master activate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_master_activate(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_MASTER_ACTIVATION, NULL, 0); /* write byte */
    if (res != 0)                                                                        /* check the result */
    {

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the display update control
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] red_control red control
 * @param[in] black_control black control
 * @return    status code
 *            - 0 success
 *            - 1 set display update control failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_display_update_control(ssd1681_handle_t *handle, ssd1681_display_control_t red_control,
                                           ssd1681_display_control_t black_control) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = (uint8_t)((red_control << 4) | (black_control << 0)); /* set the display update control */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_DISPLAY_UPDATA_CONTROL_1, buf, 1); /* write byte */
    if (res != 0)                                                                              /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the display sequence
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] sequence display sequence
 * @return    status code
 *            - 0 success
 *            - 1 set display sequence failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_display_sequence(ssd1681_handle_t *handle, ssd1681_display_sequence_t sequence) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = sequence; /* set the display sequence */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_DISPLAY_UPDATA_CONTROL_2, buf, 1); /* write byte */
    if (res != 0)                                                                              /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the black ram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set black ram failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_black_ram(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_RAM_BW, NULL, 0); /* write byte */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the red ram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set red ram failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_red_ram(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_RAM_RED, NULL, 0); /* write byte */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     get the ram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 get ram failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_get_ram(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_READ_RAM, NULL, 0); /* write byte */
    if (res != 0)                                                               /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set enter vcom sense
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set enter vcom sense failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_enter_vcom_sense(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_VCOM_SENSE, NULL, 0); /* write byte */
    if (res != 0)                                                                 /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the vcom sense duration
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] duration vcom sense duration
 * @return    status code
 *            - 0 success
 *            - 1 set vcom sense duration failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      vcom sense duration = (duration + 1) sec
 */
uint8_t ssd1681_set_vcom_sense_duration(ssd1681_handle_t *handle, uint8_t duration) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = duration;                                                                    /* set the duration */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_VCOM_SENSE_DURATION, buf, 1); /* write byte */
    if (res != 0)                                                                         /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     program vcom otp
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 otp program vcom failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_otp_program_vcom(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_PROGRAM_VCOM_OTP, NULL, 0); /* write byte */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     write register for vcom control
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set vcom control reg failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_vcom_control_reg(ssd1681_handle_t *handle) {
    uint8_t res;
    uint8_t buf[2];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = 0x04;                                                                                    /* command 1 */
    buf[1] = 0x63;                                                                                    /* command 2 */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_REGISTER_FOR_VCOM_CONTROL, buf, 2); /* write byte */
    if (res != 0) /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     write vcom register
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] vcom set vcom
 * @return    status code
 *            - 0 success
 *            - 1 set vcom register failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_vcom_register(ssd1681_handle_t *handle, ssd1681_vcom_t vcom) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = vcom;                                                                        /* set the vcom */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_VCOM_REGISTER, buf, 1); /* write byte */
    if (res != 0)                                                                         /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief      otp register read for display option
 * @param[in]  *handle pointer to an ssd1681 handle structure
 * @param[out] *param pointer to a param buffer
 * @return     status code
 *             - 0 success
 *             - 1 otp read register failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ssd1681_otp_read_register(ssd1681_handle_t *handle, uint8_t param[11]) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_read_byte(handle, SSD1681_CMD_OTP_REGISTER_READ_FOR_DISPLAY_OPTION, param,
                                       11); /* read byte */
    if (res != 0)                           /* check the result */
    {
        handle->debug_print("ssd1681: multiple read byte failed.\n"); /* multiple read byte
                                                                         failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief      get the user id
 * @param[in]  *handle pointer to an ssd1681 handle structure
 * @param[out] *id pointer to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get user id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ssd1681_get_user_id(ssd1681_handle_t *handle, uint8_t id[10]) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_read_byte(handle, SSD1681_CMD_USER_ID_READ, id, 10); /* read byte */
    if (res != 0)                                                                 /* check the result */
    {
        handle->debug_print("ssd1681: multiple read byte failed.\n"); /* multiple read byte
                                                                         failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief      get the status
 * @param[in]  *handle pointer to an ssd1681 handle structure
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ssd1681_get_status(ssd1681_handle_t *handle, uint8_t *status) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_read_byte(handle, SSD1681_CMD_STATUS_BIT_READ, status, 1); /* read byte */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("ssd1681: multiple read byte failed.\n"); /* multiple read byte
                                                                         failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     program otp of waveform setting
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 otp program waveform failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_otp_program_waveform(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_PROGRAM_WS_OTP, NULL, 0); /* write byte */
    if (res != 0)                                                                     /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     load otp of waveform setting
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 otp load waveform failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_otp_load_waveform(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_LOAD_WS_OTP, NULL, 0); /* write byte */
    if (res != 0)                                                                  /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the lut register
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] *reg pointer to a register buffer
 * @param[in] len register length
 * @return    status code
 *            - 0 success
 *            - 1 set lut register failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_lut_register(ssd1681_handle_t *handle, uint8_t *reg, uint16_t len) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_LUT_REGISTER, reg, len); /* write byte */
    if (res != 0)                                                                          /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the crc calculation
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set crc calculation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_crc_calculation(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_CRC_CALCULATION, NULL, 0); /* write byte */
    if (res != 0)                                                                      /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief      get the crc status
 * @param[in]  *handle pointer to an ssd1681 handle structure
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get crc status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ssd1681_get_crc_status(ssd1681_handle_t *handle, uint16_t *status) {
    uint8_t res;
    uint8_t buf[2];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_read_byte(handle, SSD1681_CMD_CRC_STATUS_READ, buf, 2); /* read byte */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("ssd1681: multiple read byte failed.\n"); /* multiple read byte
                                                                         failed */

        return 1; /* return error */
    }
    *status = (uint16_t)(buf[0] << 0) | buf[1]; /* set the status */

    return 0; /* success return 0 */
}

/**
 * @brief     program otp selection
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 otp program selection failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_otp_program_selection(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_PROGRAM_OTP_SELECTION, NULL, 0); /* write byte */
    if (res != 0)                                                                            /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     otp write register for display option
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] *param pointer to a param buffer
 * @return    status code
 *            - 0 success
 *            - 1 otp write register failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_otp_write_register(ssd1681_handle_t *handle, uint8_t param[11]) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_REGISTER_FOR_DISPLAY_OPTION, param,
                                        11); /* write byte */
    if (res != 0)                            /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the user id
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] *id pointer to an id buffer
 * @return    status code
 *            - 0 success
 *            - 1 set user id failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_user_id(ssd1681_handle_t *handle, uint8_t id[10]) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_REGISTER_FOR_USER_ID, id, 10); /* write byte */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the otp program mode
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] mode otp mode
 * @return    status code
 *            - 0 success
 *            - 1 otp program mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_otp_program_mode(ssd1681_handle_t *handle, ssd1681_otp_mode_t mode) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = mode;                                                                     /* set the mode */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_OTP_PROGRAM_MODE, buf, 1); /* write byte */
    if (res != 0)                                                                      /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the border waveform
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] vbd select vbd option
 * @param[in] level fix level setting for vbd
 * @param[in] gs_tran gs transition control
 * @param[in] vbd_tran gs transition setting for vbd
 * @return    status code
 *            - 0 success
 *            - 1 set border waveform failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_border_waveform(ssd1681_handle_t *handle, ssd1681_vbd_t vbd, ssd1681_vbd_fix_level_t level,
                                    ssd1681_gs_transition_t gs_tran, ssd1681_vbd_transition_t vbd_tran) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = (uint8_t)((vbd << 6) | (level << 4) | (gs_tran << 2) | (vbd_tran << 0));         /* set the mode */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_BORDER_WAVEFORM_CONTROL, buf, 1); /* write byte */
    if (res != 0)                                                                             /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     end option
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] opt end option
 * @return    status code
 *            - 0 success
 *            - 1 set end option failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_end_option(ssd1681_handle_t *handle, ssd1681_end_opt_t opt) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = opt;                                                                /* set the mode */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_END_OPTION, buf, 1); /* write byte */
    if (res != 0)                                                                /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the read ram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] ram read ram option
 * @return    status code
 *            - 0 success
 *            - 1 set read ram failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_read_ram(ssd1681_handle_t *handle, ssd1681_read_ram_t ram) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = ram;                                                                     /* set the mode */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_READ_RAM_OPTION, buf, 1); /* write byte */
    if (res != 0)                                                                     /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the ram x range
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] start start address
 * @param[in] end end address
 * @return    status code
 *            - 0 success
 *            - 1 set ram x failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start > 0x3F
 *            - 5 end > 0x3F
 * @note      none
 */
uint8_t ssd1681_set_ram_x(ssd1681_handle_t *handle, uint8_t start, uint8_t end) {
    uint8_t res;
    uint8_t buf[2];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }
    if (start > 0x3F) /* check the start */
    {
        handle->debug_print("ssd1681: start > 0x3F.\n"); /* start > 0x3F */

        return 4; /* return error */
    }
    if (end > 0x3F) /* check the end */
    {
        handle->debug_print("ssd1681: end > 0x3F.\n"); /* end > 0x3F */

        return 5; /* return error */
    }

    buf[0] = start;                                                             /* set the start address */
    buf[1] = end;                                                               /* set the end address */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SET_RAM_X, buf, 2); /* write byte */
    if (res != 0)                                                               /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the ram y range
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] start start address
 * @param[in] end end address
 * @return    status code
 *            - 0 success
 *            - 1 set ram y failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_ram_y(ssd1681_handle_t *handle, uint16_t start, uint16_t end) {
    uint8_t res;
    uint8_t buf[4];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = start & 0xFF;                                                      /* set the start address */
    buf[1] = (start >> 8) & 0x1;                                                /* set the start address */
    buf[2] = end & 0xFF;                                                        /* set the end address */
    buf[3] = (end >> 8) & 0x01;                                                 /* set the end address */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SET_RAM_Y, buf, 4); /* write byte */
    if (res != 0)                                                               /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the auto write red ram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] height step height
 * @param[in] width step width
 * @return    status code
 *            - 0 success
 *            - 1 set auto write red ram failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_auto_write_red_ram(ssd1681_handle_t *handle, ssd1681_step_height_t height,
                                       ssd1681_step_width_t width) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = (uint8_t)((height << 4) | (width << 0)); /* set the param */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_AUTO_WRITE_RED_RAM_FOR_REGULAR_PATTERN, buf,
                                        1); /* write byte */
    if (res != 0)                           /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the auto write black ram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] height step height
 * @param[in] width step width
 * @return    status code
 *            - 0 success
 *            - 1 set auto write black ram failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_set_auto_write_black_ram(ssd1681_handle_t *handle, ssd1681_step_height_t height,
                                         ssd1681_step_width_t width) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = (uint8_t)((height << 4) | (width << 0)); /* set the param */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_AUTO_WRITE_BW_RAM_FOR_REGULAR_PATTERN, buf,
                                        1); /* write byte */
    if (res != 0)                           /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the ram x address counter
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] cnt address counter
 * @return    status code
 *            - 0 success
 *            - 1 set ram x address counter failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 cnt > 0x3F
 * @note      none
 */
uint8_t ssd1681_set_ram_x_address_counter(ssd1681_handle_t *handle, uint8_t cnt) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }
    if (cnt > 0x3F) /* check the result */
    {
        handle->debug_print("ssd1681: cnt > 0x3F.\n"); /* cnt > 0x3F */

        return 4; /* return error */
    }

    buf[0] = cnt;                                                                               /* set the counter */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SET_RAM_X_ADDRESS_COUNTER, buf, 1); /* write byte */
    if (res != 0)                                                                               /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     set the ram y address counter
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] cnt address counter
 * @return    status code
 *            - 0 success
 *            - 1 set ram y address counter failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 cnt > 0x1FF
 * @note      none
 */
uint8_t ssd1681_set_ram_y_address_counter(ssd1681_handle_t *handle, uint16_t cnt) {
    uint8_t res;
    uint8_t buf[2];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }
    if (cnt > 0x1FF) /* check the result */
    {
        handle->debug_print("ssd1681: cnt > 0x1FF.\n"); /* cnt > 0x1FF */

        return 4; /* return error */
    }

    buf[0] = cnt & 0xFF;        /* set the counter low */
    buf[1] = (cnt >> 8) & 0x01; /* set the counter high */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SET_RAM_Y_ADDRESS_COUNTER, buf, 2); /* write byte */
    if (res != 0)                                                                               /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     nop command
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 nop failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_nop(ssd1681_handle_t *handle) {
    uint8_t res;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_NOP, NULL, 0); /* write byte */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     draw a point in gram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] color color type
 * @param[in] x coordinate x
 * @param[in] y coordinate y
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 gram draw point failed
 * @note      none
 */
static uint8_t a_ssd1681_gram_draw_point(ssd1681_handle_t *handle, ssd1681_color_t color, uint8_t x, uint8_t y,
                                         uint8_t data) {
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;

    pos = y / 8;                   /* get y page */
    bx = y % 8;                    /* get y point */
    temp = 1 << (7 - bx);          /* set data */
    if (color == SSD1681_COLOR_BW) /* if black */
    {
        if (data == 0) /* if 0 */
        {
            handle->bw_gram[x][pos] &= ~temp; /* set 0 */
            handle->red_gram[x][pos] &= ~temp;
        } else {
            handle->bw_gram[x][pos] |= temp; /* set 1 */
            handle->red_gram[x][pos] |= temp;
        }
    } else /* if red */
    {
        if (data != 0) /* if 1 */
        {
            handle->red_gram[x][pos] &= ~temp; /* set 0 */
        } else {
            handle->red_gram[x][pos] |= temp; /* set 1 */
        }
    }

    return 0; /* success return 0 */
}

/**
 * @brief     draw a char in gram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] color color type
 * @param[in] x coordinate x
 * @param[in] y coordinate y
 * @param[in] chr written char
 * @param[in] size char size
 * @param[in] mode display mode
 * @return    status code
 *            - 0 success
 *            - 1 gram show char failed
 * @note      none
 */
static uint8_t a_ssd1681_gram_show_char(ssd1681_handle_t *handle, ssd1681_color_t color, uint8_t x, uint8_t y,
                                        uint8_t chr, uint8_t size, uint8_t mode) {
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* get size */

    chr = chr - ' ';            /* get index */
    for (t = 0; t < csize; t++) /* write size */
    {
        if (size == 12) /* if size 12 */
        {
            temp = gsc_ssd1681_ascii_1206[chr][t]; /* get ascii 1206 */
        } else if (size == 16)                     /* if size 16 */
        {
            temp = gsc_ssd1681_ascii_1608[chr][t]; /* get ascii 1608 */
        } else if (size == 24)                     /* if size 24 */
        {
            temp = gsc_ssd1681_ascii_2412[chr][t]; /* get ascii 2412 */
        } else {
            return 1; /* return error */
        }
        for (t1 = 0; t1 < 8; t1++) /* write one line */
        {
            if ((temp & 0x80) != 0) /* if 1 */
            {
                if (a_ssd1681_gram_draw_point(handle, color, x, y, mode) != 0) /* draw point */
                {
                    return 1; /* return error */
                }
            } else {
                if (a_ssd1681_gram_draw_point(handle, color, x, y, !mode) != 0) /* draw point */
                {
                    return 1; /* return error */
                }
            }
            temp <<= 1; /* left shift 1 */
            y++;
            if ((y - y0) == size) /* reset size */
            {
                y = y0; /* set y */
                x++;    /* x++ */

                break; /* break */
            }
        }
    }

    return 0; /* success return 0 */
}

/**
 * @brief     clear the screen
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] color color type
 * @return    status code
 *            - 0 success
 *            - 1 clear failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_clear(ssd1681_handle_t *handle, ssd1681_color_t color) {
    uint8_t i;
    uint8_t j;
    uint8_t res;
    uint8_t buf[2];
    uint8_t value = 0;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = 0x00;                                                                              /* set 0x00 */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SET_RAM_X_ADDRESS_COUNTER, buf, 1); /* write byte */
    if (res != 0)                                                                               /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }
    buf[0] = 0xC7;                                                                              /* set 0xC7 */
    buf[1] = 0x00;                                                                              /* set 0x00 */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SET_RAM_Y_ADDRESS_COUNTER, buf, 2); /* write byte */
    if (res != 0)                                                                               /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }
    if (color == SSD1681_COLOR_BW) /* if black */
    {
        res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_RAM_BW, NULL, 0); /* write byte */
        if (res != 0)                                                                   /* check the result */
        {
            handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write
                                                                              byte failed */

            return 1; /* return error */
        }
        res = handle->spi_cmd_data_gpio_write(SSD1681_DATA); /* write data */
        if (res != 0)                                        /* check error */
        {
            handle->debug_print("ssd1681: spi cmd data gpio write failed.\n"); /* spi cmd data
                                                                                  gpio write
                                                                                  failed */

            return 1; /* return error */
        }
        for (i = 0; i < 200; i++) /* x */
        {
            for (j = 0; j < 25; j++) /* y */
            {
                handle->bw_gram[i][j] = 0xFF;                           /* set 0xFF */
                res = handle->spi_write_cmd(&handle->bw_gram[i][j], 1); /* write gram */
                if (res != 0)                                           /* check error */
                {
                    handle->debug_print("ssd1681: spi write cmd failed.\n"); /* spi write cmd
                                                                                failed */

                    return 1; /* return error */
                }
            }
        }
    } else /* if red */
    {
        res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_RAM_RED, NULL, 0); /* write byte */
        if (res != 0)                                                                    /* check the result */
        {
            handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write
                                                                              byte failed */

            return 1; /* return error */
        }
        res = handle->spi_cmd_data_gpio_write(SSD1681_DATA); /* write data */
        if (res != 0)                                        /* check error */
        {
            handle->debug_print("ssd1681: spi cmd data gpio write failed.\n"); /* spi cmd data
                                                                                  gpio write
                                                                                  failed */

            return 1; /* return error */
        }
        for (i = 0; i < 200; i++) /* x */
        {
            for (j = 0; j < 25; j++) /* y */
            {
                handle->red_gram[i][j] = 0x00;                           /* set 0x00 */
                res = handle->spi_write_cmd(&handle->red_gram[i][j], 1); /* write gram */
                if (res != 0)                                            /* check error */
                {
                    handle->debug_print("ssd1681: spi write cmd failed.\n"); /* spi write cmd
                                                                                failed */

                    return 1; /* return error */
                }
            }
        }
    }

    buf[0] = 0xF7;                                                                             /* set 0xF7 */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_DISPLAY_UPDATA_CONTROL_2, buf, 1); /* write byte */
    if (res != 0)                                                                              /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_MASTER_ACTIVATION, NULL, 0); /* write byte */
    if (res != 0)                                                                        /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 1; /* return error */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     update the gram data
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] color color type
 * @return    status code
 *            - 0 success
 *            - 1 gram update failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_gram_update(ssd1681_handle_t *handle, ssd1681_update_type_t type) {
    uint8_t i;
    uint8_t j;
    uint8_t res;
    uint8_t buf[2];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    if (type != SSD1681_UPDATE_TYPE_PARTIAL) {

        buf[0] = 0x00;                                                                              /* set 0x00 */
        res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SET_RAM_X_ADDRESS_COUNTER, buf, 1); /* write byte */
        if (res != 0) /* check the result */
        {
            handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                              failed */

            return 1; /* return error */
        }
        buf[0] = 0xC7; // 199
        buf[1] = 0x00;
        res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SET_RAM_Y_ADDRESS_COUNTER, buf, 2);
        if (res != 0) {
            handle->debug_print("ssd1681: multiple write byte failed.\n");
            return 1;
        }

        // BW WRITE
        res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_RAM_BW, NULL, 0); /* write byte */
        if (res != 0) {
            handle->debug_print("ssd1681: multiple write byte failed.\n");
            return 1;
        }

        res = handle->spi_cmd_data_gpio_write(SSD1681_DATA);
        if (res != 0) {
            handle->debug_print("ssd1681: spi cmd data gpio write failed.\n");
            return 1;
        }

        for (i = 0; i < 200; i++) {
            for (j = 0; j < 25; j++) {
                res = handle->spi_write_cmd(&handle->bw_gram[i][j], 1); /* write gram */
                if (res != 0) {
                    handle->debug_print("ssd1681: spi write cmd failed.\n");
                    return 1;
                }
            }
        }

        // RED WRITE (NEED FOR PARTIAL UPDATES)
        res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_WRITE_RAM_RED, NULL, 0);
        if (res != 0) {
            handle->debug_print("ssd1681: multiple write byte failed.\n");
            return 1;
        }
        res = handle->spi_cmd_data_gpio_write(SSD1681_DATA);
        if (res != 0) {
            handle->debug_print("ssd1681: spi cmd data gpio write failed.\n");
            return 1;
        }

        for (i = 0; i < 200; i++) {
            for (j = 0; j < 25; j++) {
                res = handle->spi_write_cmd(&handle->red_gram[i][j], 1);
                if (res != 0)
                    return 1;
            }
        }
    }
    buf[0] = type;
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_DISPLAY_UPDATA_CONTROL_2, buf, 1);
    if (res != 0) {
        handle->debug_print("ssd1681: multiple write byte failed.\n");
        return 1;
    }
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_MASTER_ACTIVATION, NULL, 0);
    if (res != 0) {
        handle->debug_print("ssd1681: multiple write byte failed.\n");
        return 1;
    }
    return 0;
}

/**
 * @brief     partial update
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in]
 * @return    status code
 *            -
 *            -
 *            -
 *            -
 * @note      none
 */
uint8_t ssd1681_gram_draw_partial(ssd1681_handle_t *handle, uint16_t x_start, uint16_t y_start,
                                  const unsigned char *datas, uint16_t PART_COLUMN, uint16_t PART_LINE) {
    unsigned int x_end, y_end;
    uint8_t buf[4];

    // Calculate RAM addresses.
    // Note: SSD1681 x-address is byte-oriented (8 pixels).
    x_start = x_start / 8;               // x address start
    x_end = x_start + PART_LINE / 8 - 1; // x address end

    y_start = y_start - 1;             // Y address start
    y_end = y_start + PART_COLUMN - 1; // Y address end

    // Set RAM X address window
    ssd1681_set_ram_x(handle, x_start, x_end);

    // Set RAM Y address window
    buf[0] = y_start % 256;
    buf[1] = y_start / 256;
    buf[2] = y_end % 256;
    buf[3] = y_end / 256;
    a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SET_RAM_Y, buf, 4);

    // Set RAM X address counter
    ssd1681_set_ram_x_address_counter(handle, x_start);

    // Set RAM Y address counter
    a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_SET_RAM_Y_ADDRESS_COUNTER, buf, 2);

    // --- Write data to Black/White RAM ---
    ssd1681_set_black_ram(handle);

    uint16_t byte_index = 0;
    for (uint16_t row = y_start; row <= y_end; row++) {
        for (uint16_t col = x_start; col <= x_end; col++) {
            uint8_t value = datas[byte_index];
            a_ssd1681_multiple_write_byte(handle, 0, &value, 1);
            handle->bw_gram[row][col] = value;
            handle->red_gram[row][col] = value;
            byte_index++;
        }
    }
    return 0;
}

/**
 * @brief     clear the screen in the gram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] color color type
 * @return    status code
 *            - 0 success
 *            - 1 clear failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_gram_clear(ssd1681_handle_t *handle, ssd1681_color_t color) {
    uint8_t i;
    uint8_t j;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    if (color == SSD1681_COLOR_BW) /* if black */
    {
        for (i = 0; i < 200; i++) /* x */
        {
            for (j = 0; j < 25; j++) /* y */
            {
                handle->bw_gram[i][j] = 0xFF; /* set 0xFF */
            }
        }
    } else /* if red */
    {
        for (i = 0; i < 200; i++) /* x */
        {
            for (j = 0; j < 25; j++) /* y */
            {
                handle->red_gram[i][j] = 0x00; /* set 0x00 */
            }
        }
    }

    return 0; /* success return 0 */
}

/**
 * @brief     write a point in the gram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] color color type
 * @param[in] x coordinate x
 * @param[in] y coordinate y
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 gram write point failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 x or y is invalid
 * @note      none
 */
uint8_t ssd1681_gram_write_point(ssd1681_handle_t *handle, ssd1681_color_t color, uint8_t x, uint8_t y, uint8_t data) {
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }
    if ((x > 200) || (y > 200)) /* check x, y */
    {
        handle->debug_print("ssd1681: x or y is invalid.\n"); /* x or y is invalid */

        return 4; /* return error */
    }

    pos = y / 8;                   /* get y page */
    bx = y % 8;                    /* get y point */
    temp = 1 << (7 - bx);          /* set data */
    if (color == SSD1681_COLOR_BW) /* if black */
    {
        if (data == 0) /* if 0 */
        {
            handle->bw_gram[x][pos] |= temp; /* set 1 */
        } else {
            handle->bw_gram[x][pos] &= ~temp; /* set 0 */
        }
    } else /* if red */
    {
        if (data != 0) /* if 1 */
        {
            handle->red_gram[x][pos] |= temp; /* set 1 */
        } else {
            handle->red_gram[x][pos] &= ~temp; /* set 0 */
        }
    }

    return 0; /* success return 0 */
}

/**
 * @brief      read a point from the gram
 * @param[in]  *handle pointer to an ssd1681 handle structure
 * @param[in]  color color type
 * @param[in]  x coordinate x
 * @param[in]  y coordinate y
 * @param[out] *data pointer to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 gram read point failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 x or y is invalid
 * @note       none
 */
uint8_t ssd1681_gram_read_point(ssd1681_handle_t *handle, ssd1681_color_t color, uint8_t x, uint8_t y, uint8_t *data) {
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }
    if ((x > 200) || (y > 200)) /* check x, y */
    {
        handle->debug_print("ssd1681: x or y is invalid.\n"); /* x or y is invalid */

        return 4; /* return error */
    }

    pos = y / 8;                   /* get y page */
    bx = y % 8;                    /* get y point */
    temp = 1 << (7 - bx);          /* set data */
    if (color == SSD1681_COLOR_BW) /* if black */
    {
        if ((handle->bw_gram[x][pos] & temp) != 0) /* get data */
        {
            *data = 0; /* set 0 */
        } else {
            *data = 1; /* set 1 */
        }
    } else /* if red */
    {
        if ((handle->red_gram[x][pos] & temp) != 0) /* get data */
        {
            *data = 1; /* set 1 */
        } else {
            *data = 0; /* set 0 */
        }
    }

    return 0; /* success return 0 */
}

/**
 * @brief     draw a string in the gram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] color color type
 * @param[in] x coordinate x
 * @param[in] y coordinate y
 * @param[in] *str pointer to a write string address
 * @param[in] len length of the string
 * @param[in] data display color
 * @param[in] font display font size
 * @return    status code
 *            - 0 success
 *            - 1 gram write string failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 x or y is invalid
 * @note      none
 */
uint8_t ssd1681_gram_write_string(ssd1681_handle_t *handle, ssd1681_color_t color, uint8_t x, uint8_t y, char *str,
                                  uint16_t len, uint8_t data, ssd1681_font_t font) {
    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    if ((x > 199) || (y > 199)) /* check x, y */
    {
        handle->debug_print("ssd1681: x or y is invalid.\n"); /* x or y is invalid */

        return 4; /* return error */
    }

    while ((len != 0) && (*str <= '~') && (*str >= ' ')) /* write all string */
    {
        if (x > (199 - (font / 2))) /* check x point */
        {
            x = 0;              /* set x */
            y += (uint8_t)font; /* set next row */
        }
        if (y > (199 - font)) /* check y pont */
        {
            y = x = 0; /* reset to 0,0 */
        }
        if (a_ssd1681_gram_show_char(handle, color, x, y, *str, font, data) != 0) /* show a char */
        {
            return 1; /* return error */
        }
        x += (uint8_t)(font / 2); /* x + font / 2 */
        str++;                    /* str address++ */
        len--;                    /* str length-- */
    }

    return 0; /* success return 0 */
}

/**
 * @brief     fill a rectangle in the gram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] color color type
 * @param[in] left left coordinate x
 * @param[in] top top coordinate y
 * @param[in] right right coordinate x
 * @param[in] bottom bottom coordinate y
 * @param[in] data display color
 * @return    status code
 *            - 0 success
 *            - 1 gram fill rect failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 left or top is invalid
 *            - 5 right or bottom is invalid
 *            - 6 left > right or top > bottom
 * @note      none
 */
uint8_t ssd1681_gram_fill_rect(ssd1681_handle_t *handle, ssd1681_color_t color, uint8_t left, uint8_t top,
                               uint8_t right, uint8_t bottom, uint8_t data) {
    uint8_t x, y;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }
    if ((left > 199) || (top > 199)) /* check left top */
    {
        handle->debug_print("ssd1681: left or top is invalid.\n"); /* left or top is invalid */

        return 4; /* return error */
    }
    if ((right > 199) || (bottom > 199)) /* check right bottom */
    {
        handle->debug_print("ssd1681: right or bottom is invalid.\n"); /* right or bottom is
                                                                          invalid */

        return 5; /* return error */
    }
    if ((left > right) || (top > bottom)) /* check left right top bottom */
    {
        handle->debug_print("ssd1681: left > right or top > bottom.\n"); /* left > right or top
                                                                            > bottom */

        return 6; /* return error */
    }

    for (x = left; x <= right; x++) /* write x */
    {
        for (y = top; y <= bottom; y++) /* write y */
        {
            if (a_ssd1681_gram_draw_point(handle, color, x, y, data) != 0) /* draw point */
            {
                return 1; /* return error */
            }
        }
    }

    return 0; /* return error */
}

/**
 * @brief     draw a picture in the gram
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] color color type
 * @param[in] left left coordinate x
 * @param[in] top top coordinate y
 * @param[in] right right coordinate x
 * @param[in] bottom bottom coordinate y
 * @param[in] *img pointer to an image buffer
 * @return    status code
 *            - 0 success
 *            - 1 gram draw picture failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 left or top is invalid
 *            - 5 right or bottom is invalid
 *            - 6 left > right or top > bottom
 * @note      none
 */
uint8_t ssd1681_gram_draw_picture(ssd1681_handle_t *handle, ssd1681_color_t color, uint8_t left, uint8_t top,
                                  uint8_t right, uint8_t bottom, uint8_t *img) {
    uint8_t x, y;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }
    if ((left > 199) || (top > 199)) /* check left top */
    {
        handle->debug_print("ssd1681: left or top is invalid.\n"); /* left or top is invalid */

        return 4; /* return error */
    }
    if ((right > 199) || (bottom > 199)) /* check right bottom */
    {
        handle->debug_print("ssd1681: right or bottom is invalid.\n"); /* right or bottom is
                                                                          invalid */

        return 5; /* return error */
    }
    if ((left > right) || (top > bottom)) /* check left right top bottom */
    {
        handle->debug_print("ssd1681: left > right or top > bottom.\n"); /* left > right or top
                                                                            > bottom */

        return 6; /* return error */
    }

    for (x = left; x <= right; x++) /* write x */
    {
        for (y = top; y <= bottom; y++) /* write y */
        {
            if (a_ssd1681_gram_draw_point(handle, color, x, y, *img) != 0) /* draw point */
            {
                return 1; /* return error */
            }
            img++; /* img++ */
        }
    }

    return 0; /* succeed return 0 */
}

/**
 * @brief Desenha um bitmap compactado (1bpp, Row-Major/Horizontal) em uma janela.
 * @note Esta função é LENTA, pois chama draw_point para cada pixel.
 * Ela é baseada na sua função `ssd1681_gram_draw_picture`.
 *
 * @param handle Handle do display.
 * @param color Cor de "frente" (para pixels 0x01).
 * @param left Coordenada X inicial na tela.
 * @param top Coordenada Y inicial na tela.
 * @param right Coordenada X final na tela.
 * @param bottom Coordenada Y final na tela.
 * @param img Ponteiro para o buffer do bitmap (Row-Major, 1bpp).
 * @return 0 em sucesso, >0 em erro.
 */
uint8_t ssd1681_gram_draw_bitmap(ssd1681_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom,
                                 const uint8_t *img) {
    uint16_t x, y;         // Coordenadas da TELA (são uint16_t para segurança)
    uint16_t bmp_x, bmp_y; // Coordenadas locais do BITMAP
    uint8_t pixel_data;    // O valor do pixel (0x00 ou 0x01)

    // --- Suas checagens de segurança originais ---
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (img == NULL) {
        handle->debug_print("ssd1681: bitmap nulo.\n");
        return 7; // Erro de bitmap nulo
    }
    if ((left > 199) || (top > 199)) {
        handle->debug_print("ssd1681: left or top is invalid.\n");
        return 4;
    }
    if ((right > 199) || (bottom > 199)) {
        handle->debug_print("ssd1681: right or bottom is invalid.\n");
        return 5;
    }
    if ((left > right) || (top > bottom)) {
        handle->debug_print("ssd1681: left > right or top > bottom.\n");
        return 6;
    }

    uint16_t bmp_width = right - left + 1;

    uint16_t bytes_per_row = (bmp_width + 7) / 8;

    const uint16_t DISPLAY_MAX_X = 199;

    for (y = top; y <= bottom; y++) /* write x */
    {
        for (x = left; x <= right; x++) /* write y */
        {

            bmp_x = x - left;
            bmp_y = y - top;

            // 2. Encontrar o ÍNDICE DO BYTE (Lógica Horizontal / Row-Major)
            // Índice = (linha_do_bitmap * bytes_por_linha) + (coluna_do_bitmap / 8)
            uint16_t byte_index = (bmp_y * bytes_per_row) + (bmp_x / 8);

            // 3. Encontrar o ÍNDICE DO BIT dentro desse byte
            // (Assumindo MSB first, que é o mais comum)
            uint8_t bit_index = bmp_x % 8;

            // 4. Obter o byte do buffer e isolar o bit
            uint8_t byte_from_buffer = img[byte_index];

            // (byte_from_buffer >> (7 - bit_index)) -> Pega o bit MSB-first
            if ((byte_from_buffer >> (7 - bit_index)) & 0x01) {
                pixel_data = 0x01; // Pixel "ligado" (preto)
            } else {
                pixel_data = 0x00; // Pixel "desligado" (branco)
            }

            // --- FIM DA LÓGICA DE BITMAP ---

            uint16_t x_phys = DISPLAY_MAX_X - y;
            uint16_t y_phys = x;
            // 5. Chamar a função de desenhar ponto (como no original)
            // (Assumindo que draw_point sabe que 0x00=branco e 0x01=preto)
            if (a_ssd1681_gram_draw_point(handle, SSD1681_COLOR_BW, x_phys, y_phys, pixel_data) != 0) {
                return 1; /* return error */
            }

            // (Removemos o img++ daqui, pois o byte_index cuida disso)
        }
    }

    return 0; /* succeed return 0 */
}

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 *            - 5 gpio init failed
 * @note      none
 */
// uint8_t ssd1681_init(ssd1681_handle_t *handle) {
//     if (handle == NULL) /* check handle */
//     {
//         return 2; /* return error */
//     }
//     if (handle->debug_print == NULL) /* check debug_print */
//     {
//         return 3; /* return error */
//     }
//     if (handle->spi_init == NULL) /* check spi_init */
//     {
//         handle->debug_print("ssd1681: spi_init is null.\n"); /* spi_init is null */

//         return 3; /* return error */
//     }
//     if (handle->spi_deinit == NULL) /* check spi_deinit */
//     {
//         handle->debug_print("ssd1681: spi_deinit is null.\n"); /* spi_deinit is null */

//         return 3; /* return error */
//     }
//     if (handle->spi_write_cmd == NULL) /* check spi_write_cmd */
//     {
//         handle->debug_print("ssd1681: spi_write_cmd is null.\n"); /* spi_write_cmd is null */

//         return 3; /* return error */
//     }
//     if (handle->delay_ms == NULL) /* check delay_ms */
//     {
//         handle->debug_print("ssd1681: delay_ms is null.\n"); /* delay_ms is null */

//         return 3; /* return error */
//     }
//     if (handle->spi_cmd_data_gpio_init == NULL) /* check spi_cmd_data_gpio_init */
//     {
//         handle->debug_print("ssd1681: spi_cmd_data_gpio_init is null.\n"); /* spi_cmd_data_gpio_init
//                                                                               is null */

//         return 3; /* return error */
//     }
//     if (handle->spi_cmd_data_gpio_deinit == NULL) /* check spi_cmd_data_gpio_deinit */
//     {
//         handle->debug_print("ssd1681: spi_cmd_data_gpio_deinit is null.\n"); /* spi_cmd_data_gpio_deinit
//                                                                                 is null */

//         return 3; /* return error */
//     }
//     if (handle->spi_cmd_data_gpio_write == NULL) /* check spi_cmd_data_gpio_write */
//     {
//         handle->debug_print("ssd1681: spi_cmd_data_gpio_write is null.\n"); /* spi_cmd_data_gpio_write
//                                                                                is null */

//         return 3; /* return error */
//     }
//     if (handle->reset_gpio_init == NULL) /* check reset_gpio_init */
//     {
//         handle->debug_print("ssd1681: reset_gpio_init is null.\n"); /* reset_gpio_init is null
//                                                                      */

//         return 3; /* return error */
//     }
//     if (handle->reset_gpio_deinit == NULL) /* check reset_gpio_deinit */
//     {
//         handle->debug_print("ssd1681: reset_gpio_deinit is null.\n"); /* reset_gpio_deinit is
//                                                                          null */

//         return 3; /* return error */
//     }
//     if (handle->reset_gpio_write == NULL) /* check reset_gpio_write */
//     {
//         handle->debug_print("ssd1681: reset_gpio_write is null.\n"); /* reset_gpio_write is null
//                                                                       */

//         return 3; /* return error */
//     }
//     if (handle->busy_gpio_init == NULL) /* check busy_gpio_init */
//     {
//         handle->debug_print("ssd1681: busy_gpio_init is null.\n"); /* busy_gpio_init is null */

//         return 3; /* return error */
//     }
//     if (handle->busy_gpio_deinit == NULL) /* check busy_gpio_deinit */
//     {
//         handle->debug_print("ssd1681: busy_gpio_deinit is null.\n"); /* busy_gpio_deinit is null
//                                                                       */

//         return 3; /* return error */
//     }
//     if (handle->busy_gpio_read == NULL) /* check busy_gpio_read */
//     {
//         handle->debug_print("ssd1681: busy_gpio_read is null.\n"); /* busy_gpio_read is null */

//         return 3; /* return error */
//     }

//     if (handle->spi_cmd_data_gpio_init() != 0) /* check spi_cmd_data_gpio_init */
//     {
//         handle->debug_print("ssd1681: spi cmd data gpio init failed.\n"); /* spi cmd data gpio
//                                                                              init failed */

//         return 5; /* return error */
//     }
//     if (handle->reset_gpio_init() != 0) /* reset gpio init */
//     {
//         handle->debug_print("ssd1681: reset gpio init failed.\n"); /* reset gpio init failed */
//         (void)handle->spi_cmd_data_gpio_deinit();                  /* spi_cmd_data_gpio_deinit */

//         return 5; /* return error */
//     }
//     if (handle->busy_gpio_init() != 0) /* busy gpio init */
//     {
//         handle->debug_print("ssd1681: busy gpio init failed.\n"); /* busy gpio init failed */
//         (void)handle->spi_cmd_data_gpio_deinit();                 /* spi_cmd_data_gpio_deinit */
//         (void)handle->reset_gpio_deinit();                        /* reset_gpio_deinit */

//         return 5; /* return error */
//     }

//     if (handle->reset_gpio_write(0) != 0) /* write 0 */
//     {
//         handle->debug_print("ssd1681: reset gpio write failed.\n"); /* reset gpio write failed
//                                                                      */
//         (void)handle->spi_cmd_data_gpio_deinit();                   /* spi_cmd_data_gpio_deinit */
//         (void)handle->reset_gpio_deinit();                          /* reset_gpio_deinit */
//         (void)handle->busy_gpio_deinit();                           /* busy_gpio_deinit */

//         return 4; /* return error */
//     }
//     handle->delay_ms(10);                 /* delay 10 ms */
//     if (handle->reset_gpio_write(1) != 0) /* write 1 */
//     {
//         handle->debug_print("ssd1681: reset gpio write failed.\n"); /* reset gpio write failed
//                                                                      */
//         (void)handle->spi_cmd_data_gpio_deinit();                   /* spi_cmd_data_gpio_deinit */
//         (void)handle->reset_gpio_deinit();                          /* reset_gpio_deinit */
//         (void)handle->busy_gpio_deinit();                           /* busy_gpio_deinit */

//         return 4; /* return error */
//     }
//     handle->delay_ms(200); /* delay 200 ms */

//     if (handle->spi_init() != 0) /* spi init */
//     {
//         handle->debug_print("ssd1681: spi init failed.\n"); /* spi init failed */
//         (void)handle->spi_cmd_data_gpio_deinit();           /* spi_cmd_data_gpio_deinit */
//         (void)handle->reset_gpio_deinit();                  /* reset_gpio_deinit */
//         (void)handle->busy_gpio_deinit();                   /* busy_gpio_deinit */

//         return 1; /* return error */
//     }

//     handle->inited = 1; /* flag inited */

//     return 0; /* success return 0 */
// }

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 *            - 5 reset gpio deinit failed
 *            - 6 command && data deinit failed
 *            - 7 busy gpio deinit failed
 * @note      none
 */
uint8_t ssd1681_deinit(ssd1681_handle_t *handle) {
    uint8_t res;
    uint8_t buf[1];

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    buf[0] = 0x01;                                                                    /* set 0x01 */
    res = a_ssd1681_multiple_write_byte(handle, SSD1681_CMD_DEEP_SLEEP_MODE, buf, 1); /* write byte */
    if (res != 0)                                                                     /* check the result */
    {
        handle->debug_print("ssd1681: multiple write byte failed.\n"); /* multiple write byte
                                                                          failed */

        return 4; /* return error */
    }
    handle->delay_ms(100); /* delay 100 ms */

    if (handle->reset_gpio_deinit() != 0) /* reset gpio deinit */
    {
        handle->debug_print("ssd1681: reset gpio deinit failed.\n"); /* reset gpio deinit failed
                                                                      */

        return 5; /* return error */
    }
    if (handle->spi_cmd_data_gpio_deinit() != 0) /* spi cmd data gpio deinit */
    {
        handle->debug_print("ssd1681: spi cmd data gpio deinit failed.\n"); /* spi cmd data gpio
                                                                               deinit failed */

        return 6; /* return error */
    }
    if (handle->busy_gpio_deinit() != 0) /* busy gpio deinit */
    {
        handle->debug_print("ssd1681: busy gpio deinit failed.\n"); /* busy gpio deinit failed
                                                                     */

        return 7; /* return error */
    }

    if (handle->spi_deinit() != 0) /* spi deinit */
    {
        handle->debug_print("ssd1681: spi deinit failed.\n"); /* spi deinit failed */

        return 1; /* return error */
    }

    handle->inited = 0; /* flag close */

    return 0; /* success return 0 */
}

/**
 * @brief     write the register command
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_write_cmd(ssd1681_handle_t *handle, uint8_t *buf, uint8_t len) {
    uint8_t res;
    uint8_t i;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = handle->spi_cmd_data_gpio_write(SSD1681_CMD); /* write command */
    if (res != 0)                                       /* check error */
    {
        return 1; /* return error */
    }
    for (i = 0; i < len; i++) /* len times */
    {
        if (handle->spi_write_cmd(buf + i, 1) != 0) /* write command */
        {
            return 1; /* return error */
        }
    }

    return 0; /* success return 0 */
}

/**
 * @brief     write the register data
 * @param[in] *handle pointer to an ssd1681 handle structure
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1681_write_data(ssd1681_handle_t *handle, uint8_t *buf, uint8_t len) {
    uint8_t res;
    uint8_t i;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = handle->spi_cmd_data_gpio_write(SSD1681_DATA); /* write data */
    if (res != 0)                                        /* check error */
    {
        return 1; /* return error */
    }
    for (i = 0; i < len; i++) /* len times */
    {
        if (handle->spi_write_cmd(buf + i, 1) != 0) /* write command */
        {
            return 1; /* return error */
        }
    }

    return 0; /* success return 0 */
}

/**
 * @brief      read the register data
 * @param[in]  *handle pointer to an ssd1681 handle structure
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ssd1681_read_data(ssd1681_handle_t *handle, uint8_t *buf, uint8_t len) {
    uint8_t res;
    uint8_t i;

    if (handle == NULL) /* check handle */
    {
        return 2; /* return error */
    }
    if (handle->inited != 1) /* check handle initialization */
    {
        return 3; /* return error */
    }

    res = handle->spi_cmd_data_gpio_write(SSD1681_DATA); /* write data */
    if (res != 0)                                        /* check error */
    {
        return 1; /* return error */
    }
    for (i = 0; i < len; i++) /* len times */
    {
        if (handle->spi_read_cmd(buf + i, 1) != 0) /* read command */
        {
            return 1; /* return error */
        }
    }

    return 0; /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an ssd1681 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
// uint8_t ssd1681_info(ssd1681_info_t *info);

void EPD_Update(ssd1681_handle_t *handle, ssd1681_update_type_t type) {
    uint8_t buf[1];
    buf[0] = type;
    a_ssd1681_multiple_write_byte(handle, 0x22, buf, 1);
    a_ssd1681_multiple_write_byte(handle, 0x20, 0, 0);
}