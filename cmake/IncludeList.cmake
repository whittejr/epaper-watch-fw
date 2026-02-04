set(include_list ${include_list}

${PROJ_PATH}lib/STM32WBxx_HAL_Driver/Inc
${PROJ_PATH}lib/STM32WBxx_HAL_Driver/Inc/Legacy
${PROJ_PATH}lib/CMSIS/Core/Include
${PROJ_PATH}lib/CMSIS/Device/ST/STM32WBxx/Include
${PROJ_PATH}lib/CMSIS/DSP/Include
${PROJ_PATH}lib/CMSIS/Include
${PROJ_PATH}lib/CMSIS/NN/Include
${PROJ_PATH}lib/CMSIS/RTOS2/Include
# # # # # #  # # # #
${PROJ_PATH}core/inc
${PROJ_PATH}lib/bsp/inc
# display
# epaper
${PROJ_PATH}app/display
${PROJ_PATH}app/display/assets
${PROJ_PATH}lib/components/display/ssd1681
# mip
${PROJ_PATH}lib/components/display/ls013b7dh03

# oximeter
${PROJ_PATH}app/oximeter
${PROJ_PATH}lib/components/max30102
# system
${PROJ_PATH}app/system
)