/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#include <Driver_USBH.h>
#include <stdint.h>

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wunused-parameter"

ARM_DRIVER_VERSION
Usbh_GetVersion (void)
{
  static ARM_DRIVER_VERSION version =
    { .api = 0x1234, .drv = 0x5678 };
  return version;
}

ARM_USBH_CAPABILITIES
Usbh_GetCapabilities (void)
{
  static ARM_USBH_CAPABILITIES capa =
    {
    //
        .port_mask = 77, ///< Root HUB available Ports Mask
        .auto_split = 1, ///< Automatic SPLIT packet handling
        .event_connect = 1, ///< Signal Connect event
        .event_disconnect = 0, ///< Signal Disconnect event
        .event_overcurrent = 0, ///< Signal Overcurrent event
      };
  return capa;
}

ARM_USBH_PORT_STATE
Usbh_PortGetState (uint8_t port)
{
  ARM_USBH_PORT_STATE status =
    {
    //
        .connected = 0, //
        .overcurrent = 1, //
        .speed = 2 //
      };
  return status;
}

int32_t
Usbh_Initialize (ARM_USBH_SignalPortEvent_t cb_port_event,
                 ARM_USBH_SignalPipeEvent_t cb_pipe_event)
{
  return 0; // Must be 0
}

int32_t
Usbh_Uninitialize (void)
{
  return 0;
}

int32_t
Usbh_PowerControl (ARM_POWER_STATE state)
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
Usbh_PortVbusOnOff (uint8_t port, bool vbus)
{
  return port + 1;
}

int32_t
Usbh_PortReset (uint8_t port)
{
  return port + 2;
}

int32_t
Usbh_PortSuspend (uint8_t port)
{
  return port + 3;
}

int32_t
Usbh_PortResume (uint8_t port)
{
  return port + 4;
}

ARM_USBH_PIPE_HANDLE
Usbh_PipeCreate (uint8_t dev_addr, uint8_t dev_speed, uint8_t hub_addr,
                 uint8_t hub_port, uint8_t ep_addr, uint8_t ep_type,
                 uint16_t ep_max_packet_size, uint8_t ep_interval)
{
  return dev_addr + 5;
}

int32_t
Usbh_PipeModify (ARM_USBH_PIPE_HANDLE pipe_hndl, uint8_t dev_addr,
                 uint8_t dev_speed, uint8_t hub_addr, uint8_t hub_port,
                 uint16_t ep_max_packet_size)
{
  return pipe_hndl + 6;
}

int32_t
Usbh_PipeDelete (ARM_USBH_PIPE_HANDLE pipe_hndl)
{
  return pipe_hndl + 7;
}

int32_t
Usbh_PipeReset (ARM_USBH_PIPE_HANDLE pipe_hndl)
{
  return pipe_hndl + 8;
}

int32_t
Usbh_PipeTransfer (ARM_USBH_PIPE_HANDLE pipe_hndl, uint32_t packet,
                   uint8_t *data, uint32_t num)
{
  return pipe_hndl + 9;
}

uint32_t
Usbh_PipeTransferGetResult (ARM_USBH_PIPE_HANDLE pipe_hndl)
{
  return pipe_hndl + 10;
}

int32_t
Usbh_PipeTransferAbort (ARM_USBH_PIPE_HANDLE pipe_hndl)
{
  return pipe_hndl + 11;
}

uint16_t
Usbh_GetFrameNumber (void)
{
  return 9;
}

#pragma GCC diagnostic pop

ARM_DRIVER_USBH test_usbh_driver =
  { Usbh_GetVersion, Usbh_GetCapabilities, Usbh_Initialize, Usbh_Uninitialize,
      Usbh_PowerControl, Usbh_PortVbusOnOff, Usbh_PortReset, Usbh_PortSuspend,
      Usbh_PortResume, Usbh_PortGetState, Usbh_PipeCreate, Usbh_PipeModify,
      Usbh_PipeDelete, Usbh_PipeReset, Usbh_PipeTransfer,
      Usbh_PipeTransferGetResult, Usbh_PipeTransferAbort, Usbh_GetFrameNumber };
