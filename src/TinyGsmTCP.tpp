/**
 * @file       TinyGsmTCP.tpp
 * @brief      TCP socket helper mixin and socket trait presets.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMTCP_TPP_
#define SRC_TINYGSMTCP_TPP_

#include "TinyGsmCommon.h"

#ifndef TINY_GSM_MODEM_HAS_TCP
/// flag to indicate that the modem has TCP functions
#define TINY_GSM_MODEM_HAS_TCP
#endif

#include "TinyGsmFifo.h"

/// Supported receive buffer handling modes for TCP clients.
enum class TinyGsmTcpBufferMode {
  /// the modem does not have a buffer for incoming data
  NoModemBuffer,
  /// the modem does have a buffer for incoming data but there is no way to
  /// check how much data is in the buffer
  BufferReadNoCheck,
  /// the modem does have a buffer for incoming data and there is a way to check
  /// how much data is in the buffer
  BufferReadAndCheckSize,
};

/// Supported socket multiplexing assignment modes.
enum class TinyGsmTcpMuxMode {
  /// the modem will always use the user-specified MUX channel numbers for TCP
  /// connections
  Static,
  /// the modem can and will dynamically assign MUX channels for TCP connections
  Dynamic,
};

/**
 * @brief Template class for modem TCP config traits.
 *
 * @tparam bufferMode How the modem exposes its receive buffer.
 * @tparam muxMode Whether the caller or the modem assigns mux channels.
 * @tparam muxCount The number of simultaneous TCP channels.
 * @tparam sendMaxSize The maximum number of bytes in one send command.
 * @tparam connectTimeoutS The connection timeout, in seconds.
 * @tparam stopTimeoutS The clean-disconnect timeout, in seconds.
 * @tparam minFreeTxBuffer The minimum free send-buffer space, in bytes,
 * required before a send is attempted.
 */
template <TinyGsmTcpBufferMode bufferMode, TinyGsmTcpMuxMode muxMode,
          uint8_t muxCount, size_t sendMaxSize = 1500, int connectTimeoutS = 75,
          int stopTimeoutS = 15, size_t minFreeTxBuffer = 1>
struct TinyGsmTcpConfigPreset {
  /// The buffer mode for the modem's TCP receive buffer.
  static constexpr TinyGsmTcpBufferMode kBufferMode = bufferMode;
  /// The multiplexing mode for the modem's TCP connections.
  static constexpr TinyGsmTcpMuxMode kMuxMode = muxMode;

  /// The number of simultaneous TCP channels supported by the modem.
  static constexpr uint8_t kMuxCount = muxCount;
  /// The timeout in seconds to wait for a connection to be established.
  static constexpr int kConnectTimeoutS = connectTimeoutS;
  /// The timeout in seconds to wait for a connection to disconnect cleanly.
  static constexpr int kStopTimeoutS = stopTimeoutS;
  /// The maximum size of data that can be sent in a single transmission.
  static constexpr size_t kSendMaxSize = sendMaxSize;
  /**
   * @brief This is the minimum amount of free send buffer space the modem must
   * report before attempting a send.
   *
   * If the minimum amount of free space in the Tx is not defined, we assume
   * it's 1 byte - ie, there only needs to be a non-zero amount of space.
   *
   * Some modules (SIM7080G) will freeze or crash if you pummel them with data
   * when the send buffer isn't empty.
   */
  static constexpr size_t kMinFreeTxBuffer = minFreeTxBuffer;
};

// Forward declaration of the GSM Client class so it can be friended.
template <class modemType, class tcpConfig>
class GsmClient;

/**
 * @brief The CRTP parent class for TCP functions of the modem.
 * @tparam modemType The derived modem class
 * @tparam tcpConfig Trait type controlling TCP behavior and limits.
 */
template <class modemType, class tcpConfig>
class TinyGsmTCP {
  using TcpConfig = tcpConfig;
  // Grant the paired GsmClient access to the protected modem-command helpers
  // below (mirrors the access GsmClient used to get automatically when it was
  // a nested class of this mixin).
  friend class GsmClient<modemType, tcpConfig>;

 public:
  /// Compile-time capability flag indicating TCP socket support
  static constexpr bool hasTCP = true;

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the interface
   */
 public:
  /**
   * @anchor tcp_functions
   * @name TCP functions
   */
  /**@{*/
  /// Maintain the modem connection and check for incoming data.
  void maintain() {
    return thisModem().maintainImpl();
  }

 protected:
  template <typename T>
  bool isValidMux(T mux) {
    return mux >= 0 && mux < TcpConfig::kMuxCount &&
        thisModem().sockets[mux] != nullptr;
  }
  template <typename T>
  bool isExpectedMux(T query_mux, uint8_t known_mux) {
    return query_mux >= 0 && query_mux == static_cast<T>(known_mux);
  }

  /**
   * @brief Find the number of the first unassigned mux socket
   * @return The mux number of the first unassigned socket, or 255 (0xFF,
   * static_cast<uint8_t>(-1)) if all sockets are assigned
   * @note This returns an unsigned int instead of a signed int to be easily
   * comparable with the socket mux value, which is also unsigned.
   */
  uint8_t findFirstUnassignedMux() {
    // Try to iterate through the assigned client sockets to find the next spot
    // in the array of client pointers that has not been linked to an object.
    for (int next_mux = 0; next_mux < TcpConfig::kMuxCount; next_mux++) {
      if (thisModem().sockets[next_mux] == nullptr) { return next_mux; }
    }
    DBG("### WARNING: No empty mux sockets found!");
    return static_cast<uint8_t>(-1);
  }

