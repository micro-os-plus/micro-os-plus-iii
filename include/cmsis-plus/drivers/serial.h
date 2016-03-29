/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 * Copyright (c) 2013-2014 ARM Ltd.
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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * The code is inspired by ARM CMSIS Driver_USART.h file, v2.02,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#ifndef CMSIS_PLUS_DRIVERS_SERIAL_H_
#define CMSIS_PLUS_DRIVERS_SERIAL_H_

// ----------------------------------------------------------------------------

#ifdef __cplusplus

#include <cmsis-plus/drivers/common.h>

#include <cstdint>
#include <cstddef>

namespace os
{
  namespace driver
  {
    // ----------------------------------------------------------------------

    namespace serial
    {
      using bit_number_t = uint32_t;
      using config_t = uint32_t;
      using config_arg_t = uint32_t;
      using control_t = uint32_t;
      using control_arg_t = uint32_t;

      // ****** Serial Control Codes *****

      constexpr bit_number_t CONFIG_Pos = 0;
      constexpr config_t CONFIG_Msk (0xFFUL << CONFIG_Pos);

      // ----- Serial Control Codes: Mode -----
      ///< UART = (Asynchronous);; arg = Baudrate
      constexpr config_t MODE_ASYNCHRONOUS = (0x01UL << CONFIG_Pos);

      ///< Synchronous Master = (generates clock signal); arg = Baudrate
      constexpr config_t MODE_SYNCHRONOUS_MASTER = (0x02UL << CONFIG_Pos);

      ///< Synchronous Slave = (external clock signal);
      constexpr config_t MODE_SYNCHRONOUS_SLAVE = (0x03UL << CONFIG_Pos);

      ///< UART Single-wire = (half-duplex);; arg = Baudrate
      constexpr config_t MODE_SINGLE_WIRE = (0x04UL << CONFIG_Pos);

      ///< UART IrDA; arg = Baudrate
      constexpr config_t MODE_IRDA = (0x05UL << CONFIG_Pos);

      ///< UART Smart Card; arg = Baudrate
      constexpr config_t MODE_SMART_CARD = (0x06UL << CONFIG_Pos);

      // ----- Serial Control Codes: Mode Parameters: Data Bits -----
      constexpr bit_number_t DATA_BITS_Pos = 8;
      constexpr config_t DATA_BITS_Msk = (7UL << DATA_BITS_Pos);

      ///< 5 Data bits
      constexpr config_t DATA_BITS_5 = (5UL << DATA_BITS_Pos);

      ///< 6 Data bit
      constexpr config_t DATA_BITS_6 = (6UL << DATA_BITS_Pos);

      ///< 7 Data bits
      constexpr config_t DATA_BITS_7 = (7UL << DATA_BITS_Pos);

      ///< 8 Data bits = (default);
      constexpr config_t DATA_BITS_8 = (0UL << DATA_BITS_Pos);

      ///< 9 Data bits
      constexpr config_t DATA_BITS_9 = (1UL << DATA_BITS_Pos);

      // ----- Serial Control Codes: Mode Parameters: Parity -----
      constexpr bit_number_t PARITY_Pos = 12;
      constexpr config_t PARITY_Msk = (3UL << PARITY_Pos);

      ///< No Parity = (default);
      constexpr config_t PARITY_NONE = (0UL << PARITY_Pos);

      ///< Even Parity
      constexpr config_t PARITY_EVEN = (1UL << PARITY_Pos);

      ///< Odd Parity
      constexpr config_t PARITY_ODD = (2UL << PARITY_Pos);

      // ----- Serial Control Codes: Mode Parameters: Stop Bits -----
      constexpr bit_number_t STOP_BITS_Pos = 14;
      constexpr config_t STOP_BITS_Msk = (3UL << STOP_BITS_Pos);

      ///< 1 Stop bit = (default);
      constexpr config_t STOP_BITS_1 = (0UL << STOP_BITS_Pos);

      ///< 2 Stop bits
      constexpr config_t STOP_BITS_2 = (1UL << STOP_BITS_Pos);

      ///< 1.5 Stop bits
      constexpr config_t STOP_BITS_1_5 = (2UL << STOP_BITS_Pos);

      ///< 0.5 Stop bits
      constexpr config_t STOP_BITS_0_5 = (3UL << STOP_BITS_Pos);

