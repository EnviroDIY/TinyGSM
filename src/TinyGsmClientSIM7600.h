/**
 * @file       TinyGsmClientSIM7600.h
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCLIENTSIM7600_H_
#define SRC_TINYGSMCLIENTSIM7600_H_

// #define TINY_GSM_DEBUG Serial
// #define TINY_GSM_USE_HEX

#define TINY_GSM_MUX_COUNT 10
#define TINY_GSM_SECURE_MUX_COUNT 2
// SRGD Note: I think these two numbers are independent of each other and
// managed completely differently.  That is, I think there can be two connection
// 0's, one using the SSL application on the module and the other using the TCP
// application on the module.
// TODO(?) Could someone who has this module test this?

#define TINY_GSM_SEND_MAX_SIZE 1500
// CCHSEND can handle up to 2048 bytes of input, but CIPSEND will only accept
// 1500, so we'll take the smaller number

// #define TINY_GSM_DEFAULT_SSL_CTX 0

#define TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
#define TINY_GSM_MUX_STATIC
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
#if defined(TINY_GSM_MODEM_SIM7500)
#define MODEM_MODEL "SIM7500";
#elif defined(TINY_GSM_MODEM_SIM7800)
#define MODEM_MODEL "SIM7800";
#else
#define MODEM_MODEL "SIM7600";
#endif

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmSSL.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmGSMLocation.tpp"
#include "TinyGsmGPS.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"
#include "TinyGsmBattery.tpp"
#include "TinyGsmTemperature.tpp"


enum SIM7600RegStatus {
  REG_NO_RESULT    = -1,
  REG_UNREGISTERED = 0,
  REG_SEARCHING    = 2,
  REG_DENIED       = 3,
  REG_OK_HOME      = 1,
  REG_OK_ROAMING   = 5,
  REG_UNKNOWN      = 4,
};

class TinyGsmSim7600 : public TinyGsmModem<TinyGsmSim7600>,
                       public TinyGsmGPRS<TinyGsmSim7600>,
                       public TinyGsmTCP<TinyGsmSim7600, TINY_GSM_MUX_COUNT>,
                       public TinyGsmSSL<TinyGsmSim7600, TINY_GSM_MUX_COUNT>,
                       public TinyGsmSMS<TinyGsmSim7600>,
                       public TinyGsmGSMLocation<TinyGsmSim7600>,
                       public TinyGsmGPS<TinyGsmSim7600>,
                       public TinyGsmTime<TinyGsmSim7600>,
                       public TinyGsmNTP<TinyGsmSim7600>,
                       public TinyGsmBattery<TinyGsmSim7600>,
                       public TinyGsmTemperature<TinyGsmSim7600>,
                       public TinyGsmCalling<TinyGsmSim7600> {
  friend class TinyGsmModem<TinyGsmSim7600>;
  friend class TinyGsmGPRS<TinyGsmSim7600>;
  friend class TinyGsmTCP<TinyGsmSim7600, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmSSL<TinyGsmSim7600, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmSMS<TinyGsmSim7600>;
  friend class TinyGsmGPS<TinyGsmSim7600>;
  friend class TinyGsmGSMLocation<TinyGsmSim7600>;
  friend class TinyGsmTime<TinyGsmSim7600>;
  friend class TinyGsmNTP<TinyGsmSim7600>;
  friend class TinyGsmBattery<TinyGsmSim7600>;
  friend class TinyGsmTemperature<TinyGsmSim7600>;
  friend class TinyGsmCalling<TinyGsmSim7600>;

  /*
   * Inner Client
   */
 public:
  class GsmClientSim7600
      : public TinyGsmTCP<TinyGsmSim7600, TINY_GSM_MUX_COUNT>::GsmClient {
    friend class TinyGsmSim7600;

   public:
    GsmClientSim7600() {
      is_secure = false;
    }

    explicit GsmClientSim7600(TinyGsmSim7600& modem, uint8_t mux = 0) {
      init(&modem, mux);
      is_secure = false;
    }

    bool init(TinyGsmSim7600* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_available = 0;
      prev_check     = 0;
      sock_connected = false;
      got_data       = false;
      is_mid_send    = false;

      // The SIM7600 series generally lets you choose the mux number, but we
      // want to try to find an empty place in the socket array for it.

      // TODO: Ensure the secure socket mux isn't out of range

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
  class GsmClientSecureSim7600 : public GsmClientSim7600,
                                 public GsmSecureClient {
    friend class TinyGsmSim7600;

   public:
    TINY_GSM_SECURE_CLIENT_CTORS(Sim7600)

    virtual int connect(const char* host, uint16_t port, int timeout_s) {
      stop();
      TINY_GSM_YIELD();
      rx.clear();
      if (!sslCtxConfigured) {
        if (sslAuthMode == SSLAuthMode::PRE_SHARED_KEYS) {
          DBG("### The SIM7600 does not support SSL using pre-shared keys.");
          sslCtxConfigured = false;
        } else {
          sslCtxConfigured = at->configureSSLContext(
              sslCtxIndex, sslAuthMode, sslVersion, CAcertName, clientCertName,
              clientKeyName);
        }
      }
      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    virtual void stop(uint32_t maxWaitMs) override {
      is_mid_send = false;
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+CCHCLOSE="), mux);
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
  explicit TinyGsmSim7600(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = nullptr) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientSIM7600"));

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

  bool factoryDefaultImpl() {  // these commands aren't supported
    return false;
  }

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin = nullptr) {
    if (!testAT()) { return false; }
    sendAT(GF("+CRESET"));
    if (waitResponse(10000L) != 1) { return false; }
    delay(16000L);
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+CPOF"));
    return waitResponse() == 1;
  }

  bool radioOffImpl() {
    if (!setPhoneFunctionality(4)) { return false; }
    delay(3000);
    return true;
  }

  bool sleepEnableImpl(bool enable = true) {
    sendAT(GF("+CSCLK="), enable);
    return waitResponse() == 1;
  }

  bool setPhoneFunctionalityImpl(uint8_t fun, bool reset = false) {
    sendAT(GF("+CFUN="), fun, reset ? ",1" : "");
    return waitResponse(10000L) == 1;
  }

  /*
   * Generic network functions
   */
 public:
  SIM7600RegStatus getRegistrationStatus() {
    return (SIM7600RegStatus)getRegistrationStatusXREG("CGREG");
  }

 protected:
  bool isNetworkConnectedImpl() {
    SIM7600RegStatus s = getRegistrationStatus();
    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
  }

 public:
  String getNetworkModes() {
    // Get the help string, not the setting value
    sendAT(GF("+CNMP=?"));
    if (waitResponse(GF(AT_NL "+CNMP:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    return res;
  }

  int16_t getNetworkMode() {
    sendAT(GF("+CNMP?"));
    if (waitResponse(GF(AT_NL "+CNMP:")) != 1) { return false; }
    int16_t mode = streamGetIntBefore('\n');
    waitResponse();
    return mode;
  }

  bool setNetworkMode(uint8_t mode) {
    sendAT(GF("+CNMP="), mode);
    return waitResponse() == 1;
  }

  bool getNetworkSystemMode(bool& n, int16_t& stat) {
    // n: whether to automatically report the system mode info
    // stat: the current service. 0 if it not connected
    sendAT(GF("+CNSMOD?"));
    if (waitResponse(GF(AT_NL "+CNSMOD:")) != 1) { return false; }
    n    = streamGetIntBefore(',') != 0;
    stat = streamGetIntBefore('\n');
    waitResponse();
    return true;
  }

  String getLocalIPImpl() {
    sendAT(GF("+IPADDR"));  // Inquire Socket PDP address
    // sendAT(GF("+CGPADDR=1"));  // Show PDP address
    String res;
    if (waitResponse(10000L, res) != 1) { return ""; }
    cleanResponseString(res);
    res.trim();
    return res;
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
 public:
  bool loadCertificateImpl(const char* certificateName, const char* cert,
                           const uint16_t len) {
    sendAT(GF("+CCERTDOWN=\""), certificateName, GF("\","), len);
    if (!waitResponse(GF(">"))) { return false; }
    stream.write(cert, len);
    stream.flush();
    return waitResponse() == 1;
  }

  bool deleteCertificateImpl(const char* certificateName) {
    sendAT(GF("+CCERTDELE=\""), certificateName, GF("\""));
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
  bool convertPSKTableImpl(const char*) {
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
  bool gprsConnectImpl(const char* apn, const char* user = nullptr,
                       const char* pwd = nullptr) {
    gprsDisconnect();  // Make sure we're not connected first

    // Define the PDP context

    // The CGDCONT commands set up the "external" PDP context

    // Set the external authentication
    if (user && strlen(user) > 0) {
      sendAT(GF("+CGAUTH=1,0,\""), pwd, GF("\",\""), user, '"');
      waitResponse();
    }

    // Define external PDP context 1
    sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, '"', ",\"0.0.0.0\",0,0");
    waitResponse();

    // Configure TCP parameters

    // Select TCP/IP application mode (command mode)
    sendAT(GF("+CIPMODE=0"));
    waitResponse();

    // Set Sending Mode - send without waiting for peer TCP ACK
    sendAT(GF("+CIPSENDMODE=0"));
    waitResponse();

    // Configure socket parameters
    // AT+CIPCCFG= <NmRetry>, <DelayTm>, <Ack>, <errMode>, <HeaderType>,
    //            <AsyncMode>, <TimeoutVal>
    // NmRetry = number of retransmission to be made for an IP packet
    //         = 10 (default)
    // DelayTm = number of milliseconds to delay before outputting received data
    //          = 0 (default)
    // Ack = sets whether reporting a string "Send ok" = 0 (don't report)
    // errMode = mode of reporting error result code = 0 (numberic values)
    // HeaderType = which data header of receiving data in multi-client mode
    //            = 1 (+RECEIVE,<link num>,<data length>)
    // AsyncMode = sets mode of executing commands
    //           = 0 (synchronous command executing)
    // TimeoutVal = minimum retransmission timeout in milliseconds = 75000
    sendAT(GF("+CIPCCFG=10,0,0,0,1,0,75000"));
    if (waitResponse() != 1) { return false; }

    // Configure timeouts for opening and closing sockets
    // AT+CIPTIMEOUT=<netopen_timeout> <cipopen_timeout>, <cipsend_timeout>
    sendAT(GF("+CIPTIMEOUT="), 75000, ',', 15000, ',', 15000);
    waitResponse();

    // Set to get data manually on TCP (unsecured) sockets
    sendAT(GF("+CIPRXGET=1"));
    if (waitResponse() != 1) { return false; }

    // Start the (unsecured) socket service

    // This activates and attaches to the external PDP context that is tied
    // to the embedded context for TCP/IP (ie AT+CGACT=1,1 and AT+CGATT=1)
    // Response may be an immediate "OK" followed later by "+NETOPEN: 0".
    // We to ignore any immediate response and wait for the
    // URC to show it's really connected.
    sendAT(GF("+NETOPEN"));
    if (waitResponse(75000L, GF(AT_NL "+NETOPEN: 0")) != 1) { return false; }

    // Set the module to require manual reading of rx buffer data on SSL sockets

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
    waitResponse(5000L);

    // Start the SSL client (doesn't mean we have to use it!)
    sendAT(GF("+CCHSTART"));  // Start the SSL client
    if (waitResponse(5000L) != 1) return false;

    return true;
  }

  bool gprsDisconnectImpl() {
    // Close all sockets and stop the socket service
    // Note: On the LTE models, this single command closes all sockets and the
    // service and deactivates the PDP context
    sendAT(GF("+NETCLOSE"));
    waitResponse(60000L, GF(AT_NL "+NETCLOSE: 0"));

    // We assume this works, so we can do SSL disconnect too
    // stop the SSL client
    sendAT(GF("+CCHSTOP"));
    return (waitResponse(60000L, GF(AT_NL "+CCHSTOP: 0")) != 1);

    // TODO: Should CCHSTOP come before NETCLOSE?  Is it needed in addition to
    // NETCLOSE?
  }

  bool isGprsConnectedImpl() {
    sendAT(GF("+NETOPEN?"));
    // May return +NETOPEN: 1, 0.  We just confirm that the first number is 1
    if (waitResponse(GF(AT_NL "+NETOPEN: 1")) != 1) { return false; }
    waitResponse();

    sendAT(GF("+IPADDR"));  // Inquire Socket PDP address
    // sendAT(GF("+CGPADDR=1")); // Show PDP address
    if (waitResponse() != 1) { return false; }

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
  // Gets the CCID of a sim card via AT+CCID
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
 protected:
  bool callHangupImpl() {
    sendAT(GF("+CHUP"));
    return waitResponse() == 1;
  }

  /*
   * Audio functions
   */
  //  No functions of this type supported

  /*
   * Text messaging (SMS) functions
   */
  // Follows all text messaging (SMS) functions as inherited from TinyGsmSMS.tpp

  /*
   * GSM Location functions
   */
  // Follows all GSM-based location functions as inherited from
  // TinyGsmGSMLocation.tpp

  /*
   * GPS/GNSS/GLONASS location functions
   */
 protected:
  // enable GPS
  bool enableGPSImpl() {
    sendAT(GF("+CGPS=1"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  bool disableGPSImpl() {
    sendAT(GF("+CGPS=0"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  // get the RAW GPS output
  String getGPSrawImpl() {
    sendAT(GF("+CGNSSINFO"));
    if (waitResponse(GF(AT_NL "+CGNSSINFO:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  // get GPS informations
  bool getGPSImpl(float* lat, float* lon, float* speed = 0, float* alt = 0,
                  int* vsat = 0, int* usat = 0, float* accuracy = 0,
                  int* year = 0, int* month = 0, int* day = 0, int* hour = 0,
                  int* minute = 0, int* second = 0) {
    sendAT(GF("+CGNSSINFO"));
    if (waitResponse(GF(AT_NL "+CGNSSINFO:")) != 1) { return false; }

    uint8_t fixMode = streamGetIntBefore(',');  // mode 2=2D Fix or 3=3DFix
                                                // TODO(?) Can 1 be returned
    if (fixMode == 1 || fixMode == 2 || fixMode == 3) {
      // init variables
      float ilat = 0;
      char  north;
      float ilon = 0;
      char  east;
      float ispeed       = 0;
      float ialt         = 0;
      int   ivsat        = 0;
      int   iusat        = 0;
      float iaccuracy    = 0;
      int   iyear        = 0;
      int   imonth       = 0;
      int   iday         = 0;
      int   ihour        = 0;
      int   imin         = 0;
      float secondWithSS = 0;

      streamSkipUntil(',');               // GPS satellite valid numbers
      streamSkipUntil(',');               // GLONASS satellite valid numbers
      streamSkipUntil(',');               // BEIDOU satellite valid numbers
      ilat  = streamGetFloatBefore(',');  // Latitude in ddmm.mmmmmm
      north = stream.readStringUntil(',').charAt(
          0);                            // N/S Indicator, N=north or S=south
      ilon = streamGetFloatBefore(',');  // Longitude in ddmm.mmmmmm
      east = stream.readStringUntil(',').charAt(
          0);  // E/W Indicator, E=east or W=west

      // Date. Output format is ddmmyy
      iday   = streamGetIntLength(2);    // Two digit day
      imonth = streamGetIntLength(2);    // Two digit month
      iyear  = streamGetIntBefore(',');  // Two digit year

      // UTC Time. Output format is hhmmss.s
      ihour = streamGetIntLength(2);  // Two digit hour
      imin  = streamGetIntLength(2);  // Two digit minute
      secondWithSS =
          streamGetFloatBefore(',');  // 4 digit second with subseconds

      ialt   = streamGetFloatBefore(',');  // MSL Altitude. Unit is meters
      ispeed = streamGetFloatBefore(',');  // Speed Over Ground. Unit is knots.
      streamSkipUntil(',');                // Course Over Ground. Degrees.
      iaccuracy = streamGetFloatBefore(',');  // Position Dilution Of Precision
      streamSkipUntil(',');   // Horizontal Dilution Of Precision
      streamSkipUntil('\n');  // Vertical Dilution Of Precision

      // Set pointers
      if (lat != nullptr)
        *lat = (floor(ilat / 100) + fmod(ilat, 100.) / 60) *
            (north == 'N' ? 1 : -1);
      if (lon != nullptr)
        *lon = (floor(ilon / 100) + fmod(ilon, 100.) / 60) *
            (east == 'E' ? 1 : -1);
      if (speed != nullptr) *speed = ispeed;
      if (alt != nullptr) *alt = ialt;
      if (vsat != nullptr) *vsat = ivsat;
      if (usat != nullptr) *usat = iusat;
      if (accuracy != nullptr) *accuracy = iaccuracy;
      if (iyear < 2000) iyear += 2000;
      if (year != nullptr) *year = iyear;
      if (month != nullptr) *month = imonth;
      if (day != nullptr) *day = iday;
      if (hour != nullptr) *hour = ihour;
      if (minute != nullptr) *minute = imin;
      if (second != nullptr) *second = static_cast<int>(secondWithSS);

      waitResponse();
      return true;
    }

    waitResponse();
    return false;
  }

  /**
   *  CGNSSMODE: <gnss_mode>,<dpo_mode>
   *  This command is used to configure GPS, GLONASS, BEIDOU and QZSS support
   * mode. 0 : GLONASS 1 : BEIDOU 2 : GALILEO 3 : QZSS dpo_mode: 1 enable , 0
   * disable
   */
  String setGNSSModeImpl(uint8_t mode, bool dpo) {
    String res;
    sendAT(GF("+CGNSSMODE="), mode, ",", dpo);
    if (waitResponse(10000L, res) != 1) { return ""; }
    res.replace(AT_NL, "");
    res.trim();
    return res;
  }

  uint8_t getGNSSModeImpl() {
    sendAT(GF("+CGNSSMODE?"));
    if (waitResponse(GF(AT_NL "+CGNSSMODE:")) != 1) { return 0; }
    return stream.readStringUntil(',').toInt();
  }


  /*
   * Time functions
   */
  // Follows all clock functions as inherited from TinyGsmTime.tpp

  /*
   * NTP server functions
   */
  // Follows all NTP server functions as inherited from TinyGsmNTP.tpp

  /*
   * BLE functions
   */
  // No functions of this type supported

  /*
   * Battery functions
   */
 protected:
  // returns volts, multiply by 1000 to get mV
  int16_t getBattVoltageImpl() {
    sendAT(GF("+CBC"));
    if (waitResponse(GF(AT_NL "+CBC:")) != 1) { return 0; }

    // get voltage in VOLTS
    float voltage = streamGetFloatBefore('\n');
    // Wait for final OK
    waitResponse();
    // Return millivolts
    uint16_t res = voltage * 1000;
    return res;
  }

  int8_t getBattPercentImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  int8_t getBattChargeStateImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool getBattStatsImpl(int8_t& chargeState, int8_t& percent,
                        int16_t& milliVolts) {
    chargeState = 0;
    percent     = 0;
    milliVolts  = getBattVoltage();
    return true;
  }

  /*
   * Temperature functions
   */
 protected:
  // get temperature in degree celsius
  float getTemperatureImpl() {
    sendAT(GF("+CPMUTEMP"));
    if (waitResponse(GF(AT_NL "+CPMUTEMP:")) != 1) { return 0; }
    // return temperature in C
    float res = streamGetIntBefore('\n');
    // Wait for final OK
    waitResponse();
    return res;
  }

  /*
   * Client related functions
   */
 public:
  bool configureSSLContext(uint8_t context_id, SSLAuthMode sslAuthMode,
                           SSLVersion sslVersion, const char* CAcertName,
                           const char* clientCertName,
                           const char* clientKeyName) {
    bool success = true;

    // List the certs available
    //   sendAT(GF("+CCERTLIST"));
    //   waitResponse(5000L);

    // NOTE: The SSL context (<ssl_ctx_index>) is not the same as the
    // connection identifier.  The SSL context is the grouping of SSL
    // settings, the connection identifier is the mux/socket number. For this,
    // we will *always* configure SSL context 0, just as we always configured
    // PDP context 1.
    // CSSLCFG commands reference the SSL context number; C**A**SSLCFG
    // commands reference the connection number (aka, the mux).

    // set the ssl version
    // AT+CSSLCFG="sslversion",<ssl_ctx_index>,<sslversion>
    // <ssl_ctx_index> The SSL context ID. The range is 0-9. We always use 0.
    // <sslversion> 0: SSL3.0
    //              1: TLS1.0
    //              2: TLS1.1
    //              3: TLS1.2
    //              4: All
    if (static_cast<int8_t>(sslVersion) < 0 ||
        static_cast<int8_t>(sslVersion) > 4) {
      // Not supported; select "ALL" and hope for the best
      sslVersion = SSLVersion::ALL_SSL;
    }
    sendAT(GF("+CSSLCFG=\"sslversion\","), context_id, GF(","),
           static_cast<int8_t>(sslVersion));
    success &= waitResponse(5000L) == 1;

    // apply the correct certificates to the connection
    if (CAcertName != nullptr &&
        (sslAuthMode == SSLAuthMode::CA_VALIDATION ||
         sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION)) {
      /* Configure the server root CA of the specified SSL context
      AT + CSSLCFG = "cacert", <ssl_ctx_index>,<ca_file> */
      sendAT(GF("+CSSLCFG=\"cacert\","), context_id, GF(","), CAcertName);
      success &= waitResponse(5000L) == 1;
    }
    if (clientCertName != nullptr &&
        (sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION ||
         sslAuthMode == SSLAuthMode::CLIENT_VALIDATION)) {
      sendAT(GF("+CSSLCFG=\"clientcert\","), context_id, GF(","),
             clientCertName);
      success &= waitResponse(5000L) == 1;
    }
    if (clientKeyName != nullptr &&
        (sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION ||
         sslAuthMode == SSLAuthMode::CLIENT_VALIDATION)) {
      sendAT(GF("+CSSLCFG=\"clientkey\","), context_id, GF(","), clientKeyName);
      success &= waitResponse(5000L) == 1;
    }

    // set authentication mode
    // AT+CSSLCFG="authmode",<ssl_ctx_index>,<authmode>
    // <ssl_ctx_index> The SSL context ID. The range is 0-9. We always use 0.
    // <authmode> 0: No authentication (SSLAuthMode::NO_VALIDATION)
    //            1: server authentication (SSLAuthMode::CA_VALIDATION)
    //            2: server and client authentication
    //            (SSLAuthMode::MUTUAL_AUTHENTICATION)
    //            3: client authentication and no server authentication
    //            (SSLAuthMode::CLIENT_VALIDATION)
    sendAT(GF("+CSSLCFG=\"authmode\","), context_id, GF(","),
           static_cast<int8_t>(sslAuthMode));
    success &= waitResponse(5000L) == 1;

    return success;
  }

  bool linkSSLContext(uint8_t mux, uint8_t context_id) {
    // set the configured SSL context for the session
    // AT+CCHSSLCFG=<session_id>,<ssl_ctx_index>
    sendAT(GF("+CCHSSLCFG="), mux, ',', context_id);
    return waitResponse(5000L) == 1;
  }

 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t mux,
                        int timeout_s) {
    int8_t   rsp;
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    bool     ssl        = sockets[mux]->is_secure;

    if (ssl) {
      // If we have a secure socket, use a static cast to get the authentication
      // mode and certificate names. This isn't really "safe" but since we've
      // already checked that the socket is a secure one, we're pretty sure of
      // the type and it should work.
      GsmClientSecureSim7600* thisClient =
          static_cast<GsmClientSecureSim7600*>(sockets[mux]);
      uint8_t sslCtxIndex = thisClient->sslCtxIndex;


      linkSSLContext(mux, sslCtxIndex);

      // Establish a connection in multi-socket mode
      // AT+CCHOPEN=<session_id>,"<host>",<port>[,<client_type>,[<bind_port>]]
      // TODO: Should we specify the client_type as 2=SSL/TLS?
      sendAT(GF("+CCHOPEN="), mux, GF(",\""), host, GF("\","), port);
      // The reply is OK or ERROR followed by +CCHOPEN: <session_id>,<err> where
      // <session_id> is the mux number and <err> should be 0 if there's no
      // error
      rsp = waitResponse(timeout_ms);  // capture the OK or ERROR
      rsp &= waitResponse(timeout_ms, GF(AT_NL "+CCHOPEN:")) != 1;
      // TODO: verify this
    } else {
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


  // NOTE: The implementations of modemSend(...), modemRead(...), and
  // modemGetAvailable(...) are almost completely different for SSL and
  // unsecured sockets.
  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    if (!sockets[mux]) return 0;
    bool ssl = sockets[mux]->is_secure;
    if (ssl) {
      sendAT(GF("+CCHSEND="), mux, ',', (uint16_t)len);
    } else {
      sendAT(GF("+CIPSEND="), mux, ',', (uint16_t)len);
    }
    return waitResponse(GF(">")) == 1;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  int16_t modemEndSendImpl(uint16_t len, uint8_t mux) {
    if (waitResponse() != 1) { return 0; }
    bool ssl = sockets[mux]->is_secure;
    if (ssl) {
      // Because we set CCHSET to return the send result, we should get a
      // +CCHSEND: <session_id>,<err>
      if (waitResponse(10000L, GF("+CCHSEND:"), GF("ERROR" AT_NL),
                       GF("CLOSE OK" AT_NL)) != 1) {
        return 0;
      }
      uint8_t ret_mux = streamGetIntBefore(',');        // check mux
      bool    result  = streamGetIntBefore('\n') == 0;  // check error code
      if (ret_mux == mux && result) { return len; }
      return 0;
    } else {
      // after OK, returns +CIPSEND: <link_num>,<reqSendLength>,<cnfSendLength>
      if (waitResponse(GF(AT_NL "+CIPSEND:")) != 1) { return 0; }
      uint8_t ret_mux = streamGetIntBefore(',');  // check mux
      streamSkipUntil(',');  // Skip requested bytes to send
      // TODO(?):  make sure requested and confirmed bytes match
      int16_t sent = streamGetIntBefore('\n');  // check send length
      if (mux == ret_mux) { return sent; }
      return 0;
    }
  }

  size_t modemReadImpl(size_t size, uint8_t mux) {
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
      //+CIPRXGET: <mode>,<link_num>,<read_len>,<rest_len><data_in_hex>
      //<read_len> Integer type, the length of data that has been read.
      //<rest_len> Integer type, the length of data which has not been read in
      // the buffer.
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
      // Returns +CCHRECV: {mux},0 after the data
      String await_response = "+CCHRECV: " + String(mux) + ",0";
      waitResponse(GFP(await_response.c_str()));
      // we need to check how much is left after the read
      sockets[mux]->sock_available = (uint16_t)modemGetAvailable(mux);
    } else {
      // the read call already told us how much is left
      sockets[mux]->sock_available = len_remaining;
      waitResponse();
    }
    return len_returned;
  }

  size_t modemGetAvailableImpl(uint8_t mux) {
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
        result                 = len_on_1;
        GsmClientSim7600* sock = sockets[mux];
        if (sock) { sock->sock_available = len_on_1; }
      } else if (mux == 0) {
        result                 = len_on_0;
        GsmClientSim7600* sock = sockets[mux];
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

  bool modemGetConnectedImpl(uint8_t mux) {
    // TODO(SRGD): I think this only returns the TCP socket connection status,
    // not the SSL connection status
    // Read the status of all sockets at once
    sendAT(GF("+CIPOPEN?"));
    if (waitResponse(GF("+CIPOPEN:")) != 1) { return false; }
    for (int muxNo = 0; muxNo < TINY_GSM_MUX_COUNT; muxNo++) {
      // +CIPOPEN:<mux>,<State or blank...>
      String state = stream.readStringUntil('\n');
      if (state.indexOf(',') > 0) { sockets[muxNo]->sock_connected = true; }
    }
    waitResponse();  // Should be an OK at the end
    if (!sockets[mux]) return false;
    return sockets[mux]->sock_connected;
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
        // DBG("### Got Data:", mux);
        return true;
      } else {
        data += mode;
        return false;
      }
    } else if (data.endsWith(GF(AT_NL "+RECEIVE:"))) {
      int8_t  mux = streamGetIntBefore(',');
      int16_t len = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->got_data = true;
        if (len >= 0 && len <= 1024) { sockets[mux]->sock_available = len; }
      }
      data = "";
      // DBG("### Got Data:", len, "on", mux);
      return true;
    } else if (data.endsWith(GF("+IPCLOSE:"))) {
      int8_t mux = streamGetIntBefore(',');
      streamSkipUntil('\n');  // Skip the reason code
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      data = "";
      DBG("### Closed: ", mux);
      return true;
    } else if (data.endsWith(GF("+CIPEVENT:"))) {
      // Need to close all open sockets and release the network library.
      // User will then need to reconnect.
      DBG("### Network error!");
      if (!isGprsConnected()) { gprsDisconnect(); }
      data = "";
      return true;
    }
    return false;
  }

 public:
  Stream& stream;

 protected:
  GsmClientSim7600* sockets[TINY_GSM_MUX_COUNT];
  // TODO(SRGD): I suspect we need to have two separate socket arrays, a secure
  // and not secure one
};

#endif  // SRC_TINYGSMCLIENTSIM7600_H_