  /**
   * @brief Move a client from one mux socket to another.
   *
   * This function will check if the requested mux socket is valid and not
   * already in use. If it is invalid or in use, it will find the next available
   * mux socket and put the pointer to the client at the old mux socket into
   * that position, moving the client there instead. The assignedMux parameter
   * can be used to retrieve the actual mux socket number that was assigned to
   * the client after the move.  When moving a client, the internal mux number
   * of the moved client will be updated to reflect the new mux socket number.
   * After moving a socket, the pointer in the array at the old mux position
   * will be set to nullptr.
   *
   * @param oldMux The zero-indexed socket number to move from.
   * @param requestedMux The zero-indexed socket number to move to. Use
   * static_cast<uint8_t>(-1) [255] to find and move to the next available mux
   * socket.
   * @param assignedMux Optional pointer to a variable to receive the actual
   * zero-indexed socket number that was assigned to the client after the move.
   * @return true if the move was successful, false otherwise.
   */
  bool moveSocket(uint8_t oldMux, uint8_t requestedMux,
                  uint8_t* assignedMux = nullptr) {
    if (!(oldMux < TcpConfig::kMuxCount)) {
      DBG(GF("ERROR: Cannot move from an invalid socket:"), oldMux);
      return false;
    }
    if (oldMux == requestedMux || thisModem().sockets[oldMux] == nullptr) {
      DBG(GF("INFO: Nothing to move for socket:"), oldMux);
      if (assignedMux) { *assignedMux = oldMux; }
      return true;  // Nothing to do, but not an error
    }

    uint8_t destination_mux = requestedMux;
    if (!(requestedMux < TcpConfig::kMuxCount) ||
        thisModem().sockets[requestedMux] != nullptr) {
      DBG(GF("Warning: The requested mux number ("), requestedMux,
          GF(") is invalid or already in use, moving to the next empty socket "
             "instead."));

      uint8_t next_empty_mux = findFirstUnassignedMux();
      if (next_empty_mux == static_cast<uint8_t>(-1)) {
        DBG(GF("ERROR: No empty mux sockets available!"));
        return false;
      }

      destination_mux = next_empty_mux;
    }

    // if the requested mux is valid and not in use or we found empty mux
    // socket, move the client pointer to that socket, reset the internal mux
    // number of the moved socket to its destination, set the value of the
    // assignedMux pointer to the new mux number, and return true
    thisModem().sockets[destination_mux]      = thisModem().sockets[oldMux];
    thisModem().sockets[destination_mux]->mux = destination_mux;
    thisModem().sockets[oldMux]               = nullptr;
    if (assignedMux) { *assignedMux = destination_mux; }
    return true;
  }

  /**
   * @brief Convert a multiplexing channel number to the modem's internal
   * connection identifier.
   * @param mux The multiplexing channel number
   * @return The modem's internal connection identifier
   */
  inline uint8_t muxToConnectionId(uint8_t mux) {
    return mux;
  };
  /**
   * @brief Convert the modem's internal connection identifier to a multiplexing
   * channel number.
   * @param connId The modem's internal connection identifier
   * @return The multiplexing channel number
   */
  inline uint8_t connectionIdToMux(uint8_t connId) {
    return connId;
  };

  bool modemConnect(const char* host, uint16_t port, uint8_t staticMux,
                    int timeout_s = TcpConfig::kConnectTimeoutS) {
    return thisModem().modemConnectImpl(host, port, staticMux, timeout_s);
  }

  bool modemConnect(const char* host, uint16_t port, uint8_t* dynamicMux,
                    int timeout_s = TcpConfig::kConnectTimeoutS) {
    return thisModem().modemConnectImpl(host, port, dynamicMux, timeout_s);
  }

  bool modemStop(uint8_t mux, uint32_t maxWaitMs) {
    return thisModem().modemStopImpl(mux, maxWaitMs);
  }

  /**
   * @brief Sends a buffer of data to the modem
   *
   * By default this breaks the data into chunks of size
   * TcpConfig::kSendMaxSize. Then for each chunk it calls modemWaitForSend
   * (which calls modemGetSendLength), then modemBeginSend, then writes the
   * buffer content, then calls modemEndSend.
   *
   * @param buff The buffer of data to send
   * @param len The length of the buffer
   * @param mux The **zero-indexed** position of the client in the modem's
   * socket array.
   * @return The number of bytes sent
   */
  size_t modemSend(const uint8_t* buff, size_t len, uint8_t mux) {
    return thisModem().modemSendImpl(buff, len, mux);
  }
  // Initiates the AT commands for a send, up to the point of getting an input
  // prompt
  bool modemBeginSend(size_t len, uint8_t mux) {
    return thisModem().modemBeginSendImpl(len, mux);
  }
  // Finishes off the modem send, checking for a response from the modem
  // This is for everything after the input prompt
  size_t modemEndSend(size_t len, uint8_t mux) {
    return thisModem().modemEndSendImpl(len, mux);
  }
  // check for the amount of space left in the send buffer
  size_t modemGetSendLength(uint8_t mux) {
    return thisModem().modemGetSendLengthImpl(mux);
  }
  // wait until the modem has more than the minimum required send buffer space
  // available
  // returns the number of bytes available in the send buffer at the end of the
  // wait
  size_t modemWaitForSend(uint8_t mux, uint32_t timeout_ms = 15000L) {
    return thisModem().modemWaitForSendImpl(mux, timeout_ms);
  }
  size_t modemRead(size_t size, uint8_t mux) {
    return thisModem().modemReadImpl(size, mux);
  }