      // ----- Serial Control Codes: Mode Parameters: Flow Control -----
      constexpr bit_number_t FLOW_CONFIG_Pos = 16;
      constexpr config_t FLOW_CONTROL_Msk = (3UL << FLOW_CONFIG_Pos);

      ///< No Flow Control = (default);
      constexpr config_t FLOW_CONTROL_NONE = (0UL << FLOW_CONFIG_Pos);

      ///< RTS Flow Control
      constexpr config_t FLOW_CONTROL_RTS = (1UL << FLOW_CONFIG_Pos);

      ///< CTS Flow Control
      constexpr config_t FLOW_CONTROL_CTS = (2UL << FLOW_CONFIG_Pos);

      ///< RTS/CTS Flow Control
      constexpr config_t FLOW_CONTROL_RTS_CTS = (3UL << FLOW_CONFIG_Pos);

      // ----- Serial Control Codes: Mode Parameters: Clock Polarity = (Synchronous mode); -----
      constexpr bit_number_t CPOL_Pos = 18;
      constexpr config_t CPOL_Msk = (1UL << CPOL_Pos);

      ///< CPOL = 0 = (default);
      constexpr config_t CPOL0 = (0UL << CPOL_Pos);

      ///< CPOL = 1
      constexpr config_t CPOL1 = (1UL << CPOL_Pos);

      // ----- Serial Control Codes: Mode Parameters: Clock Phase = (Synchronous mode); -----
      constexpr bit_number_t CPHA_Pos = 19;
      constexpr config_t CPHA_Msk = (1UL << CPHA_Pos);

      ///< CPHA = 0 = (default);
      constexpr config_t CPHA0 = (0UL << CPHA_Pos);

      ///< CPHA = 1
      constexpr config_t CPHA1 = (1UL << CPHA_Pos);

      // ----- Serial Control Codes: Miscellaneous Controls  -----

      ///< Set default Transmit value = (Synchronous Receive only); arg = value
      constexpr config_t DEFAULT_TX_VALUE = (0x10UL << CONFIG_Pos);

      ///< Set IrDA Pulse in ns; arg: 0=3/16 of bit period
      constexpr config_t IRDA_PULSE = (0x11UL << CONFIG_Pos);

      ///< Set Smart Card Guard Time; arg = number of bit periods
      constexpr config_t SMART_CARD_GUARD_TIME = (0x12UL << CONFIG_Pos);

      ///< Set Smart Card Clock in Hz; arg: 0=Clock not generated
      constexpr config_t SMART_CARD_CLOCK = (0x13UL << CONFIG_Pos);

      ///< Smart Card NACK generation; arg: 0=disabled, 1=enabled
      constexpr config_t SMART_CARD_NACK = (0x14UL << CONFIG_Pos);

      // --------------------------------------------------------------------
      // ----- Commands -----

      enum Control
        : control_t
          {
            //
        ///< Enable Transmitter
        enable_tx = (0x15UL << CONFIG_Pos),

        ///< Enable Receiver
        enable_rx = (0x16UL << CONFIG_Pos),

        ///< Enable Continuous Break transmission
        enable_break = (0x17UL << CONFIG_Pos),

        ///< Abort @ref Serial::send()
        abort_send = (0x18UL << CONFIG_Pos),

        ///< Abort @ref Serial::receive()
        abort_receive = (0x19UL << CONFIG_Pos),

        ///< Abort @ref Serial::transfer()
        abort_transfer = (0x1AUL << CONFIG_Pos),

        ///< Disable Transmitter
        disable_tx = (0x25UL << CONFIG_Pos),

        ///< Disable Receiver
        disable_rx = (0x26UL << CONFIG_Pos),

        ///< Disable Continuous Break transmission;
        disable_break = (0x27UL << CONFIG_Pos)
      };

      // --------------------------------------------------------------------
      // ****** Serial specific error codes *****

      ///< Specified Mode not supported
      constexpr return_t ERROR_MODE = ERROR_SPECIFIC - 1;

      ///< Specified baudrate not supported
      constexpr return_t ERROR_BAUDRATE = ERROR_SPECIFIC - 2;

      ///< Specified number of Data bits not supported
      constexpr return_t ERROR_DATA_BITS = ERROR_SPECIFIC - 3;

      ///< Specified Parity not supported
      constexpr return_t ERROR_PARITY = ERROR_SPECIFIC - 4;

