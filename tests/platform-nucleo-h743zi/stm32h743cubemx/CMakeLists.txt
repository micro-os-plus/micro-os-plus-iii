# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus/)
# Copyright (c) 2023 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/mit/.
#
# -----------------------------------------------------------------------------

message(VERBOSE "Processing 'tests/platform-nucleo-h743zi/stm32h743cubemx'...")

# -----------------------------------------------------------------------------
# Define the stm32cubemx library.

# Note: It cannot be a library, since it make extensive use of `weak` symbols.

add_library(stm32cubemx-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------

target_include_directories(stm32cubemx-interface INTERFACE
  "Core/Inc"
  "Drivers/CMSIS/Include"
  "Drivers/CMSIS/Device/ST/STM32H7xx/Include"
  "Drivers/STM32H7xx_HAL_Driver/Inc"
)

# find * -type f -name "*.c" -o -name "*.S" -o -name "*.s" | sed -e 's|\(.*\)|  "\1"|'
target_sources(stm32cubemx-interface INTERFACE
  "Core/Src/stm32h7xx_hal_msp.c"
  "Core/Src/gpio.c"
  "Core/Src/system_stm32h7xx.c"
  "Core/Src/main.c"
  "Core/Src/stm32h7xx_it.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c"
  "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c"
  # DO NOT include the assembly startup, use the .c one.
  # "startup_stm32h743xx.s"
)

target_compile_definitions(stm32cubemx-interface INTERFACE
  # From stm32h7xx.h
  STM32H743xx
  USE_HAL_DRIVER
)


target_compile_options(stm32cubemx-interface INTERFACE
  # None
)

target_link_libraries(stm32cubemx-interface INTERFACE
  # None
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(stm32cubemx-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

add_library(micro-os-plus::stm32cubemx ALIAS stm32cubemx-interface)
message(VERBOSE "> micro-os-plus::stm32cubemx -> stm32cubemx-interface")

# -----------------------------------------------------------------------------
