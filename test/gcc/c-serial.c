/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Driver_USART.h>
#include <stdint.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"

ARM_DRIVER_VERSION
Test_GetVersion (void)
{
  static ARM_DRIVER_VERSION version =
    { .api = 0x1234, .drv = 0x5678 };
  return version;
}

ARM_USART_CAPABILITIES
Test_GetCapabilities (void)
{
  static ARM_USART_CAPABILITIES capa =
    {
    //
        .asynchronous = 1,      ///< supports UART (Asynchronous) mode
        .synchronous_master = 0,      ///< supports Synchronous Master mode
        .synchronous_slave = 1,      ///< supports Synchronous Slave mode
        .single_wire = 0,      ///< supports UART Single-wire mode
        .irda = 0,      ///< supports UART IrDA mode
        .smart_card = 1,      ///< supports UART Smart Card mode
        .smart_card_clock = 0,      ///< Smart Card Clock generator available
        .flow_control_rts = 1,      ///< RTS Flow Control available
        .flow_control_cts = 1,      ///< CTS Flow Control available
        .event_tx_complete = 1, ///< Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
        .event_rx_timeout = 1, ///< Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
        .rts = 1,      ///< RTS Line: 0=not available, 1=available
        .cts = 0,      ///< CTS Line: 0=not available, 1=available
        .dtr = 0,      ///< DTR Line: 0=not available, 1=available
        .dsr = 0,      ///< DSR Line: 0=not available, 1=available
        .dcd = 0,      ///< DCD Line: 0=not available, 1=available
        .ri = 1,      ///< RI Line: 0=not available, 1=available
        .event_cts = 0, ///< Signal CTS change event: \ref ARM_USART_EVENT_CTS
        .event_dsr = 1, ///< Signal DSR change event: \ref ARM_USART_EVENT_DSR
        .event_dcd = 0, ///< Signal DCD change event: \ref ARM_USART_EVENT_DCD
        .event_ri = 1,   ///< Signal RI change event: \ref ARM_USART_EVENT_RI
      };
  return capa;
}

ARM_USART_STATUS
Test_GetStatus (void)
{
  ARM_USART_STATUS status =
    { .tx_busy = 1, .rx_busy = 0, .tx_underflow = 1, .rx_overflow = 1,
        .rx_break = 0, .rx_framing_error = 0, .rx_parity_error = 1, };
  return status;
}

ARM_USART_MODEM_STATUS
Test_GetModemStatus (void)
{
  ARM_USART_MODEM_STATUS modem_status =
    { .cts = 1, .dsr = 0, .dcd = 1, .ri = 1, };
  return modem_status;
}

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"

int32_t
Test_Initialize (ARM_USART_SignalEvent_t cb_event)
{
  return ARM_DRIVER_OK;
}

int32_t
Test_Uninitialize (void)
{
  return ARM_DRIVER_OK;
}

int32_t
Test_PowerControl (ARM_POWER_STATE state)
{
  switch (state)
    {
    case ARM_POWER_FULL:
      return 11;
    case ARM_POWER_OFF:
      return 22;
    case ARM_POWER_LOW:
      return 33;
    }
  return 44;
}

int32_t
Test_Send (const void *data, uint32_t num)
{
  return (int32_t) num;
}

int32_t
Test_Receive (void *data, uint32_t num)
{
  return (int32_t) num;
}

int32_t
Test_Transfer (const void *data_out, void *data_in, uint32_t num)
{
  return (int32_t) num;
}

uint32_t
Test_GetTxCount (void)
{
  return 78;
}
uint32_t
Test_GetRxCount (void)
{
  return 87;
}

int32_t
Test_Control (uint32_t control, uint32_t arg)
{
  return (int32_t) (control + arg);
}

int32_t
Test_SetModemControl (ARM_USART_MODEM_CONTROL control)
{
  switch (control)
    {
    case ARM_USART_RTS_CLEAR:
      return 21;
    case ARM_USART_RTS_SET:
      return 22;
    case ARM_USART_DTR_CLEAR:
      return 23;
    case ARM_USART_DTR_SET:
      return 24;
    }
  return 25;
}

#pragma GCC diagnostic pop

ARM_DRIVER_USART test_usart =
  { Test_GetVersion, Test_GetCapabilities, Test_Initialize, Test_Uninitialize,
      Test_PowerControl, Test_Send, Test_Receive, Test_Transfer,
      Test_GetTxCount, Test_GetRxCount, Test_Control, Test_GetStatus,
      Test_SetModemControl, Test_GetModemStatus, };
