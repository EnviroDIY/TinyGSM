/**
 * @file       TinyGsmClientBC95G.h
 * @author     Volodymyr Shymanskyy, Pacman Pereira, and Replicade Ltd.
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy, (c)2017 Replicade Ltd.
 * <http://www.replicade.com>
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCLIENTBC95G_H_
#define SRC_TINYGSMCLIENTBC95G_H_
// #pragma message("TinyGSM:  TinyGsmClientBC95G")

// #define TINY_GSM_DEBUG Serial

#define TINY_GSM_MUX_COUNT 7
#define TINY_GSM_BUFFER_READ_NO_CHECK

#include "TinyGsmBattery.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmModem.tpp"
#include "TinyGsmSMS.tpp"
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
  REG_NO_RESULT    = -1,
  REG_UNREGISTERED = 0,
  REG_SEARCHING    = 2,
  REG_DENIED       = 3,
  REG_OK_HOME      = 1,
  REG_OK_ROAMING   = 5,
  REG_UNKNOWN      = 4,
};

class TinyGsmBC95G : public TinyGsmModem<TinyGsmBC95G>,
                   public TinyGsmGPRS<TinyGsmBC95G>,
                   public TinyGsmTCP<TinyGsmBC95G, TINY_GSM_MUX_COUNT>,
                   public TinyGsmSMS<TinyGsmBC95G>,
                   public TinyGsmTime<TinyGsmBC95G>,
                   public TinyGsmBattery<TinyGsmBC95G>,
                   public TinyGsmTemperature<TinyGsmBC95G> {
  friend class TinyGsmModem<TinyGsmBC95G>;
  friend class TinyGsmGPRS<TinyGsmBC95G>;
  friend class TinyGsmTCP<TinyGsmBC95G, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmSMS<TinyGsmBC95G>;
  friend class TinyGsmTime<TinyGsmBC95G>;
  friend class TinyGsmBattery<TinyGsmBC95G>;
  friend class TinyGsmTemperature<TinyGsmBC95G>;

  /*
   * Inner Client
   */
 public:
  class GsmClientBC95G : public GsmClient {
    friend class TinyGsmBC95G;

   public:
    GsmClientBC95G() {}

    explicit GsmClientBC95G(TinyGsmBC95G& modem, uint8_t mux = 0) {
      init(&modem, mux);
    }

    bool init(TinyGsmBC95G* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_available = 0;
      sock_connected = false;

      if (mux < TINY_GSM_MUX_COUNT) {
        this->mux = mux;
      } else {
        this->mux = (mux % TINY_GSM_MUX_COUNT);
      }
      at->sockets[this->mux] = this;

      return true;
    }

   public:
    virtual int connect(const char* host, uint16_t port, int timeout_s) {
      // stop();  // DON'T stop!
      TINY_GSM_YIELD();
      rx.clear();

      uint8_t oldMux = mux;
      sock_connected = at->modemConnect(host, port, &mux, timeout_s);
      if (mux != oldMux) {
        DBG("WARNING:  Mux number changed from", oldMux, "to", mux);
        at->sockets[oldMux] = NULL;
      }
      at->sockets[mux] = this;
      at->maintain();

      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    void stop(uint32_t maxWaitMs) {
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+NSOCL="), mux);
      at->waitResponse();  // should return within 1s
      sock_connected = false;
    }
    void stop() override { stop(15000L); }

    /*
     * Extended API
     */

    String remoteIP() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  };

  /*
   * Constructor
   */
 public:
  explicit TinyGsmBC95G(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = NULL) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);

    if (!testAT()) { return false; }

    // sendAT(GF("&FZ"));  // Factory + Reset
    // waitResponse();

    sendAT(GF("E0"));  // Echo Off
    if (waitResponse() != 1) { return false; }

#ifdef TINY_GSM_DEBUG
    sendAT(GF("+CMEE=1"));  // turn on verbose error codes
