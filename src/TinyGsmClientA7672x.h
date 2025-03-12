/**
 * @file       TinyGsmClientA7672x.h
 * @author     Giovanni de Rosso Unruh
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2022 Giovanni de Rosso Unruh
 * @date       Oct 2022
 */

#ifndef SRC_TINYGSMCLIENTA7672X_H_
#define SRC_TINYGSMCLIENTA7672X_H_

// #define TINY_GSM_DEBUG Serial

#define TINY_GSM_MUX_COUNT 10
#define TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
#ifdef AT_NL
#undef AT_NL
#endif
#define AT_NL "\r\n"

#ifdef MODEM_MANUFACTURER
#undef MODEM_MANUFACTURER
#endif
#define MODEM_MANUFACTURER "SIMCom"

#ifdef MODEM_MODEL
#undef MODEM_MODEL
#endif
#define MODEM_MODEL "A7672x"

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmSSL.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmGSMLocation.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"
#include "TinyGsmBattery.tpp"
#include "TinyGsmTemperature.tpp"

enum A7672xRegStatus {
  REG_NO_RESULT    = -1,
  REG_UNREGISTERED = 0,
  REG_SEARCHING    = 2,
  REG_DENIED       = 3,
  REG_OK_HOME      = 1,
  REG_OK_ROAMING   = 5,
  REG_UNKNOWN      = 4,
};
class TinyGsmA7672X : public TinyGsmModem<TinyGsmA7672X>,
                      public TinyGsmGPRS<TinyGsmA7672X>,
                      public TinyGsmTCP<TinyGsmA7672X, TINY_GSM_MUX_COUNT>,
                      public TinyGsmSSL<TinyGsmA7672X, TINY_GSM_MUX_COUNT>,
                      public TinyGsmCalling<TinyGsmA7672X>,
                      public TinyGsmSMS<TinyGsmA7672X>,
                      public TinyGsmGSMLocation<TinyGsmA7672X>,
                      public TinyGsmTime<TinyGsmA7672X>,
                      public TinyGsmNTP<TinyGsmA7672X>,
                      public TinyGsmBattery<TinyGsmA7672X>,
                      public TinyGsmTemperature<TinyGsmA7672X> {
  friend class TinyGsmModem<TinyGsmA7672X>;
  friend class TinyGsmGPRS<TinyGsmA7672X>;
  friend class TinyGsmTCP<TinyGsmA7672X, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmSSL<TinyGsmA7672X, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmCalling<TinyGsmA7672X>;
  friend class TinyGsmSMS<TinyGsmA7672X>;
  friend class TinyGsmGSMLocation<TinyGsmA7672X>;
  friend class TinyGsmTime<TinyGsmA7672X>;
  friend class TinyGsmNTP<TinyGsmA7672X>;
  friend class TinyGsmBattery<TinyGsmA7672X>;
  friend class TinyGsmTemperature<TinyGsmA7672X>;

  /*
   * Inner Client
   */
 public:
  class GsmClientA7672X
      : public TinyGsmTCP<TinyGsmA7672X, TINY_GSM_MUX_COUNT>::GsmClient {
    friend class TinyGsmA7672X;

   public:
    GsmClientA7672X() {
      is_secure = false;
    }

    explicit GsmClientA7672X(TinyGsmA7672X& modem, uint8_t mux = 0) {
      init(&modem, mux);
      is_secure = false;
    }

    bool init(TinyGsmA7672X* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_available = 0;
      prev_check     = 0;
      sock_connected = false;
      got_data       = false;

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
      stop();
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    virtual void stop(uint32_t maxWaitMs) {
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+CIPCLOSE="), mux);
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
  class GsmClientSecureA7672X
      : public GsmClientA7672X,
        public TinyGsmSSL<TinyGsmA7672X, TINY_GSM_MUX_COUNT>::GsmSecureClient {
    friend class TinyGsmA7672X;

   public:
    GsmClientSecureA7672X() {
      is_secure = true;
    }

    explicit GsmClientSecureA7672X(TinyGsmA7672X& modem, uint8_t mux = 0)
        : GsmClientA7672X(modem, mux),
          TinyGsmSSL<TinyGsmA7672X, TINY_GSM_MUX_COUNT>::GsmSecureClient() {}

   public:
    bool loadCertificate(const char* certificateName, const char* cert,
                         const uint16_t len) {
      return at->loadCertificate(certificateName, cert, len);
    }

    bool deleteCertificate(const char* certificateName) {
      return at->deleteCertificate(certificateName);
    }

    virtual void stop(uint32_t maxWaitMs) {
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+CCHCLOSE="), mux);  //, GF(",1"));  // Quick close
      sock_connected = false;
      at->waitResponse();
    }
    void stop() override {
      stop(15000L);
    }
  };

  /*
   * GSM Modem Constructor
   */
 public:
  explicit TinyGsmA7672X(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = nullptr) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  A7672X"));

    if (!testAT(2000)) { return false; }

    // sendAT(GF("&FZ"));  // Factory + Reset
    // waitResponse();

    sendAT(GF("E0"));  // Echo Off
    if (waitResponse() != 1) { return false; }

#ifdef TINY_GSM_DEBUG
    sendAT(GF("V1"));  // turn on verbose error codes
#else
    sendAT(GF("V0"));  // turn off error codes
#endif
    waitResponse();

    DBG(GF("### Modem:"), getModemName());

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
    sendAT(GF("&F"));  // Factory + Reset
    waitResponse();
    sendAT(GF("+IFC=0,0"));  // No Flow Control
    waitResponse();
    sendAT(GF("+ICF=2,2"));  // 8 data 0 parity 1 stop
    waitResponse();
    sendAT(GF("+CSCLK=0"));  // Control UART Sleep always work
    waitResponse();
    sendAT(GF("&W"));  // Write configuration
    return waitResponse() == 1;
  }

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin = nullptr) {
    if (!testAT()) { return false; }
    sendAT(GF("+CRESET"));
    waitResponse();
    if (!setPhoneFunctionality(0)) { return false; }
    if (!setPhoneFunctionality(1, true)) { return false; }
    delay(3000);
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+CPOF"));
    return waitResponse(10000L) == 1;
  }

  //  This command is used to enable UART Sleep or always work. If set to 0,
  //  UART always work. If set to 1, ensure that DTR is pulled high and the
  //  module can go to DTR sleep. If set to 2, the module will enter RXsleep. RX
  //  wakeup directly sends data through the serial port (for example: AT) to
  //  wake up
  bool sleepEnableImpl(bool enable = true) {
    sendAT(GF("+CSCLK="),
           enable ? "2" : "1");  // 2: RXsleep (at wakeup) 1: DTR sleep
    return waitResponse() == 1;
  }

  // <fun> 0 minimum functionality
  // <fun> 1 full functionality, online mode
  // <fun> 4 disable phone both transmit and receive RF circuits
  // <fun> 5 Factory Test Mode (The A7600's 5 and 1 have the same function)
  // <fun> 6 Reset
  // <fun> 7 Offline Mode
  // <rst> 0 do not reset the ME before setting it to <fun> power level
  // <rst> 1 reset the ME before setting it to <fun> power level. This
  // valueonlytakes effect when <fun> equals 1
  bool setPhoneFunctionalityImpl(uint8_t fun, bool reset = false) {
    sendAT(GF("+CFUN="), fun, reset ? ",1" : ",0");
    return waitResponse(10000L) == 1;
  }

  /*
   * Generic network functions
   */
 public:
  A7672xRegStatus getRegistrationStatus() {
    return (A7672xRegStatus)getRegistrationStatusXREG("CREG");
  }

  String getLocalIPImpl() {
    if (hasSSL) {
      sendAT(GF("+CCHADDR"));
      if (waitResponse(GF("+CCHADDR:")) != 1) { return ""; }
    } else {
      sendAT(GF("+CGPADDR=1"));
      if (waitResponse(GF("+CGPADDR:")) != 1) { return ""; }
    }
    streamSkipUntil(',');  // Skip context id
    String res = stream.readStringUntil('\r');
    if (waitResponse() != 1) { return ""; }
    return res;
  }

 protected:
  bool isNetworkConnectedImpl() {
    A7672xRegStatus s = getRegistrationStatus();
    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
 protected:
  // The name of the certificate/key/password file. The file name must
  // have type like ".pem" or ".der".
  // The certificate like - const char ca_cert[] PROGMEM =  R"EOF(-----BEGIN...
  // len of certificate like - sizeof(ca_cert)
  // NOTE: Uploading the certificate only happens by filename, the type of
  // certificate does not matter here
  bool loadCertificateImpl(const char* certificateName, const char* cert,
                           const uint16_t len) {
    sendAT(GF("+CCERTDOWN="), certificateName, GF(","), len);
    if (waitResponse(GF(">")) != 1) { return false; }
    stream.write(cert, len);
    stream.flush();
    return waitResponse() == 1;
  }

  // NOTE: Deleting the certificate only happens by filename, the type of
  // certificate does not matter here
  bool deleteCertificateImpl(const char* certificateName) {  // todo test
    sendAT(GF("+CCERTDELE="), certificateName);
    return waitResponse() == 1;
  }

  // no certificate conversion needed
  bool convertCertificateImpl(CertificateType, const char*) {
    return true;
  }
  bool convertCACertificateImpl(const char*) {
    return true;
  }
  bool convertClientCertificatesImpl(const char*, const char*) {
    return true;
  }
  bool convertPSKandIDImpl(const char*, const char*) {
    return true;
  }
  bool convertPSKTableImpl(const char* psk_table_name) {
    return true;
  }

  /*
   * WiFi functions
   */
  // No functions of this type supported

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* = nullptr,
                       const char* = nullptr) {
    gprsDisconnect();

    // Define the PDP context
    sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, '"');
    waitResponse();

    // Activate the PDP context
    sendAT(GF("+CGACT=1,1"));
    waitResponse(60000L);

    // Attach to GPRS
    sendAT(GF("+CGATT=1"));
    if (waitResponse(60000L) != 1) { return false; }

    // Set to get data manually on TCP (unsecured) sockets
    sendAT(GF("+CIPRXGET=1"));
    if (waitResponse() != 1) { return false; }

    // Get Local IP Address, only assigned after connection
    sendAT(GF("+CGPADDR=1"));
    if (waitResponse(10000L) != 1) { return false; }

    // Configure Domain Name Server (DNS)
    sendAT(GF("+CDNSCFG=\"8.8.8.8\",\"8.8.4.4\""));
    if (waitResponse() != 1) { return false; }

    return true;
  }

  bool gprsDisconnectImpl() {
    // Shut the TCP/IP connection
    sendAT(GF("+NETCLOSE"));
    if (waitResponse(60000L) != 1) { return false; }

    sendAT(GF("+CGATT=0"));  // Detach from GPRS
    if (waitResponse(60000L) != 1) { return false; }

    return true;
  }

  String getProviderImpl() {
    sendAT(GF("+CSPN?"));
    if (waitResponse(GF("+CSPN:")) != 1) { return ""; }
    streamSkipUntil('"'); /* Skip mode and format */
    String res = stream.readStringUntil('"');
    waitResponse();
    return res;
  }

  /*
   * SIM card functions
   */
 protected:
  SimStatus getSimStatusImpl(uint32_t timeout_ms = 10000L) {
    for (uint32_t start = millis(); millis() - start < timeout_ms;) {
      sendAT(GF("+CPIN?"));
      if (waitResponse(GF("+CPIN:")) != 1) {
        delay(1000);
        continue;
      }
      int8_t status = waitResponse(GF("READY"), GF("SIM PIN"), GF("SIM PUK"),
                                   GF("SIM not inserted"), GF("SIM REMOVED"));
      waitResponse();
      switch (status) {
        case 2:
        case 3: return SIM_LOCKED;
        case 1: return SIM_READY;
        default: return SIM_ERROR;
      }
    }
    return SIM_ERROR;
  }

  String getSimCCIDImpl() {
    sendAT(GF("+CICCID"));
    if (waitResponse(GF(AT_NL "+ICCID:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  /*
   * Phone Call functions
   */
 public:
  bool setGsmBusy(bool busy = true) {
    sendAT(GF("+CCFC=1,"), busy ? 1 : 0);
    return waitResponse() == 1;
  }

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
 protected:
  float getTemperatureImpl() {
    String res = "";
    sendAT(GF("+CPMUTEMP"));
    if (waitResponse(1000L, res)) { return 0; }
    res        = res.substring(res.indexOf(':'), res.indexOf('\r'));
    float temp = res.toFloat();
    waitResponse();
    return temp;
  }

  /*
   * Client related functions
   */
 protected:
  bool modemConnect(const char* host, uint16_t port, uint8_t mux,
                    int timeout_s = 75) {
    int8_t   rsp;
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    bool     ssl        = sockets[mux]->is_secure;

    // +CTCPKA:<keepalive>,<keepidle>,<keepcount>,<keepinterval>
    sendAT(GF("+CTCPKA=1,2,5,1"));
    if (waitResponse(2000L) != 1) { return false; }

    if (ssl) {
      // If we have a secure socket, use a static cast to get the authentication
      // mode and certificate names. This isn't really "safe" but since we've
      // already checked that the socket is a secure one, we're pretty sure of
      // the type and it should work.
      GsmClientSecureA7672X* thisClient =
          static_cast<GsmClientSecureA7672X*>(sockets[mux]);
      SSLAuthMode sslAuthMode    = thisClient->sslAuthMode;
      SSLVersion  sslVersion     = thisClient->sslVersion;
      const char* CAcertName     = thisClient->CAcertName;
      const char* clientCertName = thisClient->clientCertName;
      const char* clientKeyName  = thisClient->clientKeyName;
      // const char* pskIdent       = thisClient->pskIdent;
      // const char* psKey          = thisClient->psKey;

      if (sslAuthMode == SSLAuthMode::PRE_SHARED_KEYS) {
        DBG("### The A7672x does not support SSL using pre-shared keys.");
        return false;
      }

      // NOTE: The SSL context (<ssl_ctx_index>) is not the same as the
      // connection identifier.  The SSL context is the grouping of SSL
      // settings, the connection identifier is the mux/socket number. For this,
      // we will *always* configure SSL context 0, just as we always configured
      // PDP context 1.

      hasSSL = true;
      // set the ssl version
      // AT+CSSLCFG="sslversion",<ssl_ctx_index>,<sslversion>
      // <ssl_ctx_index> The SSL context ID. The range is 0-9. We always use 0.
      // <sslversion> 0: SSL3.0
      //              1: TLS1.0
      //              2: TLS1.1
      //              3: TLS1.2
      //              4: All
      sendAT(GF("+CSSLCFG=\"sslversion\",0,"), static_cast<int8_t>(sslVersion));
      if (waitResponse(5000L) != 1) return false;

      // set authentication mode
      // AT+CSSLCFG="authmode",<ssl_ctx_index>,<authmode>
      // <ssl_ctx_index> The SSL context ID. The range is 0-9. We always use 0.
      // <authmode> 0: No authentication (SSLAuthMode::NO_VALIDATION)
      //            1: server authentication (SSLAuthMode::CA_VALIDATION)
      //            2: server and client authentication
      //            (SSLAuthMode::MUTUAL_AUTHENTICATION)
      //            3: client authentication and no server authentication
      //            (SSLAuthMode::CLIENT_VALIDATION)
      sendAT(GF("+CSSLCFG=\"authmode\",0,"), static_cast<int8_t>(sslAuthMode));
      if (waitResponse(5000L) != 1) return false;

      // apply the correct certificates to the connection
      if (CAcertName != nullptr &&
          (sslAuthMode == SSLAuthMode::CA_VALIDATION ||
           sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION)) {
        /* Configure the server root CA of the specified SSL context
        AT + CSSLCFG = "cacert", <ssl_ctx_index>,<ca_file> */
        sendAT(GF("+CSSLCFG=\"cacert\",0,"), CAcertName);
        if (waitResponse(5000L) != 1) return false;
      }
      if (clientCertName != nullptr &&
          (sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION ||
           sslAuthMode == SSLAuthMode::CLIENT_VALIDATION)) {
        sendAT(GF("+CSSLCFG=\"clientcert\",0,"), clientCertName);
        if (waitResponse(5000L) != 1) return false;
      }
      if (clientKeyName != nullptr &&
          (sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION ||
           sslAuthMode == SSLAuthMode::CLIENT_VALIDATION)) {
        sendAT(GF("+CSSLCFG=\"clientkey\",0,"), clientKeyName);
        if (waitResponse(5000L) != 1) return false;
      }

      // set the SSL SNI (server name indication)
      // AT+CSSLCFG="enableSNI",<ssl_ctx_index>,<enableSNI_flag>
      sendAT(GF("+CSSLCFG=\"enableSNI\",0,1"));
      if (waitResponse(2000L) != 1) { return false; }

      // Configure the report mode of sending and receiving data
      // +CCHSET=<report_send_result>,<recv_mode>
      // <report_send_result> Whether to report result of CCHSEND, the default
      //   value is 0: 0 No. 1 Yes. Module will report +CCHSEND:
      // <session_id>,<err> to MCU when complete sending data.
      // <recv_mode> The receiving mode, the default value is 0:
      //   0 Output the data to MCU whenever received data.
      //   1 Module caches the received data and notifies MCU with+CCHEVENT:
      // <session_id>,RECV EVENT. MCU can use AT+CCHRECV to receive the cached
      //   data (only in manual receiving mode).
      sendAT(GF("+CCHSET=1,1"));
      if (waitResponse(2000L) != 1) { return false; }

      // Start SSL service
      sendAT(GF("+CCHSTART"));
      if (waitResponse(2000L) != 1) { return false; }

      // set the connection identifier that the above SSL context settings
      // apply to (ie, tie connection mux to SSL context)
      // AT+CCHSSLCFG=<session_id>,<ssl_ctx_index>
      sendAT(GF("+CCHSSLCFG="), mux, GF(",0"));
      if (waitResponse(2000L) != 1) { return false; }

      // Connect to server
      // AT+CCHOPEN=<session_id>,<host>,<port>[,<client_type>,[<bind_port>]]
      sendAT(GF("+CCHOPEN="), mux, GF(",\""), host, GF("\","), port, GF(",2"));
      // The reply is OK or ERROR followed by +CCHOPEN: <session_id>,<err> where
      // <session_id> is the mux number and <err> should be 0 if there's no
      // error
      rsp = waitResponse(timeout_ms);  // capture the OK or ERROR
      rsp &= waitResponse(timeout_ms, GF(AT_NL "+CCHOPEN:")) != 1;
      // TODO: verify this
    } else {
      sendAT(GF("+NETOPEN"));
      if (waitResponse(2000L) != 1) { return false; }

      sendAT(GF("+NETOPEN?"));
      if (waitResponse(2000L) != 1) { return false; }

      // AT+CIPOPEN=<link_num>,"TCP",<serverIP>,<serverPort>[,<localPort>]
      sendAT(GF("+CIPOPEN="), mux, ',', GF("\"TCP"), GF("\",\""), host,
             GF("\","), port);
      // The reply is OK or ERROR followed by +CIPOPEN: <link_num>,<err> where
      // <link_num> is the mux number and <err> should be 0 if there's no
      // error
      // There may also be an ERROR returned after the +CIPOPEN: line if the PDP
      // context wasn't activated first. We ignore this case.
      rsp = waitResponse(timeout_ms);  // capture the OK or ERROR
      if (rsp) { rsp &= waitResponse(timeout_ms, GF(AT_NL "+CIPOPEN:")) != 1; }
    }

    // Since both CIPOPEN and CCHOPEN return the same response, we can handle it
    // here
    if (rsp) {
      uint8_t opened_mux    = streamGetIntBefore(',');
      uint8_t opened_result = streamGetIntBefore('\n');
      if (opened_mux != mux || opened_result != 0) return false;
    }
    return true;
  }

  int16_t modemSend(const void* buff, size_t len, uint8_t mux) {
    if (!sockets[mux]) return 0;
    bool ssl = sockets[mux]->is_secure;
    if (ssl) {
      sendAT(GF("+CCHSEND="), mux, ',', (uint16_t)len);
    } else {
      sendAT(GF("+CIPSEND="), mux, ',', (uint16_t)len);
    }
    if (waitResponse(GF(">")) != 1) { return 0; }
    stream.write(reinterpret_cast<const uint8_t*>(buff), len);
    stream.flush();

    if (waitResponse() != 1) { return 0; }

    if (ssl) {
      // Because we set CCHSET to return the send result, we should get a
      // +CCHSEND: <session_id>,<err>
      if (waitResponse(10000L, GF("+CCHSEND:"), GF("ERROR" AT_NL),
                       GF("CLOSE OK" AT_NL)) != 1) {
        return 0;
      }
      streamSkipUntil(',');  // skip the outgoing mux
      // TODO: validate mux
      streamSkipUntil('\n');  // skip the error code
      return len;
    } else {
      // after OK, returns +CIPSEND: <link_num>,<reqSendLength>,<cnfSendLength>
      if (waitResponse(GF(AT_NL "+CIPSEND:")) != 1) { return 0; }
      streamSkipUntil(',');  // Skip mux
      // TODO:  verify the returned mux
      streamSkipUntil(',');  // Skip requested bytes to send
      // TODO:  make sure requested and confirmed bytes match
      return streamGetIntBefore('\n');
    }
  }

  size_t modemRead(size_t size, uint8_t mux) {
    if (!sockets[mux]) return 0;
    bool    ssl           = sockets[mux]->is_secure;
    int16_t len_returned  = 0;
    int16_t len_remaining = 0;
#ifdef TINY_GSM_USE_HEX
    if (ssl) {
      // it does not appear to be possible to send/recieve hex data on SSL
      // sockets
      return 0;
    }
    // <mode> - 3 – read data in HEX form, the max read length is 750
    int8_t rx_mode = 3;
#else
    // <mode> - 2 – read data in ASCII, the max read length is 1500
    int8_t rx_mode = 2;
#endif
    if (ssl) {
      // AT+CCHRECV=<session_id>[,<max_recv_len>]
      sendAT(GF("+CCHRECV="), mux, ',', (uint16_t)size);
      // response is +CCHRECV: DATA, <session_id>,<len>\n<data>
      if (waitResponse(GF("+CCHRECV:")) != 1) { return 0; }
      streamSkipUntil(',');  // Skip the word "DATA"
      streamSkipUntil(',');  // Skip mux/cid (connecion id)
      // TODO: validate mux
      len_returned = streamGetIntBefore('\n');
    } else {
      sendAT(GF("+CIPRXGET="), rx_mode, ',', mux, ',', (uint16_t)size);
      if (waitResponse(GF("+CIPRXGET:")) != 1) { return 0; }
      streamSkipUntil(',');  // Skip Rx mode 2/normal or 3/HEX
      streamSkipUntil(',');  // Skip mux/cid (connecion id)
      // TODO: verify the mux/cid number
      len_returned = streamGetIntBefore(',');
      // ^^ Integer type, the length of data that has been read.
      len_remaining = streamGetIntBefore('\n');
      // ^^ Integer type, the length of data which has not been read in the
      // buffer.
    }
    for (int i = 0; i < len_returned; i++) {
      uint32_t startMillis = millis();
#ifdef TINY_GSM_USE_HEX
      while (stream.available() < 2 &&
             (millis() - startMillis < sockets[mux]->_timeout)) {
        TINY_GSM_YIELD();
      }
      char buf[4] = {
          0,
      };
      buf[0] = stream.read();
      buf[1] = stream.read();
      char c = strtol(buf, NULL, 16);
#else
      while (!stream.available() &&
             (millis() - startMillis < sockets[mux]->_timeout)) {
        TINY_GSM_YIELD();
      }
      char c = stream.read();
#endif
      sockets[mux]->rx.put(c);
    }
    // DBG("### READ:", len_returned, " bytes from connection ", mux);
    if (ssl) {
      // we need to check how much is left after the read
      sockets[mux]->sock_available = modemGetAvailable(mux);
    } else {
      // the read call already told us how much is left
      sockets[mux]->sock_available = len_remaining;
    }
    waitResponse();
    return len_returned;
  }

  size_t modemGetAvailable(uint8_t mux) {
    if (!sockets[mux]) return 0;
    bool   ssl    = sockets[mux]->is_secure;
    size_t result = 0;
    if (ssl) {
      // NOTE: Only two SSL sockets are supported (0 and 1) and AT+CCHRECV?
      // returns the number of characters availalable on both.
      sendAT(GF("+CCHRECV?"));
      // +CCHRECV: LEN,<cache_len_0>,<cache_len_1>
      // <cache_len_0> = The length of RX data cached for connection 0.
      // <cache_len_1> = The length of RX data cached for connection 1.
      if (waitResponse(GF(AT_NL "+CCHRECV: ")) != 1) { return 0; }
      streamSkipUntil(',');                        // Skip the text "LEN"
      size_t len_on_0 = streamGetIntBefore(',');   // read cache_len_0
      size_t len_on_1 = streamGetIntBefore('\n');  // read cache_len_1

      // set the sock available for both sockets (if they exist)
      if (mux == 1) {
        result                = len_on_1;
        GsmClientA7672X* sock = sockets[mux];
        if (sock) { sock->sock_available = len_on_1; }
      } else if (mux == 0) {
        result                = len_on_0;
        GsmClientA7672X* sock = sockets[mux];
        if (sock) { sock->sock_available = len_on_0; }
      } else {
        DBG("### ERROR: Invalid mux number");
        result = 0;
      }
    } else {
      sendAT(GF("+CIPRXGET=4,"), mux);
      if (waitResponse(GF("+CIPRXGET:")) == 1) {
        streamSkipUntil(',');  // Skip returned mode (4)
        streamSkipUntil(',');  // Skip mux
        // TODO(?): verify the mux number
        result = streamGetIntBefore('\n');
      }
    }
    waitResponse();  // final ok
    // DBG("### Available:", result, "on", mux);
    if (result == 0) { sockets[mux]->sock_connected = modemGetConnected(mux); }
    return result;
  }

  bool modemGetConnected(uint8_t mux) {
    int8_t res = 0;
    bool   ssl = sockets[mux]->is_secure;
    if (ssl) {
      bool connected = this->sockets[mux]->sock_connected;
      // DBG("### Connected:", connected);
      return connected;
    } else {
      sendAT(GF("+CIPACK="), mux);
      waitResponse(GF("+CIPACK:"));
      res = waitResponse(2000L);  //(GF(",\"CONNECTED\""), GF(",\"CLOSED\""),
                                  // GF(",\"CLOSING\""), GF(",\"REMOTE
                                  // CLOSING\""), GF(",\"INITIAL\""));
      waitResponse();
    }
    return 1 == res;
  }

  /*
   * Utilities
   */
 public:
  bool handleURCs(String& data) {
    if (data.endsWith(GF(AT_NL "+CIPRXGET:"))) {
      int8_t mode = streamGetIntBefore(',');
      if (mode == 1) {
        int8_t mux = streamGetIntBefore('\n');
        if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
          sockets[mux]->got_data = true;
        }
        data = "";
        DBG("### Got Data:", mux);
        return true;
      } else {
        data += mode;
        return false;
      }
    } else if (data.endsWith(GF("RECV EVENT" AT_NL))) {
      sendAT(GF("+CCHRECV?"));
      String res = "";
      waitResponse(2000L, res);
      int8_t  mux = res.substring(res.lastIndexOf(',') + 1).toInt();
      int16_t len =
          res.substring(res.indexOf(',') + 1, res.lastIndexOf(',')).toInt();
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->got_data = true;
        if (len >= 0 && len <= 1024) { sockets[mux]->sock_available = len; }
      }
      data = "";
      DBG("### Got Data:", len, "on", mux);
      return true;
    } else if (data.endsWith(GF("+CCHRECV: 0,0" AT_NL))) {
      int8_t mux = data.substring(data.lastIndexOf(',') + 1).toInt();
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->sock_connected = true;
      }
      data = "";
      DBG("### ACK:", mux);
      return true;
    } else if (data.endsWith(GF("+IPCLOSE:"))) {
      int8_t mux = streamGetIntBefore(',');
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      data = "";
      streamSkipUntil('\n');
      DBG("### TCP Closed: ", mux);
      return true;
    } else if (data.endsWith(GF("+CCHCLOSE:"))) {
      int8_t mux = streamGetIntBefore(',');
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      data = "";
      streamSkipUntil('\n');
      DBG("### SSL Closed: ", mux);
      return true;
    } else if (data.endsWith(GF("+CCH_PEER_CLOSED:"))) {
      int8_t mux = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      data = "";
      DBG("### SSL Closed: ", mux);
      return true;
    } else if (data.endsWith(GF("*PSNWID:"))) {
      streamSkipUntil('\n');  // Refresh network name by network
      data = "";
      DBG("### Network name updated.");
      return true;
    } else if (data.endsWith(GF("*PSUTTZ:"))) {
      streamSkipUntil('\n');  // Refresh time and time zone by network
      data = "";
      DBG("### Network time and time zone updated.");
      return true;
    } else if (data.endsWith(GF("+CTZV:"))) {
      streamSkipUntil('\n');  // Refresh network time zone by network
      data = "";
      DBG("### Network time zone updated.");
      return true;
    } else if (data.endsWith(GF("DST:"))) {
      streamSkipUntil('\n');  // Refresh Network Daylight Saving Time by network
      data = "";
      DBG("### Daylight savings time state updated.");
      return true;
    }
    return false;
  }

 public:
  Stream& stream;

 protected:
  GsmClientA7672X* sockets[TINY_GSM_MUX_COUNT];
  bool             hasSSL = false;
};

#endif  // SRC_TINYGSMCLIENTA7672X_H_
