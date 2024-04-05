# This is converter generated file, and shall not be touched by user
#
# It is always re-generated if converter script is called multiple times
# Use CMakeLists.txt to apply user changes
cmake_minimum_required(VERSION 3.22)

# Core MCU flags, CPU, instruction set and FPU setup
set(cpu_PARAMS ${cpu_PARAMS}
    -mthumb

    # -mcpu, -mfloat, -mfloat-abi config
    -mcpu=cortex-m3
)

# Linker script
set(linker_script_SRC ${linker_script_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/STM32F103C8TX_FLASH.ld)

# Sources
set(sources_SRCS ${sources_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/core/src/core_cm3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/core/src/startup_stm32f103c8tx.s
    ${CMAKE_CURRENT_SOURCE_DIR}/core/src/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/core/src/core_cm3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/core/src/startup_stm32f103c8tx.s
    ${CMAKE_CURRENT_SOURCE_DIR}/core/src/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/misc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_bkp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_can.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_cec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_crc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_dac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_dbgmcu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_fsmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_iwdg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_sdio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/src/stm32f10x_wwdg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/sys/sys.c
)

# Include directories
set(include_c_DIRS ${include_c_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/core/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_FWLib/sys
)
set(include_cxx_DIRS ${include_cxx_DIRS})
set(include_asm_DIRS ${include_asm_DIRS})

# Symbols definition
set(symbols_c_SYMB ${symbols_c_SYMB})
set(symbols_cxx_SYMB ${symbols_cxx_SYMB})
set(symbols_asm_SYMB ${symbols_asm_SYMB}
    "DEBUG"
)

# Link directories
set(link_DIRS ${link_DIRS})

# Link libraries
set(link_LIBS ${link_LIBS})

# Compiler options
set(compiler_OPTS ${compiler_OPTS})

# Linker options
set(linker_OPTS ${linker_OPTS})