  size_t modemGetAvailable(uint8_t mux) {
    return thisModem().modemGetAvailableImpl(mux);
  }
  bool modemGetConnected(uint8_t mux) {
    return thisModem().modemGetConnectedImpl(mux);
  }
  /**@}*/

  // destructor (protected!)
  ~TinyGsmTCP() {}

  /*
   * CRTP Helper
   */
  inline const modemType& thisModem() const {
    return static_cast<const modemType&>(*this);
  }
  inline modemType& thisModem() {
    return static_cast<modemType&>(*this);
  }

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the default function implementations
   */
 protected:
  /*
   * TCP functions
   */

  void maintainImpl() {
    if (TcpConfig::kBufferMode ==
        TinyGsmTcpBufferMode::BufferReadAndCheckSize) {
      // Keep listening for modem URC's and proactively iterate through
      // sockets asking if any data is available
      for (uint8_t mux = 0; mux < TcpConfig::kMuxCount; mux++) {
        GsmClient<modemType, tcpConfig>* sock = thisModem().sockets[mux];
        if (sock && sock->got_data && sock->sock_available == 0) {
          sock->got_data       = false;
          sock->sock_available = thisModem().modemGetAvailable(mux);
        }
      }
      while (thisModem().stream.available()) {
        thisModem().waitResponse(15, nullptr, nullptr);
      }
    } else {
      // Just listen for any URC's
      thisModem().waitResponse(100, nullptr, nullptr);
    }
  }

  // Yields up to a time-out period and then reads a single character from the
  // stream into the mux FIFO
  inline bool moveCharFromStreamToFifo(uint8_t mux) {
    if (!thisModem().sockets[mux]) { return false; }
    uint32_t startMillis = millis();
    while (!thisModem().stream.available() &&
           (millis() - startMillis < thisModem().sockets[mux]->_timeout)) {
      TINY_GSM_YIELD();
    }
    if (thisModem().stream.available()) {
      char c = thisModem().stream.read();
      thisModem().sockets[mux]->rx.put(c);
      return true;
    } else {
      return false;
    }
  }

  /**
   * @brief Move a block of characters from the stream to the mux FIFO.
   *
   * Consumes the number of characters reported by the modem and places as many
   * of them as possible into the FIFO of the specified mux socket. If the
   * modem reports more characters than can currently fit in the FIFO, the
   * excess characters are consumed from the stream and discarded so that the
   * modem stream remains synchronized.
   *
   * If the modem is configured to use hex mode (i.e., compiled with
   * `TINY_GSM_USE_HEX`), each pair of received hexadecimal characters is
   * converted into one byte before being placed into the FIFO.
   *
   * Data already available on the stream is transferred in blocks. If
   * insufficient data is available, the function waits for more data, up to
   * the timeout period of the socket. A maximum of three timeout events are
   * tolerated before the read is abandoned.
   *
   * @note In hex mode, a partial hex pair is never consumed.
   *
   * @note `expected_len` is the number of decoded characters reported by the
   *       modem, not necessarily the number that will fit in the FIFO.
   *
   * @note If the reported length exceeds the available FIFO space, excess
   *       data is consumed and discarded. The return value reports only the
   *       number of characters actually placed into the FIFO.
   *
   * @param mux The **zero-indexed** position in the modem's socket array of the
   * client whose FIFO the characters will be placed in.
   * @param expected_len The number of decoded characters reported by the modem.
   *                     A negative value indicates an invalid/error length.
   * @return The number of decoded characters actually placed into the FIFO.
   */
  size_t moveCharsFromStreamToFifo(uint8_t mux, int16_t expected_len) {
    if (!thisModem().sockets[mux] || expected_len <= 0) { return 0; }

    uint32_t startMillis   = millis();
    size_t   len           = static_cast<size_t>(expected_len);
    size_t   len_read      = 0;
    uint8_t  char_failures = 0;
    uint8_t  buf[32];

#ifdef TINY_GSM_USE_HEX
    // DBG("### Reading input in HEX mode");
    constexpr size_t readCharLen    = 2;
    char             hanging_nibble = '\0';
    // ^^ Used to store a single nibble if we get an odd number of hex
    // characters from the stream
#else
    // DBG("### Reading input in ASCII mode");
    constexpr size_t readCharLen = 1;
#endif

    // allow up to 3 timeouts on individual characters before we quit the whole
    // read operation
    while (len && char_failures < 3) {
      size_t available = thisModem().stream.available();

      if (available >= readCharLen) {
        size_t count = len;
        // don't read more than the size of the temporary buffer
        if (count > sizeof(buf) / readCharLen) {
          count = sizeof(buf) / readCharLen;
        }
        // don't read more than the number of characters available in the stream
        if (count > available / readCharLen) {
          count = available / readCharLen;
        }

        count *= readCharLen;
        // if there's nothing to read, move on
        if (!count) { continue; }

#ifdef TINY_GSM_USE_HEX
        // Never consume a partial hex pair.
        count &= ~static_cast<size_t>(1);
#endif

        size_t bytesRead = thisModem().stream.readBytes(buf, count);

#ifdef TINY_GSM_USE_HEX
        // Detect short reads with odd byte count that would break hex alignment
        if ((bytesRead + (hanging_nibble != '\0' ? 1 : 0)) & 1) {
          // truncate to even
          bytesRead &= ~static_cast<size_t>(1);
          // save the hanging nibble into the buffer for the next read
          hanging_nibble = buf[bytesRead];
        } else {
          hanging_nibble = '\0';
        }
#endif

        bytesRead -= bytesRead % readCharLen;

#ifdef TINY_GSM_USE_HEX
        for (size_t i = 0; i < bytesRead; i += 2) {
          uint8_t c = '\0';
          uint8_t d = '\0';
          if (hanging_nibble != '\0' && i == 0) {
            // If we have a hanging nibble from the previous read, use it as the
            // first nibble of this pair.
            c = hanging_nibble;
            d = buf[i];
          } else {
            c = buf[i];
            d = buf[i + 1];
          }

          c = (c <= '9') ? c - '0' : (c & 0x0F) + 9;
          d = (d <= '9') ? d - '0' : (d & 0x0F) + 9;

          buf[i >> 1] = (c << 4) | d;

          // if we used a hanging nibble, we need to step i back by 1 so that
          // the next iteration uses the correct index
          if (hanging_nibble != '\0' && i == 0) { i -= 1; }
        }

        bytesRead >>= 1;
#endif

        // NOTE: We can't directly memcpy into the rx fifo!
        // The fifo is a template class that can hold any data type and the
        // actual memory space of the buffer is protected.
        if (bytesRead) {
          size_t fifo_free = thisModem().sockets[mux]->rx.free();
          size_t to_store  = bytesRead;

          if (to_store > fifo_free) { to_store = fifo_free; }

          if (to_store) {
            int added = thisModem().sockets[mux]->rx.put(
                buf, static_cast<int>(to_store));

            if (added > 0) { len_read += static_cast<size_t>(added); }

            if (added != static_cast<int>(bytesRead)) {
              DBG("### FIFO buffer overflow: ", bytesRead, "->", added);
            }
          }

          // Consume the entire reported payload, even when the FIFO is full.
          len -= bytesRead;
        }

        continue;
      }

      // Wait for at least one complete character (or HEX pair).
      while (static_cast<size_t>(thisModem().stream.available()) <
                 readCharLen &&
             (millis() - startMillis < thisModem().sockets[mux]->_timeout)) {
        TINY_GSM_YIELD();
      }

      if (static_cast<size_t>(thisModem().stream.available()) < readCharLen) {
        DBG("### ERROR: Timed out waiting for character from stream!");
        ++char_failures;
      }
    }

    if (len_read) { DBG("### READ:", len_read, "from", mux); }

    if (len) {
      DBG("\n### Different number of characters received than expected: ",
          len_read, "read vs ", expected_len, "expected");
    }

    return len_read;
  }

