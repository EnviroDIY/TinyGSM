/**
 * @file       TinyGsmGPRS.tpp
 * @brief      GPRS and packet-data connection helper mixin.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMGPRS_TPP_
#define SRC_TINYGSMGPRS_TPP_

#include "TinyGsmCommon.h"

#ifndef TINY_GSM_MODEM_HAS_GPRS
/// flag to indicate that the modem has GPRS functions
#define TINY_GSM_MODEM_HAS_GPRS
#endif

/// SIM card status
enum SimStatus {
  /// SIM card error
  SIM_ERROR = 0,
  /// SIM card is ready
  SIM_READY = 1,
  /// SIM card is locked (PIN required)
  SIM_LOCKED = 2,
  /// SIM card is locked due to anti-theft protection
  SIM_ANTITHEFT_LOCKED = 3,
};

/**
 * @class TinyGsmGPRS
 * @brief The CRTP parent class for GPRS functions.
 * @tparam modemType The derived modem class
 */
template <class modemType>
class TinyGsmGPRS {
 public:
  /// Compile-time capability flag indicating GPRS/cellular data support
  static constexpr bool hasGPRS = true;

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the interface
   */
 public:
  /**
   * @anchor sim_card_functions
   * @name SIM card functions
   */
  /**@{*/
  /**
   * @brief Unlock the SIM card with a PIN code.
   * @param pin The PIN code to unlock the SIM card.
   * @return True if the SIM card was successfully unlocked, false otherwise
   */
  bool simUnlock(const char* pin) {
    return thisModem().simUnlockImpl(pin);
  }
  /**
   * @brief Get the SIM card's CCID via AT+CCID.
   * @return The SIM card's CCID as a String.
   */
  String getSimCCID() {
    return thisModem().getSimCCIDImpl();
  }
  /**
   * @brief Get the modem's TA Serial Number Identification (IMEI) via AT+GSN.
   * @return The modem's IMEI as a String.
   */
  String getIMEI() {
    return thisModem().getIMEIImpl();
  }
  /**
   * @brief Get the modem's International Mobile Subscriber Identity (IMSI) via
   * AT+CIMI.
   *
   * @return The modem's IMSI as a String.
   */
  String getIMSI() {
    return thisModem().getIMSIImpl();
  }
  /**
   * @brief Get the SIM card status.
   * @param timeout_ms The timeout in milliseconds to wait for a response.
   * @return The SIM card status as a SimStatus enum value.
   */
  SimStatus getSimStatus(uint32_t timeout_ms = 10000L) {
    return thisModem().getSimStatusImpl(timeout_ms);
  }
  /**@}*/

  /**
   * @anchor gprs_functions
   * @name GPRS functions
   */
  /**@{*/

  /**
   * @brief Connect to a GPRS network.
   *
   * @param apn The Access Point Name (APN) of the network.
   * @param user The username for the APN (optional).
   * @param pwd The password for the APN (optional).
   *
   * @return True if the connection was successful, false otherwise.
   */
  bool gprsConnect(const char* apn, const char* user = nullptr,
                   const char* pwd = nullptr) {
    return thisModem().gprsConnectImpl(apn, user, pwd);
  }
  /**
   * @brief Disconnect from the GPRS network.
   * @return True if the disconnection was successful, false otherwise.
   */
  bool gprsDisconnect() {
    return thisModem().gprsDisconnectImpl();
  }
  /**
   * @brief Check if currently attached to GPRS/EPS service.
   * @return True if connected, false otherwise.
   */
  bool isGprsConnected() {
    return thisModem().isGprsConnectedImpl();
  }
  /**
   * @brief Get the current network operator.
   * @return The current network operator as a String.
   */
  String getOperator() {
    return thisModem().getOperatorImpl();
  }

  /**
   * @brief Get the current network provider.
   * @return The current network provider as a String.
   */
  String getProvider() {
    return thisModem().getProviderImpl();
  }
  /**@}*/


 protected:
  // destructor (protected!)
  ~TinyGsmGPRS() {}

  /*
   * CRTP Helper
   */
  inline const modemType& thisModem() const {
    return static_cast<const modemType&>(*this);
  }
  inline modemType& thisModem() {
    return static_cast<modemType&>(*this);
  }

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the default function implementations
   */
 protected:
  /*
   * SIM card functions
   */

  // Unlocks a sim via the 3GPP TS command AT+CPIN
  bool simUnlockImpl(const char* pin) {
    if (pin && strlen(pin) > 0) {
      thisModem().sendAT(GF("+CPIN=\""), pin, '"');
      return thisModem().waitResponse() == 1;
    }
    return true;
  }

  // Gets the CCID of a sim card via AT+CCID
  String getSimCCIDImpl() {
    thisModem().sendAT(GF("+CCID"));
    if (thisModem().waitResponse(GF("+CCID:")) != 1) { return ""; }
    String res = thisModem().stream.readStringUntil('\n');
    thisModem().waitResponse();
    res.trim();
    return res;
  }

  // Asks for TA Serial Number Identification (IMEI) via the V.25TER standard
  // AT+GSN command
  String getIMEIImpl() {
    thisModem().sendAT(GF("+GSN"));
    thisModem().streamSkipUntil('\n');  // skip first newline
    String res = thisModem().stream.readStringUntil('\n');
    thisModem().waitResponse();
    res.trim();
    return res;
  }

  // Asks for International Mobile Subscriber Identity IMSI via the AT+CIMI
  // command
  String getIMSIImpl() {
    thisModem().sendAT(GF("+CIMI"));
    thisModem().streamSkipUntil('\n');  // skip first newline
    String res = thisModem().stream.readStringUntil('\n');
    thisModem().waitResponse();
    res.trim();
    return res;
  }

  SimStatus getSimStatusImpl(uint32_t timeout_ms) {
    for (uint32_t start = millis(); millis() - start < timeout_ms;) {
      thisModem().sendAT(GF("+CPIN?"));
      if (thisModem().waitResponse(GF("+CPIN:")) != 1) {
        delay(1000);
        continue;
      }
      int8_t status = thisModem().waitResponse(
          GF("READY"), GF("SIM PIN"), GF("SIM PUK"), GF("NOT INSERTED"),
          GF("NOT READY"), GFP(ModemConfig::GSM_ERROR));
      thisModem().waitResponse();
      switch (status) {
        case 2:
        case 3: return SIM_LOCKED;
        case 1: return SIM_READY;
        default: return SIM_ERROR;
      }
    }
    return SIM_ERROR;
  }

  /*
   * GPRS functions
   */
 protected:
  // Checks if current attached to GPRS/EPS service
  bool isGprsConnectedImpl() {
    thisModem().sendAT(GF("+CGATT?"));
    if (thisModem().waitResponse(GF("+CGATT:")) != 1) { return false; }
    int8_t res = thisModem().streamGetIntBefore('\n');
    thisModem().waitResponse();
    if (res != 1) { return false; }

    return thisModem().localIP() != IPAddress(0, 0, 0, 0);
  }

  // Gets the current network operator via the 3GPP TS command AT+COPS
  String getOperatorImpl() {
    thisModem().sendAT(GF("+COPS?"));
    if (thisModem().waitResponse(GF("+COPS:")) != 1) { return ""; }
    thisModem().streamSkipUntil('"'); /* Skip mode and format */
    String res = thisModem().stream.readStringUntil('"');
    thisModem().waitResponse();
    return res;
  }

  String getProviderImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;
};

#endif  // SRC_TINYGSMGPRS_TPP_
