/**
 * @file       TinyGsmModem.tpp
 * @brief      Core modem trait presets and shared modem interface helpers.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMMODEM_TPP_
#define SRC_TINYGSMMODEM_TPP_

#include "TinyGsmCommon.h"

/**
 * @brief Template class for modem config traits.
 *
 * Most modems share common defaults; this base lets modem-specific config
 * structs specify only the values that differ.
 *
 * @tparam regStatusType The type used for registration status values.
 */
template <typename regStatusType = int8_t>
struct TinyGsmModemConfigPreset {
  /// The type used for registration status values.
  using RegStatus = regStatusType;

  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "unknown";
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "unknown";

  /// The newline character(s) used in AT commands.
  static constexpr char GSM_NL[] TINY_GSM_PROGMEM = "\r\n";
  /// The AT string
  static constexpr char GSM_AT[] TINY_GSM_PROGMEM = "AT";
  /// The OK string.
  static constexpr char GSM_OK[] TINY_GSM_PROGMEM = "OK\r\n";
  /// The ERROR string
  static constexpr char GSM_ERROR[] TINY_GSM_PROGMEM = "ERROR\r\n";

#if defined TINY_GSM_DEBUG
  /// The verbose error string
  static constexpr char GSM_VERBOSE[] TINY_GSM_PROGMEM = "+CME ERROR:";
  /// A second verbose error string
  static constexpr char GSM_VERBOSE_2[] TINY_GSM_PROGMEM = "+CMS ERROR:";
#endif
};

// Out-of-class definitions for C++11 compliance
template <typename regStatusType>
/// @brief Out-of-class definition of the default modem manufacturer string.
constexpr char TinyGsmModemConfigPreset<regStatusType>::MODEM_MANUFACTURER[];

template <typename regStatusType>
/// @brief Out-of-class definition of the default modem model string.
constexpr char TinyGsmModemConfigPreset<regStatusType>::MODEM_MODEL[];

template <typename regStatusType>
/// @brief Out-of-class definition of the default AT newline sequence.
constexpr char TinyGsmModemConfigPreset<regStatusType>::GSM_NL[];

template <typename regStatusType>
/// @brief Out-of-class definition of the default AT command prefix.
constexpr char TinyGsmModemConfigPreset<regStatusType>::GSM_AT[];

template <typename regStatusType>
/// @brief Out-of-class definition of the default modem OK response string.
constexpr char TinyGsmModemConfigPreset<regStatusType>::GSM_OK[];

template <typename regStatusType>
/// @brief Out-of-class definition of the default modem ERROR response string.
constexpr char TinyGsmModemConfigPreset<regStatusType>::GSM_ERROR[];

#if defined TINY_GSM_DEBUG
template <typename regStatusType>
/// @brief Out-of-class definition of the first verbose error response prefix.
constexpr char TinyGsmModemConfigPreset<regStatusType>::GSM_VERBOSE[];

template <typename regStatusType>
/// @brief Out-of-class definition of the second verbose error response prefix.
constexpr char TinyGsmModemConfigPreset<regStatusType>::GSM_VERBOSE_2[];
#endif

/**
 * @class TinyGsmModem
 * @brief The CRTP parent class for basic modem functions.
 * @tparam modemType The derived modem class
 * @tparam modemConfig The modem-specific configuration class, which must be
 * derived from TinyGsmModemConfigPreset
 */
template <class modemType, class modemConfig = TinyGsmModemConfigPreset<int8_t>>
class TinyGsmModem {
  using ModemConfig   = modemConfig;
  using regStatusType = typename ModemConfig::RegStatus;

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the interface
   */
 public:
  /**
   * @anchor basic_functions
   * @name Basic functions
   */
  /**@{*/

  /**
   * @brief Sets up the GSM module
   * @param pin A pin code to unlock the SIM, if necessary
   * @return True if the module was set up as expected, false otherwise.
   */
  bool begin(const char* pin = nullptr) {
    return thisModem().initImpl(pin);
  }
  /**
   * @copydoc TinyGsmModem::begin()
   */
  bool init(const char* pin = nullptr) {
    return thisModem().initImpl(pin);
  }