      ///< Specified number of Stop bits not supported
      constexpr return_t ERROR_STOP_BITS = ERROR_SPECIFIC - 5;

      ///< Specified Flow Control not supported
      constexpr return_t ERROR_FLOW_CONTROL = ERROR_SPECIFIC - 6;

      ///< Specified Clock Polarity not supported
      constexpr return_t ERROR_CPOL = ERROR_SPECIFIC - 7;

      ///< Specified Clock Phase not supported
      constexpr return_t ERROR_CPHA = ERROR_SPECIFIC - 8;

      // ====================================================================
      // ***** Serial Status *****

      /**
       * @brief %Serial port status
       */
      class Status
      {
      public:

        bool
        is_tx_busy (void);

        bool
        is_rx_busy (void);

        bool
        is_tx_underflow (void);

        bool
        is_rx_overflow (void);

        bool
        is_rx_break (void);

        bool
        is_rx_framing_error (void);

        bool
        is_rx_parity_error (void);

        // ------------------------------------------------------------------

      public:

        // For compatibility with ARM CMSIS, these bits should be
        // exactly in this order.

        ///< Transmitter busy flag
        bool tx_busy :1;

        ///< Receiver busy flag
        bool rx_busy :1;

        ///< Transmit data underflow detected (cleared on start of next send operation)
        bool tx_underflow :1;

        ///< Receive data overflow detected (cleared on start of next receive operation)
        bool rx_overflow :1;

        ///< Break detected on receive (cleared on start of next receive operation)
        bool rx_break :1;

        ///< Framing error detected on receive (cleared on start of next receive operation)
        bool rx_framing_error :1;

        ///< Parity error detected on receive (cleared on start of next receive operation)
        bool rx_parity_error :1;
      };

      // ====================================================================
      // ***** Serial Modem Control *****

      using modem_config_t = uint32_t;

      /**
       * @brief Configuration to change the serial modem lines.
       */
      enum class Modem_control
        : modem_config_t
          {
            //

        ///< Deactivate RTS
        clear_rts,

        ///< Activate RTS
        set_rts,

        ///< Deactivate DTR
        clear_dtr,

        ///< Activate DTR
        set_dtr
      };

      // ====================================================================

      /**
       * @brief %Serial modem status
       */
      class Modem_status
      {
      public:

        bool
        is_cts_active (void);

        bool
        is_dsr_active (void);

        bool
        is_dcd_active (void);

        bool
        is_ri_active (void);

        // ------------------------------------------------------------------
      public:

        // For compatibility with ARM CMSIS, these bits should be
        // exactly in this order.

        ///< CTS state: true=Active, false=Inactive
        bool cts :1;

        ///< DSR state: true=Active, false=Inactive
        bool dsr :1;

        ///< DCD state: true=Active, false=Inactive
        bool dcd :1;

        ///< RI  state: true=Active, false=Inactive
        bool ri :1;
      };

      // ====================================================================
      // ****** Serial Events *****

      /**
       * @brief Serial Events
       */
      enum Event
        : event_t
          {
            //

        ///< Send completed; however USART may still transmit data
        send_complete = (1UL << 0),

        ///< Receive completed
        receive_complete = (1UL << 1),

        ///< Transfer completed
        transfer_complete = (1UL << 2),

        ///< Transmit completed (optional)
        tx_complete = (1UL << 3),

        ///< Transmit data not available (Synchronous Slave)
        tx_underflow = (1UL << 4),

        ///< Receive data overflow
        rx_overflow = (1UL << 5),

        ///< Receive character timeout (optional)
        rx_timeout = (1UL << 6),

        ///< Break detected on receive
        rx_break = (1UL << 7),

        ///< Framing error detected on receive
        rx_framing_error = (1UL << 8),

        ///< Parity error detected on receive
        rx_parity_error = (1UL << 9),

        ///< CTS state changed (optional)
        cts = (1UL << 10),

        ///< DSR state changed (optional)
        dsr = (1UL << 11),

        ///< DCD state changed (optional)
        dcd = (1UL << 12),

        ///< RI  state changed (optional)
        ri = (1UL << 13),
      };

      // ====================================================================

      /**
       * @brief %Serial device driver capabilities.
       */
      class Capabilities
      {
      public:

        // For compatibility with ARM CMSIS, these bits should be
        // exactly in this order.

        ///< supports Asynchronous mode
        bool asynchronous :1;

