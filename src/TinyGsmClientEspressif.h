/**
 * @file       TinyGsmClientEspressif.h
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCLIENTESPRESSIF_H_
#define SRC_TINYGSMCLIENTESPRESSIF_H_
// #pragma message("TinyGSM:  TinyGsmClientEspressif")

// #define TINY_GSM_DEBUG Serial

#ifdef TINY_GSM_MUX_COUNT
#undef TINY_GSM_MUX_COUNT
#endif
#define TINY_GSM_MUX_COUNT 5
#ifdef TINY_GSM_SECURE_MUX_COUNT
#undef TINY_GSM_SECURE_MUX_COUNT
#endif
#define TINY_GSM_SECURE_MUX_COUNT 5
// NOTE: There's a total limit of 5 sockets, any of them can be SSL. BUT the
// manual warns that module may not be able to handle more than 1 SSL socket at
// a time.
// These modules don't have "SSL Contexts" per-say, but they only support 2
// certificate sets.

// The ESP8266 devices can receive 2048 bytes and send 1460 bytes at most each
// time; the other ESP devices can receive 8192 bytes and send 2920 bytes at
// most each time.

#ifdef AT_NL
#undef AT_NL
#endif
#define AT_NL "\r\n"

#ifdef MODEM_MANUFACTURER
#undef MODEM_MANUFACTURER
#endif
#define MODEM_MANUFACTURER "Espressif"

#ifdef MODEM_MODEL
#undef MODEM_MODEL
#endif
#if defined(TINY_GSM_MODEM_ESP8266) || defined(TINY_GSM_MODEM_ESP8266_NONOS)
#define MODEM_MODEL "ESP8266"
#ifdef TINY_GSM_SEND_MAX_SIZE
#undef TINY_GSM_SEND_MAX_SIZE
#endif
#define TINY_GSM_SEND_MAX_SIZE 2048
#elif defined(TINY_GSM_MODEM_ESP32)
#define MODEM_MODEL "ESP32"
#ifdef TINY_GSM_SEND_MAX_SIZE
#undef TINY_GSM_SEND_MAX_SIZE
#endif
#define TINY_GSM_SEND_MAX_SIZE 8192
#else
#define MODEM_MODEL "Espressif AT"
#endif

#include "TinyGsmModem.tpp"
#include "TinyGsmWifi.tpp"

template <class EspressifType>
class TinyGsmEspressif : public TinyGsmModem<EspressifType>,
                         public TinyGsmWifi<EspressifType> {
  friend class TinyGsmModem<EspressifType>;
  friend class TinyGsmWifi<EspressifType>;

  /*
   * CRTP Helper
   */
 protected:
  inline const EspressifType& thisModem() const {
    return static_cast<const EspressifType&>(*this);
  }
  inline EspressifType& thisModem() {
    return static_cast<EspressifType&>(*this);
  }
  ~TinyGsmEspressif() {}

  /*
   * GSM Modem Constructor
   */
 public:
  explicit TinyGsmEspressif(Stream& stream) : stream(stream) {}

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = nullptr) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientEspressif"));

    bool success = true;

    if (!thisModem().testAT()) { return false; }
    if (pin && strlen(pin) > 0) {
      DBG("Espressif modules do not use an unlock pin!");
    }
    thisModem().sendAT(GF("E0"));  // Echo Off
    success &= thisModem().waitResponse() == 1;

#ifdef TINY_GSM_DEBUG
    thisModem().sendAT(GF("+SYSLOG=1"));  // turn on verbose error codes
#else
    thisModem().sendAT(GF("+SYSLOG=0"));  // turn off error codes
