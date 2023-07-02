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

#include <Driver_USBD.h>
#include <stdint.h>

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif
#pragma GCC diagnostic ignored "-Wmissing-prototypes"

ARM_DRIVER_VERSION
Usbd_GetVersion (void)
{
  static ARM_DRIVER_VERSION version =
    { .api = 0x1234, .drv = 0x5678 };
  return version;
}

ARM_USBD_CAPABILITIES
Usbd_GetCapabilities (void)
{
  static ARM_USBD_CAPABILITIES capa =
    {
    //
        .vbus_detection = 1, ///< VBUS detection
        .event_vbus_on = 1, ///< Signal VBUS On event
        .event_vbus_off = 0, ///< Signal VBUS Off event
      };
  return capa;
}

ARM_USBD_STATE
Usbd_DeviceGetState (void)
{
  ARM_USBD_STATE status =
    {
    //
        .vbus = 0, //
        .speed = 2, //
        .active = 1 //
      };
  return status;
}

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#endif

int32_t
Usbd_Initialize (ARM_USBD_SignalDeviceEvent_t cb_device_event,
                 ARM_USBD_SignalEndpointEvent_t cb_endpoint_event)
{
  return 0;
}

int32_t
Usbd_Uninitialize (void)
{
  return 0;
}

int32_t
Usbd_PowerControl (ARM_POWER_STATE state)
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
Usbd_DeviceConnect (void)
{
  return 4;
}

int32_t
Usbd_DeviceDisconnect (void)
{
  return 5;
}


int32_t
Usbd_DeviceRemoteWakeup (void)
{
  return 6;
}

int32_t
Usbd_DeviceSetAddress (uint8_t dev_addr)
{
  return dev_addr;
}

int32_t
Usbd_ReadSetupPacket (uint8_t* setup)
{
  return 7;
}

int32_t
Usbd_EndpointConfigure (uint8_t ep_addr, uint8_t ep_type,
                        uint16_t ep_max_packet_size)
{
  return ep_addr;
}

int32_t
Usbd_EndpointUnconfigure (uint8_t ep_addr)
{
  return ep_addr;
}

int32_t
Usbd_EndpointStall (uint8_t ep_addr, bool stall)
{
  return ep_addr;
}

int32_t
Usbd_EndpointTransfer (uint8_t ep_addr, uint8_t* data, uint32_t num)
{
  return ep_addr;
}

uint32_t
Usbd_EndpointTransferGetResult (uint8_t ep_addr)
{
  return ep_addr;
}

int32_t
Usbd_EndpointTransferAbort (uint8_t ep_addr)
{
  return ep_addr;
}

uint16_t
Usbd_GetFrameNumber (void)
{
  return 8;
}

#pragma GCC diagnostic pop

ARM_DRIVER_USBD test_usbd_driver =
  { Usbd_GetVersion, Usbd_GetCapabilities, Usbd_Initialize, Usbd_Uninitialize,
      Usbd_PowerControl, Usbd_DeviceConnect, Usbd_DeviceDisconnect,
      Usbd_DeviceGetState, Usbd_DeviceRemoteWakeup, Usbd_DeviceSetAddress,
      Usbd_ReadSetupPacket, Usbd_EndpointConfigure, Usbd_EndpointUnconfigure,
      Usbd_EndpointStall, Usbd_EndpointTransfer, Usbd_EndpointTransferGetResult,
      Usbd_EndpointTransferAbort, Usbd_GetFrameNumber };
