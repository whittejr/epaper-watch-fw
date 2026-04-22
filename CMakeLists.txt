cmake_minimum_required(VERSION 3.22)
project(${TARGET_MCU} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE "firmware")

if(NOT DEFINED MCU_FAMILY)
    message(FATAL_ERROR "MCU_FAMILY não definida!")
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/mcu/${MCU_FAMILY}.cmake")

if("${LINKER_SCRIPT}" STREQUAL "")
    message(FATAL_ERROR "ERRO: A variável LINKER_SCRIPT está vazia! Verifique o seu arquivo ${MCU_FAMILY}.cmake.")
endif()

if("${HAL_SOURCES}" STREQUAL "")
    message(FATAL_ERROR "ERRO: A variável HAL_SOURCES está vazia! A HAL não será compilada.")
endif()

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "core/src/*.c"
    "lib/bsp/src/*.c"
    "lib/components/*.c"
    "app/*.c"
    "app/alarm/*.c"
    "app/oximeter/*.c"
    "app/ui/*.c"
    "app/ui/assets/*.c"
    "app/ui/screens/*.c"
)

# FreeRTOS Sources
set(FREERTOS_DIR "lib/third_party/stm32cubewb/Middlewares/Third_Party/FreeRTOS/Source")
set(FREERTOS_SOURCES
    "${FREERTOS_DIR}/tasks.c"
    "${FREERTOS_DIR}/queue.c"
    "${FREERTOS_DIR}/list.c"
    "${FREERTOS_DIR}/timers.c"
    "${FREERTOS_DIR}/event_groups.c"
    "${FREERTOS_DIR}/stream_buffer.c"
    "${FREERTOS_DIR}/portable/GCC/ARM_CM4F/port.c"
    "${FREERTOS_DIR}/portable/MemMang/heap_4.c"
)

# CMSIS-DSP
set(CMSIS_DSP_DIR "lib/third_party/stm32cubewb/Drivers/CMSIS/DSP")
file(GLOB_RECURSE CMSIS_DSP_SOURCES 
    "${CMSIS_DSP_DIR}/Source/FilteringFunctions/*.c"
    "${CMSIS_DSP_DIR}/Source/StatisticsFunctions/*.c"
    "${CMSIS_DSP_DIR}/Source/SupportFunctions/*.c"
    "${CMSIS_DSP_DIR}/Source/BasicMathFunctions/*.c"
    "${CMSIS_DSP_DIR}/Source/CommonTables/*.c"
)

list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX "FilteringFunctions/FilteringFunctions.c$")
list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX "StatisticsFunctions/StatisticsFunctions.c$")
list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX "SupportFunctions/SupportFunctions.c$")
list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX "BasicMathFunctions/BasicMathFunctions.c$")
list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX "CommonTables/CommonTables.c$")
list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX "ComplexMathFunctions/ComplexMathFunctions.c$")
list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX "FastMathFunctions/FastMathFunctions.c$")
list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX "MatrixFunctions/MatrixFunctions.c$")
list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX "TransformFunctions/TransformFunctions.c$")
list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX "ControllerFunctions/ControllerFunctions.c$")

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
    "core/**/*.h"
    "lib/bsp/**/*.h"
    "lib/components/**/*.h"
    "app/**/*.h"
)
set(APP_INCLUDES "")
foreach(HEADER ${HEADER_FILES})
    get_filename_component(HEADER_DIR ${HEADER} DIRECTORY)
    list(APPEND APP_INCLUDES ${HEADER_DIR})
endforeach()

if(APP_INCLUDES)
    list(REMOVE_DUPLICATES APP_INCLUDES)
endif()

add_executable(${EXECUTABLE} ${APP_SOURCES} ${HAL_SOURCES} ${FREERTOS_SOURCES} ${CMSIS_DSP_SOURCES})

target_include_directories(${EXECUTABLE} PRIVATE 
    ${APP_INCLUDES}
    ${HAL_INCLUDES} 
    "${FREERTOS_DIR}/include"
    "${FREERTOS_DIR}/portable/GCC/ARM_CM4F"
    "${CMSIS_DSP_DIR}/Include"
)

target_compile_definitions(${EXECUTABLE} PRIVATE 
    USE_HAL_DRIVER 
    ${TARGET_MCU}
    ARM_MATH_CM4
)

target_compile_options(${EXECUTABLE} PRIVATE 
    -Wall 
    -Wpedantic 
    -Wno-unused-parameter
    -fdiagnostics-color=always 
)

target_link_options(${EXECUTABLE} PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    --specs=nano.specs
    -Wl,-u,_printf_float
    -Wl,--print-memory-usage
    -Wl,--gc-sections
)

target_link_libraries(${EXECUTABLE} PRIVATE -lc -lm)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)
set_target_properties(${EXECUTABLE} PROPERTIES SUFFIX ".elf")
