/**
 * @file    clock.c
 * @brief   none
 * @version 0.1.0
 * @author  Alessandro Davi
 * @date    2026-01-07
 */

#include "stm32wbxx_hal.h"

uint8_t clock_init() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_TIM17_CLK_ENABLE();

    // RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    // RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
    //                           |RCC_OSCILLATORTYPE_MSI;
    // RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    // RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    // RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    // RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    // RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    // RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    // RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    // RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    // RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    // RCC_OscInitStruct.PLL.PLLN = 24;
    // RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    // RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    // RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;

    // if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    //     return 1;
    // }

    //   /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
    // */
    // RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
    //                           |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
    //                           |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    // RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    // RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    // RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    // RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    // RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
    // RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

    // if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    //     return 1;
    // }

    return 0;
}
