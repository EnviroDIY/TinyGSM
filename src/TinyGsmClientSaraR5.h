/**
 * @file       TinyGsmClientSaraR5.h
 * @author     Sebastian Bergner, Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Aug 2023
 */

#ifndef SRC_TINYGSMCLIENTSARAR5_H_
#define SRC_TINYGSMCLIENTSARAR5_H_
// #pragma message("TinyGSM:  TinyGsmClientSaraR5")

// #define TINY_GSM_DEBUG Serial

#define TINY_GSM_MUX_COUNT 7
#define TINY_GSM_SECURE_MUX_COUNT 7
// I **think** all sockets can be SSL, but the manual is not clear (to me).
// Also supports 5 SSL contexts (0-4)
// The SSL context is collection of SSL settings, not the connection identifier.
// WARNING: You cannot control the socket mux number on this module! The module
// opens the connection and returns the connection number it opened.

#define TINY_GSM_SEND_MAX_SIZE 1024
// USOWR accepts up to 1024 bytes in "normal" and "binary extended" modes and up
// to 512 bytes in "HEX" mode.

#if !defined(TINY_GSM_CONNECT_TIMEOUT)
#define TINY_GSM_CONNECT_TIMEOUT 120
#endif

#define TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
#define TINY_GSM_MUX_DYNAMIC
#ifdef AT_NL
#undef AT_NL
#endif
#define AT_NL "\r\n"

#ifdef MODEM_MANUFACTURER
#undef MODEM_MANUFACTURER
#endif
#define MODEM_MANUFACTURER "u-blox"

#ifdef MODEM_MODEL
#undef MODEM_MODEL
#endif
#define MODEM_MODEL "SARA-R5"

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"

// NOTE: This module supports SSL, but we do not support any certificate
// management yet. TINY_GSM_MODEM_HAS_SSL here and do no include the SSL module
// so as not to waste space.
#define TINY_GSM_MODEM_HAS_SSL

#include "TinyGsmGPRS.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmGSMLocation.tpp"
#include "TinyGsmGPS.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmBattery.tpp"

enum SaraR5RegStatus {
  REG_NO_RESULT        = -1,
  REG_UNREGISTERED     = 0,
  REG_SEARCHING        = 2,
  REG_DENIED           = 3,
  REG_OK_HOME          = 1,
  REG_OK_ROAMING       = 5,
  REG_UNKNOWN          = 4,
  REG_SMS_ONLY_HOME    = 6,
  REG_SMS_ONLY_ROAMING = 7,
  REG_EMERGENCY_ONLY =
      8,  // ublox AT command manual states: attached for emergency bearer
          // services only (see 3GPP TS 24.008 [85] and 3GPP TS 24.301 [120]
          // that specify the condition when the MS is considered as attached
          // for emergency bearer services)
  REG_NO_FALLBACK_LTE_HOME =
      9,  // not 100% certain, ublox AT command manual states: registered for
          // "CSFB not preferred", home network (applicable only when
          // <AcTStatus> indicates E-UTRAN)
  REG_NO_FALLBACK_LTE_ROAMING =
      10  // not 100% certain, ublox AT command manual states: registered for
          // "CSFB not preferred", roaming (applicable only when <AcTStatus>
          // indicates E-UTRAN)
};

