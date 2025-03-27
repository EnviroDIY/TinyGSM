/**
 * @file       TinyGsmTCP.tpp
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMTCP_H_
#define SRC_TINYGSMTCP_H_

#include "TinyGsmCommon.h"

#define TINY_GSM_MODEM_HAS_TCP

#include "TinyGsmFifo.h"

#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 64
#endif

#if !defined(TINY_GSM_UNREAD_CHECK_MS)
#define TINY_GSM_UNREAD_CHECK_MS 500
#endif

#if !defined(TINY_GSM_CONNECT_TIMEOUT)
#define TINY_GSM_CONNECT_TIMEOUT 75
#endif

#if !defined(TINY_GSM_SEND_MAX_SIZE)
// This **should** be defined for each modem, but if it's not, we'll assume it's
// the TCP MTU.
#define TINY_GSM_SEND_MAX_SIZE 1500
#endif

#if !defined(TINY_GSM_MIN_SEND_BUFFER)
// This is the minimum amount of free send buffer space the modem must report
// before attempting a send. If the minimum send buffer size is not defined,
// we'll assume it's 1 byte. Some modules (SIM7080G) will freeze or crash if you
// pummel it with data when the send buffer isn't empty.
#define TINY_GSM_MIN_SEND_BUFFER 1
#endif

// Because of the ordering of resolution of overrides in templates, these need
// to be written out every time.  This macro is to shorten that.
#define TINY_GSM_CLIENT_CONNECT_OVERRIDES                             \
  int connect(IPAddress ip, uint16_t port, int timeout_s) {           \
    return connect(TinyGsmStringFromIp(ip).c_str(), port, timeout_s); \
  }                                                                   \
  int connect(const char* host, uint16_t port) override {             \
    return connect(host, port, TINY_GSM_CONNECT_TIMEOUT);             \
  }                                                                   \
  int connect(IPAddress ip, uint16_t port) override {                 \
    return connect(ip, port, TINY_GSM_CONNECT_TIMEOUT);               \
  }

// // For modules that do not store incoming data in any sort of buffer
// #define TINY_GSM_NO_MODEM_BUFFER
// // Data is stored in a buffer, but we can only read from the buffer,
// // not check how much data is stored in it
// #define TINY_GSM_BUFFER_READ_NO_CHECK
// // Data is stored in a buffer and we can both read and check the size
// // of the buffer
// #define TINY_GSM_BUFFER_READ_AND_CHECK_SIZE

// // For modules that can and will change the mux number on connection
// #define TINY_GSM_MUX_DYNAMIC
// // For modules that always use the mux you assign them
// #define TINY_GSM_MUX_STATIC

template <class modemType, uint8_t muxCount>
class TinyGsmTCP {
  /* =========================================== */
  /* =========================================== */
  /*
   * Define the interface
   */
 public:
  /*
   * Basic functions
   */
  void maintain() {
    return thisModem().maintainImpl();
  }

  uint8_t findFirstUnassignedMux() {
    // Try to iterate through the assigned client sockets to find the next spot
    // in the array of client pointers that has not been linked to an object.
    for (int next_mux = 0; next_mux < muxCount; next_mux++) {
      if (thisModem().sockets[next_mux] == nullptr) { return next_mux; }
    }
    DBG("### WARNING: No empty mux sockets found!");
    return static_cast<uint8_t>(-1);
  }

 protected:
#if defined(TINY_GSM_MUX_STATIC)
  bool modemConnect(const char* host, uint16_t port, uint8_t mux,
                    int timeout_s = TINY_GSM_CONNECT_TIMEOUT) {
    return thisModem().modemConnectImpl(host, port, mux, timeout_s);
  }
#elif defined(TINY_GSM_MUX_DYNAMIC)
  bool modemConnect(const char* host, uint16_t port, uint8_t* mux,
                    int timeout_s = TINY_GSM_CONNECT_TIMEOUT) {
    return thisModem().modemConnectImpl(host, port, mux, timeout_s);
  }
