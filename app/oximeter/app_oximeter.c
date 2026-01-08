/**
 * @file    max30102_app.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-12-26
 */

#include "app_oximeter.h"

max30102_handle_t max_handle; /**< max30102 handle */
static volatile uint8_t g_oximeter_data_ready = 0;

/**
 * @brief  fifo receive callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
static void _max30102_interface_receive_callback(uint8_t type) {
    switch (type) {
    case MAX30102_INTERRUPT_STATUS_FIFO_FULL:
    case MAX30102_INTERRUPT_STATUS_PPG_RDY:
        // Levanta a bandeira interna
        g_oximeter_data_ready = 1;
        break;
    default:
        break;
    }
}

/**
 * @brief  fifo example irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t max30102_fifo_irq_handler(void) {
    /* run irq handler */
    if (max30102_irq_handler(&max_handle) != 0) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief     fifo init
 * @param[in] *fifo_receive_callback pointer to a fifo receive callback
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t app_oximeter_init(void) {
    uint8_t res;
    max30102_bool_t enable;

    /* link interface function */
    DRIVER_MAX30102_LINK_INIT(&max_handle, max30102_handle_t);
    DRIVER_MAX30102_LINK_I2C_INIT(&max_handle, max30102_interface_i2c_init);
    DRIVER_MAX30102_LINK_I2C_DEINIT(&max_handle, max30102_interface_i2c_deinit);
    DRIVER_MAX30102_LINK_I2C_READ(&max_handle, max30102_interface_i2c_read);
    DRIVER_MAX30102_LINK_I2C_WRITE(&max_handle, max30102_interface_i2c_write);
    DRIVER_MAX30102_LINK_DELAY_MS(&max_handle, max30102_interface_delay_ms);
    DRIVER_MAX30102_LINK_DEBUG_PRINT(&max_handle, max30102_interface_debug_print);
    DRIVER_MAX30102_LINK_RECEIVE_CALLBACK(&max_handle, _max30102_interface_receive_callback);

    /* init the max30102 */
    res = max30102_init(&max_handle);
    if (res != 0) {
        max30102_interface_debug_print("max30102: init failed.\n");

        return 1;
    }

    /* enable shutdown */
    res = max30102_set_shutdown(&max_handle, MAX30102_BOOL_TRUE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set shutdown failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set fifo sample averaging */
    res = max30102_set_fifo_sample_averaging(&max_handle, MAX30102_FIFO_DEFAULT_SAMPLE_AVERAGING);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set fifo sample averaging failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set fifo roll */
    res = max30102_set_fifo_roll(&max_handle, MAX30102_FIFO_DEFAULT_FIFO_ROLL);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set fifo roll failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set fifo almost full */
    res = max30102_set_fifo_almost_full(&max_handle, MAX30102_FIFO_DEFAULT_FIFO_ALMOST_FULL);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set fifo almost full failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set mode */
    res = max30102_set_mode(&max_handle, MAX30102_FIFO_DEFAULT_MODE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set mode failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set spo2 adc range */
    res = max30102_set_spo2_adc_range(&max_handle, MAX30102_FIFO_DEFAULT_SPO2_ADC_RANGE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set spo2 adc range failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set spo2 sample rate */
    res = max30102_set_spo2_sample_rate(&max_handle, MAX30102_FIFO_DEFAULT_SPO2_SAMPLE_RATE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set spo2 sample rate failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set adc resolution */
    res = max30102_set_adc_resolution(&max_handle, MAX30102_FIFO_DEFAULT_ADC_RESOLUTION);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set adc resolution failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set led red pulse amplitude */
    res = max30102_set_led_red_pulse_amplitude(&max_handle, MAX30102_FIFO_DEFAULT_LED_RED_PULSE_AMPLITUDE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set led red pulse amplitude failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set led ir pulse amplitude */
    res = max30102_set_led_ir_pulse_amplitude(&max_handle, MAX30102_FIFO_DEFAULT_LED_IR_PULSE_AMPLITUDE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set led ir pulse amplitude failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set slot */
    res = max30102_set_slot(&max_handle, MAX30102_SLOT_1, MAX30102_FIFO_DEFAULT_SLOT1);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set slot failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set slot */
    res = max30102_set_slot(&max_handle, MAX30102_SLOT_2, MAX30102_FIFO_DEFAULT_SLOT2);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set slot failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set slot */
    res = max30102_set_slot(&max_handle, MAX30102_SLOT_3, MAX30102_FIFO_DEFAULT_SLOT3);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set slot failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set slot */
    res = max30102_set_slot(&max_handle, MAX30102_SLOT_4, MAX30102_FIFO_DEFAULT_SLOT4);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set slot failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set die temperature */
    res = max30102_set_die_temperature(&max_handle, MAX30102_FIFO_DEFAULT_DIE_TEMPERATURE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set die temperature failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set interrupt */
    res = max30102_set_interrupt(&max_handle, MAX30102_INTERRUPT_FIFO_FULL_EN, MAX30102_BOOL_TRUE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set interrupt failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set interrupt */
    res = max30102_set_interrupt(&max_handle, MAX30102_INTERRUPT_PPG_RDY_EN, MAX30102_FIFO_DEFAULT_INTERRUPT_PPG_RDY_EN);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set interrupt failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set interrupt */
    res = max30102_set_interrupt(&max_handle, MAX30102_INTERRUPT_ALC_OVF_EN, MAX30102_FIFO_DEFAULT_INTERRUPT_ALC_OVF_EN);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set interrupt failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* set interrupt */
    res = max30102_set_interrupt(&max_handle, MAX30102_INTERRUPT_DIE_TEMP_RDY_EN, MAX30102_FIFO_DEFAULT_INTERRUPT_DIE_TEMP_RDY_EN);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set interrupt failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* disable shutdown */
    res = max30102_set_shutdown(&max_handle, MAX30102_BOOL_FALSE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set shutdown failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    /* get status */
    res = max30102_get_interrupt_status(&max_handle, MAX30102_INTERRUPT_STATUS_FIFO_FULL, &enable);
    if (res != 0) {
        max30102_interface_debug_print("max30102: get interrupt status failed.\n");
        (void)max30102_deinit(&max_handle);

        return 1;
    }

    return 0;
}

/**
 * @brief  fifo example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max30102_fifo_deinit(void) {
    uint8_t res;

    res = max30102_deinit(&max_handle);
    if (res != 0) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief         read the data
 * @param[out]    *raw_red pointer to a read raw data buffer
 * @param[out]    *raw_ir pointer to a ir raw data buffer
 * @param[in,out] *len pointer to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 * @note          none
 */
uint8_t max30102_fifo_read(uint32_t *raw_red, uint32_t *raw_ir, uint8_t *len) {
    uint8_t res;

    res = max30102_read(&max_handle, raw_red, raw_ir, len);
    if (res != 0) {
        return 1;
    } else {
        return 0;
    }
}