#else
    sendAT(GF("+CMEE=0"));  // turn off error codes
#endif
    waitResponse();

    DBG(GF("### Modem:"), getModemName());

    // Disable registration function of Huawei’s IoT platform
    // When Huawei's IoT platform is not used, the registration function of
    // Huawei's IoT platform needs to be disabled before the module is connected
    // to the network.
    sendAT(GF("+QREGSWT=2"));
    if (waitResponse(10000L) != 1) {
      return false;
    }

    // Disable time and time zone URC's
    sendAT(GF("+CTZR=0"));
    if (waitResponse(10000L) != 1) {
      return false;
    }

    // Enable automatic time zone update
    sendAT(GF("+NITZ=1"));
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

  void setBaudImpl(uint32_t baud) {
    // AT+NATSPEED=<baud_rate>, <timeout>, <store>, <sync_mode>
    // [, <stopbits>[, <parity>[, <xonxoff>]]]
    // <baud_rate> - NOTE:  <baud_rate> value higher than the fastest speed
    // supported by the low power UART (9600) will disable deep sleep low power
    // operation.
    // <timeout> = time to wait for communication before switching back to the
    // original speed (0 = default of 3 seconds)
    // <store> = 1 -store value to NV memory so value is preserved after reboot
    // <sync_mode> = synchronization on start bit (2 = default = sample earlier)
    // <stopbits> = number stop bits (1)
    // <parity> = 0 for no parity (1=odd, 2=even)
    // <xonxoff> = AT UART Software (XON/XOFF) Flow Control 0/1 dis/enabled
    sendAT(GF("+NATSPEED="), baud, GF("0,1,2,1,0,0"));
    waitResponse();
  }

  String getModemNameImpl() {
    sendAT(GF("+CGMI"));
    String res1;
    if (waitResponse(1000L, res1) != 1) {
      return "unknown";
    }
    res1.replace("\r\nOK\r\n", "");
    res1.replace("\rOK\r", "");
    res1.trim();

    sendAT(GF("+CGMM"));  // NOTE:  **C**GMM
    String res2;
    if (waitResponse(1000L, res2) != 1) {
      return "unknown";
    }
    res2.replace("\r\nOK\r\n", "");
    res2.replace("\rOK\r", "");
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
  bool restartImpl() {
    if (!testAT()) { return false; }
    sendAT(GF("+NRB"));
    if (waitResponse(10000L, GF("REBOOTING")) != 1) {
      return false;
    }
    // Boot: Unsigned
    // Security B.. Verified
    // Protocol A.. Verified
    // Apps A...... Verified

    // REBOOT_CAUSE_APPLICATION_AT
    // Neul
    // OK
    if (waitResponse(30000L) != 1) {
      return false;
    }
    return init();
  }

  // Module can only be powered down by actually turning off the power
  // Use power saving mode instead
  bool powerOffImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

// Sleep is dependent on network connection timers
  bool sleepEnableImpl(bool enable = true) {
    // Enable SIM power saving
    sendAT(GF("+NCONFIG:NAS_SIM_POWER_SAVING_ENABLE,"),
           enable ? GF("TRUE") : GF("FALSE"));
    waitResponse();
    // Enable power saving mode
    sendAT(GF("+CPSMS="), enable);
    return waitResponse() == 1;
  }

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

  // Should be able to use CGPADDR
  // String getLocalIPImpl() {
  //   sendAT(GF("+NIPINFO"));
  //   if (waitResponse(10000L, GF("+NIPINFO")) != 1) {
  //     return "";
  //   }
  //   streamSkipUntil(',');  // skip cid
  //   streamSkipUntil(',');  // skip IP type
  //   String res = stream.readStringUntil('\n');
  //   res.trim();
  //   return res;
  // }

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* user = NULL,
                       const char* pwd = NULL) {
    gprsDisconnect();

    // Define the PDP context (This module ONLY supports context 0!)
    sendAT(GF("+CGDCONT=0,\"IPV4V6\",\""), apn, '"');
    waitResponse();

    // Set authentication
    if (user && strlen(user) > 0) {
      sendAT(GF("+CGAUTH=1,1,\""), user, GF("\",\""), pwd, GF("\""));
      waitResponse();
    }

    // Activate the PDP context
    sendAT(GF("+CGACT=1,1"));
    waitResponse(60000L);

    // Attach to GPRS
    sendAT(GF("+CGATT=1"));
    if (waitResponse(60000L) != 1) {
      return false;
    }

    return true;
  }

  bool gprsDisconnectImpl() {
    sendAT(GF("+CGATT=0"));
    if (waitResponse(60000L) != 1) {
      return false;
    }

    return true;
  }

  /*
   * SIM card functions
   */
 protected:
  String getSimCCIDImpl() {
    sendAT(GF("+NCCID"));
    if (waitResponse(GF(GSM_NL "+NCCID:")) != 1) {
      return "";
    }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  String getIMEIImpl() {
    sendAT(GF("+CGSN"));
    streamSkipUntil('\n');  // skip first newline
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  /*
   * Messaging functions
   */
 protected:
  String sendUSSDImpl(const String& code) TINY_GSM_ATTR_NOT_AVAILABLE;

  bool sendSMSImpl(const String& number,
                   const String& text) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  bool sendSMS_UTF16Impl(const char* const number, const void* text,
                         size_t len) TINY_GSM_ATTR_NOT_AVAILABLE;

  /*
   * Time functions
   */
 protected:
  // Can follow the standard CCLK function in the template

  /*
   * Battery functions
   */
  // Use: float vBatt = modem.getBattVoltage() / 1000.0;
  uint16_t getBattVoltageImpl() {
    sendAT(GF("+QCHIPINFO=VBAT"));
    if (waitResponse(GF("+QCHIPINFO:VBAT,")) != 1) {
      return 0;
    }
    // return voltage in mV
    uint16_t res = streamGetIntBefore('\n');
    // Wait for final OK
    waitResponse();
    return res;
  }

  int8_t getBattPercentImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  uint8_t getBattChargeStateImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool getBattStatsImpl(uint8_t& chargeState, int8_t& percent,
                        uint16_t& milliVolts) {
    chargeState = 0;
    percent = 0;
    milliVolts = getBattVoltageImpl();
    // Wait for final OK
    waitResponse();
    return true;
  }

  /*
   * Temperature functions
   */
 protected:
  float getTemperatureImpl() {
    sendAT(GF("+QCHIPINFO=TEMP"));
    if (waitResponse(GF(GSM_NL "+QCHIPINFO=TEMP,")) != 1) {
      return static_cast<float>(-9999);
    }
    float temp = streamGetFloatBefore('\n');
    // Wait for final OK
    waitResponse();
    return temp;
  }

  /*
   * Client related functions
   */
 protected:
  bool modemConnect(const char* host, uint16_t port, uint8_t* mux,
                    int timeout_s = 120) {
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    uint32_t startMillis = millis();

    // Ask for a URD in format of "+NSONMI:<socket>,<length>"
    sendAT(GF("+NSONMI=1"));
    waitResponse();

    // create a socket
    // AT+NSOCR=<type>,<protocol>,<listenport>[,
    // <receive control>[,<af_type>[,<ip address>]]]
    // <type> = DGRAM or STREAM for UDP or TCP, respectively
    // <protocol> = 17 or 6 for UDP or TCP, respectively
    // <listenport> = if omitted, a random port is assigned
    // <receive control> = default to 1 => accepting incoming messages
    // <af_type> = "AF_INET" for IPv4 (default) or "AF_INET6" for IPv6
    // <ip address> = IP address assigned by network to UE
    sendAT(GF("+NSOCR=STREAM,6,0,1"));
    // reply is bare ## of socket created
    *mux = streamGetIntBefore('\n');
    waitResponse();

    // connect on the allocated socket
    sendAT(GF("+NSOCO="), *mux, ",\"", host, "\",", port);
    int8_t rsp = waitResponse(timeout_ms - (millis() - startMillis));
    return (1 == rsp);
  }

  int16_t modemSend(const void* buff, size_t len, uint8_t mux) {
    // There's no request for a prompt here; the data to write is put right into
    // the AT command
    stream.print(GF("+NSOSD="));
    stream.print(mux);
    stream.print(',');
    stream.print((uint16_t)len);
    stream.print(GF(",\""));
    stream.write(reinterpret_cast<const uint8_t*>(buff), len);
    stream.println('\"');
    stream.flush();
    streamSkipUntil(',');  // Skip mux
    int16_t sent = streamGetIntBefore('\n');
    waitResponse();  // sends back OK after the confirmation of number sent
    return sent;
  }

  size_t modemRead(size_t size, uint8_t mux) {
    // AT+NSORF=<socket>,<req_length>
    sendAT(GF("+NSORF="), mux, ',', (uint16_t)size);
    if (waitResponse(GF(GSM_NL "+USORD:")) != 1) {
      return 0;
    }
    // Response:  <socket>,<ip_addr>,<port>,<length>,<data>,<remaining_length>
    streamSkipUntil(',');  // Skip mux
    streamSkipUntil(',');  // Skip IP address
    streamSkipUntil(',');  // Skip port
    int16_t len = streamGetIntBefore(',');  // get length

    // Read the data
    streamSkipUntil('\"');  // Skip opening quote
    for (int i = 0; i < len; i++) {
      moveCharFromStreamToFifo(mux);
    }
    streamSkipUntil('\"');  // Skip closing quote
    streamSkipUntil(',');  // Skip comma
    // DBG("### READ:", len, "from", mux);
    sockets[mux]->sock_available = streamGetIntBefore('\n');
    waitResponse();
    return len;
  }

  // I don't think this is possible
  size_t modemGetAvailable(uint8_t) { return 0; }

  // Must have firmware revision >= 14 for this to work
  bool modemGetConnected(uint8_t mux) {
    // AT+NSOSTATUS=<socket id>
    sendAT(GF("+NSOSTATUS="), mux);
    if (waitResponse(GF(GSM_NL "+NSOSTATUS:")) != 1) {
      return false;
    }
    streamSkipUntil(',');  // Skip mux
    // <status> 0 - Available
    // 1 - Not exist.  This status is also returned if the socket is not bound to
    // the correct IP address NW assigned
    // 2 - Flow control
    // 3 - Back off
    return streamGetIntBefore('\n') == 0;
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
    uint8_t index = 0;
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
        } else if (data.endsWith(GF("+NSONMI:"))) {
          int8_t mux = streamGetIntBefore(',');
          int16_t len = streamGetIntBefore('\n');
          if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
            sockets[mux]->got_data = true;
            // max size is 1358
            if (len >= 0 && len <= 1358) {
              sockets[mux]->sock_available = len;
            }
          }
          data = "";
          DBG("### URC Data Received:", len, "on", mux);
        } else if (data.endsWith(GF("+NSOCLI:"))) {
          int8_t mux = streamGetIntBefore('\n');
          if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
            sockets[mux]->sock_connected = false;
          }
          data = "";
          DBG("### URC Sock Closed: ", mux);
        }
      }
    } while (millis() - startMillis < timeout_ms);
  finish:
    if (!index) {
      data.trim();
      if (data.length()) {
        DBG("### Unhandled:", data);
      }
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
  Stream& stream;
  GsmClientBC95G* sockets[TINY_GSM_MUX_COUNT];
  const char* gsmNL = GSM_NL;
};

#endif  // SRC_TINYGSMCLIENTBC95G_H_