  /**
   * @brief Recursive variadic template to send AT commands
   *
   * @tparam Args
   * @param cmd The commands to send
   */
  template <typename... Args>
  void sendAT(Args... cmd) {
    thisModem().streamWrite(ModemConfig::GSM_AT, cmd..., ModemConfig::GSM_NL);
    thisModem().stream.flush();
    TINY_GSM_YIELD(); /* DBG("### AT:", cmd...); */
  }

  /**
   * @brief Set the module baud rate
   * @param baud The baud rate the use
   * @return True if the baud rate was set successfully, false otherwise.
   *
   * @note After setting and applying the new baud rate, you will have to end()
   * and begin() the serial object.
   */
  bool setBaud(uint32_t baud) {
    return thisModem().setBaudImpl(baud);
  }

  /**
   * @brief Attempt to set the modem baud rate by trying set the command to
   * change the baud rate to various common baud rates and seeing if the modem
   * responds to AT commands at that baud rate.
   *
   * Unlike TinyGsmAutoBaud, this function will try to set the baud rate on the
   * modem and will attempt to communicate with the modem at the new baud rate
   * whether or not it gets a response at the old baud rate.  This is useful in
   * cases where the modem is set to a baud rate that is just a little too fast
   * for the processor, where you might get a response but it will be garbled
   * and not recognized as a response, because in this case you still want to
   * try to set the baud rate.
   *
   * @param at_serial The serial object to use for communicating with the modem
   * @param targetBaud The final baud rate to try to set the modem to
   * @return True if the modem responded after the baud rate was set, false
   * otherwise.
   *
   * @note After setting and applying the new baud rate, you will have to end()
   * and begin() the serial object.
   */
  template <class StreamObject>
  bool forceModemBaud(StreamObject& at_serial, uint32_t targetBaud) {
    static uint32_t rates[] = {115200, 57600,  9600,   921600, 38400,
                               19200,  460800, 230400, 74400,  74880,
                               2400,   4800,   14400,  28800};

    // start the modem serial at the current baud rate
    at_serial.end();
    at_serial.begin(targetBaud);
    // test for at response from the modem
    bool at_success = thisModem().testAT(1500L);
    // if we got a response and it's the baud rate we want, we're done
    if (at_success) {
      DBG("Modem responded at rate", targetBaud);
      return true;
    }

    uint32_t maximum = 921600;
#if defined(F_CPU)
    if (F_CPU <= 8000000L) {
      maximum = 57600;
    } else if (F_CPU <= 16000000L) {
      maximum = 115200;
    }
#endif
    if (targetBaud > maximum) {
      DBG("Target baud rate", targetBaud,
          "is too high for this processor.  Maximum is", maximum);
      targetBaud = maximum;
    }

    // If we didn't get the right response, or if we got a response but it's
    // not the baud we want, try to set the baud rate.
    // NOTE: We try to set the baud rate even if we *didn't* get a response
    // because if the modem is set in a baud that's just a *little* too fast
    // for the processor, we might get a response but it will be garbled and
    // not recognized as a response.  In this case, we still want to try to
    // set the baud rate.

    for (uint8_t i = 0; i < sizeof(rates) / sizeof(rates[0]); i++) {
      uint32_t rate = rates[i];
      for (uint8_t j = 0; j < 3; j++) {
        DBG("Trying to set the baud rate from a rate of", rate, "...");
        at_serial.end();
        at_serial.begin(rate);
        delay(25);  // settle

#if defined(TINY_GSM_MODEM_ESP32) || defined(TINY_GSM_MODEM_ESP8266)
        thisModem().setDefaultBaud(targetBaud);
#else
        thisModem().setBaud(targetBaud);
#endif

        at_serial.end();
        at_serial.begin(targetBaud);
        delay(25);  // settle

        // test for at response from the modem
        DBG("Checking for a response at", targetBaud, "...");
        at_success = thisModem().testAT(1500L);
        // if we got a response and it's the baud rate we want, we're done
        if (at_success) {
          DBG(GF("Successfully changed the baud rate from"), rate, GF("to"),
              targetBaud);
          return true;
        }
      }
    }
    DBG("Failed to successfully find the baud at any common rate or to change "
        "the baud rate to",
        targetBaud, "...");
    at_serial.begin(targetBaud);
    return false;
  }

