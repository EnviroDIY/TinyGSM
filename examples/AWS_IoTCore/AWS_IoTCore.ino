/**************************************************************
 *
 * For this example, you need to install PubSubClient library:
 *   https://github.com/knolleary/pubsubclient
 *   or from http://librarymanager/all#PubSubClient
 *
 * NOTE: This example only works for modules that have support for writing
 * certificates. Modules that support SSL in this library, but not writing
 * certificates, cannot use this example!
 *
 **************************************************************
 * This example connects to AWS IoT Core using MQTT over SSL.
 **************************************************************/

// Select your modem:
// #define TINY_GSM_MODEM_SIM7000SSL
// #define TINY_GSM_MODEM_SIM7080
// #define TINY_GSM_MODEM_A7672X
// #define TINY_GSM_MODEM_ESP32
// #define TINY_GSM_MODEM_SEQUANS_MONARCH
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_XBEE

#define TINY_GSM_TCP_KEEP_ALIVE 180

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include "aws_iot_config.h"

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#ifndef __AVR_ATmega328P__
#define SerialAT SerialBee

// or Software Serial on Uno, Nano
#else
#include <SoftwareSerialMon.h>
SoftwareSerial SerialAT(2, 3);  // RX, TX
#endif

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
// #define TINY_GSM_DEBUG SerialMon

// Range to attempt to autobaud
// NOTE:  DO NOT AUTOBAUD in production code.  Once you've established
// communication, set a fixed baud rate using modem.setBaud(#).
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 921600

// Add a reception delay, if needed.
// This may be needed for a fast processor at a slow baud rate.
// #define TINY_GSM_YIELD() { delay(2); }

// Define how you're planning to connect to the internet.
// This is only needed for this example, not in other code.
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[]      = "YourAPN";
const char gprsUser[] = "";
const char gprsPass[] = "";

// Your WiFi connection credentials, if applicable
// const char wifiSSID[] = "YourSSID";
// const char wifiPass[] = "YourWiFiPass";
const char wifiSSID[] = "4DsAndCounting";
const char wifiPass[] = "W4171n843";

// MQTT details
// get the broker host/endpoint from AWS IoT Core / Connect / Domain
// Configurations
const char* broker = AWS_IOT_ENDPOINT;
// the secure connection port for MQTT is always 8883
uint16_t port = 8883;
// the client ID should be the name of your "thing" in AWS IoT Core
const char* clientId = THING_NAME;

static const char topicInit[] TINY_GSM_PROGMEM = THING_NAME "/init";
static const char msgInit[] TINY_GSM_PROGMEM   = "{\"" THING_NAME
                                               "\":\"connected\"}";
static const char topicLed[] TINY_GSM_PROGMEM       = THING_NAME "/led";
static const char topicLedStatus[] TINY_GSM_PROGMEM = THING_NAME "/ledStatus";

// whether to print certs after uploading
// not all modules support printing the content of certificates after uploading
// them
bool print_certs  = false;
bool delete_certs = false;

// NOTE: some modems (SIM70xx modules) suggest that you delete the
// certificate file from the file system after converting the certificate so
// that they cannot be read back.  On other modules (SIM7600, A7672, ESP32,
// BG96, XBee) the certificate must be in the file system to be used and cannot
// be deleted.
#if defined(TINY_GSM_MODEM_ESP32) && defined(TINY_GSM_MODEM_BG96)
delete_certs = false;
#endif


// The certificates should generally be formatted as ".pem", ".der", or (for
// some modules) ".p7b" files.

// For most modules the actual filename doesn't matter much but it CANNOT HAVE
// SPACES.

// For Espressif modules, only two certificate sets are supported and the
// certificates must be named "client_ca.{0|1}", "client_cert.{0|1}", or
// "client_key.{0|1}"
#ifdef TINY_GSM_MODEM_ESP32
const char* root_ca_name     = "client_ca.1";
const char* client_cert_name = "client_cert.1";
const char* client_key_name  = "client_key.1";
#else
// const char* root_ca_name = "root_ca_1.crt";
const char* root_ca_name = "AmazonRootCA1.pem";
// const char* root_ca_name = "pca3-g5.crt.pem";
// const char* root_ca_name = "SFSRootCAG2.pem";
const char* client_cert_name = "client_cert_1.crt";
const char* client_key_name  = "client_key_1.key";
#endif

