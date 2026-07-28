/**
 * @file       TinyGsmCapabilities.h
 * @brief      Compile-time capability detection traits for TinyGSM modems.
 * @author     Sara Damiano
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2026
 * @date       2026
 */

#ifndef SRC_TINYGSMCAPABILITIES_H_
#define SRC_TINYGSMCAPABILITIES_H_

// Forward declarations of capability template classes
template <class modemType>
class TinyGsmSSL;
template <class modemType>
class TinyGsmGPS;
template <class modemType>
class TinyGsmGPRS;
template <class modemType>
class TinyGsmWifi;
template <class modemType>
class TinyGsmSMS;
template <class modemType>
class TinyGsmCalling;
template <class modemType>
class TinyGsmBattery;
template <class modemType>
class TinyGsmTemperature;
template <class modemType>
class TinyGsmNTP;
template <class modemType>
class TinyGsmTime;
template <class modemType>
class TinyGsmGSMLocation;
template <class modemType>
class TinyGsmBluetooth;
template <class modemType, class tcpConfig>
class TinyGsmTCP;

/**
 * @brief Namespace for TinyGSM capability detection traits.
 *
 * These traits check if a modem type inherits from specific capability
 * templates to determine what features are available.
 *
 * Custom implementations of type traits for AVR compatibility (no
 * <type_traits>). Compatible with C++11 and later.
 *
 * Usage:
 * @code
 * // Query capability at compile time
 * if (TinyGsmCapabilities::has_ssl<TinyGsm>::value) {
 *   Serial.println("SSL is supported");
 * }
 *
 * // For conditional compilation, use C++17 if constexpr:
 * if constexpr (TinyGsmCapabilities::has_ssl<TinyGsm>::value) {
 *   TinyGsmClientSecure client(modem);  // Only compiles if SSL available
 * }
 * @endcode
 */
namespace TinyGsmCapabilities {

/**
 * @brief Custom integral_constant (like std::integral_constant)
 */
template <typename T, T v>
struct integral_constant {
  static const T            value = v;
  typedef T                 value_type;
  typedef integral_constant type;
};

typedef integral_constant<bool, true>  true_type;
typedef integral_constant<bool, false> false_type;

/**
 * @brief Custom is_base_of implementation (simplified)
 * Tests if Base is a base class of Derived
 */
template <typename Base, typename Derived>
struct is_base_of {
 private:
  // Use sizeof trick to determine inheritance at compile time
  // If Derived* can be converted to Base*, inheritance exists
  typedef char yes[1];
  typedef char no[2];

  static yes& test(Base*);
  static no&  test(...);

  // Create a pointer to Derived for the test
  static Derived* getDerived();

 public:
  static const bool value = sizeof(test(getDerived())) == sizeof(yes);
};

/**
 * @brief Detect if a modem type has SSL/TLS support
 */
template <typename T>
struct has_ssl : is_base_of<TinyGsmSSL<T>, T> {};

/**
 * @brief Detect if a modem type can specify certificates
 * @note If SSL is supported, certificate specification is also supported
 */
template <typename T>
struct can_specify_certs : is_base_of<TinyGsmSSL<T>, T> {};

/**
 * @brief Detect if a modem type can load certificates
 * @note If SSL is supported, certificate loading is also supported
 */
template <typename T>
struct can_load_certs : is_base_of<TinyGsmSSL<T>, T> {};

/**
 * @brief Detect if a modem type has GPS/GNSS support
 */
template <typename T>
struct has_gps : is_base_of<TinyGsmGPS<T>, T> {};

/**
 * @brief Detect if a modem type has GPRS/cellular data support
 */
template <typename T>
struct has_gprs : is_base_of<TinyGsmGPRS<T>, T> {};

/**
 * @brief Detect if a modem type has WiFi support
 */
template <typename T>
struct has_wifi : is_base_of<TinyGsmWifi<T>, T> {};

/**
 * @brief Detect if a modem type has SMS messaging support
 */
template <typename T>
struct has_sms : is_base_of<TinyGsmSMS<T>, T> {};

/**
 * @brief Detect if a modem type has voice calling support
 */
template <typename T>
struct has_calling : is_base_of<TinyGsmCalling<T>, T> {};

/**
 * @brief Detect if a modem type has battery status support
 */
template <typename T>
struct has_battery : is_base_of<TinyGsmBattery<T>, T> {};

/**
 * @brief Detect if a modem type has temperature sensor support
 */
template <typename T>
struct has_temperature : is_base_of<TinyGsmTemperature<T>, T> {};

/**
 * @brief Detect if a modem type has NTP support
 */
template <typename T>
struct has_ntp : is_base_of<TinyGsmNTP<T>, T> {};

/**
 * @brief Detect if a modem type has time/clock functions
 */
template <typename T>
struct has_time : is_base_of<TinyGsmTime<T>, T> {};

/**
 * @brief Detect if a modem type has GSM location support
 */
template <typename T>
struct has_gsm_location : is_base_of<TinyGsmGSMLocation<T>, T> {};

/**
 * @brief Detect if a modem type has Bluetooth support
 */
template <typename T>
struct has_bluetooth : is_base_of<TinyGsmBluetooth<T>, T> {};

/**
 * @brief Detect if a modem type has TCP socket support
 * @note Always true since all modems must support TCP
 */
template <typename T>
struct has_tcp : true_type {};

}  // namespace TinyGsmCapabilities

#endif  // SRC_TINYGSMCAPABILITIES_H_
