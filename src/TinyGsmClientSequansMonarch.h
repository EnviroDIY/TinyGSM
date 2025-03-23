/**
 * @file       TinyGsmClientSequansMonarch.h
 * @author     Michael Krumpus
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2019 Michael Krumpus
 * @date       Jan 2019
 */

#ifndef SRC_TINYGSMCLIENTSEQUANSMONARCH_H_
#define SRC_TINYGSMCLIENTSEQUANSMONARCH_H_

// #define TINY_GSM_DEBUG Serial

#define TINY_GSM_MUX_COUNT 6
#define TINY_GSM_SECURE_MUX_COUNT 6
// NOTE: Unlike most modules, the sockets are numbered starting at 1, not 0.
// Also supports 6 "security profiles" (1-6)
// The security profiles is collection of SSL settings, not the connection
// identifier.

#define TINY_GSM_SEND_MAX_SIZE 750
// SQNSSENDEXT accepts up to 1500 bytes of input, but this is configured to send
// HEX, so only 1/2 of that is available.

#define TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
#define TINY_GSM_MUX_STATIC
#ifdef AT_NL
#undef AT_NL
#endif
#define AT_NL "\r\n"

#ifdef MODEM_MANUFACTURER
#undef MODEM_MANUFACTURER
#endif
#define MODEM_MANUFACTURER "Sequans"

#ifdef MODEM_MODEL
#undef MODEM_MODEL
#endif
#define MODEM_MODEL "Monarch"

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"

// NOTE: This module supports SSL, but we do not support any certificate
// management yet. TINY_GSM_MODEM_HAS_SSL here and do no include the SSL module
// so as not to waste space.
#define TINY_GSM_MODEM_HAS_SSL

#include "TinyGsmGPRS.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmTemperature.tpp"

enum MonarchRegStatus {
  REG_NO_RESULT    = -1,
  REG_UNREGISTERED = 0,
  REG_SEARCHING    = 2,
  REG_DENIED       = 3,
  REG_OK_HOME      = 1,
  REG_OK_ROAMING   = 5,
  REG_UNKNOWN      = 4,
};

enum SocketStatus {
  SOCK_CLOSED                 = 0,
  SOCK_ACTIVE_DATA            = 1,
  SOCK_SUSPENDED              = 2,
  SOCK_SUSPENDED_PENDING_DATA = 3,
  SOCK_LISTENING              = 4,
  SOCK_INCOMING               = 5,
  SOCK_OPENING                = 6,
};

