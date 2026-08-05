/**
 * @file       TinyGsmCommon.h
 * @brief      Common TinyGSM macros, utilities, and shared constants.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCOMMON_H_
#define SRC_TINYGSMCOMMON_H_

/// The current library version number
#define TINYGSM_VERSION "0.12.0"

#if defined(SPARK) || defined(PARTICLE)
#include "Particle.h"
#elif defined(ARDUINO)
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#endif

#if defined(ARDUINO_DASH)
#include <ArduinoCompat/Client.h>
#else
#include <Client.h>
#endif

#ifndef TINY_GSM_YIELD_MS
/// The number of milliseconds to yield to the system in the main loop
#define TINY_GSM_YIELD_MS 0
#endif

#ifndef TINY_GSM_YIELD
/// The macro to yield to the system in the main loop
#define TINY_GSM_YIELD() \
  { delay(TINY_GSM_YIELD_MS); }
#endif

/**
 * @def TINY_GSM_RX_BUFFER
 * @brief The size of the receive buffer for the modem.
 *
 * This is used in the TinyGsm class to store incoming data from the modem. This
 * buffer is used to store incoming data from the modem before it is read by the
 * user.  If this buffer is too small, data may be lost if the user does not
 * read it quickly enough - especially for modems that do not internally buffer
 * data.  If this buffer is too large, it may use more memory than necessary.
 *
 * @note This is *not* the size of the modem's internal buffer!
 *
 * @important This is a library-wide setting.  It applies to all modems and all
 * clients.  If you need different buffer sizes for different modems, you will
 * need to modify the library.
 */
#if !defined(TINY_GSM_RX_BUFFER)
// Fallback log buffer size based on processor type
#if defined(__SAMD51__)
#define TINY_GSM_RX_BUFFER 1024
#elif defined(ARDUINO_ARCH_SAMD)
#define TINY_GSM_RX_BUFFER 256
#else
#define TINY_GSM_RX_BUFFER 64
#endif
#endif

/**
 * @def TINY_GSM_UNREAD_CHECK_MS
 * @brief The time in milliseconds to wait before checking for unread data from
 * the modem.
 *
 * This is used to ask the modem if there is any unread data available because
 * they don't always issue a URC to notify the host that data is available.
 *
 * @important This is a library-wide setting.  It applies to all modems and all
 * clients.  If you need different buffer sizes for different modems, you will
 * need to modify the library.
 */
#if !defined(TINY_GSM_UNREAD_CHECK_MS)
#define TINY_GSM_UNREAD_CHECK_MS 500
#endif

/**
 * @def TINY_GSM_ATTR_NOT_AVAILABLE
 * @brief Helper macro to mark a function as not available on this modem type
 * @def TINY_GSM_ATTR_NOT_IMPLEMENTED
 * @brief Helper macro to mark a function as not implemented on this modem type
 */
#define TINY_GSM_ATTR_NOT_AVAILABLE \
  __attribute__((error("Not available on this modem type")))
#define TINY_GSM_ATTR_NOT_IMPLEMENTED __attribute__((error("Not implemented")))

/**
 * @def TINY_GSM_PROGMEM
 * @brief Helper macro for memory storage location
 * @typedef GsmConstStr
 * @brief Helper typedef for a constant string stored in program memory
 * (PROGMEM) on AVR platforms and in standard memory on other systems.
 * @def GFP
 * @brief Helper macro to cast data as a constant string in program memory
 * (PROGMEM) on AVR platforms and in standard memory on other systems.
 * @def GF
 * @brief Helper macro to store data in program memory (PROGMEM) on AVR
 * platforms and in standard memory on other systems.
 */
#if defined(PROGMEM) && (defined(__AVR__) || defined(ARDUINO_ARCH_AVR)) && \
    !defined(__AVR_ATmega4809__) && !defined(GFP) && !defined(GF)
#define TINY_GSM_PROGMEM PROGMEM
typedef const __FlashStringHelper* GsmConstStr;
#define GFP(x) (reinterpret_cast<GsmConstStr>(x))
#define GF(x) F(x)
#elif !defined(TINY_GSM_PROGMEM) && !defined(GFP) && !defined(GF)
#define TINY_GSM_PROGMEM
typedef const char* GsmConstStr;
#define GFP(x) x
#define GF(x) x
#endif

#ifdef TINY_GSM_DEBUG
namespace {
/**
 * @brief Debug print function for a single argument
 *
 * @tparam T The type of the argument
 * @param last The argument to print
 */
template <typename T>
static void DBG_PLAIN(T last) {
  TINY_GSM_DEBUG.println(last);
}

/**
 * @brief Debug print function for multiple arguments
 *
 * @tparam T The type of the first argument
 * @tparam Args The types of the remaining arguments
 * @param head The first argument to print
 * @param tail The remaining arguments to print
 */
template <typename T, typename... Args>
static void DBG_PLAIN(T head, Args... tail) {
  TINY_GSM_DEBUG.print(head);
  TINY_GSM_DEBUG.print(' ');
  DBG_PLAIN(tail...);
}

/**
 * @brief Debug print function for multiple arguments with timestamp
 *
 * @tparam Args The types of the arguments
 * @param args The arguments to print
 */
template <typename... Args>
static void DBG(Args... args) {
  TINY_GSM_DEBUG.print('[');
  TINY_GSM_DEBUG.print(millis());
  TINY_GSM_DEBUG.print(GF("] "));
  DBG_PLAIN(args...);
}
}  // namespace
#else
#define DBG_PLAIN(...)
#define DBG(...)
#endif

/*
 * Min/Max Helpers
 */
template <class T>
const T& TinyGsmMin(const T& a, const T& b) {
  return (b < a) ? b : a;
}
template <class T>
const T& TinyGsmMax(const T& a, const T& b) {
  return (b < a) ? a : b;
}

/**
 * @brief Attempts to automatically find the baud rate for the modem.
 * @note This DOES NOT work with the XBee module
 *
 * @param at_serial The serial port connected to the modem
 * @param minimum The minimum baud rate to try (default: 9600)
 * @param maximum The maximum baud rate to try (default: 921600)
 * @return The baud rate that the modem responded to, or 0 if no response was
 * received
 */
template <class T>
uint32_t TinyGsmAutoBaud(T& at_serial, uint32_t minimum = 9600,
                         uint32_t maximum = 921600) {
  static uint32_t rates[] = {115200, 57600, 9600,  921600, 38400, 19200, 460800,
                             230400, 74400, 74880, 2400,   4800,  14400, 28800};

  for (uint8_t i = 0; i < sizeof(rates) / sizeof(rates[0]); i++) {
    uint32_t rate = rates[i];
    if (rate < minimum || rate > maximum) continue;

    DBG("Trying baud rate", rate, "...");
    at_serial.end();
    unsigned long origTimeout = at_serial.getTimeout();
    at_serial.setTimeout(100);  // avoid 1s default blocking wait
    at_serial.begin(rate);
    delay(10);
    for (int j = 0; j < 10; j++) {
      at_serial.print("AT\r\n");
      String input = at_serial.readString();
      if (input.indexOf("OK") >= 0) {
        DBG("Modem responded at rate", rate);
        at_serial.setTimeout(origTimeout);  // reset timeout
        return rate;
      }
    }
    at_serial.setTimeout(origTimeout);  // reset timeout
  }
  at_serial.begin(minimum);
  return 0;
}

#endif  // SRC_TINYGSMCOMMON_H_
