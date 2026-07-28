/** ============================================================================
 * @example CapabilityQuery.ino
 *
 * @brief Demonstrates compile-time modem capability detection.
 *
 * This example shows how to query modem capabilities at compile time using
 * capability detection traits. This is particularly useful when working with
 * multiple modem types in the same codebase.
 *
 * @author  Sara Damiano
 * @license LGPL-3.0
 * @date    2026
 * ========================================================================== */

// Select your modem:
#define TINY_GSM_MODEM_SIM7080
// #define TINY_GSM_MODEM_SIM7600
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_ESP8266

#include <TinyGsmClient.h>
#include <TinyGsmCapabilities.h>

// Set serial for debug console (to the Serial Monitor)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1

// Create the modem object
TinyGsm modem(SerialAT);

void setup() {
  SerialMon.begin(115200);
  delay(10);

  SerialMon.println("\n\n===========================================");
  SerialMon.println("TinyGSM Capability Query Example");
  SerialMon.println("===========================================\n");

  // Print the modem type
  SerialMon.println("Modem: SIM7080");
  SerialMon.println();

  // Query capabilities using compile-time traits
  // Note: These traits are evaluated at compile time, even with C++11
  SerialMon.println("Compile-time capability detection:");
  SerialMon.println("----------------------------------");

  // SSL/TLS capabilities
  if (TinyGsmCapabilities::has_ssl<TinyGsm>::value) {
    SerialMon.println("✓ SSL/TLS support: YES");
    if (TinyGsmCapabilities::can_specify_certs<TinyGsm>::value) {
      SerialMon.println("  ✓ Can specify certificates");
    }
    if (TinyGsmCapabilities::can_load_certs<TinyGsm>::value) {
      SerialMon.println("  ✓ Can load certificates");
    }
  } else {
    SerialMon.println("✗ SSL/TLS support: NO");
  }

  // GPS capability
  if (TinyGsmCapabilities::has_gps<TinyGsm>::value) {
    SerialMon.println("✓ GPS/GNSS support: YES");
  } else {
    SerialMon.println("✗ GPS/GNSS support: NO");
  }

  // Cellular data capability
  if (TinyGsmCapabilities::has_gprs<TinyGsm>::value) {
    SerialMon.println("✓ GPRS/Cellular data: YES");
  } else {
    SerialMon.println("✗ GPRS/Cellular data: NO");
  }

  // WiFi capability
  if (TinyGsmCapabilities::has_wifi<TinyGsm>::value) {
    SerialMon.println("✓ WiFi support: YES");
  } else {
    SerialMon.println("✗ WiFi support: NO");
  }

  // SMS capability
  if (TinyGsmCapabilities::has_sms<TinyGsm>::value) {
    SerialMon.println("✓ SMS messaging: YES");
  } else {
    SerialMon.println("✗ SMS messaging: NO");
  }

  // Voice calling capability
  if (TinyGsmCapabilities::has_calling<TinyGsm>::value) {
    SerialMon.println("✓ Voice calling: YES");
  } else {
    SerialMon.println("✗ Voice calling: NO");
  }

  // Battery status capability
  if (TinyGsmCapabilities::has_battery<TinyGsm>::value) {
    SerialMon.println("✓ Battery status: YES");
  } else {
    SerialMon.println("✗ Battery status: NO");
  }

  // Temperature sensor capability
  if (TinyGsmCapabilities::has_temperature<TinyGsm>::value) {
    SerialMon.println("✓ Temperature sensor: YES");
  } else {
    SerialMon.println("✗ Temperature sensor: NO");
  }

  // Network Time Protocol capability
  if (TinyGsmCapabilities::has_ntp<TinyGsm>::value) {
    SerialMon.println("✓ NTP support: YES");
  } else {
    SerialMon.println("✗ NTP support: NO");
  }

  // Time/clock functions capability
  if (TinyGsmCapabilities::has_time<TinyGsm>::value) {
    SerialMon.println("✓ Time/clock functions: YES");
  } else {
    SerialMon.println("✗ Time/clock functions: NO");
  }

  // GSM-based location capability
  if (TinyGsmCapabilities::has_gsm_location<TinyGsm>::value) {
    SerialMon.println("✓ GSM location: YES");
  } else {
    SerialMon.println("✗ GSM location: NO");
  }

  // Bluetooth capability
  if (TinyGsmCapabilities::has_bluetooth<TinyGsm>::value) {
    SerialMon.println("✓ Bluetooth support: YES");
  } else {
    SerialMon.println("✗ Bluetooth support: NO");
  }

  // TCP sockets capability (should always be true)
  if (TinyGsmCapabilities::has_tcp<TinyGsm>::value) {
    SerialMon.println("✓ TCP sockets: YES");
  } else {
    SerialMon.println("✗ TCP sockets: NO");
  }

  SerialMon.println("\n===========================================\n");

  // Note: For conditional compilation (e.g., creating different client types
  // based on capabilities), you would need C++17 and 'if constexpr'.
  // With C++11, use preprocessor macros or template metaprogramming instead.
  //
  // C++17 example (requires -std=c++17):
  //   if constexpr (TinyGsmCapabilities::has_ssl<TinyGsm>::value) {
  //     TinyGsmClientSecure secureClient(modem);
  //   } else {
  //     TinyGsmClient client(modem);
  //   }

  SerialMon.println("\n===========================================\n");
}

void loop() {
  // Nothing to do here
}
