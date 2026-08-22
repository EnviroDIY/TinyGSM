/** ============================================================================
 * @example{lineno} Multiple_Modules.ino
 *
 * @brief This example shows using two different modems together in the same
 * code.
 *
 * The key thing to notice is that instead of defining the modem type and
 * including the TinyGsmClient.h file, it includes the specific modem headers.
 * You cannot use the typedefs `TinyGsm`, `TinyGsmClient`, and
 * `TinyGsmClientSecure` for the modem and the clients. You must use the full
 * class names instead.
 * ========================================================================== */

// NOTE: DO NOT set a modem define!

// WARNING:  In this example, both of the modems and the communication with the
// serial monitor are all using the same hardware serial port.  This WILL NOT
// WORK IN REALITY.  You must use different serial ports for each modem and the
// serial monitor.  To communicate with both modules and to output to the serial
// monitor your board must have **3** serial ports.  If your board does not have
// 3 hardware serial ports, you can use SoftwareSerial for one or both of the
// modems, but you must use different pins for each modem and the serial
// monitor. This example uses a single port for easier CI compilation testing.

// Set serial for debug console (to the Serial Monitor)
#define SerialMon Serial

// Set serial for AT commands (to the first module)
// In real life, you must use different serial ports for each modem and the
// serial monitor.

// Use Hardware Serial on Mega, Leonardo, Micro
#if !defined(__AVR_ATmega328P__) && !defined(SerialAT1)
#define SerialAT1 Serial1
// or Software Serial on Uno, Nano
#elif !defined(SerialAT1)
#include <SoftwareSerial.h>
SoftwareSerial SerialAT1(2, 3);  // RX, TX
#endif

// Set serial for AT commands (to the second module)
// In real life, you must use different serial ports for each modem and the
// serial monitor.

// Use Hardware Serial on Mega, Leonardo, Micro
#if !defined(__AVR_ATmega328P__) && !defined(SerialAT2)
#define SerialAT2 Serial1
// or Software Serial on Uno, Nano
#elif !defined(SerialAT2)
#include <SoftwareSerial.h>
SoftwareSerial SerialAT2(2, 3);  // RX, TX
#endif

// Increase RX buffer to capture the entire response
// Chips without internal buffering (A6/A7, ESP8266, M590)
// need enough space in the buffer for the entire response
// else data will be lost (and the http library will fail).
#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 1024
#endif

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
// #define TINY_GSM_DEBUG SerialMon

// Range to attempt to autobaud
// NOTE:  DO NOT AUTOBAUD in production code.  Once you've established
// communication, set a fixed baud rate using modem1.setBaud(#).
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 921600

// Add a reception delay, if needed.
// This may be needed for a fast processor at a slow baud rate.
// #define TINY_GSM_YIELD_MS 2

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[]      = "YourAPN";
const char gprsUser[] = "";
const char gprsPass[] = "";

// Your WiFi connection credentials, if applicable
const char wifiSSID[] = "YourSSID";
const char wifiPass[] = "YourWiFiPass";

// Server 1 details (this example site requires SSL)
const char server1[]   = "vsh.pp.ua";
const char resource1[] = "/TinyGSM/logo.txt";

// Server 2 details (in this example, identical to server 1, but could be
// different)
const char server2[]   = "vsh.pp.ua";
const char resource2[] = "/TinyGSM/logo.txt";

// NOTE: Do NOT include the generic TinyGsmClient headers for specific modems.
// Include for the SIM7080 modem
#include <TinyGsmClientSIM7080.h>
// Include for the ESP32
#include <TinyGsmClientESP32.h>

#if (defined(ARDUINO_NRF52840_FEATHER)) && !defined(ADAFRUIT_TINYUSB_H_)
#include <Adafruit_TinyUSB.h>  // for Serial
#endif

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger1(SerialAT1, SerialMon);
TinyGsmSim7080 modem1(debugger1);
StreamDebugger debugger2(SerialAT2, SerialMon);
TinyGsmESP32   modem2(debugger2);
#else
TinyGsmSim7080 modem1(SerialAT1);
TinyGsmESP32   modem2(SerialAT2);
#endif

TinyGsmSim7080::GsmClientSecureSim7080 client1(modem1);
const int                              port1 = 443;