  /**
   * @brief Test response to AT commands
   *
   * @param timeout_ms the amount of time to test for; optional with a
   * default value of 10s.
   * @return True if the module responded to AT commands, false otherwise.
   */
  bool testAT(uint32_t timeout_ms = 10000L) {
    return thisModem().testATImpl(timeout_ms);
  }

  /**
   * @brief Listen for responses to commands and handle URCs
   *
   * @param timeout_ms The time to wait for a response
   * @param data A string of data to fill in with response results
   * @param r1 The first output to test against, optional with a default value
   * of "OK"
   * @param r2 The second output to test against, optional with a default value
   * of "ERROR"
   * @param r3 The third output to test against, optional with a default value
   * of nullptr
   * @param r4 The fourth output to test against, optional with a default value
   * of nullptr
   * @param r5 The fifth output to test against, optional with a default value
   * of nullptr
   * @param r6 The sixth output to test against, optional with a default value
   * of nullptr
   * @param r7 The seventh output to test against, optional with a default value
   * of nullptr
   * @param r8 The eighth output to test against, optional with a default value
   * of nullptr
   * @return *int8_t* the index of the response input
   */
  int8_t waitResponse(uint32_t timeout_ms, String& data,
                      GsmConstStr r1 = GFP(ModemConfig::GSM_OK),
                      GsmConstStr r2 = GFP(ModemConfig::GSM_ERROR),
                      GsmConstStr r3 = nullptr, GsmConstStr r4 = nullptr,
                      GsmConstStr r5 = nullptr, GsmConstStr r6 = nullptr,
                      GsmConstStr r7 = nullptr, GsmConstStr r8 = nullptr) {
    return thisModem().waitResponseImpl(timeout_ms, data, r1, r2, r3, r4, r5,
                                        r6, r7, r8);
  }

  /**
   * @brief Listen for responses to commands and handle URCs
   *
   * @param timeout_ms The time to wait for a response
   * @param r1 The first output to test against, optional with a default value
   * of "OK"
   * @param r2 The second output to test against, optional with a default value
   * of "ERROR"
   * @param r3 The third output to test against, optional with a default value
   * of nullptr
   * @param r4 The fourth output to test against, optional with a default value
   * of nullptr
   * @param r5 The fifth output to test against, optional with a default value
   * of nullptr
   * @param r6 The sixth output to test against, optional with a default value
   * of nullptr
   * @param r7 The seventh output to test against, optional with a default value
   * of nullptr
   * @param r8 The eighth output to test against, optional with a default value
   * of nullptr
   * @return *int8_t* the index of the response input
   */
  int8_t waitResponse(uint32_t    timeout_ms,
                      GsmConstStr r1 = GFP(ModemConfig::GSM_OK),
                      GsmConstStr r2 = GFP(ModemConfig::GSM_ERROR),
                      GsmConstStr r3 = nullptr, GsmConstStr r4 = nullptr,
                      GsmConstStr r5 = nullptr, GsmConstStr r6 = nullptr,
                      GsmConstStr r7 = nullptr, GsmConstStr r8 = nullptr) {
    String data;
    return waitResponse(timeout_ms, data, r1, r2, r3, r4, r5, r6, r7, r8);
  }

