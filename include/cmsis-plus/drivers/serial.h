/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 * Copyright (c) 2013-2014 ARM Ltd.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
  namespace cmsis
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

        constexpr bit_number_t CONTROL_Pos = 0;
        constexpr config_t CONTROL_Msk (0xFFUL << CONTROL_Pos);

        // ----- Serial Control Codes: Mode -----
        ///< UART = (Asynchronous);; arg = Baudrate
        constexpr config_t MODE_ASYNCHRONOUS = (0x01UL << CONTROL_Pos);

        ///< Synchronous Master = (generates clock signal); arg = Baudrate
        constexpr config_t MODE_SYNCHRONOUS_MASTER = (0x02UL << CONTROL_Pos);

        ///< Synchronous Slave = (external clock signal);
        constexpr config_t MODE_SYNCHRONOUS_SLAVE = (0x03UL << CONTROL_Pos);

        ///< UART Single-wire = (half-duplex);; arg = Baudrate
        constexpr config_t MODE_SINGLE_WIRE = (0x04UL << CONTROL_Pos);

        ///< UART IrDA; arg = Baudrate
        constexpr config_t MODE_IRDA = (0x05UL << CONTROL_Pos);

        ///< UART Smart Card; arg = Baudrate
        constexpr config_t MODE_SMART_CARD = (0x06UL << CONTROL_Pos);

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
        constexpr bit_number_t FLOW_CONTROL_Pos = 16;
        constexpr config_t FLOW_CONTROL_Msk = (3UL << FLOW_CONTROL_Pos);

        ///< No Flow Control = (default);
        constexpr config_t FLOW_CONTROL_NONE = (0UL << FLOW_CONTROL_Pos);

        ///< RTS Flow Control
        constexpr config_t FLOW_CONTROL_RTS = (1UL << FLOW_CONTROL_Pos);

        ///< CTS Flow Control
        constexpr config_t FLOW_CONTROL_CTS = (2UL << FLOW_CONTROL_Pos);

        ///< RTS/CTS Flow Control
        constexpr config_t FLOW_CONTROL_RTS_CTS = (3UL << FLOW_CONTROL_Pos);

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
        constexpr config_t DEFAULT_TX_VALUE = (0x10UL << CONTROL_Pos);

        ///< Set IrDA Pulse in ns; arg: 0=3/16 of bit period
        constexpr config_t IRDA_PULSE = (0x11UL << CONTROL_Pos);

        ///< Set Smart Card Guard Time; arg = number of bit periods
        constexpr config_t SMART_CARD_GUARD_TIME = (0x12UL << CONTROL_Pos);

        ///< Set Smart Card Clock in Hz; arg: 0=Clock not generated
        constexpr config_t SMART_CARD_CLOCK = (0x13UL << CONTROL_Pos);

        ///< Smart Card NACK generation; arg: 0=disabled, 1=enabled
        constexpr config_t SMART_CARD_NACK = (0x14UL << CONTROL_Pos);

        // --------------------------------------------------------------------
        // ----- Commands -----

        ///< Enable Transmitter
        constexpr control_t ENABLE_TX = (0x15UL << CONTROL_Pos);

        ///< Enable Receiver
        constexpr control_t ENABLE_RX = (0x16UL << CONTROL_Pos);

        ///< Enable Continuous Break transmission
        constexpr control_t ENABLE_BREAK = (0x17UL << CONTROL_Pos);

        ///< Abort @ref Send
        constexpr control_t ABORT_SEND = (0x18UL << CONTROL_Pos);

        ///< Abort @ref Receive
        constexpr control_t ABORT_RECEIVE = (0x19UL << CONTROL_Pos);

        ///< Abort @ref Transfer
        constexpr control_t ABORT_TRANSFER = (0x1AUL << CONTROL_Pos);

        ///< Disable Transmitter
        constexpr control_t DISABLE_TX = (0x25UL << CONTROL_Pos);

        ///< Disable Receiver
        constexpr control_t DISABLE_RX = (0x26UL << CONTROL_Pos);

        ///< Disable Continuous Break transmission;
        constexpr control_t DISABLE_BREAK = (0x27UL << CONTROL_Pos);

        // --------------------------------------------------------------------
        // ****** Serial specific error codes *****

        ///< Specified Mode not supported
        constexpr status_t ERROR_MODE = ERROR_SPECIFIC - 1;

        ///< Specified baudrate not supported
        constexpr status_t ERROR_BAUDRATE = ERROR_SPECIFIC - 2;

        ///< Specified number of Data bits not supported
        constexpr status_t ERROR_DATA_BITS = ERROR_SPECIFIC - 3;

        ///< Specified Parity not supported
        constexpr status_t ERROR_PARITY = ERROR_SPECIFIC - 4;

        ///< Specified number of Stop bits not supported
        constexpr status_t ERROR_STOP_BITS = ERROR_SPECIFIC - 5;

        ///< Specified Flow Control not supported
        constexpr status_t ERROR_FLOW_CONTROL = ERROR_SPECIFIC - 6;

        ///< Specified Clock Polarity not supported
        constexpr status_t ERROR_CPOL = ERROR_SPECIFIC - 7;

        ///< Specified Clock Phase not supported
        constexpr status_t ERROR_CPHA = ERROR_SPECIFIC - 8;

        // ====================================================================
        // ***** Serial Status *****

        /**
         * @brief Serial port status
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
         * @brief Serial Modem Status
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
         * @brief Serial device driver capabilities.
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

          ///< Transmit completed event: @ref ARM_USART_EVENT_TX_COMPLETE
          bool event_tx_complete :1;

          ///< Signal receive character timeout event: @ref ARM_USART_EVENT_RX_TIMEOUT
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

          ///< Signal CTS change event: @ref ARM_USART_EVENT_CTS
          bool event_cts :1;

          ///< Signal DSR change event: @ref ARM_USART_EVENT_DSR
          bool event_dsr :1;

          ///< Signal DCD change event: @ref ARM_USART_EVENT_DCD
          bool event_dcd :1;

          ///< Signal RI change event: @ref ARM_USART_EVENT_RI
          bool event_ri :1;
        };

      } /* namespace serial */

      // ======================================================================

      // TODO: move get_version() and power() to base class.

      class Serial : public Base
      {

      public:

        // --------------------------------------------------------------------

        Serial () noexcept;

        virtual
        ~Serial () noexcept = default;

        // --------------------------------------------------------------------

        /**
         * @brief       Get driver capabilities
         * @return      @ref ARM_USART_CAPABILITIES
         */
        virtual const serial::Capabilities&
        get_capabilities (void) const noexcept = 0;

        /**
         * @brief       Register event callback.
         * @param[in]   cb_event  Pointer to @ref ARM_USART_SignalEvent
         * @return      @ref execution_status
         */
        void
        register_callback (signal_event_t cb_func, const void* cb_object =
                               nullptr) noexcept;

        /**
         * @brief       Start the serial transmitter.
         * @param[in]   data  Pointer to buffer with data to send to USART transmitter
         * @param[in]   num   Number of data items to send
         * @return      @ref execution_status
         */
        status_t
        send (const void* data, std::size_t num) noexcept;

        /**
         * @brief       Start the serial receiver.
         * @param[out]  data  Pointer to buffer for data to receive from USART receiver
         * @param[in]   num   Number of data items to receive
         * @return      @ref execution_status
         */
        status_t
        receive (void* data, std::size_t num) noexcept;

        /**
         * @brief       Start sending/receiving data to/from the serial transmitter/receiver.
         * @param[in]   data_out  Pointer to buffer with data to send to USART transmitter
         * @param[out]  data_in   Pointer to buffer for data to receive from USART receiver
         * @param[in]   num       Number of data items to transfer
         * @return      @ref execution_status
         */
        status_t
        transfer (const void* data_out, void* data_in, std::size_t num)
            noexcept;

        /**
         * @brief       Get transmitted data count.
         * @return      number of data items transmitted
         */
        virtual std::size_t
        get_tx_count (void) noexcept = 0;

        /**
         * @brief       Get received data count.
         * @return      number of data items received
         */
        virtual std::size_t
        get_rx_count (void) noexcept = 0;

        /**
         * @brief       Configure the serial interface.
         * @param[in]   control  Operation
         * @param[in]   arg      Argument of operation (optional)
         * @return      common @ref execution_status and driver specific @ref usart_execution_status
         */
        virtual status_t
        configure (serial::config_t cfg, serial::config_arg_t arg) noexcept = 0;

        /**
         * @brief       Control the serial interface.
         * @param[in]   control  Operation
         * @param[in]   arg      Argument of operation (optional)
         * @return      common @ref execution_status and driver specific @ref usart_execution_status
         */
        virtual status_t
        control (serial::control_t ctrl) noexcept = 0;

        /**
         * @brief       Get serial port status.
         * @return      USART status @ref ARM_USART_STATUS
         */
        virtual serial::Status&
        get_status (void) noexcept = 0;

        /**
         * @brief       Configure serial modem lines.
         * @param[in]   control  @ref ARM_USART_MODEM_CONTROL
         * @return      @ref execution_status
         */
        virtual status_t
        control_modem_line (serial::Modem_control ctrl) noexcept = 0;

        /**
         * @brief       Get serial modem lines state.
         * @return      modem status @ref ARM_USART_MODEM_STATUS
         */
        virtual serial::Modem_status&
        get_modem_status (void) noexcept = 0;

        /**
         * @brief       Signal serial events.
         * @param[in]   event  @ref USART_events notification mask
         * @return      none
         */
        void
        signal_event (event_t event) noexcept;

      protected:

        // ----- To be implemented by derived classes -----

        virtual status_t
        do_send (const void* data, std::size_t num) noexcept = 0;

        virtual status_t
        do_receive (void* data, std::size_t num) noexcept = 0;

        virtual status_t
        do_transfer (const void* data_out, void* data_in, std::size_t num)
            noexcept = 0;

      protected:

        /// Pointer to static function that implements the callback.
        signal_event_t cb_func_;

        /// Pointer to object instance associated with this driver.
        const void* cb_object_;

      };

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

      // ----------------------------------------------------------------------
      } /* namespace serial */

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_DRIVERS_SERIAL_H_ */