TinyGsmESP32::GsmClientSecureESP32 client2(modem2);
const int                          port2 = 443;

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  while (!SerialMon && millis() < 10000L) {}
  delay(10);

  SerialMon.println("Wait...");

  // Set GSM module baud rate
  SerialAT1.begin(115200);
  SerialAT2.begin(115200);

  SerialMon.print("Modem 1 is compiled as ");
  SerialMon.print(GFP(decltype(modem1)::ModemConfig::MODEM_MANUFACTURER));
  SerialMon.print(" ");
  SerialMon.print(GFP(decltype(modem1)::ModemConfig::MODEM_MODEL));
  SerialMon.print(" (");
  SerialMon.print((modem1).getConfiguredModem());
  SerialMon.println(")");
  SerialMon.print("Modem 2 is compiled as ");
  SerialMon.print(GFP(decltype(modem2)::ModemConfig::MODEM_MANUFACTURER));
  SerialMon.print(" ");
  SerialMon.print(GFP(decltype(modem2)::ModemConfig::MODEM_MODEL));
  SerialMon.print(" (");
  SerialMon.print((modem2).getConfiguredModem());
  SerialMon.println(")");

  // !!!!!!!!!!!
  // Set your reset, enable, power pins here
  // !!!!!!!!!!!

  DBG("Attempting to force baud rate of SIM7080 to 115200");
  modem1.forceModemBaud(SerialAT1, 115200);
  DBG("Attempting to force baud rate of ESP32 to 115200");
  modem2.forceModemBaud(SerialAT2, 115200);

  SerialMon.println(F("\n==============================="));
  SerialMon.println(F("===============================\n"));

  SerialMon.println("Initializing cellular modem...");
  modem1.init();
  String modemInfo1 = modem1.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo1);

  SerialMon.println(F("\n==============================="));
  SerialMon.println(F("===============================\n"));

  SerialMon.println("Initializing WiFi modem...");
  modem2.init();
  String modemInfo2 = modem2.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo2);

  SerialMon.println(F("\n==============================="));
  SerialMon.println(F("===============================\n"));
}

void loop() {
  // Connect the ESP32 to WiFi
  SerialMon.print(F("Setting SSID/password..."));
  if (!modem2.networkConnect(wifiSSID, wifiPass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");

  SerialMon.print("Waiting for WiFi network...");
  if (!modem2.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");
  if (modem2.isNetworkConnected()) {
    SerialMon.println("WiFi network connected");
  }

  SerialMon.println(F("\n==============================="));
  SerialMon.println(F("===============================\n"));

  // Wait for the SIM7080 to connect to the cellular network
  SerialMon.print("Waiting for cellular network...");
  if (!modem1.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");
  if (modem1.isNetworkConnected()) {
    SerialMon.println("Cellular network connected");
  }

  // GPRS connection parameters are usually set after network registration
  SerialMon.print(F("Connecting to "));
  SerialMon.print(apn);
  if (!modem1.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");
  if (modem1.isGprsConnected()) { SerialMon.println("GPRS connected"); }

  SerialMon.println(F("\n==============================="));
  SerialMon.println(F("===============================\n"));

  // get some data from a server via HTTP GET request with the cellular
  // connection
  SerialMon.print("Connecting to ");
  SerialMon.println(server1);
  if (!client1.connect(server1, port1)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");

  // Make a HTTP GET request:
  SerialMon.println("Performing HTTP GET request...");
  client1.print(String("GET ") + resource1 + " HTTP/1.1\r\n");
  client1.print(String("Host: ") + server1 + "\r\n");
  client1.print("Connection: keep-alive\r\n\r\n");

  uint32_t timeout = millis();
  while (client1.connected() && millis() - timeout < 50000L) {
    // Print available data
    while (client1.available()) {
      SerialMon.println(client1.readString());
      timeout = millis();
    }
  }
  SerialMon.println();

  // Shutdown
  client1.stop();
  SerialMon.println(F("Server disconnected"));

  SerialMon.println(F("\n==============================="));
  SerialMon.println(F("===============================\n"));

  // get some data from a server via HTTP GET request with the WiFi connection
  SerialMon.print("Connecting to ");
  SerialMon.println(server2);
  if (!client2.connect(server2, port2)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");

  // Make a HTTP GET request:
  SerialMon.println("Performing HTTP GET request...");
  client2.print(String("GET ") + resource2 + " HTTP/1.1\r\n");
  client2.print(String("Host: ") + server2 + "\r\n");
  client2.print("Connection: keep-alive\r\n\r\n");

  timeout = millis();
  while (client2.connected() && millis() - timeout < 50000L) {
    // Print available data
    while (client2.available()) {
      SerialMon.println(client2.readString());
      timeout = millis();
    }
  }
  SerialMon.println();

  client2.stop();
  SerialMon.println(F("Server disconnected"));

  SerialMon.println(F("\n==============================="));
  SerialMon.println(F("===============================\n"));

  modem1.gprsDisconnect();
  SerialMon.println(F("GPRS disconnected\n"));
  modem2.networkDisconnect();
  SerialMon.println(F("WiFi disconnected"));

  // Do nothing forevermore
  while (true) { delay(1000); }
}
