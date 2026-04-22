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
#include "uart.h"

static max30102_handle_t gs_handle;
volatile uint8_t g_flag; 
static uint32_t g_raw_red[32]; 
static uint32_t g_raw_ir[32];   
volatile uint8_t g_oximeter_data_ready = 0;
volatile uint8_t g_oximeter_event = 0;

static uint8_t oximeter_setup(void);

uint8_t oximeter_init(void) {
    uint8_t res;

    /* link interface function */
    DRIVER_MAX30102_LINK_INIT(&gs_handle, max30102_handle_t);
    DRIVER_MAX30102_LINK_I2C_INIT(&gs_handle, max30102_interface_i2c_init);
    DRIVER_MAX30102_LINK_I2C_DEINIT(&gs_handle, max30102_interface_i2c_deinit);
    DRIVER_MAX30102_LINK_I2C_READ(&gs_handle, max30102_interface_i2c_read);
    DRIVER_MAX30102_LINK_I2C_WRITE(&gs_handle, max30102_interface_i2c_write);
    DRIVER_MAX30102_LINK_DELAY_MS(&gs_handle, max30102_interface_delay_ms);
    DRIVER_MAX30102_LINK_DEBUG_PRINT(&gs_handle, max30102_interface_debug_print);
    DRIVER_MAX30102_LINK_RECEIVE_CALLBACK(&gs_handle, max30102_interface_receive_callback);

    /* init the max30102 */
    res = max30102_init(&gs_handle);
    if (res != 0) {
        max30102_interface_debug_print("max30102: init failed.\n");

        return 1;
    }

    res = oximeter_setup();
    if (res != 0) {
        max30102_interface_debug_print("max30102: setup failed.\n");
        
        return 1;
    }
    
        return 0;
}

static uint8_t oximeter_setup(void) {
    uint8_t res;

     /* enable shutdown */
    res = max30102_set_shutdown(&gs_handle, MAX30102_BOOL_TRUE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set shutdown failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set fifo sample averaging */
    res = max30102_set_fifo_sample_averaging(&gs_handle, MAX30102_FIFO_DEFAULT_SAMPLE_AVERAGING);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set fifo sample averaging failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set fifo roll */
    res = max30102_set_fifo_roll(&gs_handle, MAX30102_FIFO_DEFAULT_FIFO_ROLL);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set fifo roll failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set fifo almost full */
    res = max30102_set_fifo_almost_full(&gs_handle, MAX30102_FIFO_DEFAULT_FIFO_ALMOST_FULL);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set fifo almost full failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set mode */
    res = max30102_set_mode(&gs_handle, MAX30102_FIFO_DEFAULT_MODE); // configured for hr_mode (red led only)
    if (res != 0) {
        max30102_interface_debug_print("max30102: set mode failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set spo2 adc range */
    res = max30102_set_spo2_adc_range(&gs_handle, MAX30102_FIFO_DEFAULT_SPO2_ADC_RANGE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set spo2 adc range failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set spo2 sample rate */
    res = max30102_set_spo2_sample_rate(&gs_handle, MAX30102_FIFO_DEFAULT_SPO2_SAMPLE_RATE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set spo2 sample rate failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set adc resolution */
    res = max30102_set_adc_resolution(&gs_handle, MAX30102_FIFO_DEFAULT_ADC_RESOLUTION);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set adc resolution failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set led red pulse amplitude */
    res = max30102_set_led_red_pulse_amplitude(&gs_handle, 0x7f);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set led red pulse amplitude failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set led ir pulse amplitude */
    res = max30102_set_led_ir_pulse_amplitude(&gs_handle, 0x7f);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set led ir pulse amplitude failed.\n");
        (void)max30102_deinit(&gs_handle);
        return 1;
    }

    /* Slots are only for MULTI_LED mode. In SPO2 mode, they are ignored and setting them returns an error. */

    /* set die temperature */
    res = max30102_set_die_temperature(&gs_handle, MAX30102_FIFO_DEFAULT_DIE_TEMPERATURE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set die temperature failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set interrupt */
    res = max30102_set_interrupt(&gs_handle, MAX30102_INTERRUPT_FIFO_FULL_EN, MAX30102_BOOL_TRUE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set interrupt failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set interrupt */
    res = max30102_set_interrupt(&gs_handle, MAX30102_INTERRUPT_PPG_RDY_EN, MAX30102_FIFO_DEFAULT_INTERRUPT_PPG_RDY_EN);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set interrupt failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set interrupt */
    res = max30102_set_interrupt(&gs_handle, MAX30102_INTERRUPT_ALC_OVF_EN, MAX30102_FIFO_DEFAULT_INTERRUPT_ALC_OVF_EN);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set interrupt failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* set interrupt */
    res = max30102_set_interrupt(&gs_handle, MAX30102_INTERRUPT_DIE_TEMP_RDY_EN, MAX30102_FIFO_DEFAULT_INTERRUPT_DIE_TEMP_RDY_EN);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set interrupt failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* disable shutdown */
    res = max30102_set_shutdown(&gs_handle, MAX30102_BOOL_FALSE);
    if (res != 0) {
        max30102_interface_debug_print("max30102: set shutdown failed.\n");
        (void)max30102_deinit(&gs_handle);

        return 1;
    }

    /* get status */
    // res = max30102_get_interrupt_status(&gs_handle, MAX30102_INTERRUPT_STATUS_FIFO_FULL, &enable);
    // if (res != 0) {
    //     max30102_interface_debug_print("max30102: get interrupt status failed.\n");
    //     (void)max30102_deinit(&gs_handle);

    //     return 1;
    // }

    return 0;
}

uint8_t oximeter_read(uint32_t *red_buf, uint32_t *ir_buf, uint8_t *len) {
    uint8_t res;
    // O 'len' entra dizendo o tamanho máximo do buffer e sai dizendo quantas amostras foram lidas
    res = max30102_read(&gs_handle, red_buf, ir_buf, len);
    return res;

    
    return 0;
}

void oximeter_irq_handler(void) {
    max30102_irq_handler(&gs_handle);
}

uint8_t oximeter_event() {
    oximeter_irq_handler();
    
    if (g_oximeter_data_ready) {
        g_oximeter_data_ready = 0;

        uint8_t samples = 32;
        uint16_t len;
        if (oximeter_read(g_raw_red, g_raw_ir, &samples) == 0) {

            char tx_buffer[64];

            for (uint8_t i = 0; i < samples; i++) {
                len = snprintf(tx_buffer, sizeof(tx_buffer), "%lu,%lu\r\n", 
                                                    g_raw_red[i], g_raw_ir[i]);

                if (len > 0)
                    uart_write((uint8_t*) tx_buffer, len);  
            }
        }
    }
    return 0;
}
