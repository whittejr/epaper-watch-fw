/**
 * @file    max30102_app.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2025-12-26
 */

#include "app_oximeter.h"
#include "gpio.h"

 /**< max30102 handle */
volatile uint8_t g_oximeter_data_ready = 0;
volatile uint8_t g_oximeter_event = 0;
uint32_t red_buffer[32];
uint32_t ir_buffer[32];
uint8_t len = 32;

/**
 * @brief  fifo receive callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
static void _max30102_interface_receive_callback(uint8_t type) {
    switch (type) {
    case MAX30102_INTERRUPT_STATUS_PPG_RDY:        
        break;
    case MAX30102_INTERRUPT_STATUS_FIFO_FULL:
        g_oximeter_data_ready = 1;
        break;
    default:
        break;
    }
}


uint8_t max30102_event() {
    if (g_oximeter_event) {
        g_oximeter_event = 0;
        return 1;
    }
    return 0;
}

// /**
//  * @brief  fifo example irq handler
//  * @return status code
//  *         - 0 success
//  *         - 1 run failed
//  * @note   none
//  */
// uint8_t max30102_fifo_irq_handler(void) {
//     /* run irq handler */
//     if (max30102_irq_handler(&max_handle) != 0) {
//         return 1;
//     } else {
//         return 0;
//     }
// }

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


    res = oximeter_init();
    if (res != 0) {
        return 1;
    }

}

// /**
//  * @brief  fifo example deinit
//  * @return status code
//  *         - 0 success
//  *         - 1 deinit failed
//  * @note   none
//  */
// uint8_t max30102_fifo_deinit(void) {
//     uint8_t res;

//     res = max30102_deinit(&max_handle);
//     if (res != 0) {
//         return 1;
//     } else {
//         return 0;
//     }
// }

// /**
//  * @brief         read the data
//  * @param[out]    *raw_red pointer to a read raw data buffer
//  * @param[out]    *raw_ir pointer to a ir raw data buffer
//  * @param[in,out] *len pointer to a length buffer
//  * @return        status code
//  *                - 0 success
//  *                - 1 read failed
//  * @note          none
//  */
// uint8_t max30102_fifo_read(uint32_t *raw_red, uint32_t *raw_ir, uint8_t *len) {
//     uint8_t res;

//     res = max30102_read(&max_handle, raw_red, raw_ir, len);
//     if (res != 0) {
//         return 1;
//     } else {
//         return 0;
//     }
// }

// uint8_t oximeter_proccess() {

//     if (max30102_event()) {
//         max30102_irq_handler(&max_handle);

//         if (g_oximeter_data_ready) {
//             len = 32;
//             max30102_fifo_read(red_buffer, ir_buffer, &len);
//             g_oximeter_data_ready = 0;
//         }
//     }
    
// }