  /**
   * @brief Listen for responses to commands and handle URCs; listening for 1
   * second.
   *
   * @param r1 The first output to test against, optional with a default value
   * of "OK"
   * @param r2 The second output to test against, optional with a default value
   * of "ERROR"
   * @param r3 The third output to test against, optional with a default value
   * of nullptr
   * @param r4 The fourth output to test against, optional with a default value
   * of nullptr
   * @param r5 The fifth output to test against, optional with a default value
   * of nullptr
   * @param r6 The sixth output to test against, optional with a default value
   * of nullptr
   * @param r7 The seventh output to test against, optional with a default value
   * of nullptr
   * @param r8 The eighth output to test against, optional with a default value
   * of nullptr
   * @return *int8_t* the index of the response input
   */
  int8_t waitResponse(GsmConstStr r1 = GFP(ModemConfig::GSM_OK),
                      GsmConstStr r2 = GFP(ModemConfig::GSM_ERROR),
                      GsmConstStr r3 = nullptr, GsmConstStr r4 = nullptr,
                      GsmConstStr r5 = nullptr, GsmConstStr r6 = nullptr,
                      GsmConstStr r7 = nullptr, GsmConstStr r8 = nullptr) {
    return waitResponse(1000L, r1, r2, r3, r4, r5, r6, r7, r8);
  }

  /**
   * @brief Asks for modem information via the 3GPP TS 27.007 standard ATI
   * command
   *
   * @note  The actual value and style of the response is quite varied
   * @return *String* Some info about the GSM module.
   */
  String getModemInfo() {
    return thisModem().getModemInfoImpl();
  }

  /**
   * @brief Get the modem name - a combination of the manufacturer and model, as
   * the modem calls itself
   *
   * @return *String*  The modem name
   */
  String getModemName() {
    return thisModem().getModemNameImpl();
  }

  /**
   * @brief Get the modem manufacturer
   * @return *String* The modem manufacturer
   */
  String getModemManufacturer() {
    return thisModem().getModemManufacturerImpl();
  }

  /**
   * @brief Get the modem model
   * @return *String* The modem model, as it calls itself
   */
  String getModemModel() {
    return thisModem().getModemModelImpl();
  }

  /**
   * @brief Get the modem revision information.
   *
   * What is returned as the revision may be either a hardware or a firmware
   * version or some combination of both.
   *
   * @return *String* The modem revision information
   */
  String getModemRevision() {
    return thisModem().getModemRevisionImpl();
  }

  /**
   * @brief Get the modem serial number
   * @return *String* The modem serial number
   */
  String getModemSerialNumber() {
    return thisModem().getModemSerialNumberImpl();
  }

  /**
   * @brief Reset the module to factory defaults.
   *
   * This generally restarts the module as well.
   *
   * @return *True if the module successfully reset to default, false otherwise.
   */
  bool factoryDefault() {
    return thisModem().factoryDefaultImpl();
  }
  /**@}*/

  /**
   * @anchor power_functions
   * @name Power functions
   */
  /**@{*/

  /**
   * @brief Restart the module
   * @param pin A pin code to unlock the SIM, if necessary
   * @return True if the module was successfully restarted, false otherwise.
   */
  bool restart(const char* pin = nullptr) {
    return thisModem().restartImpl(pin);
  }
  /**
   * @brief Power off the module
   * @return True if the module was successfully powered down, false otherwise.
   */
  bool poweroff() {
    return thisModem().powerOffImpl();
  }
  /**
   * @brief Turn off the module radio
   * @return True if the module radio was successfully turned off, false
   * otherwise.
   */
  bool radioOff() {
    return thisModem().radioOffImpl();
  }

  /**
   * @brief Enable sleep on the module.
   *
   * For some modules this immediately puts
   * the module to sleep, for others this sets them to be able to sleep based on
   * pin levels.
   *
   * @param enable True to enable sleep, false to disable
   * @return True if sleep was successfully enabled or disabled, false
   * otherwise.
   */
  bool sleepEnable(bool enable = true) {
    return thisModem().sleepEnableImpl(enable);
  }