// Just in case someone defined the wrong thing..
#if TINY_GSM_USE_GPRS && not defined TINY_GSM_MODEM_HAS_GPRS
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS false
#define TINY_GSM_USE_WIFI true
#endif
#if TINY_GSM_USE_WIFI && not defined TINY_GSM_MODEM_HAS_WIFI
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#endif

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm        modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

TinyGsmClientSecure secureClient(modem, (uint8_t)0);
PubSubClient        mqtt(secureClient);

#define LED_PIN 13
int ledStatus = LOW;

uint32_t lastReconnectAttempt = 0;

void mqttCallback(char* topic, byte* payload, unsigned int len) {
  SerialMon.print("Message arrived [");
  SerialMon.print(topic);
  SerialMon.print("]: ");
  SerialMon.write(payload, len);
  SerialMon.println();

  // Only proceed if incoming message's topic matches
  if (String(topic) == topicLed) {
    ledStatus = !ledStatus;
    digitalWrite(LED_PIN, ledStatus);
    if (ledStatus) {
      mqtt.publish(topicLedStatus, "{\"LED status\":\"1\"}");
    } else {
      mqtt.publish(topicLedStatus, "{\"LED status\":\"0\"}");
    }
  }
}

boolean mqttConnect() {
  SerialMon.print("Connecting to ");
  SerialMon.print(broker);
  SerialMon.print(" with client ID ");
  SerialMon.println(clientId);

  // Connect to MQTT Broker
  boolean status = mqtt.connect(clientId);

  if (status == false) {
    SerialMon.println(" ...failed to connect to AWS IoT MQTT broker!");
    return false;
  }
  SerialMon.println(" ...success");

  SerialMon.print("Publishing a message to ");
  SerialMon.println(topicInit);
  bool got_pub = mqtt.publish(topicInit, msgInit);
  SerialMon.println(got_pub ? "published" : "failed to publish");
  SerialMon.print("Subscribing to ");
  SerialMon.println(topicLed);
  bool got_sub = mqtt.subscribe(topicLed);
  SerialMon.println(got_sub ? "subscribed" : "failed to subscribe");

  return mqtt.connected();
}


void setup() {
  // Set console baud rate
  SerialMon.begin(921600);
  delay(10);

  while (!SerialMon && millis() < 10000L) {}

  pinMode(LED_PIN, OUTPUT);

  // !!!!!!!!!!!
  // Set your reset, enable, power pins here
  // pins
  int8_t _modemPowerPin   = 18;  // Mayfly 1.1
  int8_t _modemSleepRqPin = 23;  // Mayfly 1.1
  int8_t _modemStatusPin  = 19;  // Mayfly 1.1
  // set pin modes
  pinMode(_modemPowerPin, OUTPUT);
  pinMode(_modemSleepRqPin, OUTPUT);
  pinMode(_modemStatusPin, INPUT);
  // wake settings
  uint32_t _wakeDelay_ms = 1000L;  // SIM7080G
  uint32_t _wakePulse_ms = 1100L;  // SIM7080G
#ifdef TINY_GSM_MODEM_SIM7080
  bool _wakeLevel = HIGH;  // SIM7080G is low, but EnviroDIY LTE Bee inverts it
#else
  bool _wakeLevel = LOW;
#endif

  // start with the modem powered off
  DBG(F("Starting with modem powered down. Wait..."));
#ifdef TINY_GSM_MODEM_SIM7080
  digitalWrite(_modemSleepRqPin, !_wakeLevel);
#endif
  digitalWrite(_modemPowerPin, LOW);
  delay(5000L);

  // power the modem
  DBG(F("Powering modem with pin"), _modemPowerPin, F("and waiting"),
      _wakeDelay_ms, F("ms for power up."));
#ifdef TINY_GSM_MODEM_XBEE
  digitalWrite(_modemSleepRqPin, _wakeLevel);
#endif
  digitalWrite(_modemPowerPin, HIGH);

#ifdef TINY_GSM_MODEM_SIM7080
  delay(_wakeDelay_ms);  // SIM7080G wake delay
  // wake the modem
  DBG(F("Sending a"), _wakePulse_ms, F("ms"), _wakeLevel ? F("HIGH") : F("LOW"),
      F("wake-up pulse on pin"), _modemSleepRqPin);
  digitalWrite(_modemSleepRqPin, _wakeLevel);
  delay(_wakePulse_ms);  // >1s
  digitalWrite(_modemSleepRqPin, !_wakeLevel);
  // !!!!!!!!!!!
#endif

  DBG("Wait...");
  delay(500L);

  // Set GSM module baud rate
#ifndef TINY_GSM_MODEM_XBEE
  TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);