#else
#error Modem client has been incorrectly created
#endif

  /**
   * @brief Sends a buffer of data to the modem
   *
   * By default this breaks the data into chunks of size TINY_GSM_SEND_MAX_SIZE.
   * Then for each chunk it calls modemWaitForSend (which calls
   * modemGetSendLength), then modemBeginSend, then writes the buffer content,
   * then calls modemEndSend.
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
#if defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE || \
    defined TINY_GSM_BUFFER_READ_NO_CHECK
  size_t modemRead(size_t size, uint8_t mux) {
    return thisModem().modemReadImpl(size, mux);
  }
#endif
#if defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
  size_t modemGetAvailable(uint8_t mux) {
    return thisModem().modemGetAvailableImpl(mux);
  }
#endif
  bool modemGetConnected(uint8_t mux) {
    return thisModem().modemGetConnectedImpl(mux);
  }

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

  /*
   * Inner Client
   */
 public:
  class GsmClient : public Client {
    // Make all classes created from the modem template friends
    friend class TinyGsmTCP<modemType, muxCount>;
    typedef TinyGsmFifo<uint8_t, TINY_GSM_RX_BUFFER> RxFifo;

   public:
    // bool init(modemType* modem, uint8_t);
    // int connect(const char* host, uint16_t port, int timeout_s);

    // Connect to a IP address given as an IPAddress object by
    // converting said IP address to text
    // virtual int connect(IPAddress ip,uint16_t port, int timeout_s) {
    //   return connect(TinyGsmStringFromIp(ip).c_str(), port,
    //   timeout_s);
    // }
    // int connect(const char* host, uint16_t port) override {
    //   return connect(host, port, TINY_GSM_CONNECT_TIMEOUT);
    // }
    // int connect(IPAddress ip,uint16_t port) override {
    //   return connect(ip, port, TINY_GSM_CONNECT_TIMEOUT);
    // }

    static inline String TinyGsmStringFromIp(IPAddress ip) {
      String host;
      host.reserve(16);
      host += ip[0];
      host += ".";
      host += ip[1];
      host += ".";
      host += ip[2];
      host += ".";
      host += ip[3];
      return host;
    }

    // void stop(uint32_t maxWaitMs);
    // void stop() override {
    //   stop(15000L);
    // }

    // Writes data out on the client using the modem send functionality
    size_t write(const uint8_t* buf, size_t size) override {
      if (is_mid_send) {
        // if we're in the middle of a write, pass directly to the stream
        return at->stream.write(buf, size);
      }
      TINY_GSM_YIELD();
      at->maintain();
#if defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
      // If the modem is one where we can read and check the size of the buffer,
      // then the 'available()' function will call a check of the current size
      // of the buffer and state of the connection. [available calls maintain,
      // maintain calls modemGetAvailable, modemGetAvailable calls
      // modemGetConnected]  This cascade means that the sock_connected value
      // should be correct and we can trust it if it says we're not connected to
      // send.
      if (!sock_connected) { return 0; }
#endif
      return at->modemSend(buf, size, mux);
    }

    size_t write(uint8_t c) override {
      return write(&c, 1);
    }

    size_t write(const char* str) {
      if (str == nullptr) return 0;
      return write((const uint8_t*)str, strlen(str));
    }

    int available() override {
      is_mid_send = false;  // Any calls to the AT when mid-send will cause the
                            // send to fail
      TINY_GSM_YIELD();
#if defined TINY_GSM_NO_MODEM_BUFFER
      // Returns the number of characters available in the TinyGSM fifo
      if (!rx.size() && sock_connected) { at->maintain(); }
      return rx.size();

#elif defined TINY_GSM_BUFFER_READ_NO_CHECK
      // Returns the combined number of characters available in the TinyGSM
      // fifo and the modem chips internal fifo.
      if (!rx.size()) { at->maintain(); }
      return static_cast<uint16_t>(rx.size()) + sock_available;

#elif defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
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

#else
#error Modem client has been incorrectly created
#endif
    }

    int read(uint8_t* buf, size_t size) override {
      TINY_GSM_YIELD();
      is_mid_send = false;  // Any calls to the AT when mid-send will cause the
                            // send to fail
      size_t cnt = 0;

#if defined TINY_GSM_NO_MODEM_BUFFER
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
        if (!rx.size() && sock_connected) { at->maintain(); }
      }
      return cnt;

#elif defined TINY_GSM_BUFFER_READ_NO_CHECK
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

#elif defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
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

#else
#error Modem client has been incorrectly created
#endif
    }

    int read() override {
      uint8_t c;
      if (read(&c, 1) == 1) { return c; }
      return -1;
    }

    int peek() override {
      return (uint8_t)rx.peek();
    }

    void flush() override {
      at->stream.flush();
    }

    uint8_t connected() override {
      if (is_mid_send) { return true; }  // Don't interrupt a send
      if (available()) { return true; }
#if defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
      // If the modem is one where we can read and check the size of the buffer,
      // then the 'available()' function will call a check of the current size
      // of the buffer and state of the connection. [available calls maintain,
      // maintain calls modemGetAvailable, modemGetAvailable calls
      // modemGetConnected]  This cascade means that the sock_connected value
      // should be correct and all we need
      return sock_connected;
#elif defined TINY_GSM_NO_MODEM_BUFFER || defined TINY_GSM_BUFFER_READ_NO_CHECK
      // If the modem doesn't have an internal buffer, or if we can't check how
      // many characters are in the buffer then the cascade won't happen.
      // We need to call modemGetConnected to check the sock state.
      return at->modemGetConnected(mux);
#else
#error Modem client has been incorrectly created
#endif
    }
    operator bool() override {
      return connected();
    }

    // destructor - need to remove self from the socket pointer array
    virtual ~GsmClient() {
      if (mux < muxCount) {
        if (at->sockets[mux] == this) { at->sockets[mux] = nullptr; }
      }
    }

    /*
     * Extended API
     */

    String remoteIP() TINY_GSM_ATTR_NOT_IMPLEMENTED;

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
      if (size > TINY_GSM_SEND_MAX_SIZE) {
        DBG(GF("### ERROR: You are attempting send"), size,
            GF("bytes, which is more than the"), TINY_GSM_SEND_MAX_SIZE,
            GF("that can be sent at once by this modem!"));
        return false;
      }
      is_mid_send = true;
      return at->modemBeginSend(size, mux);
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

   protected:
    // Read and dump anything remaining in the modem's internal buffer.
    // Using this in the client stop() function.
    // The socket will appear open in response to connected() even after it
    // closes until all data is read from the buffer.
    // Doing it this way allows the external mcu to find and get all of the
    // data that it wants from the socket even if it was closed externally.
    inline void dumpModemBuffer(uint32_t maxWaitMs) {
#if defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE || \
    defined TINY_GSM_BUFFER_READ_NO_CHECK
      TINY_GSM_YIELD();
      uint32_t startMillis = millis();
      while (sock_available > 0 && (millis() - startMillis < maxWaitMs)) {
        rx.clear();
        at->modemRead(TinyGsmMin((uint16_t)rx.free(), sock_available), mux);
      }
      rx.clear();
      at->streamClear();

#elif defined TINY_GSM_NO_MODEM_BUFFER
      rx.clear();
      at->streamClear();

#else
#error Modem client has been incorrectly created
#endif
    }

    modemType* at;
    uint8_t    mux;
    uint16_t   sock_available;
    uint32_t   prev_check;
    bool       sock_connected;
    bool       got_data;
    bool       is_secure;
    bool       is_mid_send = false;
    RxFifo     rx;
  };

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the default function implementations
   */

  /*
   * Basic functions
   */
 protected:
  void maintainImpl() {
#if defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
    // Keep listening for modem URC's and proactively iterate through
    // sockets asking if any data is available
    for (int mux = 0; mux < muxCount; mux++) {
      GsmClient* sock = thisModem().sockets[mux];
      if (sock && sock->got_data && sock->sock_available == 0) {
        sock->got_data       = false;
        sock->sock_available = thisModem().modemGetAvailable(mux);
      }
    }
    while (thisModem().stream.available()) {
      thisModem().waitResponse(15, nullptr, nullptr);
    }

#elif defined TINY_GSM_NO_MODEM_BUFFER || defined TINY_GSM_BUFFER_READ_NO_CHECK
    // Just listen for any URC's
    thisModem().waitResponse(100, nullptr, nullptr);

#else
#error Modem client has been incorrectly created
#endif
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
#define READ_CHAR_LEN 2
#else
// DBG("### Reading input in ASCII mode");
#define READ_CHAR_LEN 1
#endif
    // allow up to 3 timeouts on individual characters before we quit the whole
    // read operation
    while (len && char_failures < 3) {
      // if something is available, read it
      if (thisModem().stream.available() >= READ_CHAR_LEN) {
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
        len -= READ_CHAR_LEN;
        len_read += READ_CHAR_LEN;
      } else {
        // wait for a new character to be available on the stream
        while (thisModem().stream.available() < READ_CHAR_LEN &&
               (millis() - startMillis < thisModem().sockets[mux]->_timeout)) {
          TINY_GSM_YIELD();
        }
        if (thisModem().stream.available() < READ_CHAR_LEN) {
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
#undef READ_CHAR_LEN


#if defined(TINY_GSM_MUX_STATIC)
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t* mux,
                        int timeout_s) TINY_GSM_ATTR_NOT_IMPLEMENTED;
#elif defined(TINY_GSM_MUX_DYNAMIC)
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t* mux,
                        int timeout_s) TINY_GSM_ATTR_NOT_IMPLEMENTED;
#else
#error Modem client has been incorrectly created
#endif

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
    return TINY_GSM_SEND_MAX_SIZE;
  }

  size_t modemWaitForSendImpl(uint8_t mux, uint32_t timeout_ms = 15000L) {
    for (uint32_t start = millis(); millis() - start < timeout_ms;) {
      if (thisModem().modemGetSendLength(mux) > 0) { return true; }
      delay(250);
    }
    return false;

    size_t sendLength = modemGetSendLength(mux);
#if defined(TINY_GSM_DEBUG)
    if (sendLength != TINY_GSM_SEND_MAX_SIZE) {
      DBG(GF("### Full send buffer not available! Expected it to have"),
          TINY_GSM_SEND_MAX_SIZE, GF("bytes, but it has"), sendLength);
    }
    if (sendLength < TINY_GSM_MIN_SEND_BUFFER) {
      DBG(GF(
          "### Waiting up to 15s for sufficient available send buffer space"));
    }
#endif
    uint32_t start = millis();
    while (sendLength < TINY_GSM_MIN_SEND_BUFFER && millis() - start < 15000L &&
           thisModem().sockets[mux]->sock_connected) {
      delay(250);
      sendLength = modemGetSendLength(mux);
#if defined(TINY_GSM_DEBUG)
      if (sendLength >= TINY_GSM_MIN_SEND_BUFFER) {
        DBG(GF("### Send buffer has"), sendLength, GF("available after"),
            millis() - start, GF("ms"));
      }
#endif
    }
#if defined(TINY_GSM_DEBUG)
    if (sendLength == 0) {
      DBG(GF("### No available send buffer on attempt"), send_attempts);
    }
#endif
    return sendLength;
  }

#if defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE || \
    defined TINY_GSM_BUFFER_READ_NO_CHECK
  size_t modemReadImpl(size_t size, uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;
#endif
#if defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
  size_t modemGetAvailableImpl(uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;
#endif
  bool modemGetConnectedImpl(uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;
};

#endif  // SRC_TINYGSMTCP_H_
