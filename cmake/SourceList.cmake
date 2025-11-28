# Lista de arquivos-fonte do projeto
set(source_list
# --- Arquivo de Startup (Assembly) ---
    ${PROJ_PATH}/Core/src/startup_stm32wb55xx_cm4.s


    # --- Arquivos do Core da Aplicação ---
    ${PROJ_PATH}/Core/src/main.c
    ${PROJ_PATH}/Core/src/it.c              # Adicione se tiver este arquivo para interrupções
    # ${PROJ_PATH}/Core/src/stm32wbxx_hal_msp.c        # Adicione se tiver este arquivo para inicialização de periféricos
    ${PROJ_PATH}/Core/src/system_stm32wbxx.c

    
    # --- Seus Drivers Customizados ---
    # Display E-Paper SSD1681
    ${PROJ_PATH}/Drivers/Custom/Display/Src/ssd1681.c
    ${PROJ_PATH}/Drivers/Custom/Display/Src/stm32wb55_driver_ssd1681_interface.c
    ${PROJ_PATH}/Drivers/Custom/Display/Src/user_config.c

    # Interface (SPI, etc)
    ${PROJ_PATH}/Drivers/Custom/Interface/Src/spi.c
    ${PROJ_PATH}/Drivers/Custom/Interface/Src/gpio.c
    ${PROJ_PATH}/Drivers/Custom/Interface/Src/delay.c

    # --- Drivers da ST HAL (apenas os que você usa) ---
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_cortex.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_dma.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_dma_ex.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_exti.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_flash.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_flash_ex.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_gpio.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pwr.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pwr_ex.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rcc.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rcc_ex.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_spi.c       # Essencial para o seu display
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_spi_ex.c
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_tim.c       # Adicione se usar Timers
    ${PROJ_PATH}/Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_tim_ex.c
    # Adicione outros drivers HAL aqui conforme for usando (ex: i2c, uart)
)