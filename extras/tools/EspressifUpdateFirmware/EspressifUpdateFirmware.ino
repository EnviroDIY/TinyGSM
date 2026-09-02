/** ============================================================================
 * @example{lineno} EspressifUpdateFirmware.ino
 *
 * @brief This sketch connects to WiFi and attempts to update the firmware for
 * an Espressif module.
 * ========================================================================== */

// Select your modem:
// #define TINY_GSM_MODEM_ESP32
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_ESP8266_NONOS

// Set serial for debug console (to the Serial Monitor)
#define SerialMon Serial

// If DBG isn't enabled, this sketch won't print anything to the console. Unless
// you want this to run silently, you should enable DBG.
#ifdef TINY_GSM_DEBUG
#undef TINY_GSM_DEBUG
#endif
#define TINY_GSM_DEBUG SerialMon

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#if !defined(__AVR_ATmega328P__) && !defined(SerialAT)
#define SerialAT Serial1

// or Software Serial on Uno, Nano
#elif !defined(SerialAT)
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2, 3);  // RX, TX
#endif

// See all AT commands, if wanted
// WARNING: At high baud rates, incoming data may be lost when dumping AT
// commands
// #define DUMP_AT_COMMANDS

// Range to attempt to autobaud
// NOTE:  DO NOT AUTOBAUD in production code.  Once you've established
// communication, set a fixed baud rate using modem.setBaud(#).
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 921600
#ifndef TARGET_BAUD
#define TARGET_BAUD 115200
#endif

// Add a reception delay, if needed.
// This may be needed for a fast processor at a slow baud rate.
// #define TINY_GSM_YIELD_MS 2

// Your WiFi connection credentials, if applicable
const char wifiSSID[] = "YourSSID";
const char wifiPass[] = "YourWiFiPass";

#include <TinyGsmClient.h>

TinyGsm modem(SerialAT);

void setup() {
  // Set console baud rate
  SerialMon.begin(921600);
  while (!SerialMon && millis() < 10000L) {}
  delay(10);

  SerialMon.println("Wait...");

  // !!!!!!!!!!!
  // Set your reset, enable, power pins here
  // !!!!!!!!!!!

  SerialMon.println(GF("Wait..."));
  delay(500L);

  SerialMon.println(F("Espressif firmware update tool for TinyGSM..."));
  SerialMon.println(F("The current version of TinyGSM is " TINYGSM_VERSION));
  SerialMon.print(F("The configured modem is "));
  SerialMon.println(modem.getConfiguredModem());
  SerialMon.println("=====================================");

  SerialMon.print(GF("Looking for modem at "));
  SerialMon.print(TARGET_BAUD);
  SerialMon.print(GF(" baud and setting baud rate to "));
  SerialMon.print(TARGET_BAUD);
  SerialMon.println(GF(" if it is not already the baud rate of the modem."));

  uint32_t maximum = 921600;
#if defined(F_CPU)
  if (F_CPU <= 8000000L) {
    maximum = 57600;
  } else if (F_CPU <= 16000000L) {
    maximum = 115200;
  }
#endif
  uint32_t targetBaud = TARGET_BAUD;
  if (targetBaud > maximum) {
    SerialMon.print(GF("Target baud rate "));
    SerialMon.print(targetBaud);
    SerialMon.print(GF(" is too high for this processor.  Maximum is "));
    SerialMon.println(maximum);
    targetBaud = maximum;
  }
  // Set GSM module baud rate
  uint32_t found_baud = TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN,
                                        GSM_AUTOBAUD_MAX);
  if (found_baud != 0 && found_baud != targetBaud) {
    SerialMon.print(GF("Changing baud rate from "));
    SerialMon.print(found_baud);
    SerialMon.print(GF(" to "));
    SerialMon.println(targetBaud);
    modem.setDefaultBaud(targetBaud);
    SerialAT.end();
    SerialAT.begin(targetBaud);
  } else if (found_baud == 0) {
    SerialMon.print(GF("Attempting to force baud rate to "));
    SerialMon.println(targetBaud);
    modem.forceModemBaud(SerialAT, targetBaud);
  }
}

void loop() {
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println(GF("Initializing modem..."));
  if (!modem.restart()) {
    // if (!modem.init()) {
    SerialMon.println(GF("Failed to restart modem, delaying 10s and retrying"));
    // restart autobaud in case GSM just rebooted
    // TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);
    delay(10000L);
    return;
  }

  String modemInfo = modem.getModemInfo();
  SerialMon.print(GF("Modem Info: "));
  SerialMon.println(modemInfo);
  (void)modemInfo;

  String name = modem.getModemName();
  SerialMon.print(GF("Modem Name: "));
  SerialMon.println(name);
  (void)name;

  String manufacturer = modem.getModemManufacturer();
  SerialMon.print(GF("Modem Manufacturer: "));
  SerialMon.println(manufacturer);
  (void)manufacturer;

  String hw_ver = modem.getModemModel();
  SerialMon.print(GF("Modem Hardware Version: "));
  SerialMon.println(hw_ver);
  (void)hw_ver;

  String fv_ver = modem.getModemRevision();
  SerialMon.print(GF("Modem Firmware Version: "));
  SerialMon.println(fv_ver);
  (void)fv_ver;

  SerialMon.println(GF("Setting SSID/password..."));
  if (!modem.networkConnect(wifiSSID, wifiPass)) {
    SerialMon.println(GF(" fail"));
    delay(10000);
    return;
  }
  SerialMon.println(" success");

  SerialMon.println(GF("Waiting for network..."));
  if (!modem.waitForNetwork(600000L, true)) {
    delay(10000);
    return;
  }

  if (modem.isNetworkConnected()) {
    SerialMon.println(GF("Network connected"));
  }

  SerialMon.println(GF("Attempting to update firmware..."));
  bool success         = true;
  bool update_complete = false;
  modem.sendAT(GF("+CIUPDATE"));
  success &= modem.waitResponse(GF("+CIPUPDATE:")) == 1;
  if (success) { success &= modem.waitResponse() == 1; }
  if (success) {
    SerialMon.println(GF("Firmware update started"));
    while (!update_complete) {
      modem.sendAT(GF("+CIUPDATE?"));
      success = modem.waitResponse(GF("+CIPUPDATE:")) == 1;
      int16_t res;
      if (success) {
        res = SerialAT.parseInt();
        success &= modem.waitResponse() == 1;
      } else {
        res = -1;
      }
      if (res == 4) {
        SerialMon.println(GF("Firmware update completed successfully"));
        update_complete = true;
      } else if (res == -1) {
        SerialMon.println(GF("Firmware update failed"));
        update_complete = true;
      } else {
        SerialMon.println(GF("Firmware update ongoing"));
        delay(1000L);
      }
    }
  }

  modem.networkDisconnect();
  SerialMon.println(GF("WiFi disconnected"));

  // Do nothing forevermore
  while (true) { modem.maintain(); }
}

// cSpell:ignore isrgrootx1
