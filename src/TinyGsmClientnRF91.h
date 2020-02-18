/**
 * @file       TinyGsmClientnRF91.h
 * @author     Sara Geleskie Damiano
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2020 Sara Geleskie Damiano
 * @date       Feb 2020
 * @note       This is based on TCP-AT commands supplied in the serial_lte_modem sample in Nordic Playground.  At this time, only one socket connection is supported.
 */

#ifndef SRC_TINYGSMCLIENTNRF91_H_
#define SRC_TINYGSMCLIENTNRF91_H_
// #pragma message("TinyGSM:  TinyGsmClientnRF91")

// #define TINY_GSM_DEBUG Serial

// NOTE:  The nRF91 series supports up to 8 socket instances,
// but the TCP AT commands I'm using only implement 1
#define TINY_GSM_MUX_COUNT 1
#define TINY_GSM_BUFFER_READ_NO_CHECK

#include "TinyGsmBattery.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmTemperature.tpp"
#include "TinyGsmTime.tpp"

#define GSM_NL "\r\n"
static const char GSM_OK[] TINY_GSM_PROGMEM    = "OK" GSM_NL;
static const char GSM_ERROR[] TINY_GSM_PROGMEM = "ERROR" GSM_NL;
#if defined       TINY_GSM_DEBUG
static const char GSM_CME_ERROR[] TINY_GSM_PROGMEM = GSM_NL "+CME ERROR:";
#endif

enum RegStatus {
  REG_NO_RESULT = -1,
  REG_UNREGISTERED = 0,
  REG_SEARCHING = 2,
  REG_DENIED = 3,
  REG_OK_HOME = 1,
  REG_OK_ROAMING = 5,
  REG_UNKNOWN = 4,
  REG_EMERGENCY_ONLY = 8,
  REG_UICC_FAILURE = 90,
};