  bool modemConnectImpl(const char* host, uint16_t port, uint8_t /*static*/ mux,
                        int timeout_s) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  bool modemConnectImpl(const char* host, uint16_t port, uint8_t* dynamicMux,
                        int timeout_s) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  bool modemStopImpl(uint8_t  mux,
                     uint32_t maxWaitMs) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  size_t modemSendImpl(const uint8_t* buff, size_t len, uint8_t mux) {
    // Pointer to where in the buffer we're up to
    // A const cast is need to cast-away the constant-ness of the buffer (ie,
    // modify it).
    uint8_t* txPtr     = const_cast<uint8_t*>(buff);
    size_t   bytesSent = 0;

    do {
      // make no more than 3 attempts at the single send command
      int8_t send_attempts = 0;
      bool   send_success  = false;
      while (send_attempts < 3 && !send_success) {
        size_t sendLength = thisModem().modemWaitForSend(mux);
        if (sendLength < TcpConfig::kMinFreeTxBuffer) {
          send_attempts++;
          DBG(GF("### Insufficient send buffer ("), sendLength,
              GF("of required"), TcpConfig::kMinFreeTxBuffer,
              GF(") on attempt"), send_attempts);
          continue;
        }
        // Ensure the program doesn't read past the allocated memory
        if (txPtr + sendLength > const_cast<uint8_t*>(buff) + len) {
          sendLength = const_cast<uint8_t*>(buff) + len - txPtr;
        }
        // start up a send command
        send_success = thisModem().modemBeginSend(sendLength, mux);
        if (!send_success) {
          send_attempts++;
          DBG(GF("### Failed to start send command on attempt"), send_attempts);
          continue;
        }
        // write out the number of bytes for this chunk
        int16_t attempted = thisModem().stream.write(
            reinterpret_cast<const uint8_t*>(txPtr), sendLength);
        // let the transfer finish
        thisModem().stream.flush();
        // End this send command and check its responses
        // NOTE: In many cases, confirmed is just a passthrough of sendLength
        int16_t confirmed = thisModem().modemEndSend(sendLength, mux);
#if defined(TINY_GSM_DEBUG)
        if (confirmed < attempted) {
          DBG(GF("### Fewer bytes were confirmed ("), confirmed,
              F(") than attempted ("), attempted, F(") on send attempt"),
              send_attempts);
        }
#endif
        // A modem implementation can report a negative value on failure.
        // Never move the pointer or the counter backwards.
        int16_t accepted = TinyGsmMin(attempted, confirmed);
        if (accepted < 0) { accepted = 0; }
        bytesSent += accepted;  // bump up number of bytes sent
        txPtr += accepted;      // bump up the pointer
        send_success &= accepted > 0;
        send_attempts++;
      }
      // if we failed after 3 attempts at the same chunk, bail from the whole
      // thing
      if (!send_success) { break; }
    } while (bytesSent < len && thisModem().sockets[mux]->sock_connected);
    return bytesSent;
  }