class TinyGsmSequansMonarch
    : public TinyGsmModem<TinyGsmSequansMonarch>,
      public TinyGsmGPRS<TinyGsmSequansMonarch>,
      public TinyGsmTCP<TinyGsmSequansMonarch, TINY_GSM_MUX_COUNT>,
      public TinyGsmCalling<TinyGsmSequansMonarch>,
      public TinyGsmSMS<TinyGsmSequansMonarch>,
      public TinyGsmTime<TinyGsmSequansMonarch>,
      public TinyGsmTemperature<TinyGsmSequansMonarch> {
  friend class TinyGsmModem<TinyGsmSequansMonarch>;
  friend class TinyGsmGPRS<TinyGsmSequansMonarch>;
  friend class TinyGsmTCP<TinyGsmSequansMonarch, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmCalling<TinyGsmSequansMonarch>;
  friend class TinyGsmSMS<TinyGsmSequansMonarch>;
  friend class TinyGsmTime<TinyGsmSequansMonarch>;
  friend class TinyGsmTemperature<TinyGsmSequansMonarch>;

  /*
   * Inner Client
   */
 public:
  class GsmClientSequansMonarch
      : public TinyGsmTCP<TinyGsmSequansMonarch,
                          TINY_GSM_MUX_COUNT>::GsmClient {
    friend class TinyGsmSequansMonarch;

   public:
    GsmClientSequansMonarch() {
      is_secure = false;
    }

    explicit GsmClientSequansMonarch(TinyGsmSequansMonarch& modem,
                                     uint8_t                mux = 1) {
      init(&modem, mux);
      is_secure = false;
    }

    bool init(TinyGsmSequansMonarch* modem, uint8_t mux = 1) {
      this->at       = modem;
      sock_available = 0;
      prev_check     = 0;
      sock_connected = false;
      got_data       = false;
      is_mid_send    = false;

      // adjust for zero indexed socket array vs Sequans' 1 indexed mux numbers
      // using modulus will force 6 back to 0
      if (mux >= 1 && mux <= TINY_GSM_MUX_COUNT) {
        this->mux = mux;
      } else {
        this->mux = (mux % TINY_GSM_MUX_COUNT) + 1;
      }
      at->sockets[this->mux % TINY_GSM_MUX_COUNT] = this;

      return true;
    }

   public:
    virtual int connect(const char* host, uint16_t port, int timeout_s) {
      is_mid_send = false;
      if (sock_connected) stop();
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    virtual void stop(uint32_t maxWaitMs) {
      is_mid_send = false;
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+SQNSH="), mux);
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
 public:
  class GsmClientSecureSequansMonarch : public GsmClientSequansMonarch {
    friend class TinyGsmSequansMonarch;

   public:
    GsmClientSecureSequansMonarch() {
      is_secure = true;
    }

    explicit GsmClientSecureSequansMonarch(TinyGsmSequansMonarch& modem,
                                           uint8_t                mux = 1)
        : GsmClientSequansMonarch(modem, mux) {
      is_secure = true;
    }

   protected:
    bool strictSSL = false;

   public:
    virtual int connect(const char* host, uint16_t port,
                        int timeout_s) override {
      stop();
      TINY_GSM_YIELD();
      rx.clear();

      // configure security profile 1 with parameters:
      if (strictSSL) {
        // require minimum of TLS 1.2 (3)
        // only support cipher suite 0x3D: TLS_RSA_WITH_AES_256_CBC_SHA256
        // verify server certificate against imported CA certs 0 and enforce
        // validity period (3)
        at->sendAT(GF("+SQNSPCFG=1,3,\"0x3D\",3,0,,,\"\",\"\""));
      } else {
        // use TLS 1.0 or higher (1)
        // support wider variety of cipher suites
        // do not verify server certificate (0)
        at->sendAT(GF("+SQNSPCFG=1,1,\"0x2F;0x35;0x3C;0x3D\",0,,,,\"\",\"\""));
      }
      if (at->waitResponse() != 1) {
        DBG("failed to configure security profile");
        return false;
      }

      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    void setStrictSSL(bool strict) {
      strictSSL = strict;
    }
  };

  /*
   * GSM Modem Constructor
   */
 public:
  explicit TinyGsmSequansMonarch(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = nullptr) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientSequansMonarch"));

    if (!testAT()) { return false; }

    sendAT(GF("E0"));  // Echo Off
    if (waitResponse() != 1) { return false; }

#ifdef TINY_GSM_DEBUG
    sendAT(GF("+CMEE=2"));  // turn on verbose error codes
#else
    sendAT(GF("+CMEE=0"));  // turn off error codes
#endif
    waitResponse();

    DBG(GF("### Modem:"), getModemName());

    // Make sure the module is enabled. Unlike others, the VZN20Q powers on
    // with CFUN=0 not CFUN=1 (that is, at minimum functionality instead of full
    // functionality The module cannot even detect the sim card if the cellular
    // functionality is disabled so unless we explicitly enable the
    // functionality the init will fail.
    sendAT(GF("+CFUN=1"));
    waitResponse();

    // Disable time and time zone URC's
    sendAT(GF("+CTZR=0"));
    if (waitResponse(10000L) != 1) { return false; }

    // Enable automatic time zome update
    sendAT(GF("+CTZU=1"));
    if (waitResponse(10000L) != 1) { return false; }

    SimStatus ret = getSimStatus();
    // if the sim isn't ready and a pin has been provided, try to unlock the sim
    if (ret != SIM_READY && pin != nullptr && strlen(pin) > 0) {
      simUnlock(pin);
      return (getSimStatus() == SIM_READY);
    } else {
      // if the sim is ready, or it's locked but no pin has been provided,
      // return true
      return (ret == SIM_READY || ret == SIM_LOCKED);
    }
  }

  bool factoryDefaultImpl() {
    sendAT(GF("&F0"));  // Factory
    waitResponse();
    sendAT(GF("Z"));  // default configuration
    waitResponse();
    sendAT(GF("+IPR=0"));  // Auto-baud
    return waitResponse() == 1;
  }

  void maintainImpl() {
    for (int mux = 1; mux <= TINY_GSM_MUX_COUNT; mux++) {
      GsmClientSequansMonarch* sock = sockets[mux % TINY_GSM_MUX_COUNT];
      if (sock && sock->got_data) {
        sock->got_data       = false;
        sock->sock_available = modemGetAvailable(mux);
        // modemGetConnected() always checks the state of ALL socks
        modemGetConnected(1);
      }
    }
    while (stream.available()) { waitResponse(15, nullptr, nullptr); }
  }

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin = nullptr) {
    if (!testAT()) { return false; }

    sendAT(GF("+CFUN=0"));
    int8_t res = waitResponse(20000L, GFP(GSM_OK), GFP(GSM_ERROR),
                              GF("+SYSSTART"));
    if (res != 1 && res != 3) { return false; }

    sendAT(GF("+CFUN=1,1"));
    res = waitResponse(20000L, GF("+SYSSTART"), GFP(GSM_ERROR));
    if (res != 1 && res != 3) { return false; }
    delay(1000);
    return init(pin);
  }

  bool powerOffImpl() {
    // NOTE:  The only way to turn the modem back on after this shutdown is with
    // a hard reset
    sendAT(GF("+SQNSSHDN"));
    return waitResponse();
  }

  //  When power saving is enabled, UART0 interface is activated with sleep mode
  //  support. Module power state is controlled by RTS0 line.  When no activity
  //  on UART, CTS line will be set to OFF state (driven high level) <timeout>
  //  milliseconds (100ms to 10s, default 5s) after the last sent character,
  //  then module will go to sleep mode as soon as DTE set RTS line to OFF state
  //  (driver high level).
  bool sleepEnableImpl(bool enable = true) {
    sendAT(GF("+SQNIPSCFG="), enable);
    return waitResponse() == 1;
  }

  bool setPhoneFunctionalityImpl(uint8_t fun, bool reset = false)
      TINY_GSM_ATTR_NOT_IMPLEMENTED;

  /*
   * Generic network functions
   */
 public:
  MonarchRegStatus getRegistrationStatus() {
    return (MonarchRegStatus)getRegistrationStatusXREG("CEREG");
  }

 protected:
  bool isNetworkConnectedImpl() {
    MonarchRegStatus s = getRegistrationStatus();
    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
  }
  String getLocalIPImpl() {
    sendAT(GF("+CGPADDR=3"));
    if (waitResponse(10000L, GF("+CGPADDR: 3,\"")) != 1) { return ""; }
    String res = stream.readStringUntil('\"');
    waitResponse();
    return res;
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // This module supports SSL, but there are no certificate management functions
  // are supported yet.
  // If you wish to add certificate management for this module you must (in
  // addition to adding the functions here):
  //  - Add `#include "TinyGsmSSL.tpp` to the top of the file
  //  - Remove `#define TINY_GSM_MODEM_HAS_SSL` from the top of the file
  //  - Add `public TinyGsmSSL<TinyGsmSequansMonarch, TINY_GSM_MUX_COUNT>,` to
  //    the constructor's initializer list
  //  - Add `friend class TinyGsmSSL<TinyGsmSequansMonarch,
  //    TINY_GSM_MUX_COUNT>;` to the friend list.
  //  - Make the secure client inherit from the secure client class in the SSL
  //  template.

  /*
   * WiFi functions
   */
  // No functions of this type supported

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* user = nullptr,
                       const char* pwd = nullptr) {
    gprsDisconnect();

    // Define the PDP context (This uses context #3!)
    sendAT(GF("+CGDCONT=3,\"IPV4V6\",\""), apn, '"');
    waitResponse();

    // Set authentication
    if (user && strlen(user) > 0) {
      sendAT(GF("+CGAUTH=3,1,\""), user, GF("\",\""), pwd, GF("\""));
      waitResponse();
    }

    // Activate the PDP context
    sendAT(GF("+CGACT=1,3"));
    waitResponse(60000L);

    // Attach to GPRS
    sendAT(GF("+CGATT=1"));
    if (waitResponse(60000L) != 1) { return false; }

    return true;
  }

  bool gprsDisconnectImpl() {
    // Detach from PS network
    sendAT(GF("+CGATT=0"));
    if (waitResponse(60000L) != 1) { return false; }
    // Dectivate all PDP contexts
    sendAT(GF("+CGACT=0"));
    if (waitResponse(60000L) != 1) { return false; }

    return true;
  }

  /*
   * SIM card functions
   */
 protected:
  String getSimCCIDImpl() {
    sendAT(GF("+SQNCCID"));
    if (waitResponse(GF(AT_NL "+SQNCCID:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  /*
   * Phone Call functions
   */
 protected:
  bool callAnswerImpl() TINY_GSM_ATTR_NOT_AVAILABLE;
  bool dtmfSendImpl(char cmd,
                    int  duration_ms = 100) TINY_GSM_ATTR_NOT_AVAILABLE;


  /*
   * Audio functions
   */
  // No functions of this type supported
  /*
   * Text messaging (SMS) functions
   */
  // Follows all text messaging (SMS) functions as inherited from TinyGsmSMS.tpp

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
  // Follows all clock functions as inherited from TinyGsmTime.tpp

  /*
   * NTP server functions
   */
  // No functions of this type supported

  /*
   * BLE functions
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
  // No functions of this type supportedp

  /*
   * Temperature functions
   */
 protected:
  float getTemperatureImpl() {
    sendAT(GF("+SMDTH"));
    if (waitResponse(10000L, GF("+SMDTH: ")) != 1) {
      return static_cast<float>(-9999);
    }
    String res;
    if (waitResponse(1000L, res) != 1) { return static_cast<float>(-9999); }
    if (res.indexOf("ERROR") >= 0) { return static_cast<float>(-9999); }
    return res.toFloat();
  }

  /*
   * Client related functions
   */
 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t mux,
                        int timeout_s) {
    int8_t   rsp;
    uint32_t timeout_ms  = ((uint32_t)timeout_s) * 1000;
    bool     ssl         = sockets[mux]->is_secure;
    uint32_t startMillis = millis();

    if (ssl) {
      // enable SSL and use security profile 1
      // AT+SQNSSCFG=<connId>,<enable>,<spId>
      sendAT(GF("+SQNSSCFG="), mux, GF(",1,1"));
      if (waitResponse() != 1) {
        DBG("### WARNING: failed to configure secure socket");
        return false;
      }
    }

    // Socket configuration
    // AT+SQNSCFG:<connId1>, <cid1>, <pktSz1>, <maxTo1>, <connTo1>, <txTo1>
    // <connId1> = Connection ID = mux
    // <cid1> = PDP context ID = 3 - this is number set up above in the
    // GprsConnect function
    // <pktSz1> = Packet Size, used for online data mode only = 300 (default)
    // <maxTo1> = Max timeout in seconds = 90 (default)
    // <connTo1> = Connection timeout in hundreds of milliseconds
    //           = 600 (default)
    // <txTo1> = Data sending timeout in hundreds of milliseconds,
    // used for online data mode only = 50 (default)
    sendAT(GF("+SQNSCFG="), mux, GF(",3,300,90,600,50"));
    waitResponse(5000L);

    // Socket configuration extended
    // AT+SQNSCFGEXT:<connId1>, <srMode1>, <recvDataMode1>, <keepalive1>,
    // <listenAutoRsp1>, <sendDataMode1>
    // <connId1> = Connection ID = mux
    // <srMode1> = Send/Receive URC model = 1 - data amount mode
    // <recvDataMode1> = Receive data mode = 0  - data as text (1 for hex)
    // <keepalive1> = unused = 0
    // <listenAutoRsp1> = Listen auto-response mode = 0 - deactivated
    // <sendDataMode1> = Send data mode = 1  - data as hex (0 for text)
    sendAT(GF("+SQNSCFGEXT="), mux, GF(",1,0,0,0,1"));
    waitResponse(5000L);

    // Socket dial
    // AT+SQNSD=<connId>,<txProt>,<rPort>,<IPaddr>[,<closureType>[,<lPort>[,<connMode>[,acceptAnyRemote]]]]
    // <connId> = Connection ID = mux
    // <txProt> = Transmission protocol = 0 - TCP (1 for UDP)
    // <rPort> = Remote host port to contact
    // <IPaddr> = Any valid IP address in the format xxx.xxx.xxx.xxx or any
    // host name solved with a DNS query
    // <closureType> = Socket closure behaviour for TCP, has no effect for UDP
    //               = 0 - local port closes when remote does (default)
    // <lPort> = UDP connection local port, has no effect for TCP connections.
    // <connMode> = Connection mode = 1 - command mode connection
    // <acceptAnyRemote> = Applies to UDP only
    sendAT(GF("+SQNSD="), mux, ",0,", port, ',', GF("\""), host, GF("\""),
           ",0,0,1");
    rsp = waitResponse((timeout_ms - (millis() - startMillis)), GFP(GSM_OK),
                       GFP(GSM_ERROR), GF("NO CARRIER" AT_NL));

    // creation of socket failed immediately.
    if (rsp != 1) { return false; }

    // wait until we get a good status
    bool connected = false;
    while (!connected && ((millis() - startMillis) < timeout_ms)) {
      connected = modemGetConnected(mux);
      delay(100);  // socket may be in opening state
    }
    return connected;
  }

  size_t modemSendImpl(const uint8_t* buff, size_t len, uint8_t mux) {
    if (sockets[mux % TINY_GSM_MUX_COUNT]->sock_connected == false) {
      DBG("### Sock closed, cannot send data!");
      return 0;
    }
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
        // Number of bytes to send from buffer in this command
        size_t sendLength = TINY_GSM_SEND_MAX_SIZE;
        // Ensure the program doesn't read past the allocated memory
        if (txPtr + TINY_GSM_SEND_MAX_SIZE > const_cast<uint8_t*>(buff) + len) {
          sendLength = const_cast<uint8_t*>(buff) + len - txPtr;
        }

        sendAT(GF("+SQNSSENDEXT="), mux, ',', (uint16_t)len);
        send_success &= waitResponse(10000L, GF(AT_NL "> ")) == 1;
        if (!send_success) {
          send_attempts++;
          continue;
        }
        // Translate bytes into char to be able to send them as an hex string
        char char_command[3];
        for (size_t i = 0; i < len; i++) {
          memset(&char_command, 0, sizeof(char_command));
          sprintf(&char_command[0], "%02X",
                  reinterpret_cast<const uint8_t*>(buff)[i]);
          stream.write(char_command, sizeof(char_command));
        }
        stream.flush();
        send_success &= waitResponse() != 1;
        bytesSent += sendLength;  // bump up number of bytes sent
        txPtr += sendLength;      // bump up the pointer
        send_attempts++;
      }
      // if we failed after 3 attempts at the same chunk, bail from the whole
      // thing
      if (!send_success) { break; }
    } while (bytesSent < len && sockets[mux]->sock_connected);
    return bytesSent;

    // uint8_t nAttempts = 5;
    // bool gotPrompt = false;
    // while (nAttempts > 0 && !gotPrompt) {
    //   sendAT(GF("+SQNSSEND="), mux);
    //   if (waitResponse(5000, GF(AT_NL "> ")) == 1) {
    //     gotPrompt = true;
    //   }
    //   nAttempts--;
    //   delay(50);
    // }
    // if (gotPrompt) {
    //   stream.write(reinterpret_cast<const uint8_t*>(buff), len);
    //   stream.write(reinterpret_cast<char>0x1A);
    //   stream.flush();
    //   if (waitResponse() != 1) {
    //     DBG("### no OK after send");
    //     return 0;
    //   }
    //   return len;
    // }
    // return 0;
  }
#if 0
  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    sendAT(GF("+SQNSSENDEXT="), mux, ',', (uint16_t)len);
    return waitResponse(10000L, GF(AT_NL "> ")) == 1;
  }
  size_t modemEndSendImpl(size_t len, uint8_t) {
    if (waitResponse() != 1) {
      DBG("### no OK after send");
      return 0;
    }
    return len;
  }
#endif

  size_t modemReadImpl(size_t size, uint8_t mux) {
    sendAT(GF("+SQNSRECV="), mux, ',', (uint16_t)size);
    if (waitResponse(GF("+SQNSRECV: ")) != 1) { return 0; }
    streamSkipUntil(',');  // Skip mux
    // TODO: validate mux
    int16_t len_reported = streamGetIntBefore('\n');
    size_t  len_read     = moveCharsFromStreamToFifo(mux % TINY_GSM_MUX_COUNT,
                                                     len_reported);
    waitResponse();
    sockets[mux % TINY_GSM_MUX_COUNT]->sock_available = modemGetAvailable(mux);
    return len_read;
  }

  size_t modemGetAvailableImpl(uint8_t mux) {
    sendAT(GF("+SQNSI="), mux);
    size_t result = 0;
    if (waitResponse(GF("+SQNSI:")) == 1) {
      streamSkipUntil(',');  // Skip mux
      // TODO: validate mux
      streamSkipUntil(',');              // Skip total sent
      streamSkipUntil(',');              // Skip total received
      result = streamGetIntBefore(',');  // keep data not yet read
      waitResponse();
    }
    // DBG("### Available:", result, "on", mux);
    return result;
  }

  bool modemGetConnectedImpl(uint8_t mux = 1) {
    // This single command always returns the connection status of all
    // six possible sockets.
    sendAT(GF("+SQNSS"));
    for (int muxNo = 1; muxNo <= TINY_GSM_MUX_COUNT; muxNo++) {
      if (waitResponse(GFP(GSM_OK), GF(AT_NL "+SQNSS: ")) != 2) { break; }
      uint8_t status = 0;
      // if (streamGetIntBefore(',') != muxNo) { // check the mux no
      //   DBG("### Warning: misaligned mux numbers!");
      // }
      streamSkipUntil(',');        // skip mux [use muxNo]
      status = stream.parseInt();  // Read the status
      // if mux is in use, will have comma then other info after the status
      // if not, there will be new line immediately after status
      // streamSkipUntil('\n'); // Skip port and IP info
      // SOCK_CLOSED                 = 0,
      // SOCK_ACTIVE_DATA            = 1,
      // SOCK_SUSPENDED              = 2,
      // SOCK_SUSPENDED_PENDING_DATA = 3,
      // SOCK_LISTENING              = 4,
      // SOCK_INCOMING               = 5,
      // SOCK_OPENING                = 6,
      GsmClientSequansMonarch* sock = sockets[muxNo % TINY_GSM_MUX_COUNT];
      if (sock) {
        sock->sock_connected = ((status != SOCK_CLOSED) &&
                                (status != SOCK_INCOMING) &&
                                (status != SOCK_OPENING));
      }
    }
    waitResponse();  // Should be an OK at the end
    return sockets[mux % TINY_GSM_MUX_COUNT]->sock_connected;
  }

  /*
   * Utilities
   */
 public:
  bool handleURCs(String& data) {
    if (data.endsWith(GF(AT_NL "+SQNSRING:"))) {
      int8_t  mux = streamGetIntBefore(',');
      int16_t len = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT &&
          sockets[mux % TINY_GSM_MUX_COUNT]) {
        sockets[mux % TINY_GSM_MUX_COUNT]->got_data       = true;
        sockets[mux % TINY_GSM_MUX_COUNT]->sock_available = len;
      }
      data = "";
      DBG("### URC Data Received:", len, "on", mux);
      return true;
    } else if (data.endsWith(GF("SQNSH: "))) {
      int8_t mux = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT &&
          sockets[mux % TINY_GSM_MUX_COUNT]) {
        sockets[mux % TINY_GSM_MUX_COUNT]->sock_connected = false;
      }
      data = "";
      DBG("### URC Sock Closed: ", mux);
      return true;
    }
    return false;
  }

 public:
  Stream& stream;

 protected:
  GsmClientSequansMonarch* sockets[TINY_GSM_MUX_COUNT];
  // AT_NL (\r\n) is not accepted with SQNSSENDEXT in data mode so use \n
  const char* gsmNL = "\n";
};

#endif  // SRC_TINYGSMCLIENTSEQUANSMONARCH_H_
