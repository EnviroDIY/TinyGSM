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

#if !defined(TINY_GSM_CONNECT_TIMEOUT)
#define TINY_GSM_CONNECT_TIMEOUT 75
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

  int16_t modemSend(const void* buff, size_t len, uint8_t mux) {
    return thisModem().modemSendImpl(buff, len, mux);
  }
  bool modemBeginSend(size_t len, uint8_t mux) {
    return thisModem().modemBeginSendImpl(len, mux);
  }
  bool modemEndSend(size_t len, uint8_t mux) {
    return thisModem().modemEndSendImpl(len, mux);
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
  bool modemGetConnected(uint8_t mux) {
    return thisModem().modemGetConnectedImpl() mux;
  }
#endif

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
        return at->stream.write(buf, size, mux);
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
        if (millis() - prev_check > 500) {
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
        size_t chunk = TinyGsmMin(size - cnt, rx.size());
        if (chunk > 0) {
          rx.get(buf, chunk);
          buf += chunk;
          cnt += chunk;
          continue;
        } /* TODO: Read directly into user buffer? */
        if (!rx.size() && sock_connected) { at->maintain(); }
      }
      return cnt;

#elif defined TINY_GSM_BUFFER_READ_NO_CHECK
      // Reads characters out of the TinyGSM fifo, and from the modem chip's
      // internal fifo if available.
      at->maintain();
      while (cnt < size) {
        size_t chunk = TinyGsmMin(size - cnt, rx.size());
        if (chunk > 0) {
          rx.get(buf, chunk);
          buf += chunk;
          cnt += chunk;
          continue;
        } /* TODO: Read directly into user buffer? */
        at->maintain();
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
      at->maintain();
      while (cnt < size) {
        size_t chunk = TinyGsmMin(size - cnt, rx.size());
        if (chunk > 0) {
          rx.get(buf, chunk);
          buf += chunk;
          cnt += chunk;
          continue;
        }
        // Workaround: Some modules "forget" to notify about data arrival
        if (millis() - prev_check > 500) {
          // setting got_data to true will tell maintain to run
          // modemGetAvailable()
          got_data   = true;
          prev_check = millis();
        }
        // TODO(vshymanskyy): Read directly into user buffer?
        at->maintain();
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
      at->sockets[mux] = nullptr;
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

  // Yields up to a time-out period and then reads a character from the stream
  // into the mux FIFO
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


#if defined(TINY_GSM_MUX_STATIC)
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t* mux,
                        int timeout_s) TINY_GSM_ATTR_NOT_IMPLEMENTED;
#elif defined(TINY_GSM_MUX_DYNAMIC)
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t* mux,
                        int timeout_s) TINY_GSM_ATTR_NOT_IMPLEMENTED;
#else
#error Modem client has been incorrectly created
#endif

  int16_t modemSendImpl(const void* buff, size_t len, uint8_t mux) {
    if (!thisModem().modemBeginSend(len, mux)) { return 0; }
    int16_t attempted =
        thisModem().stream.write(reinterpret_cast<const uint8_t*>(buff), len);
    stream.flush();
    // NOTE: In many cases, confirmed is just a passthrough of len
    int16_t confirmed = modemEndSend(len, mux);
    return min(attempted, confirmed);
  }
  bool    modemBeginSendImpl(size_t  len,
                             uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  int16_t modemEndSendImpl(size_t  len,
                           uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;

#if defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE || \
    defined TINY_GSM_BUFFER_READ_NO_CHECK
  size_t modemReadImpl(size_t size, uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;
#endif
#if defined TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
  size_t modemGetAvailableImpl(uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool   modemGetConnectedImpl(uint8_t mux) TINY_GSM_ATTR_NOT_IMPLEMENTED;
#endif
};

#endif  // SRC_TINYGSMTCP_H_
