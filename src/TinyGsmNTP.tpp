/**
 * @file       TinyGsmNTP.tpp
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMNTP_H_
#define SRC_TINYGSMNTP_H_

#include "TinyGsmCommon.h"

#ifndef TINY_GSM_MODEM_HAS_NTP
#define TINY_GSM_MODEM_HAS_NTP
#endif

template <class modemType>
class TinyGsmNTP {
  /* =========================================== */
  /* =========================================== */
  /*
   * Define the interface
   */
 public:
  /*
   * NTP server functions
   */

 public:
  byte NTPServerSync(String server = "pool.ntp.org", int TimeZone = 0) {
    return thisModem().NTPServerSyncImpl(server, TimeZone);
  }
  String ShowNTPError(byte error) {
    return thisModem().ShowNTPErrorImpl(error);
  }

  /*
   * Utilities
   */
  bool TinyGsmIsValidNumber(String str) {
    if (!(str.charAt(0) == '+' || str.charAt(0) == '-' ||
          isDigit(str.charAt(0))))
      return false;

    for (byte i = 1; i < str.length(); i++) {
      if (!(isDigit(str.charAt(i)) || str.charAt(i) == '.')) { return false; }
    }
    return true;
  }

 protected:
  // destructor (protected!)
  ~TinyGsmNTP() {}

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

  /*
   * NTP server functions
   */
 protected:
  byte NTPServerSyncImpl(String server = "pool.ntp.org", int TimeZone = 0) {
    // Set GPRS bearer profile to associate with NTP sync
    // this may fail, it's not supported by all modules
    thisModem().sendAT(GF("+CNTPCID=1"));
    thisModem().waitResponse(10000L);

    // Set NTP server and timezone
    thisModem().sendAT(GF("+CNTP=\""), server, "\",", String(TimeZone));
    if (thisModem().waitResponse(10000L) != 1) { return -1; }

    // Request network synchronization
    thisModem().sendAT(GF("+CNTP"));
    if (thisModem().waitResponse(10000L, GF("+CNTP:"))) {
      String result = thisModem().stream.readStringUntil('\n');
      // Check for ',' in case the module appends the time next to the return
      // code. Eg: +CNTP: <code>[,<time>]
      int index = result.indexOf(',');
      if (index > 0) { result.remove(index); }
      result.trim();
      if (TinyGsmIsValidNumber(result)) { return result.toInt(); }
    } else {
      return -1;
    }
    return -1;
  }

  String ShowNTPErrorImpl(byte error) {
    switch (error) {
      case 1: return "Network time synchronization is successful";
      case 61: return "Network error";
      case 62: return "DNS resolution error";
      case 63: return "Connection error";
      case 64: return "Service response error";
      case 65: return "Service response timeout";
      default: return "Unknown error: " + String(error);
    }
  }
};

#endif  // SRC_TINYGSMNTP_H_