        ///< supports Synchronous Master mode
        bool synchronous_master :1;

        ///< supports Synchronous Slave mode
        bool synchronous_slave :1;

        ///< supports Single-wire mode
        bool single_wire :1;

        ///< supports IrDA mode
        bool irda :1;

        ///< supports Smart Card mode
        bool smart_card :1;

        ///< Smart Card Clock generator available
        bool smart_card_clock :1;

        ///< RTS Flow Control available
        bool flow_control_rts :1;

        ///< CTS Flow Control available
        bool flow_control_cts :1;

        ///< Transmit completed event.
        bool event_tx_complete :1;

        ///< Signal receive character timeout event.
        bool event_rx_timeout :1;

        ///< RTS Line: false=not available, true=available
        bool rts :1;

        ///< CTS Line: false=not available, true=available
        bool cts :1;

        ///< DTR Line: false=not available, true=available
        bool dtr :1;

        ///< DSR Line: false=not available, true=available
        bool dsr :1;

        ///< DCD Line: false=not available, true=available
        bool dcd :1;

        ///< RI Line: false=not available, true=available
        bool ri :1;

        ///< Signal CTS change event.
        bool event_cts :1;

        ///< Signal DSR change event.
        bool event_dsr :1;

        ///< Signal DCD change event.
        bool event_dcd :1;

        ///< Signal RI change event.
        bool event_ri :1;
      };

    } /* namespace serial */

    // ======================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    class Serial : public Base
    {

    public:

      // --------------------------------------------------------------------

      Serial () noexcept;

      Serial (const Serial&) = delete;

      Serial (Serial&&) = delete;

      Serial&
      operator= (const Serial&) = delete;

      Serial&
      operator= (Serial&&) = delete;

      virtual
      ~Serial () noexcept = default;

      // --------------------------------------------------------------------

      /**
       * @brief       Register event callback.
       * @param [in] cb_func  Pointer to function.
       * @param [in] cb_object Pointer to object passed to function.
       * @return      Execution status
       */
      void
      register_callback (signal_event_t cb_func,
                         const void* cb_object = nullptr) noexcept;

      // --------------------------------------------------------------------

      /**
       * @brief       Get driver capabilities.
       * @return      Capabilities.
       */
      const serial::Capabilities&
      get_capabilities (void) noexcept;

      /**
       * @brief       Start the serial transmitter.
       * @param [in] data  Pointer to buffer with data to send to USART transmitter.
       * @param [in] num   Number of data items to send.
       * @return      Execution status
       */
      return_t
      send (const void* data, std::size_t num) noexcept;

      /**
       * @brief       Start the serial receiver.
       * @param [out] data  Pointer to buffer for data to receive from USART receiver
       * @param [in] num   Number of data items to receive
       * @return      Execution status
       */
      return_t
      receive (void* data, std::size_t num) noexcept;

      /**
       * @brief       Start sending/receiving data to/from the serial transmitter/receiver.
       * @param [in] data_out  Pointer to buffer with data to send
       * @param [out] data_in   Pointer to buffer for data to receive
       * @param [in] num       Number of bytes to transfer
       * @return      Execution status
       */
      return_t
      transfer (const void* data_out, void* data_in, std::size_t num) noexcept;

      /**
       * @brief       Get transmitted bytes count.
       * @return      number of bytes transmitted
       */
      std::size_t
      get_tx_count (void) noexcept;

      /**
       * @brief       Get received bytes count.
       * @return      number of bytes received
       */
      std::size_t
      get_rx_count (void) noexcept;

      /**
       * @brief       Configure the serial interface.
       * @param [in] cfg  Operation
       * @param [in] arg      Argument of operation (optional)
       * @return      Common execution status and driver specific usart_execution_status
       */
      return_t
      configure (serial::config_t cfg, serial::config_arg_t arg) noexcept;

      /**
       * @brief       Control the serial interface.
       * @param [in] ctrl  Operation
       * @return      Common execution status and driver specific usart_execution_status
       */
      return_t
      control (serial::control_t ctrl) noexcept;

      /**
       * @brief       Get serial port status.
       * @return      Serial status.
       */
      serial::Status&
      get_status (void) noexcept;

      /**
       * @brief       Configure serial modem lines.
       * @param [in]   ctrl Control.
       * @return      Execution status.
       */
      return_t
      control_modem_line (serial::Modem_control ctrl) noexcept;

      /**
       * @brief       Get serial modem lines state.
       * @return      Modem status.
       */
      serial::Modem_status&
      get_modem_status (void) noexcept;

      /**
       * @brief       Signal serial events.
       * @param [in]  event Event notification mask.
       * @return      none
       */
      void
      signal_event (event_t event) noexcept;

      void
      clean (void) noexcept;

    protected:

      // ----- To be implemented by derived classes -----

      virtual const serial::Capabilities&
      do_get_capabilities (void) noexcept = 0;

      virtual return_t
      do_send (const void* data, std::size_t num) noexcept = 0;

      virtual return_t
      do_receive (void* data, std::size_t num) noexcept = 0;

      virtual return_t
      do_transfer (const void* data_out, void* data_in, std::size_t num)
          noexcept = 0;

      virtual std::size_t
      do_get_tx_count (void) noexcept = 0;

      virtual std::size_t
      do_get_rx_count (void) noexcept = 0;

      virtual return_t
      do_configure (serial::config_t cfg, serial::config_arg_t arg)
          noexcept = 0;

      virtual return_t
      do_control (serial::control_t ctrl) noexcept = 0;

      virtual serial::Status&
      do_get_status (void) noexcept = 0;

      virtual return_t
      do_control_modem_line (serial::Modem_control ctrl) noexcept = 0;

      virtual serial::Modem_status&
      do_get_modem_status (void) noexcept = 0;

    protected:

      /// Pointer to static function that implements the callback.
      signal_event_t cb_func_;

      /// Pointer to object instance associated with this driver.
      const void* cb_object_;

      serial::Status status_;
      serial::Modem_status modem_status_;

    };

#pragma GCC diagnostic pop