  /**
   * @brief Set the phone functionality
   *
   * @param fun The phone functionality setting. The value and meaning of this
   * varies by module; check your documentation.
   * @param reset True to reset the module before changing the functionality.
   * @return True if the phone functionality was successfully changed, false
   * otherwise.
   */
  bool setPhoneFunctionality(uint8_t fun, bool reset = false) {
    return thisModem().setPhoneFunctionalityImpl(fun, reset);
  }
  /**@}*/

  /**
   * @anchor network_functions
   * @name Generic Network Functions
   */
  /**@{*/

  /**
   * @brief Get the modem registration status on the network.
   * @return The modem-specific registration status value.
   */
  regStatusType getRegistrationStatus() {
    return thisModem().getRegistrationStatusImpl();
  }

  /**
   * @brief Confirm whether the module is currently connected to the
   * GSM/GPRS/LTE network.
   *
   * @return True if the module is connected to the network, false otherwise.
   */
  bool isNetworkConnected() {
    return thisModem().isNetworkConnectedImpl();
  }

  /**
   * @brief Wait until the module has connected to the network
   *
   * @param timeout_ms The time to wait for attachment in milliseconds. Optional
   * with a default value of 1 minute.
   * @param check_signal True to alternate between checking for connection and
   * checking the signal strength.
   * @return True if the module is now connected to the network, false
   * otherwise.
   */
  bool waitForNetwork(uint32_t timeout_ms = 60000L, bool check_signal = false) {
    return thisModem().waitForNetworkImpl(timeout_ms, check_signal);
  }

  /**
   * @brief Get the signal quality report
   *
   * This is often a "CSQ" value ranging from 0 to 32, but may be an RSSI or a
   * percent.
   *
   * @return *int16_t* The signal quality
   */
  int16_t getSignalQuality() {
    return thisModem().getSignalQualityImpl();
  }

  /**
   * @brief Get the Local IP address assigned to the module by the network as a
   * String
   *
   * @return *String* The local IP address
   */
  String getLocalIP() {
    return thisModem().getLocalIPImpl();
  }

  /**
   * @brief Get the Local IP address assigned to the module by the network as an
   * IPAddress object.
   *
   * @return *IPAddress* The local IP address
   */
  IPAddress localIP() {
    return thisModem().TinyGsmIpFromString(thisModem().getLocalIP());
  }
  /**@}*/

 protected:
  // destructor (protected!)
  ~TinyGsmModem() {}

  /**
   * @anchor crtp_helper
   * @name CRTP Helper
   */
  /**@{*/
  inline const modemType& thisModem() const {
    return static_cast<const modemType&>(*this);
  }
  inline modemType& thisModem() {
    return static_cast<modemType&>(*this);
  }
  /**@}*/


  /**
   * @anchor modem_utilities
   * @name Utilities
   */
  /**@{*/
 public:
  // Utility templates for writing/skipping characters on a stream
  /**
   * @brief Write a value to the modem stream.
   *
   * @tparam T The type of the value to write.
   * @param last The value to write.
   */
  template <typename T>
  void streamWrite(T last) {
    thisModem().stream.print(last);
  }

  /**
   * @brief Recursively write multiple values to the modem stream.
   *
   * @tparam T The type of the first value to write.
   * @tparam Args The types of the remaining values to write.
   * @param head The first value to write.
   * @param tail The remaining values to write.
   */
  template <typename T, typename... Args>
  void streamWrite(T head, Args... tail) {
    thisModem().stream.print(head);
    thisModem().streamWrite(tail...);
  }

  /**
   * @brief Clear the modem stream.
   */
  inline void streamClear() {
    while (thisModem().stream.available()) {
      thisModem().waitResponse(50, nullptr, nullptr);
    }
  }

