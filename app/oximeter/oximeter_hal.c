/**
* @file    oximeter_hal.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-02-20
*/

#include "max30102.h"
#include "max30102_interface.h"
#include "stdint.h"
#include "oximeter_hal.h"

max30102_handle_t max_handle;
max30102_bool_t enable;

uint8_t oximeter_init(void) {
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
    DRIVER_MAX30102_LINK_RECEIVE_CALLBACK(&max_handle, max30102_interface_receive_callback);

    /* init the max30102 */
    res = max30102_init(&max_handle);
    if (res != 0) {
        max30102_interface_debug_print("max30102: init failed.\n");

        return 1;
    }
}

uint8_t oximeter_setup(void) {
    uint8_t res;

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