    // ----------------------------------------------------------------------
    // ----- Definitions -----

    namespace serial
    {
      // --------------------------------------------------------------------

      inline bool
      Status::is_tx_busy (void)
      {
        return tx_busy;
      }

      inline bool
      Status::is_rx_busy (void)
      {
        return rx_busy;
      }

      inline bool
      Status::is_tx_underflow (void)
      {
        return tx_underflow;
      }

      inline bool
      Status::is_rx_overflow (void)
      {
        return rx_overflow;
      }

      inline bool
      Status::is_rx_break (void)
      {
        return rx_break;
      }

      inline bool
      Status::is_rx_framing_error (void)
      {
        return rx_framing_error;
      }

      inline bool
      Status::is_rx_parity_error (void)
      {
        return rx_parity_error;
      }

      // --------------------------------------------------------------------

      inline bool
      Modem_status::is_cts_active (void)
      {
        return cts;
      }

      inline bool
      Modem_status::is_dsr_active (void)
      {
        return dsr;
      }

      inline bool
      Modem_status::is_dcd_active (void)
      {
        return dcd;
      }

      inline bool
      Modem_status::is_ri_active (void)
      {
        return ri;
      }
    } /* namespace serial */

    // ----------------------------------------------------------------------

    inline const serial::Capabilities&
    Serial::get_capabilities (void) noexcept
    {
      return do_get_capabilities ();
    }

    inline std::size_t
    Serial::get_tx_count (void) noexcept
    {
      return do_get_tx_count ();
    }

    inline std::size_t
    Serial::get_rx_count (void) noexcept
    {
      return do_get_rx_count ();
    }

    inline return_t
    Serial::configure (serial::config_t cfg, serial::config_arg_t arg) noexcept
    {
      return do_configure (cfg, arg);
    }

    inline return_t
    Serial::control (serial::control_t ctrl) noexcept
    {
      return do_control (ctrl);
    }

    inline serial::Status&
    Serial::get_status (void) noexcept
    {
      return do_get_status ();
    }

    inline return_t
    Serial::control_modem_line (serial::Modem_control ctrl) noexcept
    {
      return do_control_modem_line (ctrl);
    }

    inline serial::Modem_status&
    Serial::get_modem_status (void) noexcept
    {
      return do_get_modem_status ();
    }

    inline void /* __attribute__((always_inline)) */
    Serial::signal_event (event_t event) noexcept
    {
      if (cb_func_ != nullptr)
        {
          // Forward event to registered callback.
          cb_func_ (cb_object_, event);
        }
    }

  } /* namespace driver */
} /* namespace os */

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_DRIVERS_SERIAL_H_ */