 protected:
  inline bool streamGetLength(char* buf, int8_t numChars,
                              const uint32_t timeout_ms = 1000L) {
    if (!buf) { return false; }

    int8_t   numCharsReady = -1;
    uint32_t startMillis   = millis();
    while (millis() - startMillis < timeout_ms &&
           (numCharsReady = thisModem().stream.available()) < numChars) {
      TINY_GSM_YIELD();
    }

    if (numCharsReady >= numChars) {
      thisModem().stream.readBytes(buf, numChars);
      return true;
    }

    return false;
  }

  inline int16_t streamGetIntLength(int8_t         numChars,
                                    const uint32_t timeout_ms = 1000L) {
    char buf[numChars + 1];
    if (streamGetLength(buf, numChars, timeout_ms)) {
      buf[numChars] = '\0';
      return atoi(buf);
    }

    return -9999;
  }

  inline int16_t streamGetIntBefore(char lastChar) {
    char   buf[7];
    size_t bytesRead = thisModem().stream.readBytesUntil(
        lastChar, buf, static_cast<size_t>(7));
    // if we read 7 or more bytes, it's an overflow
    if (bytesRead && bytesRead < 7) {
      buf[bytesRead] = '\0';
      int16_t res    = atoi(buf);
      return res;
    }

    return -9999;
  }

  inline float streamGetFloatLength(int8_t         numChars,
                                    const uint32_t timeout_ms = 1000L) {
    char buf[numChars + 1];
    if (streamGetLength(buf, numChars, timeout_ms)) {
      buf[numChars] = '\0';
      return atof(buf);
    }

    return -9999.0F;
  }

  inline float streamGetFloatBefore(char lastChar) {
    char   buf[16];
    size_t bytesRead = thisModem().stream.readBytesUntil(
        lastChar, buf, static_cast<size_t>(16));
    // if we read 16 or more bytes, it's an overflow
    if (bytesRead && bytesRead < 16) {
      buf[bytesRead] = '\0';
      float res      = atof(buf);
      return res;
    }

    return -9999.0F;
  }

  inline bool streamSkipUntil(const char c, const uint32_t timeout_ms = 1000L) {
    uint32_t startMillis = millis();
    while (millis() - startMillis < timeout_ms) {
      while (millis() - startMillis < timeout_ms &&
             !thisModem().stream.available()) {
        TINY_GSM_YIELD();
      }
      if (thisModem().stream.read() == c) { return true; }
    }
    return false;
  }

  inline void cleanResponseString(String& res) {
    // Remove the OK from the string, as well as any newlines
    const String nlResponse = String(GFP(ModemConfig::GSM_NL));
    String       okResponse = nlResponse + String(GFP(ModemConfig::GSM_OK));
    res.replace(okResponse, "");
    res.replace(nlResponse, " ");
    res.trim();
  }

  static inline IPAddress TinyGsmIpFromString(const String& strIP) {
    int Parts[4] = {
        0,
    };
    int Part = 0;
    for (uint8_t i = 0; i < strIP.length(); i++) {
      char c = strIP[i];
      if (c == '.') {
        Part++;
        if (Part > 3) { return IPAddress(0, 0, 0, 0); }
        continue;
      } else if (c >= '0' && c <= '9') {
        Parts[Part] *= 10;
        Parts[Part] += c - '0';
      } else {
        if (Part == 3) break;
      }
    }
    return IPAddress(Parts[0], Parts[1], Parts[2], Parts[3]);
  }
  /**@}*/

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the default function implementations
   */

  /*
   * Basic functions
   */
 protected:
  bool initImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;

  bool setBaudImpl(uint32_t baud) {
    thisModem().sendAT(GF("+IPR="), baud);
    return thisModem().waitResponse() == 1;
  }

  bool testATImpl(uint32_t timeout_ms) {
    for (uint32_t start = millis(); millis() - start < timeout_ms;) {
      thisModem().sendAT(GF(""));
      if (thisModem().waitResponse(200) == 1) { return true; }
      delay(100);
    }
    return false;
  }