#else
  SerialAT.begin(115200);
#endif
  // SerialAT.begin(921600);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.print("Initializing modem...");
  if (!modem.restart()) {  // modem.init();
    SerialMon.println(" ...failed to initialize modem!");
  }
  SerialMon.println(" ...success");

  // Max out the baud rate, if desired
  // NOTE: Do this **AFTER** the modem has been restarted - many modules revert
  // to default baud rates when reset or powered off.
  // 921600, 460800, 230400, 115200
  // modem.setBaud(921600);
  // SerialAT.end();
  // delay(100);
  // SerialAT.begin(921600);
  // delay(100);
  modem.init();  // May need to re-init to turn off echo, etc

  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);
  String modemRevision = modem.getModemRevision();
  SerialMon.print("Modem Revision: ");
  SerialMon.println(modemRevision);

#if TINY_GSM_USE_GPRS
  // Unlock your SIM card with a PIN if needed
  if (GSM_PIN && modem.getSimStatus() != 3) { modem.simUnlock(GSM_PIN); }
#endif

#if TINY_GSM_USE_WIFI
  // Wifi connection parameters must be set before waiting for the network
  SerialMon.print(F("Setting SSID/password..."));
  if (!modem.networkConnect(wifiSSID, wifiPass)) {
    SerialMon.println(" ...failed to connect to WiFi!");
  }
  SerialMon.println(" ...success");
#endif

  // ======================== CERTIFICATE NAMES ========================
  // The certificates are stored in the "certificates.h" file

  // const char* root_ca     = AWS_SERVER_CERTIFICATE;
  const char* client_cert = AWS_CLIENT_CERTIFICATE;
  const char* client_key  = AWS_CLIENT_PRIVATE_KEY;

#ifdef TINY_GSM_MODEM_CAN_LOAD_CERTS
  // ======================== CA CERTIFICATE LOADING ========================
  bool cert_success = true;
  // add the server's certificate authority certificate to the modem
  SerialMon.println("Loading Certificate Authority Certificates");
  cert_success &= modem.loadCertificate(root_ca_name, AWS_ROOT_CA1_CERTIFICATE,
                                        strlen(AWS_ROOT_CA1_CERTIFICATE));
#ifndef TINY_GSM_MODEM_ESP32
  cert_success &= modem.loadCertificate("AmazonRootCA1.pem",
                                        AWS_ROOT_CA1_CERTIFICATE,
                                        strlen(AWS_ROOT_CA1_CERTIFICATE));
  cert_success &= modem.loadCertificate("pca3-g5.crt.pem",
                                        AWS_VERISIGN_G5_CERTIFICATE,
                                        strlen(AWS_VERISIGN_G5_CERTIFICATE));
  cert_success &= modem.loadCertificate("SFSRootCAG2.pem",
                                        AWS_STARFIELD_G2_CERTIFICATE,
                                        strlen(AWS_STARFIELD_G2_CERTIFICATE));
#endif
  if (print_certs) {
    // print out the certificate to make sure it matches
    SerialMon.println(
        "Printing Certificate Authority Certificate to confirm it matches");
    modem.printCertificate(root_ca_name, SerialMon);
  }
  // convert the certificate to the modem's format
  SerialMon.println("Converting Certificate Authority Certificate");
  cert_success &= modem.convertCACertificate(root_ca_name);
  if (delete_certs) { cert_success &= modem.deleteCertificate(root_ca_name); }
  delay(1000);

  // ======================= CLIENT CERTIFICATE LOADING =======================
  // add the client's certificate and private key to the modem
  cert_success &= modem.loadCertificate(client_cert_name, client_cert,
                                        strlen(client_cert));
  if (print_certs) {
    // print out the certificate to make sure it matches
    modem.printCertificate(client_cert_name, SerialMon);
  }
  delay(1000);
  cert_success &= modem.loadCertificate(client_key_name, client_key,
                                        strlen(client_key));
  if (print_certs) {
    // print out the certificate to make sure it matches
    modem.printCertificate(client_key_name, SerialMon);
  }
  // convert the client certificate pair to the modem's format
  cert_success &= modem.convertClientCertificates(client_cert_name,
                                                  client_key_name);
  if (delete_certs) {
    cert_success &= modem.deleteCertificate(client_cert_name);
    cert_success &= modem.deleteCertificate(client_key_name);
  }
  delay(1000);
