/**
 * @file       TinyGsmClientESP8266NonOS.h
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCLIENTESP8266NONOS_H_
#define SRC_TINYGSMCLIENTESP8266NONOS_H_
// #pragma message("TinyGSM:  TinyGsmClientESP8266NonOS")

// #define TINY_GSM_DEBUG Serial

#ifdef TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
#undef TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
#endif
#ifdef TINY_GSM_BUFFER_READ_NO_CHECK
#undef TINY_GSM_BUFFER_READ_NO_CHECK
#endif
#ifndef TINY_GSM_NO_MODEM_BUFFER
#define TINY_GSM_NO_MODEM_BUFFER
#endif
#ifdef TINY_GSM_MUX_DYNAMIC
#undef TINY_GSM_MUX_DYNAMIC
#endif
#ifndef TINY_GSM_MUX_STATIC
#define TINY_GSM_MUX_STATIC
#endif

#include "TinyGsmClientEspressif.h"
#include "TinyGsmTCP.tpp"

// NOTE: This module supports SSL, but we do not support any certificate
// management for the non-OS version of the ESP8266 firmware, so we define
// TINY_GSM_MODEM_HAS_SSL here and do no include the SSL module so as not to
// waste space.
#ifndef TINY_GSM_MODEM_HAS_SSL
#define TINY_GSM_MODEM_HAS_SSL
#endif

static uint8_t TINY_GSM_TCP_KEEP_ALIVE = 120;

// <stat> status of ESP8266 station interface
// 0: ESP8266 station is not initialized.
// 1: ESP8266 station is initialized, but not started a Wi-Fi connection yet.
// 2 : ESP8266 station connected to an AP and has obtained IP
// 3 : ESP8266 station created a TCP or UDP transmission
// 4 : the TCP or UDP transmission of ESP8266 station disconnected
// 5 : ESP8266 station did NOT connect to an AP
enum ESP8266NonOSRegStatus {
  REG_UNINITIALIZED = 0,
  REG_UNREGISTERED  = 1,
  REG_OK_IP         = 2,
  REG_OK_TCP        = 3,
  REG_OK_NO_TCP     = 4,
  REG_DENIED        = 5,
  REG_UNKNOWN       = 6,
};

class TinyGsmESP8266NonOS
    : public TinyGsmEspressif<TinyGsmESP8266NonOS>,
      public TinyGsmTCP<TinyGsmESP8266NonOS, TINY_GSM_MUX_COUNT> {
  friend class TinyGsmEspressif<TinyGsmESP8266NonOS>;
  friend class TinyGsmModem<TinyGsmESP8266NonOS>;
  friend class TinyGsmWifi<TinyGsmESP8266NonOS>;
  friend class TinyGsmTCP<TinyGsmESP8266NonOS, TINY_GSM_MUX_COUNT>;

  /*
   * Inner Client
   */
 public:
  class GsmClientESP8266NonOS
      : public TinyGsmTCP<TinyGsmESP8266NonOS, TINY_GSM_MUX_COUNT>::GsmClient {
    friend class TinyGsmESP8266NonOS;

   public:
    GsmClientESP8266NonOS() {
      is_secure = false;
    }

    explicit GsmClientESP8266NonOS(TinyGsmESP8266NonOS& modem,
                                   uint8_t              mux = 0) {
      init(&modem, mux);
      is_secure = false;
    }

    bool init(TinyGsmESP8266NonOS* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_connected = false;
      is_mid_send    = false;

      // The ESP8266 (as supported) generally lets you choose the mux number,
      // but we want to try to find an empty place in the socket array for it.

      // if it's a valid mux number, and that mux number isn't in use (or it's
      // already this), accept the mux number
      if (mux < TINY_GSM_MUX_COUNT &&
          (at->sockets[mux] == nullptr || at->sockets[mux] == this)) {
        this->mux = mux;
        // If the mux number is in use or out of range, find the next available
        // one
      } else if (at->findFirstUnassignedMux() != static_cast<uint8_t>(-1)) {
        this->mux = at->findFirstUnassignedMux();
      } else {
        // If we can't find anything available, overwrite something, useing mod
        // to make sure we're in range
        this->mux = (mux % TINY_GSM_MUX_COUNT);
      }
      at->sockets[this->mux] = this;

      return true;
    }

   public:
    virtual int connect(const char* host, uint16_t port, int timeout_s) {
      stop();
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    virtual void stop(uint32_t maxWaitMs) {
      is_mid_send = false;
      TINY_GSM_YIELD();
      at->sendAT(GF("+CIPCLOSE="), mux);
      sock_connected = false;
      at->waitResponse(maxWaitMs);
      rx.clear();
    }
    void stop() override {
      stop(5000L);
    }

    /*
     * Extended API
     */

    String remoteIP() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  };

  /*
   * Inner Secure Client
   */
 public:
  class GsmClientSecureESP8266NonOS : public GsmClientESP8266NonOS {
    friend class TinyGsmESP8266NonOS;

   public:
    GsmClientSecureESP8266NonOS() {
      is_secure = true;
    }

    explicit GsmClientSecureESP8266NonOS(TinyGsmESP8266NonOS& modem,
                                         uint8_t              mux = 0)
        : GsmClientESP8266NonOS(modem, mux) {
      is_secure = true;
    }
  };

  /*
   * GSM Modem Constructor
   */
 public:
  explicit TinyGsmESP8266NonOS(Stream& stream)
      : TinyGsmEspressif<TinyGsmESP8266NonOS>(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  // Follows functions inherited from Espressif

  /*
   * Power functions
   */
 protected:
  // Follows functions inherited from Espressif

  /*
   * Generic network functions
   */
 public:
  ESP8266NonOSRegStatus getRegistrationStatus() {
    sendAT(GF("+CIPSTATUS"));
    if (waitResponse(3000, GF("STATUS:")) != 1) return REG_UNKNOWN;
    // after "STATUS:" it should return the status number (0,1,2,3,4,5),
    // followed by an OK
    // Since there are more possible status number codes than the arguments for
    // waitResponse, we'll capture the response in a string and then parse it.
    String res;
    if (waitResponse(3000L, res) != 1) { return REG_UNKNOWN; }
    res.trim();
    int8_t status = res.toInt();
    return (ESP8266NonOSRegStatus)status;
  }

 protected:
  bool isNetworkConnectedImpl() {
    ESP8266NonOSRegStatus s = getRegistrationStatus();
    if (s == REG_OK_IP || s == REG_OK_TCP) {
      // with these, we're definitely connected
      return true;
    } else if (s == REG_OK_NO_TCP) {
      // with this, we may or may not be connected
      if (getLocalIP() == "") {
        return false;
      } else {
        return true;
      }
    } else {
      return false;
    }
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // No SSL functions are supported on the ESP8266 using the non-OS AT firmware.


  /*
   * WiFi functions
   */
 protected:
  // Follows functions inherited from Espressif


  /*
   * GPRS functions
   */
  // No functions of this type supported

  /*
   * SIM card functions
   */
  // No functions of this type supported

  /*
   * Audio functions
   */
  // No functions of this type supported

  /*
   * Text messaging (SMS) functions
   */
  // No functions of this type supported

  /*
   * GSM Location functions
   */
  // No functions of this type supported

  /*
   * GPS/GNSS/GLONASS location functions
   */
  // No functions of this type supported

  /*
   * Time functions
   */
  // No functions of this type supported

  /*
   * NTP server functions
   */
  // No functions of this type supported

  /*
   * BLE functions
   */
  // No functions of this type supported

  /*
   * Battery functions
   */
  // No functions of this type supported

  /*
   * Temperature functions
   */
  // No functions of this type supported

  /*
   * Client related functions
   */
 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t mux,
                        int timeout_s) {
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    bool     ssl        = sockets[mux]->is_secure;
    if (ssl) {
      sendAT(GF("+CIPSSLCCONF="), mux, ',', '0');
      if (waitResponse() != 1) {
        sendAT(GF("+CIPSSLSIZE=4096"));
        waitResponse();
      }
    }
    sendAT(GF("+CIPSTART="), mux, ',', ssl ? GF("\"SSL") : GF("\"TCP"),
           GF("\",\""), host, GF("\","), port, GF(","),
           TINY_GSM_TCP_KEEP_ALIVE);
    // TODO(?): Check mux
    int8_t rsp = waitResponse(timeout_ms, GFP(GSM_OK), GFP(GSM_ERROR),
                              GF("ALREADY CONNECT"));
    if (rsp == 3) waitResponse();
    // May return "ERROR" after the "ALREADY CONNECT"
    return (1 == rsp);
  }

  bool modemGetConnectedImpl(uint8_t mux) {
    sendAT(GF("+CIPSTATUS"));
    if (waitResponse(3000, GF("STATUS:")) != 1) { return false; }
    // after "STATUS:" it should return the status number (0,1,2,3,4,5),
    // followed by an OK
    // Hopefully we'll catch the "3" here, but fall back to the OK or Error
    int8_t status = waitResponse(GF("3"), GFP(GSM_OK), GFP(GSM_ERROR));
    // if the status is anything but 3, there are no connections open
    if (status != 1) {
      for (int muxNo = 0; muxNo < TINY_GSM_MUX_COUNT; muxNo++) {
        if (sockets[muxNo]) { sockets[muxNo]->sock_connected = false; }
      }
      return false;
    }
    bool verified_connections[TINY_GSM_MUX_COUNT] = {0, 0, 0, 0, 0};
    for (int muxNo = 0; muxNo < TINY_GSM_MUX_COUNT; muxNo++) {
      uint8_t has_status = waitResponse(GF("+CIPSTATUS:"), GFP(GSM_OK),
                                        GFP(GSM_ERROR));
      if (has_status == 1) {
        int8_t returned_mux = streamGetIntBefore(',');
        streamSkipUntil(',');   // Skip mux
        streamSkipUntil(',');   // Skip type
        streamSkipUntil(',');   // Skip remote IP
        streamSkipUntil(',');   // Skip remote port
        streamSkipUntil(',');   // Skip local port
        streamSkipUntil('\n');  // Skip client/server type
        verified_connections[returned_mux] = 1;
      }
      if (has_status == 2) break;  // once we get to the ok, stop
    }
    for (int muxNo = 0; muxNo < TINY_GSM_MUX_COUNT; muxNo++) {
      if (sockets[muxNo]) {
        sockets[muxNo]->sock_connected = verified_connections[muxNo];
      }
    }
    return verified_connections[mux];
  }

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    sendAT(GF("+CIPSEND="), mux, ',', len);
    return waitResponse(GF(">")) == 1;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t) {
    if (waitResponse(30000L, GF(AT_NL "SEND OK" AT_NL)) != 1) { return 0; }
    return len;
  }

  /*
   * Utilities
   */
 public:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+IPD,"))) {
      int8_t  mux          = streamGetIntBefore(',');
      int16_t len_reported = streamGetIntBefore(':');
      int16_t len          = len_reported;
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        if (len > sockets[mux]->rx.free()) {
          DBG("### Buffer overflow: ", len, "->", sockets[mux]->rx.free());
          // reset the len to read to the amount free
          len = sockets[mux]->rx.free();
        }
        moveCharsFromStreamToFifo(mux, len);
        // TODO(SRGDamia1): deal with buffer overflow
      }
      data = "";
      return true;
    } else if (data.endsWith(GF("CLOSED"))) {
      int8_t muxStart = TinyGsmMax(0,
                                   data.lastIndexOf(AT_NL, data.length() - 8));
      int8_t coma     = data.indexOf(',', muxStart);
      int8_t mux      = data.substring(muxStart, coma).toInt();
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      streamSkipUntil('\n');  // throw away the new line
      data = "";
      DBG("### Closed: ", mux);
      return true;
    } else if (data.endsWith(GF("busy p..."))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Busy, please wait");
      return true;
    } else if (data.endsWith(GF(AT_NL "ready" AT_NL))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Module ready!");
      return true;
    } else if (data.endsWith(GF("WIFI GOT IP"))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Wifi got IP");
      return true;
    } else if (data.endsWith(GF("WIFI CONNECTED"))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Wifi connected");
      return true;
    } else if (data.endsWith(GF("WIFI DISCONNECT"))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Wifi disconnected");
      return true;
    }
    return false;
  }

 protected:
  GsmClientESP8266NonOS* sockets[TINY_GSM_MUX_COUNT];
};

#endif  // SRC_TINYGSMCLIENTESP8266NONOS_H_