#endif
    thisModem().waitResponse();

    thisModem().sendAT(GF("+CIPMUX=1"));  // Enable Multiple Connections
    success &= thisModem().waitResponse() == 1;
    thisModem().sendAT(GF("+CWMODE=1"));  // Put into "station" mode
    if (thisModem().waitResponse() != 1) {
      thisModem().sendAT(
          GF("+CWMODE_CUR=1"));  // Attempt "current" station mode command
                                 // for some firmware variants if needed
      success &= thisModem().waitResponse() == 1;
    }
    thisModem().sendAT(
        GF("+CIPDINFO=0"));  // do not show the remote host and port in “+IPD”
                             // and “+CIPRECVDATA” messages.
    success &= thisModem().waitResponse() == 1;
    DBG(GF("### Modem:"), thisModem().getModemName());
    return success;
  }

 public:
  bool setDefaultBaud(uint32_t baud) {
    thisModem().sendAT(GF("+UART_DEF="), baud, ",8,1,0,0");
    return thisModem().waitResponse() == 1;
  }

 protected:
  bool setBaudImpl(uint32_t baud) {
    thisModem().sendAT(GF("+UART_CUR="), baud, ",8,1,0,0");
    if (thisModem().waitResponse() != 1) {
      thisModem().sendAT(GF("+UART="), baud,
                         ",8,1,0,0");  // Really old firmwares might need this
      // if (thisModem().waitResponse() != 1) {
      //   thisModem().sendAT(GF("+IPR="), baud);  // First release firmwares
      //   might need this
      return thisModem().waitResponse() == 1;
      // }
    }
    return false;
  }

  String getModemInfoImpl() {
    thisModem().sendAT(GF("+GMR"));
    String res;
    if (thisModem().waitResponse(1000L, res) != 1) { return ""; }
    thisModem().cleanResponseString(res);
    return res;
  }

  // Gets the modem hardware version
  String getModemManufacturerImpl() {
    return MODEM_MANUFACTURER;
  }

  // Gets the modem hardware version
  String getModemModelImpl() {
    String model = MODEM_MODEL;
    thisModem().sendAT(GF("+GMR"));
    thisModem().streamSkipUntil('\n');  // skip the AT version
    thisModem().streamSkipUntil('\n');  // skip the SDK version
    thisModem().streamSkipUntil('\n');  // skip the compile time
    // read the hardware from the Bin version
    thisModem().streamSkipUntil('(');  // skip the text "Bin version"
    String wroom = stream.readStringUntil(
        ')');  // read the WRoom version in the parethesis
    thisModem().streamSkipUntil('(');            // skip the bin version itself
    if (thisModem().waitResponse(1000L) == 1) {  // wait for the ending OK
      return wroom;
    }
    return model;
  }

  // Gets the modem firmware version
  String getModemRevisionImpl() {
    thisModem().sendAT(GF("+GMR"));  // GMR instead of CGMR
    String res;
    if (thisModem().waitResponse(1000L, res) != 1) { return ""; }
    thisModem().cleanResponseString(res);
    return res;
  }

  // Gets the modem serial number
  String getModemSerialNumberImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool factoryDefaultImpl() {
    thisModem().sendAT(GF("+RESTORE"));
    return thisModem().waitResponse() == 1;
  }

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin = nullptr) {
    if (!thisModem().testAT()) { return false; }
    thisModem().sendAT(GF("+RST"));
    if (thisModem().waitResponse(10000L) != 1) { return false; }
    if (thisModem().waitResponse(10000L, GF(AT_NL "ready" AT_NL)) != 1) {
      return false;
    }
    delay(850);
    return thisModem().init(pin);
  }

  bool powerOffImpl() {
    thisModem().sendAT(
        GF("+GSLP=0"));  // Power down indefinitely - until manually reset!
    return thisModem().waitResponse() == 1;
  }

  bool radioOffImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;

  bool sleepEnableImpl(bool enable = true) TINY_GSM_ATTR_NOT_AVAILABLE;

  bool setPhoneFunctionalityImpl(uint8_t fun, bool reset = false)
      TINY_GSM_ATTR_NOT_IMPLEMENTED;

  /*
   * Generic network functions
   */
 protected:
  int8_t getSignalQualityImpl() {
    thisModem().sendAT(GF("+CWJAP?"));
    int8_t res1 = thisModem().waitResponse(GF("No AP"), GF("+CWJAP:"),
                                           GFP(GSM_OK), GFP(GSM_ERROR));
    if (res1 != 2) {
      thisModem().waitResponse();
      thisModem().sendAT(GF("+CWJAP_CUR?"));  // attempt "current" as used by
                                              // some Non-OS firmware versions
      int8_t res1 = thisModem().waitResponse(GF("No AP"), GF("+CWJAP_CUR:"),
                                             GFP(GSM_OK), GFP(GSM_ERROR));
      if (res1 != 2) {
        thisModem().waitResponse();
        return 0;
      }
    }
    thisModem().streamSkipUntil(',');  // Skip SSID
    thisModem().streamSkipUntil(',');  // Skip BSSID/MAC address
    thisModem().streamSkipUntil(',');  // Skip Chanel number
    int8_t res2 = stream.parseInt();   // Read RSSI
    thisModem().waitResponse();        // Returns an OK after the value
    return res2;
  }

  String getLocalIPImpl() {
    // attempt with and without 'current' flag
    thisModem().sendAT(GF("+CIPSTA?"));
    thisModem().waitResponse(GF("ERROR"), GF("+CIPSTA:"));
    String res2 = stream.readStringUntil('\n');
    res2.replace("ip:", "");  // newer firmwares have this
    res2.replace("\"", "");
    res2.trim();
    thisModem().waitResponse();
    return res2;
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // No functions of this type supported in this parent template


  /*
   * WiFi functions
   */
 protected:
  bool networkConnectImpl(const char* ssid, const char* pwd) {
    // attempt first without than with the 'current' flag used in some firmware
    // versions
    thisModem().sendAT(GF("+CWJAP=\""), ssid, GF("\",\""), pwd, GF("\""));
    if (thisModem().waitResponse(30000L, GFP(GSM_OK), GF(AT_NL "FAIL" AT_NL)) !=
        1) {
      thisModem().sendAT(GF("+CWJAP_CUR=\""), ssid, GF("\",\""), pwd, GF("\""));
      if (thisModem().waitResponse(30000L, GFP(GSM_OK),
                                   GF(AT_NL "FAIL" AT_NL)) != 1) {
        return false;
      }
    }

    return true;
  }

  bool networkDisconnectImpl() {
    thisModem().sendAT(GF("+CWQAP"));
    bool retVal = thisModem().waitResponse(10000L) == 1;
    thisModem().waitResponse(GF("WIFI DISCONNECT"));
    return retVal;
  }


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

  /*
   * Utilities
   */
 public:
  bool handleURCs(String& data) {
    return thisModem().handleURCs(data);
  }

 public:
  Stream& stream;
};

#endif  // SRC_TINYGSMCLIENTESPRESSIF_H_