#endif

  // =================== SET CERTIFICATES FOR THE CONNECTION ===================
  // AWS IoT Core requires mutual authentication
  DBG("Requiring mutual authentication on socket");
  secureClient.setSSLAuthMode(SSLAuthMode::MUTUAL_AUTHENTICATION);
  DBG("Requesting TLS 1.3 on socket");
  secureClient.setSSLVersion(SSLVersion::TLS1_3);
  // attach the uploaded certificates to the secure client
  DBG("Assigning", root_ca_name, "as certificate authority on socket");
  secureClient.setCACertName(root_ca_name);
  DBG("Assigning", client_cert_name, "as client certificate on socket");
  secureClient.setClientCertName(client_cert_name);
  DBG("Assigning", client_key_name, "as client key on socket");
  secureClient.setPrivateKeyName(client_key_name);

  // =================== WAIT FOR NETWORK REGISTRATION ===================
#if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_XBEE
  // The XBee must run the gprsConnect function BEFORE waiting for network!
  // All other modules must wait for network first.
  modem.gprsConnect(apn, gprsUser, gprsPass);
#endif
  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork(300000L)) {
    SerialMon.println(" ...failed to connect to network!");
  }
  SerialMon.println(" ...success");

  if (modem.isNetworkConnected()) { SerialMon.println("Network connected"); }

  // ====================== MAKE DATA CONNECTION =======================
#if TINY_GSM_USE_GPRS
  // GPRS connection parameters are usually set after network registration
  SerialMon.print(F("Connecting to "));
  SerialMon.println(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" ...failed to connect to GPRS!");
  }
  SerialMon.println(" ...success");

  if (modem.isGprsConnected()) { SerialMon.println("GPRS connected"); }
#endif

#ifdef TINY_GSM_MODEM_HAS_NTP
  // enable/force time sync with NTP server
  // This is **REQUIRED** for validated SSL connections
  DBG("Enabling time sync with NTP server");
  modem.NTPServerSync("pool.ntp.org", -4);

  String time = modem.getGSMDateTime(TinyGSMDateTimeFormat::DATE_FULL);
  DBG("Current Network Time:", time);
#endif

  // MQTT Broker setup
  mqtt.setServer(broker, port);
  mqtt.setCallback(mqttCallback);

  delay(500);
  DBG("Finished setup");
}

void loop() {
  // Make sure we're still registered on the network
  if (!modem.isNetworkConnected()) {
    SerialMon.println("Network disconnected");
    if (!modem.waitForNetwork(180000L, true)) {
      SerialMon.println(" ...failed to reconnect to network!");
      delay(10000);
      return;
    }
    if (modem.isNetworkConnected()) {
      SerialMon.println("Network re-connected");
    }

#if TINY_GSM_USE_GPRS
    // and make sure GPRS/EPS is still connected
    if (!modem.isGprsConnected()) {
      SerialMon.println("GPRS disconnected!");
      SerialMon.print(F("Connecting to "));
      SerialMon.println(apn);
      if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        SerialMon.println(" ...failed to reconnect to GPRS!");
        delay(10000);
        return;
      }
      if (modem.isGprsConnected()) { SerialMon.println("GPRS reconnected"); }
    }
#endif
  }

  if (!mqtt.connected()) {
    SerialMon.println("=== MQTT NOT CONNECTED ===");
    // Reconnect every 10 seconds
    uint32_t t = millis();
    if (t - lastReconnectAttempt > 30000L) {
      lastReconnectAttempt = t;
      if (mqttConnect()) { lastReconnectAttempt = 0; }
    }
    delay(5000L);
    return;
  }

  mqtt.loop();
}