  bool   modemBeginSendImpl(size_t  len,
                            uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  size_t modemEndSendImpl(size_t  len,
                          uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  size_t modemGetSendLengthImpl(uint8_t) {
    // by default, assume the whole space is available
    return TcpConfig::kSendMaxSize;
  }

  size_t modemWaitForSendImpl(uint8_t mux, uint32_t timeout_ms) {
    size_t sendLength = thisModem().modemGetSendLength(mux);
#if defined(TINY_GSM_DEBUG)
    if (sendLength != TcpConfig::kSendMaxSize) {
      DBG(GF("### Full send buffer not available! Expected it to have"),
          TcpConfig::kSendMaxSize, GF("bytes, but it has"), sendLength);
    }
    if (sendLength < TcpConfig::kMinFreeTxBuffer) {
      DBG(GF("### Waiting up to"), timeout_ms,
          GF("ms for sufficient available send buffer space"));
    }
#endif
    uint32_t start = millis();
    while (sendLength < TcpConfig::kMinFreeTxBuffer &&
           millis() - start < timeout_ms &&
           thisModem().sockets[mux]->sock_connected) {
      delay(250);
      sendLength = thisModem().modemGetSendLength(mux);
#if defined(TINY_GSM_DEBUG)
      if (sendLength >= TcpConfig::kMinFreeTxBuffer) {
        DBG(GF("### Send buffer has"), sendLength, GF("available after"),
            millis() - start, GF("ms"));
      }
#endif
    }
#if defined(TINY_GSM_DEBUG)
    if (sendLength == 0) { DBG(GF("### No available send buffer!")); }
#endif
    return sendLength;
  }

  size_t modemReadImpl(size_t size, uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  size_t modemGetAvailableImpl(uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  bool modemGetConnectedImpl(uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;
};

/**
 * @brief The TCP client class.
 *
 * @note This is a base class for TCP clients, but it is NOT an inner class
 * of the TinyGsmTCP class.
 *
 * @tparam modemType The derived modem class
 * @tparam tcpConfig Trait type controlling TCP behavior and limits.
 *
 * @todo In `GsmClient::connect()`, confirm that we can trust sock_connected at
 * the start of a new connection before calling stop or if we should call stop
 * regardless for static mux assignment.
 */
template <class modemType, class tcpConfig>
class GsmClient : public Client {
  using TcpConfig = tcpConfig;
  // Make all classes created from the modem template friends
  friend class TinyGsmTCP<modemType, tcpConfig>;

 public:
  // Restore the Print overloads hidden by the write() declarations below,
  // in particular Print::write(const char*, size_t).
  using Print::write;

  /**
   * @anchor client_like_functions
   * @name Functions implementing the Arduino Client interface
   */
  /**@{*/

  /**
   * @brief Create a new TCP client.
   * @warning You must call the init() method before attempting to use a
   * client created with this constructor.
   */
  GsmClient() {
    is_secure = false;
  }

  /**
   * @brief Create a new TCP client and bind it to a modem and optionally a
   * multiplexing channel.
   * @param modem Modem instance used by this client.
   * @param mux The **zero-indexed** position of this client in the
   * corresponding modem's socket array.  For most modules, this is identical to
   * the identifier the modem uses internally to identify the socket, but some
   * modules (e.g., Sequans Monarch) use a 1-indexed identifier for the socket,
   * so the mux number is not necessarily the same as the modem's internal
   * socket identifier.
   */
  explicit GsmClient(modemType& modem, uint8_t mux = 0) : at(&modem), mux(mux) {
    is_secure = false;
  }

  /**
   * @brief Initialize this client with modem context and multiplexing channel.
   *
   * @param modem Pointer to the modem instance.
   * @param mux The **zero-indexed** position of this client in the
   * corresponding modem's socket array.  For most modules, this is identical to
   * the identifier the modem uses internally to identify the socket, but some
   * modules (e.g., Sequans Monarch) use a 1-indexed identifier for the socket,
   * so the mux number is not necessarily the same as the modem's internal
   * socket identifier.
   * @return true if initialization completed.
   */
  virtual bool init(modemType* modem, uint8_t mux) = 0;

  /**
   * @brief Connect to a server using a host name and port number, with a
   * specified timeout.
   *
   * @param host The host name of the server to connect to.
   * @param port The port number to connect to on the server.
   * @param timeout_s The timeout for the connection attempt, in seconds.
   * @return 1 if the connection was successful, 0 otherwise.
   *
   * @remark Every time you call the connect() function, it will stop the socket
   * if there was one and it was connected. It will also clear the receive
   * buffer before connecting.
   */
  virtual int connect(const char* host, uint16_t port, int timeout_s) = 0;

#define TINY_GSM_STATIC_TCP_CONNECT                                       \
  int connect(const char* host, uint16_t port, int timeout_s) override {  \
    if (at == nullptr) { return 0; }                                      \
    is_mid_send = false;                                                  \
    /*free the socket if there was one and it was connected*/             \
    if (mux < TcpConfig::kMuxCount && at->sockets[mux] != nullptr &&      \
        sock_connected) {                                                 \
      stop(TcpConfig::kStopTimeoutS * 1000L);                             \
    }                                                                     \
    /* always clear the rx buffer before connecting */                    \
    rx.clear();                                                           \
    TINY_GSM_YIELD();                                                     \
    /*connect at the specified mux number, which is assumed to be valid*/ \
    sock_connected = at->modemConnect(host, port, mux, timeout_s);        \
    return sock_connected;                                                \
  }

  /**
   * @brief Connect to a server using an IPAddress and port number, with a
   * specified timeout.
   *
   * The default implementation of this function converts the IPAddress to a
   * string and calls the connect(const char* host, uint16_t port, int
   * timeout_s) function.
   *
   * @param ip The IP address of the server to connect to.
   * @param port The port number to connect to on the server.
   * @param timeout_s The timeout for the connection attempt, in seconds.
   * @return 1 if the connection was successful, 0 otherwise.
   */
  virtual int connect(IPAddress ip, uint16_t port, int timeout_s) {
    return connect(TinyGsmStringFromIp(ip).c_str(), port, timeout_s);
  }
  /**
   * @brief Connect to a server using a host name and port number
   * @param host The host name of the server to connect to.
   * @param port The port number to connect to on the server.
   * @return 1 if the connection was successful, 0 otherwise.
   */
  int connect(const char* host, uint16_t port) override {
    return connect(host, port, TcpConfig::kConnectTimeoutS);
  }
  /**
   * @brief Connect to a server using an IPAddress and port number
   * @param ip The IP address of the server to connect to.
   * @param port The port number to connect to on the server.
   * @return 1 if the connection was successful, 0 otherwise.
   */
  int connect(IPAddress ip, uint16_t port) override {
    return connect(ip, port, TcpConfig::kConnectTimeoutS);
  }

  /**
   * @brief Close the client connection, with a specified maximum wait time
   * for the operation.
   *
   * If there is modem remaining in the modem buffer before the connection is
   * closed, it will be dumped and lost.
   *
   * @param maxWaitMs The maximum time to wait for the connection to close,
   * in milliseconds.
   *
   * @note The max wait time is the time to give the modem to close the
   * connection cleanly. If there is modem remaining in the modem buffer before
   * the connection is closed, the total time before this function returns may
   * be longer than the max wait time, as dumping the modem buffer may take
   * additional time.
   */
  virtual void stop(uint32_t maxWaitMs) {
    if (at == nullptr) { return; }
    is_mid_send          = false;
    uint32_t startMillis = millis();
    // Throw away any remaining data in the modem buffer.
    // We explicitly toss it here because the socket will appear open in
    // response to connected() even after it closes until all data is read
    // to give the user a chance to recover the data if they want it.
    // Dumping the modem buffer will also clear the rx fifo.
    dumpModemBuffer(maxWaitMs);
    uint32_t elapsed = millis() - startMillis;
    // NOTE: Always give the modem at least 1 second to close the connection,
    // even if the maxWaitMs has already elapsed.
    uint32_t remainingWait = (elapsed >= maxWaitMs) ? 1000L
                                                    : (maxWaitMs - elapsed);
    at->modemStop(mux, remainingWait);
    // Mark the socket disconnected
    // Should we check the return of modemStop and only set sock_connected to
    // false if it was successful?  I suspect we should error on the side of
    // caution and assume that if we called stop, the socket is no longer
    // connected, even if the modem didn't report it cleanly.
    sock_connected = false;
  }
  /**
   * @brief Close the client connection, with a default maximum wait time
   */
  void stop() override {
    stop(TcpConfig::kStopTimeoutS * 1000L);
  }

  /**
   * @brief Writes data out on the client using the modem send functionality
   * @param buf The buffer of data to send
   * @param size The size of the buffer
   * @return The number of bytes written
   */
  size_t write(const uint8_t* buf, size_t size) override {
    if (at == nullptr) { return 0; }
    if (is_mid_send) {
      // if we're in the middle of a write, pass directly to the stream
      return at->stream.write(buf, size);
    }
    // Validate mux before entering the send path
    if (mux >= TcpConfig::kMuxCount || at->sockets[mux] == nullptr) {
      return 0;
    }
    TINY_GSM_YIELD();
    at->maintain();
    // If the modem is one where we can read and check the size of the buffer,
    // then the 'available()' function will call a check of the current size
    // of the buffer and state of the connection. [available calls maintain,
    // maintain calls modemGetAvailable, modemGetAvailable calls
    // modemGetConnected]  This cascade means that the sock_connected value
    // should be correct and we can trust it if it says we're not connected to
    // send.
    if (TcpConfig::kBufferMode ==
            TinyGsmTcpBufferMode::BufferReadAndCheckSize &&
        !sock_connected) {
      return 0;
    }
    return at->modemSend(buf, size, mux);
  }

  /**
   * @brief Writes a single byte of data to the modem for sending
   * @param c The byte of data to send
   * @return The number of bytes written
   *
   * @warning This function is not efficient for sending large amounts of
   * data. Use the write(const uint8_t* buf, size_t size) or write(const
   * char* str) function instead.
   */
  size_t write(uint8_t c) override {
    return write(&c, 1);
  }

  /**
   * @brief Get the number of bytes available for in the client's receive
   * buffer.
   * This returns the combined total of the number of bytes available
   * in the TinyGSM fifo and the modem chip's internal fifo (where supported).
   * @return int The number of bytes available in the client's receive buffer.
   */
  int available() override {
    if (at == nullptr) { return 0; }
    is_mid_send = false;  // Any calls to the AT when mid-send will cause the
                          // send to fail
    TINY_GSM_YIELD();
    // Returns the number of characters available in the TinyGSM fifo
    if (TcpConfig::kBufferMode == TinyGsmTcpBufferMode::NoModemBuffer) {
      if (!rx.size() && sock_connected) { at->maintain(); }
      return rx.size();
    }

    // Returns the combined number of characters available in the TinyGSM
    // fifo and the modem chips internal fifo.
    if (TcpConfig::kBufferMode == TinyGsmTcpBufferMode::BufferReadNoCheck) {
      if (!rx.size()) { at->maintain(); }
      return static_cast<uint16_t>(rx.size()) + sock_available;
    }

    // Returns the combined number of characters available in the TinyGSM
    // fifo and the modem chips internal fifo, doing an extra check-in
    // with the modem to see if anything has arrived without a URC.
    if (!rx.size()) {
      if (millis() - prev_check > TINY_GSM_UNREAD_CHECK_MS) {
        // setting got_data to true will tell maintain to run
        // modemGetAvailable(mux)
        got_data   = true;
        prev_check = millis();
      }
      at->maintain();
    }
    return static_cast<uint16_t>(rx.size()) + sock_available;
  }

  /**
   * @brief Read data from the client's receive buffer into a user provided
   * buffer.
   *
   * @param buf The buffer to read data into.
   * @param size The maximum number of bytes to read.
   * @return int The number of bytes actually read.
   */
  int read(uint8_t* buf, size_t size) override {
    if (at == nullptr) { return -1; }
    TINY_GSM_YIELD();
    is_mid_send = false;  // Any calls to the AT when mid-send will cause the
                          // send to fail
    size_t cnt  = 0;

    if (TcpConfig::kBufferMode == TinyGsmTcpBufferMode::NoModemBuffer) {
      // Reads characters out of the TinyGSM fifo, waiting for any URC's
      // from the modem for new data if there's nothing in the fifo.
      uint32_t _startMillis = millis();
      while (cnt < size && millis() - _startMillis < _timeout) {
        // Read out of the TinyGSM fifo
        size_t chunk = TinyGsmMin(size - cnt, rx.size());
        if (chunk > 0) {
          rx.get(buf, chunk);
          buf += chunk;
          cnt += chunk;
          continue;
        }
        // continue to parse URCs from the modem stream until the timeout
        if (!rx.size()) {
          if (!sock_connected) { break; }
          at->maintain();
        }
      }
      return cnt;
    }

    if (TcpConfig::kBufferMode == TinyGsmTcpBufferMode::BufferReadNoCheck) {
      // Reads characters out of the TinyGSM fifo, and from the modem chip's
      // internal fifo if available.
      while (cnt < size) {
        // Read out of the TinyGSM fifo
        size_t chunk = TinyGsmMin(size - cnt, rx.size());
        if (chunk > 0) {
          rx.get(buf, chunk);
          buf += chunk;
          cnt += chunk;
          continue;
        }
        at->maintain();  // clear the modem stream/parse URCs
        // Refill the TinyGSM fifo from the modem's internal buffer
        // TODO: Read directly from modem into user buffer, skipping FIFO
        if (sock_available > 0) {
          int n = at->modemRead(TinyGsmMin((uint16_t)rx.free(), sock_available),
                                mux);
          if (n == 0) break;
        } else {
          break;
        }
      }
      return cnt;
    }

    // Reads characters out of the TinyGSM fifo, and from the modem chips
    // internal fifo if available, also double checking with the modem if
    // data has arrived without issuing a URC.
    while (cnt < size) {
      // Read out of the TinyGSM fifo
      size_t chunk = TinyGsmMin(size - cnt, rx.size());
      if (chunk > 0) {
        rx.get(buf, chunk);
        buf += chunk;
        cnt += chunk;
        continue;
      }
      // Workaround: Some modules "forget" to notify about data arrival
      if (millis() - prev_check > TINY_GSM_UNREAD_CHECK_MS) {
        // setting got_data to true will tell maintain to run
        // modemGetAvailable()
        got_data   = true;
        prev_check = millis();
      }
      at->maintain();  // clear the modem stream, parse URCs, run
                       // modemGetAvailable()
      // Refill the TinyGSM fifo from the modem's internal buffer
      // TODO: Read directly from modem into user buffer, skipping FIFO
      if (sock_available > 0) {
        int n = at->modemRead(TinyGsmMin((uint16_t)rx.free(), sock_available),
                              mux);
        if (n == 0) break;
      } else {
        break;
      }
    }
    return cnt;
  }

  /**
   * @brief Read a single byte from the client's receive buffer.
   * @return int The byte read, or -1 if no data is available.
   */
  int read() override {
    uint8_t c;
    if (read(&c, 1) == 1) { return c; }
    return -1;
  }

  /**
   * @brief Peek at the next byte in the client's receive buffer without
   * removing it.
   *
   * @return int The next byte, or -1 if no data is available.
   */
  int peek() override {
    if (at == nullptr) { return -1; }
    TINY_GSM_YIELD();
    // If data is already in the FIFO, peek at it
    if (rx.size() > 0) { return rx.peek(); }
    // Without a modem buffer, the FIFO is only filled while parsing URCs.
    if (TcpConfig::kBufferMode == TinyGsmTcpBufferMode::NoModemBuffer) {
      if (!sock_connected) { return -1; }
      is_mid_send = false;  // Any calls to the AT when mid-send will cause
                            // the send to fail
      at->maintain();
      return (rx.size() > 0) ? rx.peek() : -1;
    }
    // For modes that use the modem buffer, try to refill the FIFO
    // if the modem has available data
    if (TcpConfig::kBufferMode != TinyGsmTcpBufferMode::NoModemBuffer &&
        sock_available > 0) {
      is_mid_send = false;  // Any calls to the AT when mid-send will cause
                            // the send to fail
      at->maintain();       // clear the modem stream/parse URCs
      // Pull one byte (or more if available) from the modem into the FIFO
      at->modemRead(TinyGsmMin((uint16_t)rx.free(), sock_available), mux);
      // Now peek at the FIFO again, but only if data was actually added
      return (rx.size() > 0) ? rx.peek() : -1;
    }
    // No data available
    return -1;
  }

  /**
   * @brief Flush the client's send buffer (ie, wait for all data to be sent).
   */
  void flush() override {
    if (at == nullptr) { return; }
    at->stream.flush();
  }

  /**
   * @brief Check if the client is connected.
   * @return uint8_t True if the client is connected, false otherwise.
   */
  uint8_t connected() override {
    if (at == nullptr) { return false; }
    if (is_mid_send) { return true; }  // Don't interrupt a send
    if (available()) { return true; }
    // If the modem is one where we can read and check the size of the buffer,
    // then the 'available()' function will call a check of the current size
    // of the buffer and state of the connection. [available calls maintain,
    // maintain calls modemGetAvailable, modemGetAvailable calls
    // modemGetConnected]  This cascade means that the sock_connected value
    // should be correct and all we need
    if (TcpConfig::kBufferMode ==
        TinyGsmTcpBufferMode::BufferReadAndCheckSize) {
      return sock_connected;
    }
    // If the modem doesn't have an internal buffer, or if we can't check how
    // many characters are in the buffer then the cascade won't happen.
    // We need to call modemGetConnected to check the sock state.
    return at->modemGetConnected(mux);
  }
  /// Check if the client is connected (overrides operator bool)
  operator bool() override {
    return connected();
  }

  /// destructor - need to remove self from the socket pointer array
  virtual ~GsmClient() {
    if (at != nullptr && mux < TcpConfig::kMuxCount) {
      if (at->sockets[mux] == this) { at->sockets[mux] = nullptr; }
    }
  }
  /**@}*/

  /**
   * @anchor extended_client_api
   * @name Extended Client API
   */
  /**@{*/

  /**
   * @brief Get the remote IP address of the connected client
   * @return The remote IP address as a String
   */
  virtual String remoteIP() = 0;

  /**
   * @brief Get the **zero-indexed** position of the client in the corresponding
   * modem's socket array.
   * @return The socket position as a uint8_t
   */
  uint8_t getMux() {
    return mux;
  }
  /**
   * @brief Get the number that the modem uses internally to identify the
   * connection.  In most cases, this is the same as the socket position.
   * @return The internal connection number as a uint8_t
   */
  uint8_t getConnectionID() {
    if (at != nullptr) { return at->muxToConnectionId(mux); }
    return mux;
  }

  /**
   * @brief Begin writing to the modem client
   *
   * Use this to have the modem initiate a send data prompt which you can then
   * fill using stream.write() commands. This is useful for sending large
   * amounts of data in small chunks. It is analogous to the beginPublish()
   * and endPublish() methods in PubSubClient.
   *
   * @param size The size of data to send. The maximum length varies by module
   * @return True if the module is ready to receive data to forward to the TCP
   * connection.
   */
  bool beginWrite(uint16_t size) {
    if (at == nullptr) { return false; }
    if (size > TcpConfig::kSendMaxSize) {
      DBG(GF("### ERROR: You are attempting send"), size,
          GF("bytes, which is more than the"), TcpConfig::kSendMaxSize,
          GF("that can be sent at once by this modem!"));
      return false;
    }
    if (!at->modemBeginSend(size, mux)) {
      is_mid_send = false;
      return false;
    }
    is_mid_send = true;
    return true;
  }
  /**
   * @brief Conclude a write to the module
   *
   * @param expected_size The size of data that should have been sent. If a
   * non-zero value is given, the function will check that the module has sent
   * the expected amount of data. Does not work on all modules.
   * @return True if the module has successfully sent the data to the TCP
   * connection.
   */
  bool endWrite(uint16_t expected_size = 0) {
    if (at == nullptr) { return false; }
    uint16_t sent_size = at->modemEndSend(expected_size, mux);
    is_mid_send        = false;
    if (expected_size) { return sent_size == expected_size; }
    return true;
  }

  /**
   * @brief Convert an IPAddress to a String for use in connect()
   * @param ip The IPAddress to convert
   * @return A String representation of the IPAddress
   */
  static inline String TinyGsmStringFromIp(IPAddress ip) {
    String host;
    host.reserve(16);
    host += ip[0];
    host += '.';
    host += ip[1];
    host += '.';
    host += ip[2];
    host += '.';
    host += ip[3];
    return host;
  }
  /**@}*/

 protected:
  // Read and dump anything remaining in the modem's internal buffer.
  // Using this in the client stop() function.
  // The socket will appear open in response to connected() even after it
  // closes until all data is read from the buffer.
  // Doing it this way allows the external mcu to find and get all of the
  // data that it wants from the socket even if it was closed externally.
  inline void dumpModemBuffer(uint32_t maxWaitMs) {
    if (at == nullptr) { return; }
    if (TcpConfig::kBufferMode == TinyGsmTcpBufferMode::NoModemBuffer) {
      rx.clear();
      at->streamClear();
    } else {
      TINY_GSM_YIELD();
      uint32_t startMillis = millis();
      while (sock_available > 0 && (millis() - startMillis < maxWaitMs)) {
        rx.clear();
        at->modemRead(TinyGsmMin((uint16_t)rx.free(), sock_available), mux);
      }
      rx.clear();
      at->streamClear();
    }
  }

  modemType*                               at             = nullptr;
  uint8_t                                  mux            = 0;
  uint16_t                                 sock_available = 0;
  uint32_t                                 prev_check     = 0;
  bool                                     sock_connected = false;
  bool                                     got_data       = false;
  bool                                     is_secure      = false;
  bool                                     is_mid_send    = false;
  TinyGsmFifo<uint8_t, TINY_GSM_RX_BUFFER> rx;
};

#endif  // SRC_TINYGSMTCP_TPP_