  int8_t waitResponseImpl(uint32_t timeout_ms, String& data, GsmConstStr r1,
                          GsmConstStr r2, GsmConstStr r3, GsmConstStr r4,
                          GsmConstStr r5, GsmConstStr r6, GsmConstStr r7,
                          GsmConstStr r8) {
    data.reserve(64);

    // put the possible responses into an array so we can loop through them
    const GsmConstStr responses[8] = {r1, r2, r3, r4, r5, r6, r7, r8};

#ifdef TINY_GSM_DEBUG_DEEP
    DBG(GF("r1 <"), r1 ? r1 : GF("NULL"), GF("> r2 <"), r2 ? r2 : GF("NULL"),
        GF("> r3 <"), r3 ? r3 : GF("NULL"), GF("> r4 <"), r4 ? r4 : GF("NULL"),
        GF("> r5 <"), r5 ? r5 : GF("NULL"), GF("> r6 <"), r6 ? r6 : GF("NULL"),
        GF("> r7 <"), r7 ? r7 : GF("NULL"), GF("> r8 <"), r8 ? r8 : GF("NULL"),
        '>');
#endif
    uint8_t  index       = 0;
    uint32_t startMillis = millis();
    do {
      TINY_GSM_YIELD();
      while (thisModem().stream.available() > 0) {
        TINY_GSM_YIELD();
        int8_t a = thisModem().stream.read();
        if (a <= 0) continue;  // Skip 0x00 bytes, just in case
        data += static_cast<char>(a);
        // loop through the possible responses and see if we have a match
        for (uint8_t i = 0; i < 8; i++) {
          if (responses[i] && data.endsWith(responses[i])) {
            index = i + 1;
            goto finish;
          }
        }
#if defined TINY_GSM_DEBUG
        const String verbosePrefix1 = String(GFP(ModemConfig::GSM_VERBOSE));
        const String verbosePrefix2 = String(GFP(ModemConfig::GSM_VERBOSE_2));
        if ((data.endsWith(verbosePrefix1)) ||
            (data.endsWith(verbosePrefix2))) {
          // check how long the new line is
          // should be either 1 ('\r' or '\n') or 2 ("\r\n"))
          const String atnlString  = String(GFP(ModemConfig::GSM_NL));
          const int    len_atnl    = atnlString.length();
          const char   last_atnl_c = len_atnl > 0 ? atnlString[len_atnl - 1]
                                                  : '\n';
          // Read out the verbose message, until the last character of the new
          // line
          data += thisModem().stream.readStringUntil(last_atnl_c);
#ifdef TINY_GSM_DEBUG_DEEP
          data.trim();
          DBG(GF("Verbose details <<<"), data, GF(">>>"));
#endif
          data = "";
          goto finish;
        }
#endif
        if (thisModem().handleURCs(data)) { data = ""; }
      }
    } while (millis() - startMillis < timeout_ms);
  finish:
#ifdef TINY_GSM_DEBUG_DEEP
    data.replace("\r", "←");
    data.replace("\n", "↓");
#endif
    if (!index) {
      data.trim();
      if (data.length()) { DBG("### Unhandled:", data); }
      data = "";
    } else {
#ifdef TINY_GSM_DEBUG_DEEP
      DBG('<', index, '>', data);
#endif
    }
    return index;
  }


  String getModemInfoImpl() {
    thisModem().sendAT('I');  // 3GPP TS 27.007
    String res;
    if (thisModem().waitResponse(1000L, res) != 1) { return ""; }
    thisModem().cleanResponseString(res);
    return res;
  }

  String getModemNameImpl() {
    String manufacturer = getModemManufacturer();
    String model        = getModemModel();
    String name         = manufacturer + String(" ") + model;
    DBG("### Modem:", name);
    return name;
  }

  // Gets the modem manufacturer
  String getModemManufacturerImpl() {
    thisModem().sendAT(GF("+CGMI"));  // 3GPP TS 27.007 standard
    String res;
    if (thisModem().waitResponse(1000L, res) != 1) {
      return String(ModemConfig::MODEM_MANUFACTURER);
    }
    thisModem().cleanResponseString(res);
    return res;
  }

