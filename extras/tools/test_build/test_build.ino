/**************************************************************
 *
 *  DO NOT USE THIS - this is just a compilation test!
 *  This is NOT an example for use of this library!
 *
 **************************************************************/
#define TINY_GSM_DEBUG Serial
#define TINY_GSM_DEBUG_DEEP

#include <TinyGsmClient.h>
#include <TinyGsmEnums.h>

TinyGsm modem(Serial);

void setup() {
  Serial.begin(115200);
  delay(6000);
}

void loop() {
  // Test the basic functions
  modem.begin();
  modem.begin("1234");
  modem.init();
  modem.init("1234");
  modem.sendAT("+CGMI");
  String waitData;
  modem.waitResponse(1000L, waitData);
  modem.waitResponse(1000L);
  modem.waitResponse();
  TinyGsmAutoBaud(Serial, 9600, 115200);
  modem.forceModemBaud(Serial, 115200);
  modem.setBaud(115200);
  modem.testAT();
  modem.streamWrite("AT", "\r\n");
  modem.streamClear();

  modem.getModemInfo();
  modem.getModemName();
  modem.getModemManufacturer();
  modem.getModemModel();
  modem.getModemRevision();
  modem.factoryDefault();

#if not defined(TINY_GSM_MODEM_ESP32) &&   \
    not defined(TINY_GSM_MODEM_ESP8266) && \
    not defined(TINY_GSM_MODEM_ESP8266_NONOS)
  modem.getModemSerialNumber();
#endif

  // Test Power functions
  modem.restart();
#if !defined(TINY_GSM_MODEM_ESP32) && !defined(TINY_GSM_MODEM_ESP8266) && \
    !defined(TINY_GSM_MODEM_ESP8266_NONOS) &&                             \
    !defined(TINY_GSM_MODEM_UBLOX) && !defined(TINY_GSM_MODEM_SARAR4) &&  \
    !defined(TINY_GSM_MODEM_SARAR5) && !defined(TINY_GSM_MODEM_XBEE)
  modem.sleepEnable();
  modem.sleepEnable(false);
#endif
#if !defined(TINY_GSM_MODEM_ESP32) && !defined(TINY_GSM_MODEM_ESP8266) && \
    !defined(TINY_GSM_MODEM_ESP8266_NONOS)
  modem.radioOff();
#endif
  modem.poweroff();
#if !defined(TINY_GSM_MODEM_ESP32) && !defined(TINY_GSM_MODEM_ESP8266) &&    \
    !defined(TINY_GSM_MODEM_ESP8266_NONOS) && !defined(TINY_GSM_MODEM_A6) && \
    !defined(TINY_GSM_MODEM_M95) && !defined(TINY_GSM_MODEM_SEQUANS_MONARCH)
  modem.setPhoneFunctionality(1, true);
#endif

  // Test generic network functions
  modem.getRegistrationStatus();
  modem.isNetworkConnected();
  modem.waitForNetwork();
  modem.waitForNetwork(15000L);
  modem.waitForNetwork(15000L, true);
  modem.getSignalQuality();
  modem.getLocalIP();
  modem.localIP();

// Test WiFi Functions
#if defined(TINY_GSM_MODEM_HAS_WIFI)
  modem.networkConnect("mySSID", "mySSIDPassword");
  modem.networkDisconnect();
#endif

// Test the GPRS and SIM card functions
#if defined(TINY_GSM_MODEM_HAS_GPRS)
  modem.simUnlock("1234");
  modem.getSimStatus();

  modem.gprsConnect("myAPN");
  modem.gprsConnect("myAPN", "myUser");
  modem.gprsConnect("myAPN", "myAPNUser", "myAPNPass");
  modem.gprsDisconnect();
  modem.isGprsConnected();

  modem.getSimCCID();
  modem.getIMEI();
  modem.getIMSI();
  modem.getOperator();
#if defined(TINY_GSM_MODEM_A7672X) || defined(TINY_GSM_MODEM_BG96) ||     \
    defined(TINY_GSM_MODEM_M95) || defined(TINY_GSM_MODEM_MC60) ||        \
    defined(TINY_GSM_MODEM_SIM5360) || defined(TINY_GSM_MODEM_SIM7600) || \
    defined(TINY_GSM_MODEM_SIM800)
  modem.getProvider();
#endif
#endif

  char server[]   = "somewhere";
  char resource[] = "something";

  // Test TCP functions
  modem.maintain();
  TinyGsmClient client;
  TinyGsmClient client2(modem);
  TinyGsmClient client3(modem, 1);
  client.init(&modem);
  client.init(&modem, 1);

  client.connect(server, 80);

  // Make a HTTP GET request:
  client.print(String("GET ") + resource + " HTTP/1.0\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");

#if !defined(TINY_GSM_MODEM_SEQUANS_MONARCH)
  // Write the request out to the server so it goes all at once
  client.beginWrite(63);
  client.write("GET ");
  client.write(resource);
  client.write(" HTTP/1.0\r\n");
  client.write("Host: ");
  client.write(server);
  client.write("\r\n");
  client.write("Connection: close\r\n\r\n");
  client.endWrite(63);
#endif

  uint32_t timeout = millis();
  while (client.connected() && millis() - timeout < 10000L) {
    while (client.available()) {
      client.read();
      timeout = millis();
    }
  }

  client.stop();

#if defined(TINY_GSM_MODEM_HAS_SSL)
  TinyGsmClientSecure client_secure(modem);
  TinyGsmClientSecure client_secure2(modem);
  TinyGsmClientSecure client_secure3(modem, (uint8_t)0);

#if defined(TINY_GSM_MODEM_CAN_SPECIFY_CERTS)
  TinyGsmClientSecure client_secure4(modem, 1);
  TinyGsmClientSecure(modem, SSLAuthMode::MUTUAL_AUTHENTICATION);
  TinyGsmClientSecure(modem, SSLAuthMode::MUTUAL_AUTHENTICATION,
                      SSLVersion::TLS1_2);
  TinyGsmClientSecure(modem, SSLAuthMode::MUTUAL_AUTHENTICATION,
                      SSLVersion::TLS1_2, "CAcertName");
  TinyGsmClientSecure(modem, SSLAuthMode::MUTUAL_AUTHENTICATION,
                      SSLVersion::TLS1_2, "CAcertName", "clientCertName");
  TinyGsmClientSecure(modem, 0, SSLAuthMode::MUTUAL_AUTHENTICATION,
                      SSLVersion::TLS1_2, "CAcertName", "clientCertName",
                      "clientKeyName");
  TinyGsmClientSecure(modem, "pskIdent", "psKey");
  TinyGsmClientSecure(modem, "pskIdent", "psKey", SSLVersion::TLS1_2);
  TinyGsmClientSecure(modem, 0, "pskIdent", "psKey", SSLVersion::TLS1_2);
  TinyGsmClientSecure(modem, "pskTableName", SSLVersion::TLS1_2);
  TinyGsmClientSecure(modem, "pskTableName");
  TinyGsmClientSecure(modem, (uint8_t)0, "pskTableName", SSLVersion::TLS1_2);
#endif

  client_secure.init(&modem);
  client_secure.init(&modem, 1);
#endif
#if defined(TINY_GSM_MODEM_CAN_LOAD_CERTS)
  modem.loadCertificate("certificateName", "certificate_content", 20);
#if !defined(TINY_GSM_MODEM_A7672X) && !defined(TINY_GSM_MODEM_SIM7600)
  modem.printCertificate("certificateName", Serial);
#endif
  modem.deleteCertificate("certificateName");

  modem.convertCertificate(CertificateType::CA_CERTIFICATE, "filename");
  modem.convertCertificate(CertificateType::CA_CERTIFICATE, String("filename"));
  modem.convertCACertificate("ca_cert_name");
  modem.convertCACertificate(String("ca_cert_name"));
  modem.convertClientCertificates("client_cert_name", "client_cert_key");
  modem.convertClientCertificates(String("client_cert_name"),
                                  String("client_cert_key"));
#if defined(TINY_GSM_MODEM_A7672X) || defined(TINY_GSM_MODEM_BG96) || \
    defined(TINY_GSM_MODEM_ESP32) || defined(TINY_GSM_MODEM_SIM7600)
  modem.convertPSKandID("psk", "pskIdent");
  modem.convertPSKandID(String("psk"), String("pskIdent"));
#endif
  modem.convertPSKTable("psk_table_name");
  modem.convertPSKTable(String("psk_table_name"));
#endif
#if defined(TINY_GSM_MODEM_CAN_SPECIFY_CERTS)
  client_secure.setSSLAuthMode(SSLAuthMode::NO_VALIDATION);
  client_secure.setSSLAuthMode(SSLAuthMode::MUTUAL_AUTHENTICATION);
  client_secure.setCACertName("certificateName");
  client_secure.setCACertName(String("certificateName"));
  client_secure.setClientCertName("certificateName");
  client_secure.setClientCertName(String("certificateName"));
  client_secure.setPrivateKeyName("certificateName");
  client_secure.setPrivateKeyName(String("certificateName"));
  client_secure.setPreSharedKey("pskIdent", "psKey");
  client_secure.setPreSharedKey(String("pskIdent"), String("psKey"));
#endif
#if defined(TINY_GSM_MODEM_HAS_SSL)
  client_secure.connect(server, 443);

  // Make a HTTP GET request:
  client_secure.print(String("GET ") + resource + " HTTP/1.0\r\n");
  client_secure.print(String("Host: ") + server + "\r\n");
  client_secure.print("Connection: close\r\n\r\n");

  timeout = millis();
  while (client_secure.connected() && millis() - timeout < 10000L) {
    while (client_secure.available()) {
      client_secure.read();
      timeout = millis();
    }
  }

  client_secure.stop();
#endif

// Test the calling functions
#if defined(TINY_GSM_MODEM_HAS_CALLING)
  modem.callNumber(String("+380000000000"));
  modem.callHangup();

#if not defined(TINY_GSM_MODEM_SEQUANS_MONARCH)
  modem.callAnswer();
  modem.dtmfSend('A', 1000);
#endif

#endif

// Test the SMS functions
#if defined(TINY_GSM_MODEM_HAS_SMS)
  modem.sendSMS(String("+380000000000"), String("Hello from TinyGSM!"));

#if not defined(TINY_GSM_MODEM_XBEE) && not defined(TINY_GSM_MODEM_SARAR4)
  modem.sendUSSD("*111#");
#endif

#if not defined(TINY_GSM_MODEM_XBEE) && not defined(TINY_GSM_MODEM_M590) && \
    not defined(TINY_GSM_MODEM_SARAR4)
  modem.sendSMS_UTF16("+380000000000", "Hello", 5);
#endif

#endif

// Test the GSM location functions
#if defined(TINY_GSM_MODEM_HAS_GSM_LOCATION)
  modem.getGsmLocationRaw();
  modem.getGsmLocation();
  float gsm_latitude  = 0;
  float gsm_longitude = 0;
  float gsm_accuracy  = 0;
  int   gsm_year      = 0;
  int   gsm_month     = 0;
  int   gsm_day       = 0;
  int   gsm_hour      = 0;
  int   gsm_minute    = 0;
  int   gsm_second    = 0;
  modem.getGsmLocation(&gsm_latitude, &gsm_longitude);
  modem.getGsmLocation(&gsm_latitude, &gsm_longitude, &gsm_accuracy, &gsm_year,
                       &gsm_month, &gsm_day, &gsm_hour, &gsm_minute,
                       &gsm_second);
  modem.getGsmLocationTime(&gsm_year, &gsm_month, &gsm_day, &gsm_hour,
                           &gsm_minute, &gsm_second);
  modem.getGsmLocation();
#endif

// Test the GPS functions
#if defined(TINY_GSM_MODEM_HAS_GPS)
#if defined(TINY_GSM_MODEM_SIM7600)
  modem.setGNSSMode(1, true);
  modem.getGNSSMode();
#endif
#if !defined(TINY_GSM_MODEM_SARAR5)  // not available for this module
  modem.enableGPS();
#endif
  float gps_latitude  = 0;
  float gps_longitude = 0;
  float gps_speed     = 0;
  float gps_altitude  = 0;
  int   gps_vsat      = 0;
  int   gps_usat      = 0;
  float gps_accuracy  = 0;
  int   gps_year      = 0;
  int   gps_month     = 0;
  int   gps_day       = 0;
  int   gps_hour      = 0;
  int   gps_minute    = 0;
  int   gps_second    = 0;
  modem.getGPS(&gps_latitude, &gps_longitude);
  modem.getGPS(&gps_latitude, &gps_longitude, &gps_speed, &gps_altitude,
               &gps_vsat, &gps_usat, &gps_accuracy, &gps_year, &gps_month,
               &gps_day, &gps_hour, &gps_minute, &gps_second);
  modem.getGPSTime(&gps_year, &gps_month, &gps_day, &gps_hour, &gps_minute,
                   &gps_second);
  modem.getGPSraw();
#if !defined(TINY_GSM_MODEM_SARAR5)  // not available for this module
  modem.disableGPS();
#endif
#endif

// Test the Network time functions
#if defined(TINY_GSM_MODEM_HAS_NTP)
  modem.NTPServerSync("pool.ntp.org", 3);
  modem.waitForTimeSync(1);
  modem.ShowNTPError(1);
  modem.TinyGsmIsValidNumber("1.0");
#endif

// Test the Network time function
#if defined(TINY_GSM_MODEM_HAS_TIME)
  modem.getGSMDateTime(TinyGSMDateTimeFormat::DATE_FULL);
  int   ntp_year     = 0;
  int   ntp_month    = 0;
  int   ntp_day      = 0;
  int   ntp_hour     = 0;
  int   ntp_min      = 0;
  int   ntp_sec      = 0;
  float ntp_timezone = 0;
  modem.getNetworkTime(&ntp_year, &ntp_month, &ntp_day, &ntp_hour, &ntp_min,
                       &ntp_sec, &ntp_timezone);
#if defined(TinyGSM_MODEM_BG96)
  modem.getNetworkUTCTime(&ntp_year, &ntp_month, &ntp_day, &ntp_hour, &ntp_min,
                          &ntp_sec, &ntp_timezone);
#endif
#if defined(TINY_GSM_MODEM_ESP32) || defined(TINY_GSM_MODEM_ESP8266)
  modem.getNetworkEpoch();
  modem.getNetworkEpoch(TinyGSM_EpochStart::Y2K);
#endif
#endif

// Test bluetooth functions
#if defined(TINY_GSM_MODEM_HAS_BLUETOOTH)
  modem.enableBluetooth();
  modem.disableBluetooth();
  modem.setBluetoothVisibility(true);
  modem.setBluetoothHostName("bluetooth");
#endif

// Test Battery functions
#if defined(TINY_GSM_MODEM_HAS_BATTERY)
#if !defined(TINY_GSM_MODEM_A6) && !defined(TINY_GSM_MODEM_SARAR4) && \
    !defined(TINY_GSM_MODEM_SARAR5) && !defined(TINY_GSM_MODEM_UBLOX)
  modem.getBattVoltage();
#endif
#if !defined(TINY_GSM_MODEM_SIM7600) && !defined(TINY_GSM_MODEM_XBEE)
  modem.getBattPercent();
#endif
#if !defined(TINY_GSM_MODEM_SARAR4) && !defined(TINY_GSM_MODEM_SARAR5) && \
    !defined(TINY_GSM_MODEM_UBLOX) && !defined(TINY_GSM_MODEM_SIM7600) && \
    !defined(TINY_GSM_MODEM_XBEE)
  modem.getBattChargeState();
#endif
  int8_t  chargeState   = -99;
  int8_t  chargePercent = -99;
  int16_t milliVolts    = -9999;
  modem.getBattStats(chargeState, chargePercent, milliVolts);
#endif

// Test temperature functions
#if defined(TINY_GSM_MODEM_HAS_TEMPERATURE)
  modem.getTemperature();
#endif
}
