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

 protected:
  bool modemConnect(const char* host, uint16_t port, uint8_t mux,
                    int timeout_s = TcpConfig::kConnectTimeoutS) {
    return thisModem().modemConnectImpl(host, port, mux, timeout_s);
  }

  bool modemConnect(const char* host, uint16_t port, uint8_t* mux,
                    int timeout_s = TcpConfig::kConnectTimeoutS) {
    return thisModem().modemConnectImpl(host, port, mux, timeout_s);
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
   * @param mux The socket number
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

  /*
   * TCP functions
   */
 protected:
  void maintainImpl() {
    if (TcpConfig::kBufferMode ==
        TinyGsmTcpBufferMode::BufferReadAndCheckSize) {
      // Keep listening for modem URC's and proactively iterate through
      // sockets asking if any data is available
      for (int mux = 0; mux < TcpConfig::kMuxCount; mux++) {
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

  // Yields up to a time-out period and then reads a block of characters from
  // the stream into the mux FIFO
  // If TINY_GSM_USE_HEX is defined, this will convert two received hex
  // characters into one char.
  size_t moveCharsFromStreamToFifo(uint8_t mux, size_t expected_len) {
    if (!thisModem().sockets[mux]) { return false; }
    uint32_t startMillis   = millis();
    size_t   len           = expected_len;
    size_t   len_read      = 0;
    uint8_t  char_failures = 0;
#ifdef TINY_GSM_USE_HEX
    // DBG("### Reading input in HEX mode");
    int readCharLen = 2;
#else
    // DBG("### Reading input in ASCII mode");
    int readCharLen = 1;
#endif
    // allow up to 3 timeouts on individual characters before we quit the whole
    // read operation
    while (len && char_failures < 3) {
      // if something is available, read it
      if (thisModem().stream.available() >= readCharLen) {
#ifdef TINY_GSM_USE_HEX
        // read 2 bytes and convert from hex to char
        char buf[3] = {
            0,
        };
        buf[0] = thisModem().stream.read();
        buf[1] = thisModem().stream.read();
        char c = strtol(buf, nullptr, 16);
#else
        // just read the character
        char c = thisModem().stream.read();
#endif
        // NOTE: We can't directly memcpy into the rx fifo!
        // The fifo is a template class that can hold any data type and the
        // actual memory space of the buffer is protected.
        thisModem().sockets[mux]->rx.put(c);
        len -= readCharLen;
        len_read += readCharLen;
      } else {
        // wait for a new character to be available on the stream
        while (thisModem().stream.available() < readCharLen &&
               (millis() - startMillis < thisModem().sockets[mux]->_timeout)) {
          TINY_GSM_YIELD();
        }
        if (thisModem().stream.available() < readCharLen) {
          DBG("### ERROR: Timed out waiting for character from stream!");
          char_failures++;
        }
      }
    }
    if (len_read) { DBG("### READ:", len_read, "from", mux); }
    if (expected_len != len_read) {
      DBG("\n### Different number of characters received than expected: ",
          len_read, "read vs ", expected_len, "expected");
    }
    return len_read;
  }

  bool modemConnectImpl(const char* host, uint16_t port, uint8_t mux,
                        int timeout_s) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  bool modemConnectImpl(const char* host, uint16_t port, uint8_t* mux,
                        int timeout_s) TINY_GSM_ATTR_NOT_IMPLEMENTED;

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
        if (sendLength == 0) {
          send_attempts++;
          DBG(GF("### No available send buffer on attempt"), send_attempts);
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
        // NOTE: In many cases, confirmed is just a passthrough of len
        int16_t confirmed = thisModem().modemEndSend(len, mux);
#if defined(TINY_GSM_DEBUG)
        if (confirmed < attempted) {
          DBG(GF("### Fewer bytes were confirmed ("), confirmed,
              F(") than attempted ("), attempted, F(") on send attempt"),
              send_attempts);
        }
#endif
        bytesSent += min(attempted,
                         confirmed);         // bump up number of bytes sent
        txPtr += min(attempted, confirmed);  // bump up the pointer
        send_success &= min(attempted, confirmed) > 0;
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
   * @brief Initialize this client with modem context and channel state.
   *
   * @param modem Pointer to the modem instance.
   * @param mux Multiplexing channel to assign.
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
   */
  virtual int connect(const char* host, uint16_t port, int timeout_s) = 0;

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
   * @fn int connect(const char* host, uint16_t port) override
   * @brief Connect to a server using a host name and port number
   * @param host The host name of the server to connect to.
   * @param port The port number to connect to on the server.
   * @return 1 if the connection was successful, 0 otherwise.
   */
  int connect(const char* host, uint16_t port) override {
    return connect(host, port, TcpConfig::kConnectTimeoutS);
  }
  /**
   * @fn int connect(IPAddress ip, uint16_t port) override
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
   * @param maxWaitMs The maximum time to wait for the connection to close,
   * in milliseconds.
   */
  virtual void stop(uint32_t maxWaitMs) = 0;
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
    if (at == nullptr) { return 0; }
    TINY_GSM_YIELD();
    is_mid_send = false;  // Any calls to the AT when mid-send will cause the
                          // send to fail
    size_t cnt = 0;

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
    return rx.peek();
  }

  /**
   * @brief Flush the client's send buffer (ie, wait for all data to be sent).
   */
  void flush() override {
    at->stream.flush();
  }

  /**
   * @brief Check if the client is connected.
   * @return uint8_t True if the client is connected, false otherwise.
   */
  uint8_t connected() override {
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
   * @brief Get the socket number of the connected client
   * @return The socket number as a uint8_t
   */
  uint8_t getMux() {
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