  // Gets the modem hardware version
  String getModemModelImpl() {
    thisModem().sendAT(GF("+CGMM"));  // 3GPP TS 27.007 standard
    String res;
    if (thisModem().waitResponse(1000L, res) != 1) {
      return String(ModemConfig::MODEM_MODEL);
    }
    thisModem().cleanResponseString(res);
    return res;
  }

  // Gets the modem firmware version
  String getModemRevisionImpl() {
    thisModem().sendAT(GF("+CGMR"));  // 3GPP TS 27.007 standard
    String res;
    if (thisModem().waitResponse(1000L, res) != 1) { return "unknown"; }
    thisModem().cleanResponseString(res);
    return res;
  }

  // Gets the modem serial number
  String getModemSerialNumberImpl() {
    thisModem().sendAT(GF("+CGSN"));  // 3GPP TS 27.007 standard
    String res;
    if (thisModem().waitResponse(1000L, res) != 1) { return "unknown"; }
    thisModem().cleanResponseString(res);
    return res;
  }

  bool factoryDefaultImpl() {
    thisModem().sendAT(GF("&FZE0&W"));  // Factory + Reset + Echo Off + Write
    thisModem().waitResponse();
    thisModem().sendAT(GF("+IPR=0"));  // Auto-baud
    thisModem().waitResponse();
    thisModem().sendAT(GF("&W"));  // Write configuration
    return thisModem().waitResponse() == 1;
  }

  /*
   * Power functions
   */
 protected:
  bool radioOffImpl() {
    if (!thisModem().setPhoneFunctionality(0)) { return false; }
    delay(3000);
    return true;
  }

  bool sleepEnableImpl(bool enable) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  bool setPhoneFunctionalityImpl(uint8_t fun,
                                 bool    reset) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  /*
   * Generic network functions
   */
 protected:
  regStatusType getRegistrationStatusImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;

  // Gets the modem's registration status via CREG/CGREG/CEREG
  // CREG = Generic network registration
  // CGREG = GPRS service registration
  // CEREG = EPS registration for LTE modules
  int8_t getRegistrationStatusXREG(const char* regCommand) {
    thisModem().sendAT('+', regCommand, '?');
    // check for any of the three for simplicity
    int8_t resp = thisModem().waitResponse(GF("+CREG:"), GF("+CGREG:"),
                                           GF("+CEREG:"));
    if (resp != 1 && resp != 2 && resp != 3) { return -1; }
    thisModem().streamSkipUntil(','); /* Skip format (0) */
    int status = thisModem().stream.parseInt();
    thisModem().waitResponse();
    return status;
  }

  bool waitForNetworkImpl(uint32_t timeout_ms, bool check_signal) {
    for (uint32_t start = millis(); millis() - start < timeout_ms;) {
      if (check_signal) { thisModem().getSignalQuality(); }
      if (thisModem().isNetworkConnected()) { return true; }
      delay(250);
    }
    return false;
  }

  // Gets signal quality report according to 3GPP TS command AT+CSQ
  int8_t getSignalQualityImpl() {
    thisModem().sendAT(GF("+CSQ"));
    if (thisModem().waitResponse(GF("+CSQ:")) != 1) { return 99; }
    int8_t res = thisModem().streamGetIntBefore(',');
    thisModem().waitResponse();
    return res;
  }

  String getLocalIPImpl() {
    // AT+CGPADDR=<cid> where cid is the context id, which is usually 1 for the
    // first context
    thisModem().sendAT(GF("+CGPADDR=1"));
    if (thisModem().waitResponse(GF("+CGPADDR:")) != 1) { return ""; }
    thisModem().streamSkipUntil(',');  // Skip context id
    String res = thisModem().stream.readStringUntil('\r');
    if (thisModem().waitResponse() != 1) { return ""; }
    return res;
  }
};

#endif  // SRC_TINYGSMMODEM_TPP_