class TinyGsmnRF91 : public TinyGsmModem<TinyGsmnRF91>,
                     public TinyGsmGPRS<TinyGsmnRF91>,
                     public TinyGsmTCP<TinyGsmnRF91, TINY_GSM_MUX_COUNT>,
                     public TinyGsmTime<TinyGsmnRF91>/*,
                     public TinyGsmBattery<TinyGsmnRF91>,
                     public TinyGsmTemperature<TinyGsmnRF91>*/ {
  friend class TinyGsmModem<TinyGsmnRF91>;
  friend class TinyGsmGPRS<TinyGsmnRF91>;
  friend class TinyGsmTCP<TinyGsmnRF91, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmTime<TinyGsmnRF91>;
  // friend class TinyGsmBattery<TinyGsmnRF91>;
  // friend class TinyGsmTemperature<TinyGsmnRF91>;

  /*
   * Inner Client
   */
 public:
  class GsmClientnRF91 : public GsmClient {
    friend class TinyGsmnRF91;

   public:
    GsmClientnRF91() {}

    explicit GsmClientnRF91(TinyGsmnRF91& modem, uint8_t mux = 0) {
      init(&modem, mux);
    }

    bool init(TinyGsmnRF91* modem, uint8_t) {
      this->at       = modem;
      this->mux      = 0;
      sock_available = 0;
      prev_check     = 0;
      sock_connected = false;
      got_data       = false;

      at->sockets[mux] = this;

      return true;
    }

   public:
    virtual int connect(const char* host, uint16_t port, int timeout_s) {
      stop();
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, false, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    void stop(uint32_t maxWaitMs) {
      dumpModemBuffer(maxWaitMs);
      // Only 1 socket is supported
      at->sendAT(GF("#XSOCKET=0"));
      sock_connected = false;
      at->waitResponse();
    }
    void stop() override {
      stop(15000L);
    }

    /*
     * Extended API
     */

    String remoteIP() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  };

  /*
   * Inner Secure Client
   */

  /*
  class GsmClientSecurenRF91 : public GsmClientnRF91
  {
  public:
    GsmClientSecure() {}

    GsmClientSecure(TinyGsmnRF91& modem, uint8_t mux = 1)
     : public GsmClient(modem, mux)
    {}


  public:
    int connect(const char* host, uint16_t port, int timeout_s) override {
      stop();
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, true, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES
  };
  */

  /*
   * Constructor
   */
 public:
  explicit TinyGsmnRF91(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = NULL) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);

    if (!testAT()) { return false; }

#ifdef TINY_GSM_DEBUG
    sendAT(GF("+CMEE=2"));  // turn on verbose error codes
#else
    sendAT(GF("+CMEE=0"));  // turn off error codes
#endif
    waitResponse();

    DBG(GF("### Modem:"), getModemName());

    // Disable URC's for network time
    sendAT(GF("%XTIME=0"));
    if (waitResponse(10000L) != 1) { return false;
    }

    // Enable network time support
    sendAT(GF("%XNETTIME=1"));
    if (waitResponse(10000L) != 1) {
      return false;
    }

    SimStatus ret = getSimStatus();
    // if the sim isn't ready and a pin has been provided, try to unlock the sim
    if (ret != SIM_READY && pin != NULL && strlen(pin) > 0) {
      simUnlock(pin);
      return (getSimStatus() == SIM_READY);
    } else {
      // if the sim is ready, or it's locked but no pin has been provided,
      // return true
      return (ret == SIM_READY || ret == SIM_LOCKED);
    }
  }

  void setBaudImpl(uint32_t baud) TINY_GSM_ATTR_NOT_AVAILABLE;

  String getModemInfoImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  String getModemNameImpl() {
    sendAT(GF("+CGMI"));
    String res1;
    if (waitResponse(1000L, res1) != 1) {
      return "Nordic nRF9160";
    }
    res1.replace("\r\nOK\r\n", "");
    res1.replace("\rOK\r", "");
    res1.trim();

    sendAT(GF("+CGMM"));
    String res2;
    if (waitResponse(1000L, res2) != 1) {
      return "Nordic nRF9160";
    }
    res1.replace("\r\nOK\r\n", "");
    res1.replace("\rOK\r", "");
    res2.trim();

    String name = res1 + String(' ') + res2;
    DBG("### Modem:", name);
    return name;
  }

  bool factoryDefaultImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  /*
   * Power functions
   */
 protected:
  bool restartImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool powerOffImpl() {
    sendAT(GF("#XSLEEP"));
    // no return expected
    return true;
  }

  bool radioOffImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool sleepEnableImpl(bool enable = true) TINY_GSM_ATTR_NOT_AVAILABLE;

  /*
   * Generic network functions
   */
 public:
  RegStatus getRegistrationStatus() {
    return (RegStatus)getRegistrationStatusXREG("CEREG");
  }

 protected:
  bool isNetworkConnectedImpl() {
    RegStatus s = getRegistrationStatus();
    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
  }

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* user = NULL,
                       const char* pwd = NULL) {
    gprsDisconnect();

    sendAT(GF("+CGATT=1"));  // attach to packet domain services
    if (waitResponse(360000L) != 1) {
      return false;
    }

    // Set the authentication
    if (user && strlen(user) > 0) {
      sendAT(GF("+CGAUTH=1,0,\""), user, GF("\",\""), pwd, '"');
      waitResponse();
    }

    sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, '"');  // Define PDP context 1
    waitResponse();

    sendAT(GF("+CGACT=1,1"));  // activate PDP profile/context 1
    if (waitResponse(150000L) != 1) {
      return false;
    }

    return true;
  }

  bool gprsDisconnectImpl() {
    sendAT(GF("+CGACT=0,1"));  // Deactivate PDP context 1
    if (waitResponse(40000L) != 1) {
      // return false;
    }

    sendAT(GF("+CGATT=0"));  // detach from GPRS
    if (waitResponse(360000L) != 1) {
      return false;
    }

    return true;
  }

  /*
   * SIM card functions
   */
 protected:
  String getSimCCIDImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  String getIMEIImpl() {
    sendAT(GF("+CGSN"));
    streamSkipUntil('\n');  // skip first newline
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  /*
   * Phone Call functions
   */
 protected:
  // No functions of this type supported

  /*
   * Messaging functions
   */
 protected:
  // No functions of this type implemented

  /*
   * Time functions
   */
 protected:
  // Can follow the standard CCLK function in the template

  /*
   * Battery functions
   */

  /*
   * Client related functions
   */
 protected:
  bool modemConnect(const char* host, uint16_t port, uint8_t mux,
                    bool ssl = false, int timeout_s = 20) {
    if (ssl) { DBG("SSL not yet supported on this module!"); }

    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;

    // AT#XSOCKET=<op>[,<type>] 0/1 open/close, 1/2 TCP/UDP
    sendAT(GF("#XSOCKET=1, 1"));
    waitResponse();

    // AT#XTCPCONN=<url>,<port>
    sendAT(GF("#XTCPCONN=\""), host, GF("\","), port);

    return waitResponse(timeout_ms, GF(GSM_NL "+XTCPCONN: 1")) == 1;
    }

  int16_t modemSend(const void* buff, size_t len, uint8_t mux) {
    sendAT(GF("#XTCPSEND="));
    stream.write(reinterpret_cast<const uint8_t*>(buff), len);
    stream.flush();
    if (waitResponse(GF("#XTCPSEND")) != 1) {
      return 0;
    }
    return len;
  }

  size_t modemRead(size_t size, uint8_t mux) {
    // AT#XTCPRECV=<length>,<timeout> - timeout in seconds
    sendAT(GF("#XTCPRECV="), (uint16_t)size, ',', 15);
    if (waitResponse(GF("+XTCPRECV:")) != 1) {
      return 0;
    }
    int16_t len = streamGetIntBefore('\n');

    for (int i = 0; i < len; i++) { moveCharFromStreamToFifo(mux); }
    waitResponse();
    // DBG("### READ:", len, "from", mux);
    sockets[mux]->sock_available = modemGetAvailable(mux);
    return len;
  }

  size_t modemGetAvailable(uint8_t) { return 0; }

  bool modemGetConnected(uint8_t) {
    sendAT(GF("AT#XTCPCONN?"));

    if (waitResponse(GF("+XTCPCONN:")) != 1) {
      return false;
    }

    return streamGetIntBefore('\n');
  }

  /*
   * Utilities
   */
 public:
  // TODO(vshymanskyy): Optimize this!
  int8_t waitResponse(uint32_t timeout_ms, String& data,
                      GsmConstStr r1 = GFP(GSM_OK),
                      GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
#else
                      GsmConstStr r3 = NULL,
#endif
                      GsmConstStr r4 = NULL, GsmConstStr r5 = NULL) {
    /*String r1s(r1); r1s.trim();
    String r2s(r2); r2s.trim();
    String r3s(r3); r3s.trim();
    String r4s(r4); r4s.trim();
    String r5s(r5); r5s.trim();
    DBG("### ..:", r1s, ",", r2s, ",", r3s, ",", r4s, ",", r5s);*/
    data.reserve(64);
    uint8_t  index       = 0;
    uint32_t startMillis = millis();
    do {
      TINY_GSM_YIELD();
      while (stream.available() > 0) {
        TINY_GSM_YIELD();
        int8_t a = stream.read();
        if (a <= 0) continue;  // Skip 0x00 bytes, just in case
        data += static_cast<char>(a);
        if (r1 && data.endsWith(r1)) {
          index = 1;
          goto finish;
        } else if (r2 && data.endsWith(r2)) {
          index = 2;
          goto finish;
        } else if (r3 && data.endsWith(r3)) {
#if defined TINY_GSM_DEBUG
          if (r3 == GFP(GSM_CME_ERROR)) {
            streamSkipUntil('\n');  // Read out the error
          }
#endif
          index = 3;
          goto finish;
        } else if (r4 && data.endsWith(r4)) {
          index = 4;
          goto finish;
        } else if (r5 && data.endsWith(r5)) {
          index = 5;
          goto finish;
        }
      }
    } while (millis() - startMillis < timeout_ms);
  finish:
    if (!index) {
      data.trim();
      if (data.length()) { DBG("### Unhandled:", data); }
      data = "";
    }
    // data.replace(GSM_NL, "/");
    // DBG('<', index, '>', data);
    return index;
  }

  int8_t waitResponse(uint32_t timeout_ms, GsmConstStr r1 = GFP(GSM_OK),
                      GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
#else
                      GsmConstStr r3 = NULL,
#endif
                      GsmConstStr r4 = NULL, GsmConstStr r5 = NULL) {
    String data;
    return waitResponse(timeout_ms, data, r1, r2, r3, r4, r5);
  }

  int8_t waitResponse(GsmConstStr r1 = GFP(GSM_OK),
                      GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
#else
                      GsmConstStr r3 = NULL,
#endif
                      GsmConstStr r4 = NULL, GsmConstStr r5 = NULL) {
    return waitResponse(1000, r1, r2, r3, r4, r5);
  }

 protected:
  Stream&        stream;
  GsmClientnRF91* sockets[TINY_GSM_MUX_COUNT];
  const char*    gsmNL = GSM_NL;
};

#endif  // SRC_TINYGSMCLIENTNRF91_H_
