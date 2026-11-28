/*
 * DO NOT EDIT! Automatically generated from CMSIS startup file:
 * {{libraryFilePath}}
 *
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2023-2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

// clang-format off
// ----------------------------------------------------------------------------

#include "micro-os-plus/architecture.h"
#include "micro-os-plus/architecture-cortexm/exception-handlers.h"

// ----------------------------------------------------------------------------

void __attribute__((weak))
Default_Handler(void);

/*
 * Forward declaration of the specific IRQ handlers. These are aliased
 * to the Default_Handler, which is a 'forever' loop. When the application
 * defines an actual handler (with the same name), it will automatically
 * take precedence over these weak definitions.
 */

{%- for handler in handlers %}
{%- unless handler.symbol == "0" %}
void {{handler.symbol}}(void) __attribute__ ((weak, alias ("Default_Handler")));
{%- endunless %}
{%- endfor %}

// ----------------------------------------------------------------------------

extern uint32_t __stack;

typedef void
(*handler_ptr_t)(void);

// ----------------------------------------------------------------------------

#if defined(__GNUC__)
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wpedantic"
#endif // defined(__GNUC__)

/**
 * The table of interrupt handlers. It has an explicit section name
 * and relies on the linker script to place it at the correct location
 * in memory, usually at the very beginning of the flash bank.
 */
handler_ptr_t _interrupt_vectors[]
__attribute__ ((section(".interrupt_vectors"),used)) =
  {
    // Cortex-M Core Handlers
    (handler_ptr_t) &__stack,          // The Main Stack Pointer (MSP)
    Reset_Handler,                     // The Reset handler

    NMI_Handler,                       // The NMI handler
    HardFault_Handler,                 // The Hard Fault handler
{%- if isArmArch6m %}
    0,                                 // No MPU fault handler
    0,                                 // No Bus Fault handler
    0,                                 // No Usage Fault handler
{%- else %}
    MemManage_Handler,                 // The MPU fault handler
    BusFault_Handler,                  // The Bus Fault handler
    UsageFault_Handler,                // The Usage Fault handler
{%- endif %}
{%- if isArmArch8m %}
    SecureFault_Handler,               // The Secure Fault handler
{%- else %}
    0,                                 // Reserved
{%- endif %}
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    SVC_Handler,                       // The SVCall handler
{%- if isArmArch6m %}
    0,                                 // No Debug Monitor handler
{%- else %}
    DebugMon_Handler,                  // The Debug Monitor handler
{%- endif %}
    0,                                 // Reserved
    PendSV_Handler,                    // The PendSV handler
    SysTick_Handler,                   // The SysTick handler

    // ------------------------------------------------------------------------
    // External Interrupt Handlers
{% for handler in handlers %}
{%- assign commaLength = 0 %}
{%- unless forloop.last %}{% assign commaLength = 1 %}{% endunless %}
{%- assign padLength = 39 | minus: handler.symbol.size | minus: commaLength | minus: 4 %}
    {{handler.symbol}}{% unless forloop.last %},{% endunless %}{% if handler.comment %}{{ '                                                  ' | slice: 0, padLength }}// {{handler.comment}}{% endif %}
{%- endfor %}
};

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__)

// ----------------------------------------------------------------------------

/**
 * The processor ends up here if an unexpected interrupt occurs or a
 * specific handler is not present in the application code.
 * When in DEBUG, it triggers a debug exception to clearly notify
 * the user of the exception and help identify the cause.
 * Otherwise, it enters a forever loop.
 */
void __attribute__ ((section(".after_vectors")))
Default_Handler(void)
{
#if defined(MICRO_OS_PLUS_DEBUG_ENABLED)
  micro_os_plus_architecture_brk();
#endif
  while (1)
    {
      micro_os_plus_architecture_wfi();
    }
}

// ----------------------------------------------------------------------------