class TinyGsmSaraR5 : public TinyGsmModem<TinyGsmSaraR5>,
                      public TinyGsmGPRS<TinyGsmSaraR5>,
                      public TinyGsmTCP<TinyGsmSaraR5, TINY_GSM_MUX_COUNT>,
                      public TinyGsmCalling<TinyGsmSaraR5>,
                      public TinyGsmSMS<TinyGsmSaraR5>,
                      public TinyGsmGSMLocation<TinyGsmSaraR5>,
                      public TinyGsmGPS<TinyGsmSaraR5>,
                      public TinyGsmTime<TinyGsmSaraR5>,
                      public TinyGsmBattery<TinyGsmSaraR5> {
  friend class TinyGsmModem<TinyGsmSaraR5>;
  friend class TinyGsmGPRS<TinyGsmSaraR5>;
  friend class TinyGsmTCP<TinyGsmSaraR5, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmCalling<TinyGsmSaraR5>;
  friend class TinyGsmSMS<TinyGsmSaraR5>;
  friend class TinyGsmGSMLocation<TinyGsmSaraR5>;
  friend class TinyGsmGPS<TinyGsmSaraR5>;
  friend class TinyGsmTime<TinyGsmSaraR5>;
  friend class TinyGsmBattery<TinyGsmSaraR5>;

  /*
   * Inner Client
   */
 public:
  class GsmClientSaraR5
      : public TinyGsmTCP<TinyGsmSaraR5, TINY_GSM_MUX_COUNT>::GsmClient {
    friend class TinyGsmSaraR5;

   public:
    GsmClientSaraR5() {
      is_secure = false;
    }

    explicit GsmClientSaraR5(TinyGsmSaraR5& modem, uint8_t mux = 0) {
      init(&modem, mux);
      is_secure = false;
    }

    bool init(TinyGsmSaraR5* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_available = 0;
      prev_check     = 0;
      sock_connected = false;
      got_data       = false;
      is_mid_send    = false;

      // The SARA R5 does NOT allow you to choose the mux number; this is an
      // initial place holder for before connection. We need to assign a mux
      // number here first so that we can assign the pointer for the client in
      // the socket array and in-turn allow the modem to look back at the
      // properties of the client to check if the client needs SSL and, if so,
      // what the SSL specs are. If the mux number returned at the end of the
      // connection process is different from the one we assigned here, we
      // update the position of the pointer to this in the socket array after
      // the connection finishes.

      // NOTE: Because we can't control the mux number, we don't need to worry
      // about attempting to assign a mux number for a secure socket that is out
      // of range of because the number of secure sockets is smaller than the
      // number of standard sockets.

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
      is_mid_send = false;
      // stop();  // DON'T stop! We don't know our actual mux yet!
      TINY_GSM_YIELD();
      rx.clear();

      uint8_t oldMux = mux;
      sock_connected = at->modemConnect(host, port, &mux, timeout_s);
      if (mux != oldMux) {
        DBG(GF("###  Mux number changed from"), oldMux, GF("to"), mux);
        if (!(mux < TINY_GSM_MUX_COUNT &&
              (at->sockets[mux] == nullptr || at->sockets[mux] == this))) {
          DBG(GF("WARNING: This new mux number had already been assigned to a "
                 "different client, attempting to move it!"));
          uint8_t next_empty_mux = at->findFirstUnassignedMux();
          if (next_empty_mux != static_cast<uint8_t>(-1)) {
            DBG(GF("### Socket previously assigned as"), mux, GF("moved to"),
                next_empty_mux);
            at->sockets[next_empty_mux] = at->sockets[mux];
          } else {
            DBG(GF("WARNING: Failed to move socket, it will be overwritten!"));
          }
        }
        at->sockets[oldMux] = nullptr;
      }
      at->sockets[mux] = this;
      at->maintain();

      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    virtual void stop(uint32_t maxWaitMs) {
      is_mid_send = false;
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+USOCL="), mux);
      at->waitResponse();  // should return within 1s
      sock_connected = false;
    }
    void stop() override {
      stop(15000L);
    }

    virtual ~GsmClientSaraR5() {
      // remove self from the socket array
      at->sockets[this->mux] = nullptr;
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
  class GsmClientSecureR5 : public GsmClientSaraR5 {
    friend class TinyGsmSaraR5;

   public:
    GsmClientSecureR5() {
      is_secure = true;
    }

    explicit GsmClientSecureR5(TinyGsmSaraR5& modem, uint8_t mux = 0)
        : GsmClientSaraR5(modem, mux) {
      is_secure = true;
    }
  };

  /*
   * GSM Modem Constructor
   */
 public:
  explicit TinyGsmSaraR5(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = nullptr) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientSaraR5"));

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

    // Enable automatic time zome update
    sendAT(GF("+CTZU=1"));
    waitResponse(10000L);
    // Ignore the response, in case the network doesn't support it.
    // if (waitResponse(10000L) != 1) { return false; }

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

  // only difference in implementation is the warning on the wrong type
  String getModemNameImpl() {
    String manufacturer = getModemManufacturer();
    String model        = getModemModel();
    String name         = manufacturer + String(" ") + model;
    if (name.startsWith("u-blox SARA-R4") ||
        name.startsWith("u-blox SARA-N4")) {
      DBG("### WARNING:  You are using the wrong TinyGSM modem!");
    } else if (name.startsWith("u-blox SARA-N2")) {
      DBG("### SARA N2 NB-IoT modems not supported!");
    }
    return name;
  }

  bool factoryDefaultImpl() {
    sendAT(GF("+UFACTORY=0,1"));  // No factory restore, erase NVM
    waitResponse();
    return setPhoneFunctionality(16);  // Reset
  }

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin = nullptr) {
    if (!testAT()) { return false; }
    if (!setPhoneFunctionality(16)) { return false; }
    delay(3000);  // TODO(?):  Verify delay timing here
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+CPWROFF"));
    return waitResponse(40000L) == 1;
  }

  bool sleepEnableImpl(bool enable = true) TINY_GSM_ATTR_NOT_AVAILABLE;

  bool setPhoneFunctionalityImpl(uint8_t fun, bool reset = false) {
    sendAT(GF("+CFUN="), fun, reset ? ",1" : "");
    return waitResponse(10000L) == 1;
  }

  /*
   * Generic network functions
   */
 public:
  SaraR5RegStatus getRegistrationStatus() {
    // Check first for EPS registration
    SaraR5RegStatus epsStatus =
        (SaraR5RegStatus)getRegistrationStatusXREG("CEREG");

    // If we're connected on EPS, great!
    if (epsStatus == REG_OK_HOME || epsStatus == REG_OK_ROAMING) {
      return epsStatus;
    } else {
      // Otherwise, check generic network status
      return (SaraR5RegStatus)getRegistrationStatusXREG("CREG");
    }
  }

  bool setRadioAccessTechnology(int selected, int preferred) {
    // selected:
    // 0: GSM / GPRS / eGPRS (single mode)
    // 1: GSM / UMTS (dual mode)
    // 2: UMTS (single mode)
    // 3: LTE (single mode)
    // 4: GSM / UMTS / LTE (tri mode)
    // 5: GSM / LTE (dual mode)
    // 6: UMTS / LTE (dual mode)
    // preferred:
    // 0: GSM / GPRS / eGPRS
    // 2: UTRAN
    // 3: LTE
    sendAT(GF("+URAT="), selected, GF(","), preferred);
    if (waitResponse() != 1) { return false; }
    return true;
  }

  bool getCurrentRadioAccessTechnology() {
    sendAT(GF("+URAT?"));
    String response;
    if (waitResponse(10000L, response) != 1) { return false; }

    return true;
  }

 protected:
  bool isNetworkConnectedImpl() {
    SaraR5RegStatus s = getRegistrationStatus();
    if (s == REG_OK_HOME || s == REG_OK_ROAMING || s == REG_SMS_ONLY_ROAMING ||
        s == REG_SMS_ONLY_HOME)
      return true;
    else if (s == REG_UNKNOWN)  // for some reason, it can hang at unknown..
      return isGprsConnected();
    else
      return false;
  }

  String getLocalIPImpl() {
    sendAT(GF("+UPSND=0,0"));
    if (waitResponse(GF(AT_NL "+UPSND:")) != 1) { return ""; }
    streamSkipUntil(',');   // Skip PSD profile
    streamSkipUntil('\"');  // Skip request type
    String res = stream.readStringUntil('\"');
    if (waitResponse() != 1) { return ""; }
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
  //  - Add `public TinyGsmSSL<TinyGsmSaraR5, TINY_GSM_MUX_COUNT>,` to the
  //    constructor's initializer list
  //  - Add `friend class TinyGsmSSL<TinyGsmSaraR5, TINY_GSM_MUX_COUNT>;` to the
  //    friend list.
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
  bool gprsConnectImpl(const char* apn, const char* = nullptr,
                       const char* = nullptr) {
    sendAT(GF("+CGATT=1"));  // attach to GPRS
    if (waitResponse(360000L) != 1) { return false; }

    // Setting up the PSD profile/PDP context with the UPSD commands sets up an
    // "internal" PDP context, i.e. a data connection using the internal IP
    // stack and related AT commands for sockets.

    // Packet switched data configuration
    // AT+UPSD=<profile_id>,<param_tag>,<param_val>
    // profile_id = 0 - PSD profile identifier, in range 0-6 (NOT PDP context)
    // param_tag = 1: APN
    // param_tag = 2: username  -> not working for SARA-R5
    // param_tag = 3: password  -> not working for SARA-R5
    // param_tag = 7: IP address Note: IP address set as "0.0.0.0" means
    //    dynamic IP address assigned during PDP context activation


    // check all available PDP context identifiers
    String response;
    response.reserve(1024);
    sendAT(GF("+CGDCONT?"));

    waitResponseUntilEndStream(1000, response);

    if (response.length() == 0) {
      return false;  // no apn at all found
    }
    // parse string & look for apn -> modified from SparkFun u-blox SARA-R5 lib
    // Example:
    // +CGDCONT: 0,"IP","payandgo.o2.co.uk","0.0.0.0",0,0,0,0,0,0,0,0,0,0
    // +CGDCONT:
    // 1,"IP","payandgo.o2.co.uk.mnc010.mcc234.gprs","10.160.182.234",0,0,0,2,0,0,0,0,0,0

    // create search buffer where we can search
    char* searchBuf = (char*)malloc(response.length() + 1);
    response.toCharArray(searchBuf, response.length() + 1);

    int   rcid      = -1;
    char* searchPtr = searchBuf;

    for (size_t index = 0; index <= response.length(); index++) {
      // int scanned;
      // Find the first/next occurrence of +CGDCONT:
      searchPtr = strstr(searchPtr, "+CGDCONT:");
      if (searchPtr != nullptr) {
        char strPdpType[10];
        char strApn[128];
        int  ipOct[4];

        searchPtr += strlen("+CGDCONT:");
        while (*searchPtr == ' ') searchPtr++;  // skip spaces
                                                /*scanned =*/
        sscanf(searchPtr, "%d,\"%[^\"]\",\"%[^\"]\",\"%d.%d.%d.%d", &rcid,
               strPdpType, strApn, &ipOct[0], &ipOct[1], &ipOct[2], &ipOct[3]);

        if (!strcmp(strApn, apn)) {
          // found the configuration that we want to connect to
          break;
        }
      }
    }

    free(searchBuf);

    sendAT(GF(
        "+UPSDA=0,4"));  // Deactivate the PDP context associated with profile 0
    waitResponse(360000L);  // Can return an error if previously not activated

    sendAT(GF("+UPSD=0,100,"),
           rcid);  // Deactivate the PDP context associated with profile 0
    waitResponse();

    sendAT(GF(
        "+UPSDA=0,3"));  // Activate the PDP context associated with profile 0
    if (waitResponse(360000L) != 1) { return false; }

    sendAT(GF("+UPSD=0,0,2"));  // Set protocol type to IPv4v6 with IPv4
                                // preferred for internal sockets
    waitResponse();

    return true;
  }

  bool gprsDisconnectImpl() {
    sendAT(GF(
        "+UPSDA=0,4"));  // Deactivate the PDP context associated with profile 0
    if (waitResponse(360000L) != 1) { return false; }

    sendAT(GF("+CGATT=0"));  // detach from GPRS
    if (waitResponse(360000L) != 1) { return false; }

    return true;
  }

  /*
   * SIM card functions
   */
 protected:
  // This uses "CGSN" instead of "GSN"
  String getIMEIImpl() {
    sendAT(GF("+CGSN"));
    if (waitResponse(GF(AT_NL)) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  /*
   * Phone Call functions
   */
  // Follows all phone call functions as inherited from TinyGsmCalling.tpp

  /*
   * Audio functions
   */
  // No functions of this type supported

  /*
   * Text messaging (SMS) functions
   */
  // Follows all text messaging (SMS) functions as inherited from TinyGsmSMS.tpp

  /*
   * GSM/GPS/GNSS/GLONASS Location functions
   * NOTE:  u-blox modules use the same function to get location data from both
   * GSM tower triangulation and from dedicated GPS/GNSS/GLONASS receivers.  The
   * only difference in which sensor the data is requested from.  If a GNSS
   * location is requested from a modem without a GNSS receiver installed on the
   * I2C port, the GSM-based "Cell Locate" location will be returned instead.
   */
 protected:
  String inline getUbloxLocationRaw(int8_t sensor) {
    // AT+ULOC=<mode>,<sensor>,<response_type>,<timeout>,<accuracy>
    // <mode> - 2: single shot position
    // <sensor> - 0: use the last fix in the internal database and stop the GNSS
    //          receiver
    //          - 1: use the GNSS receiver for localization
    //          - 2: use cellular CellLocate location information
    //          - 3: ?? use the combined GNSS receiver and CellLocate service
    //          information ?? - Docs show using sensor 3 and it's
    //          documented for the +UTIME command but not for +ULOC
    // <response_type> - 0: standard (single-hypothesis) response
    // <timeout> - Timeout period in seconds
    // <accuracy> - Target accuracy in meters (1 - 999999)
    sendAT(GF("+ULOC=2,"), sensor, GF(",0,120,1"));
    // wait for first "OK"
    // waitResponse(10000L) ;
    if (waitResponse(10000L) != 1) { return ""; }
    // wait for the final result - wait full timeout time
    if (waitResponse(120000L, GF(AT_NL "+UULOC:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  String getGsmLocationRawImpl() {
    return getUbloxLocationRaw(2);
  }
  String getGPSrawImpl() {
    return getUbloxLocationRaw(1);
  }

  inline bool getUbloxLocation(int8_t sensor, float* lat, float* lon,
                               float* speed = 0, float* alt = 0, int* vsat = 0,
                               int* usat = 0, float* accuracy = 0,
                               int* year = 0, int* month = 0, int* day = 0,
                               int* hour = 0, int* minute = 0,
                               int* second = 0) {
    // AT+ULOC=<mode>,<sensor>,<response_type>,<timeout>,<accuracy>
    // <mode> - 2: single shot position
    // <sensor> - 2: use cellular CellLocate location information
    //          - 0: use the last fix in the internal database and stop the GNSS
    //          receiver
    //          - 1: use the GNSS receiver for localization
    //          - 3: ?? use the combined GNSS receiver and CellLocate service
    //          information ?? - Docs show using sensor 3 and it's documented
    //          for the +UTIME command but not for +ULOC
    // <response_type> - 0: standard (single-hypothesis) response  -> +UULOC:
    // <date>,<time>,<lat>,<long>,<alt>,<uncertainty>
    //                   1: detailed (single-hypothesis) response  -> +UULOC:
    //                   <date>,<time>,<lat>,<long>,<alt>,<uncertainty>,<speed>,<direction>,<vertical_acc>,<sensor_used>,<SV_used>,<antenna_status>,<jamming_status>
    // <timeout> - Timeout period in seconds
    // <accuracy> - Target accuracy in meters (1 - 999999)
    sendAT(GF("+ULOC=2,"), sensor, GF(",1,120,1"));
    // wait for first "OK"
    if (waitResponse(10000L) != 1) { return false; }
    // wait for the final result - wait full timeout time
    if (waitResponse(120000L, GF(AT_NL "+UULOC: ")) != 1) { return false; }

    // +UULOC: <date>, <time>, <lat>, <long>, <alt>, <uncertainty>, <speed>,
    // <direction>, <vertical_acc>, <sensor_used>, <SV_used>, <antenna_status>,
    // <jamming_status>

    // init variables
    float ilat         = 0;
    float ilon         = 0;
    float ispeed       = 0;
    float ialt         = 0;
    int   iusat        = 0;
    float iaccuracy    = 0;
    int   iyear        = 0;
    int   imonth       = 0;
    int   iday         = 0;
    int   ihour        = 0;
    int   imin         = 0;
    float secondWithSS = 0;

    // Date & Time
    iday         = streamGetIntBefore('/');    // Two digit day
    imonth       = streamGetIntBefore('/');    // Two digit month
    iyear        = streamGetIntBefore(',');    // Four digit year
    ihour        = streamGetIntBefore(':');    // Two digit hour
    imin         = streamGetIntBefore(':');    // Two digit minute
    secondWithSS = streamGetFloatBefore(',');  // 6 digit second with subseconds

    ilat = streamGetFloatBefore(',');  // Estimated latitude, in degrees
    ilon = streamGetFloatBefore(',');  // Estimated longitude, in degrees
    ialt = streamGetFloatBefore(
        ',');         // Estimated altitude, in meters - only forGNSS
                      // positioning, 0 in case of CellLocate
    if (ialt != 0) {  // values not returned for CellLocate
      iaccuracy =
          streamGetFloatBefore(',');       // Maximum possible error, in meters
      ispeed = streamGetFloatBefore(',');  // Speed over ground m/s3
      streamSkipUntil(',');  // Course over ground in degree (0 deg - 360 deg)
      streamSkipUntil(',');  // Vertical accuracy, in meters
      streamSkipUntil(',');  // Sensor used for the position calculation
      iusat = streamGetIntBefore(',');  // Number of satellite used
      streamSkipUntil(',');             // Antenna status
      streamSkipUntil('\n');            // Jamming status
    } else {
      iaccuracy =
          streamGetFloatBefore('\n');  // Maximum possible error, in meters
    }

    // Set pointers
    if (lat != nullptr) *lat = ilat;
    if (lon != nullptr) *lon = ilon;
    if (speed != nullptr) *speed = ispeed;
    if (alt != nullptr) *alt = ialt;
    if (vsat != nullptr)
      *vsat = 0;  // Number of satellites viewed not reported;
    if (usat != nullptr) *usat = iusat;
    if (accuracy != nullptr) *accuracy = iaccuracy;
    if (iyear < 2000) iyear += 2000;
    if (year != nullptr) *year = iyear;
    if (month != nullptr) *month = imonth;
    if (day != nullptr) *day = iday;
    if (hour != nullptr) *hour = ihour;
    if (minute != nullptr) *minute = imin;
    if (second != nullptr) *second = static_cast<int>(secondWithSS);

    // final ok
    waitResponse();
    return true;
  }
  bool getGsmLocationImpl(float* lat, float* lon, float* accuracy = 0,
                          int* year = 0, int* month = 0, int* day = 0,
                          int* hour = 0, int* minute = 0, int* second = 0) {
    return getUbloxLocation(2, lat, lon, 0, 0, 0, 0, accuracy, year, month, day,
                            hour, minute, second);
  }
  bool getGPSImpl(float* lat, float* lon, float* speed = 0, float* alt = 0,
                  int* vsat = 0, int* usat = 0, float* accuracy = 0,
                  int* year = 0, int* month = 0, int* day = 0, int* hour = 0,
                  int* minute = 0, int* second = 0) {
    return getUbloxLocation(1, lat, lon, speed, alt, vsat, usat, accuracy, year,
                            month, day, hour, minute, second);
  }

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
   * Battery functions
   */
 protected:
  int16_t getBattVoltageImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  int8_t getBattPercentImpl() {
    sendAT(GF("+CIND?"));
    if (waitResponse(GF(AT_NL "+CIND:")) != 1) { return 0; }

    int8_t res     = streamGetIntBefore(',');
    int8_t percent = res * 20;  // return is 0-5
    // Wait for final OK
    waitResponse();
    return percent;
  }

  int8_t getBattChargeStateImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool getBattStatsImpl(int8_t& chargeState, int8_t& percent,
                        int16_t& milliVolts) {
    chargeState = 0;
    percent     = getBattPercent();
    milliVolts  = 0;
    return true;
  }

  /*
   * Temperature functions
   */

  // This would only available for a small number of modules in this group
  // (TOBY-L)
  float getTemperatureImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;

  /*
   * Client related functions
   */
 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t* mux,
                        int timeout_s) {
    uint32_t timeout_ms  = ((uint32_t)timeout_s) * 1000;
    bool     ssl         = sockets[*mux]->is_secure;
    uint32_t startMillis = millis();

    // create a socket
    sendAT(GF("+USOCR=6"));
    // reply is +USOCR: ## of socket created
    if (waitResponse(GF(AT_NL "+USOCR:")) != 1) { return false; }
    *mux = streamGetIntBefore('\n');
    waitResponse();

    if (ssl) {
      sendAT(GF("+USOSEC="), *mux, ",1");
      waitResponse();
    }

    // Enable NODELAY
    // AT+USOSO=<socket>,<level>,<opt_name>,<opt_val>[,<opt_val2>]
    // <level> - 0 for IP, 6 for TCP, 65535 for socket level options
    // <opt_name> TCP/1 = no delay (do not delay send to coalesce packets)
    // NOTE:  Enabling this may increase data plan usage
    // sendAT(GF("+USOSO="), *mux, GF(",6,1,1"));
    // waitResponse();

    // Enable KEEPALIVE, 30 sec
    // sendAT(GF("+USOSO="), *mux, GF(",6,2,30000"));
    // waitResponse();

    // connect on the allocated socket
    sendAT(GF("+USOCO="), *mux, ",\"", host, "\",", port);
    int8_t rsp = waitResponse(timeout_ms - (millis() - startMillis));
    return (1 == rsp);
  }

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    sendAT(GF("+USOWR="), mux, ',', (uint16_t)len);
    if (waitResponse(GF("@")) != 1) { return 0; }
    // 50ms delay, see AT manual section 25.10.4
    delay(50);
    return true;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t mux) {
    if (waitResponse(GF(AT_NL "+USOWR:")) != 1) { return 0; }
    uint8_t ret_mux = streamGetIntBefore(',');   // check mux
    int16_t sent    = streamGetIntBefore('\n');  // check send length
    bool    success = waitResponse() ==
        1;  // sends back OK after the confirmation of number sent
    if (mux == ret_mux && success) return sent;
    return 0;
  }

  size_t modemReadImpl(size_t size, uint8_t mux) {
    if (!sockets[mux]) return 0;
    sendAT(GF("+USORD="), mux, ',', (uint16_t)size);
    if (waitResponse(GF(AT_NL "+USORD:")) != 1) { return 0; }
    streamSkipUntil(',');  // Skip mux
    int16_t len = streamGetIntBefore(',');
    streamSkipUntil('\"');
    bool chars_remaining = true;
    while (len-- && chars_remaining) {
      chars_remaining = moveCharFromStreamToFifo(mux);
    }
    streamSkipUntil('\"');
    waitResponse();
    // DBG("### READ:", len, "from", mux);
    sockets[mux]->sock_available = modemGetAvailable(mux);
    return len;
  }

  size_t modemGetAvailableImpl(uint8_t mux) {
    if (!sockets[mux]) return 0;
    // NOTE:  Querying a closed socket gives an error "operation not allowed"
    sendAT(GF("+USORD="), mux, ",0");
    size_t  result = 0;
    uint8_t res    = waitResponse(GF(AT_NL "+USORD:"));
    // Will give error "operation not allowed" when attempting to read a socket
    // that you have already told to close
    if (res == 1) {
      streamSkipUntil(',');  // Skip mux
      result = streamGetIntBefore('\n');
      // if (result) DBG("### DATA AVAILABLE:", result, "on", mux);
      waitResponse();
    }
    if (!result) { sockets[mux]->sock_connected = modemGetConnected(mux); }
    // DBG("### Available:", result, "on", mux);
    return result;
  }

  bool modemGetConnectedImpl(uint8_t mux) {
    // NOTE:  Querying a closed socket gives an error "operation not allowed"
    sendAT(GF("+USOCTL="), mux, ",10");
    uint8_t res = waitResponse(GF(AT_NL "+USOCTL:"));
    if (res != 1) { return false; }

    streamSkipUntil(',');  // Skip mux
    streamSkipUntil(',');  // Skip type
    int8_t result = streamGetIntBefore('\n');
    // 0: the socket is in INACTIVE status (it corresponds to CLOSED status
    // defined in RFC793 "TCP Protocol Specification" [112])
    // 1: the socket is in LISTEN status
    // 2: the socket is in SYN_SENT status
    // 3: the socket is in SYN_RCVD status
    // 4: the socket is in ESTABILISHED status
    // 5: the socket is in FIN_WAIT_1 status
    // 6: the socket is in FIN_WAIT_2 status
    // 7: the sokcet is in CLOSE_WAIT status
    // 8: the socket is in CLOSING status
    // 9: the socket is in LAST_ACK status
    // 10: the socket is in TIME_WAIT status
    waitResponse();
    return (result != 0);
  }

  /*
   * Utilities
   */
 public:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+UUSORD:"))) {
      int8_t  mux = streamGetIntBefore(',');
      int16_t len = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->got_data = true;
        // max size is 1024
        if (len >= 0 && len <= 1024) { sockets[mux]->sock_available = len; }
      }
      data = "";
      // DBG("### URC Data Received:", len, "on", mux);
      return true;
    } else if (data.endsWith(GF("+UUSOCL:"))) {
      int8_t mux = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      data = "";
      DBG("### URC Sock Closed: ", mux);
      return true;
    }
    return false;
  }

 private:  // basically the same as waitResponse but without preemptive exiting
           // (except when time runs out) this is used for +CGDCONT? as it can
           // return multiple cid/apn configurations each terminated with OK\r\n
  int8_t waitResponseUntilEndStream(uint32_t timeout_ms, String& data) {
    data.reserve(1024);  // buffer of the same size as in the SparkFun lib
    uint8_t  index       = 0;
    uint32_t startMillis = millis();
    do {
      TINY_GSM_YIELD();
      while (stream.available() > 0) {
        TINY_GSM_YIELD();
        int8_t a = stream.read();

        if (a <= 0) continue;  // Skip 0x00 bytes, just in case
        data += static_cast<char>(a);
      }
    } while (millis() - startMillis < timeout_ms);
    return index;
  }

 public:
  Stream& stream;

 protected:
  GsmClientSaraR5* sockets[TINY_GSM_MUX_COUNT];
};

#endif  // SRC_TINYGSMCLIENTSARAR5_H_
