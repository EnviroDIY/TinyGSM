/**
 * @file       TinyGsmClientESP32.h
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCLIENTESP32_H_
#define SRC_TINYGSMCLIENTESP32_H_
// #pragma message("TinyGSM:  TinyGsmClientESP32")

// #define TINY_GSM_DEBUG Serial

#define TINY_GSM_NO_MODEM_BUFFER

#include "TinyGsmClientEspressif.h"
#include "TinyGsmTCP.tpp"
#include "TinyGsmSSL.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"

// <state>: current Wi-Fi state.
//   0: ESP32 station has not started any Wi-Fi connection.
//   1: ESP32 station has connected to an AP, but does not get an IPv4 address
//   yet.
//   2: ESP32 station has connected to an AP, and got an IPv4 address.
//   3: ESP32 station is in Wi-Fi connecting or reconnecting state.
//   4: ESP32 station is in Wi-Fi disconnected state.
enum ESP32RegStatus {
  REG_UNINITIALIZED = 0,
  REG_UNREGISTERED  = 1,
  REG_OK            = 2,
  REG_CONNECTING    = 3,
  REG_DISCONNECTING = 4,
  REG_UNKNOWN       = 5,
};

class TinyGsmESP32 : public TinyGsmEspressif<TinyGsmESP32>,
                     public TinyGsmTCP<TinyGsmESP32, TINY_GSM_MUX_COUNT>,
                     public TinyGsmSSL<TinyGsmESP32, TINY_GSM_MUX_COUNT>,
                     public TinyGsmTime<TinyGsmESP32>,
                     public TinyGsmNTP<TinyGsmESP32> {
  friend class TinyGsmEspressif<TinyGsmESP32>;
  friend class TinyGsmModem<TinyGsmESP32>;
  friend class TinyGsmWifi<TinyGsmESP32>;
  friend class TinyGsmTCP<TinyGsmESP32, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmSSL<TinyGsmESP32, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmTime<TinyGsmESP32>;
  friend class TinyGsmNTP<TinyGsmESP32>;

  /*
   * Inner Client
   */
 public:
  class GsmClientESP32 : public GsmClient {
    friend class TinyGsmESP32;

   public:
    GsmClientESP32() {}

    explicit GsmClientESP32(TinyGsmESP32& modem,
                            uint8_t       mux = static_cast<uint8_t>(-1)) {
      init(&modem, mux);
    }

    bool init(TinyGsmESP32* modem, uint8_t mux = static_cast<uint8_t>(-1)) {
      this->at       = modem;
      sock_connected = false;

      // if it's a valid mux number,
      // and that mux number isn't in use,
      // accept the mux number
      if (mux < TINY_GSM_MUX_COUNT && at->sockets[mux] == nullptr) {
        this->mux              = mux;
        at->sockets[this->mux] = this;
        at->sslAuthModes[mux]  = NO_VALIDATION;
        at->CAcerts[mux]       = nullptr;
        at->clientCerts[mux]   = nullptr;
        at->clientKeys[mux]    = nullptr;
        at->pskIdents[mux]     = nullptr;  // identity for PSK cipher suites
        at->psKeys[mux]        = nullptr;  // key in hex for PSK cipher suites
      } else {
        this->mux = static_cast<uint8_t>(-1);
      }

      return true;
    }

   public:
    virtual int connect(const char* host, uint16_t port, int timeout_s) {
      if (mux < TINY_GSM_MUX_COUNT && at->sockets[mux] != nullptr) { stop(); }
      TINY_GSM_YIELD();
      rx.clear();
      uint8_t oldMux = mux;
      sock_connected = at->modemConnect(host, port, &mux, false, timeout_s);
      if (mux != oldMux && oldMux < TINY_GSM_MUX_COUNT) {
        DBG("WARNING:  Mux number changed from", oldMux, "to", mux);
        at->sockets[oldMux] = nullptr;
      }
      at->sockets[mux] = this;
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    virtual void stop(uint32_t maxWaitMs) {
      TINY_GSM_YIELD();
      at->sendAT(GF("+CIPCLOSE="), mux);
      sock_connected = false;
      at->waitResponse(maxWaitMs);
      rx.clear();
    }
    void stop() override {
      stop(5000L);
    }

    /*
     * Extended API
     */

    String remoteIP() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  };

  /*
   * Inner Secure Client
   */
 public:
  class GsmClientSecureESP32 : public GSMSecureClient<GsmClientESP32> {
   public:
    friend class TinyGsmESP32;
    friend class GsmClientESP32;
    GsmClientSecureESP32() {}

    explicit GsmClientSecureESP32(TinyGsmESP32& modem, uint8_t mux = -1)
        : GSMSecureClient<GsmClientESP32>(modem, mux) {}

    bool setCACert(uint8_t certNumber) {
      char cert_name[12];
      char cert_number[2];
      itoa(certNumber, cert_number, 10);
      strcpy(cert_name, "client_ca.");
      strcat(cert_name, cert_number);
      return at->setCertificate(CA_CERTIFICATE, cert_name);
    }
    bool setClientCert(uint8_t certNumber) {
      char cert_name[14];
      char cert_number[2];
      itoa(certNumber, cert_number, 10);
      strcpy(cert_name, "client_cert.");
      strcat(cert_name, cert_number);
      return at->setCertificate(CLIENT_CERTIFICATE, cert_name, mux);
    }
    bool setPrivateKey(uint8_t keyNumber) {
      char key_name[13];
      char key_number[2];
      itoa(keyNumber, key_number, 10);
      strcpy(key_name, "client_key.");
      strcat(key_name, key_number);
      return at->setCertificate(CLIENT_KEY, key_name, mux);
    }

    int connect(const char* host, uint16_t port, int timeout_s) override {
      if (mux < TINY_GSM_MUX_COUNT && at->sockets[mux] != nullptr) { stop(); }
      TINY_GSM_YIELD();
      rx.clear();
      uint8_t oldMux = mux;
      sock_connected = at->modemConnect(host, port, &mux, true, timeout_s);
      if (mux != oldMux && oldMux < TINY_GSM_MUX_COUNT) {
        DBG("WARNING:  Mux number changed from", oldMux, "to", mux);
        at->sockets[oldMux] = nullptr;
      }
      at->sockets[mux] = this;
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES
  };

  /*
   * Constructor
   */
 public:
  explicit TinyGsmESP32(Stream& stream)
      : TinyGsmEspressif<TinyGsmESP32>(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  // Follows functions inherited from Espressif

  /*
   * Power functions
   */
 protected:
  // Follows functions inherited from Espressif

  /*
   * Generic network functions
   */
 public:
  ESP32RegStatus getRegistrationStatus() {
    sendAT(GF("+CWSTATE?"));
    if (waitResponse(3000, GF("+CWSTATE:")) != 1) return REG_UNKNOWN;
    // +CWSTATE:<state>,<"ssid">
    // followed by an OK
    int8_t status = streamGetIntBefore(',');
    streamSkipUntil('\n');  // throw away the ssid
    waitResponse();         // wait for trailing OK
    return (ESP32RegStatus)status;
  }

 protected:
  bool isNetworkConnectedImpl() {
    ESP32RegStatus s = getRegistrationStatus();
    return (s == REG_OK);
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // NOTE: You must be running AT firmware >= 3.4.0.0 for these functions to
  // work. If you are running a lower level firmware, you must update. You
  // almost certainly will need to flash your board with the new firmware using
  // esptools/ESP flash download tools instead of using the AT+CIUPDATE function
  // because the structure of the NVM space changed and the newer structure is
  // needed for the SYSMFG command used here. The CIUPDATE function does not
  // update the NVM.

  // This adds the server's CA certificate that the client connects to, used
  // in auth mode 2 and 3
  // This is the value client_ca_0x.crt in the AT firmware
  bool addCACert(uint8_t certNumber, const char* cert, const uint16_t len) {
    char cert_name[12];
    char cert_number[2];
    itoa(certNumber, cert_number, 10);
    strcpy(cert_name, "client_ca.");
    strcat(cert_name, cert_number);
    return addCertificate(CA_CERTIFICATE, cert_name, cert, len);
  }

  bool addClientCert(uint8_t certNumber, const char* cert, const uint16_t len) {
    char cert_name[14];
    char cert_number[2];
    itoa(certNumber, cert_number, 10);
    strcpy(cert_name, "client_cert.");
    strcat(cert_name, cert_number);
    return addCertificate(cert_name, cert, len);
  }

  bool addPrivateKey(uint8_t keyNumber, const char* key, const uint16_t len) {
    char key_name[13];
    char key_number[2];
    itoa(keyNumber, key_number, 10);
    strcpy(key_name, "client_key.");
    strcat(key_name, key_number);
    return addCertificate(key_name, key, len);
  }

  bool addCertificateImpl(CertificateType cert_type,
                          const char* certificateName, const char* cert,
                          const uint16_t len) {
    if (cert_type == CLIENT_PSK || cert_type == CLIENT_PSK_IDENTITY) {
      DBG("### The ESP32 does not support SSL using pre-shared keys with AT "
          "firmware.");
      return false;
    }
    // delete any old text in the cert first
    deleteCertificate(certificateName);
    // pull the namespace out of the name
    char certNamespace[12];
    memcpy(certNamespace, certificateName, strlen(certificateName) - 2);
    certNamespace[strlen(certificateName) - 2] = '\0';
    // AT+SYSMFG=<operation>,<"namespace">,<"key">,<type>,<value>
    // operation = 2 for write
    // type = 8 for binary (ie, the certificates must be stored in binary,
    // though you can enter them as strings)
    // Write a new value for client_cert.0 key into client_cert namespace
    // (That is, update the 0th client certificate)
    // AT+SYSMFG=2,"client_cert","client_cert.0",8,1164
    // Wait until AT command port returns ``>``, and then write 1164 bytes
    sendAT(GF("+SYSMFG=2,\""), certNamespace, GF("\",\""), certificateName,
           GF("\",8,"), len);
    if (waitResponse(GF(">")) != 1) { return false; }
    stream.write(reinterpret_cast<const uint8_t*>(cert), len);
    stream.flush();
    if (waitResponse(10000L) != 1) { return false; }
    return true;
  }

  bool deleteCertificateImpl(const char* certificateName) {
    // pull the namespace out of the name
    char certNamespace[12];
    memcpy(certNamespace, certificateName, strlen(certificateName) - 2);
    certNamespace[strlen(certificateName) - 2] = '\0';
    // AT+SYSMFG=<operation>,<"namespace">[,<"key">]
    // operation = 0 for erase
    // AT+SYSMFG=0,"client_cert","client_cert.0"
    sendAT(GF("+SYSMFG=0,\""), certNamespace, GF("\",\""), certificateName,
           '"');
    return waitResponse() == 1;
  }

  bool convertCertificateImpl(CertificateType cert_type, const char*) {
    if (cert_type == CLIENT_PSK || cert_type == CLIENT_PSK_IDENTITY) {
      DBG("### The ESP32 does not support SSL using pre-shared keys with AT "
          "firmware.");
      return false;
    }
    return true;  // no conversion needed on the ESP32
  }

  bool convertClientCertificatesImpl(const char*, const char*) {
    return true;  // no conversion needed on the ESP32
  }

  bool convertPSKandIDImpl(const char*, const char*) {
    DBG("### The ESP32 does not support SSL using pre-shared keys with AT "
        "firmware.");
    return false;
  }

  /*
   * WiFi functions
   */
  // Follows functions inherited from Espressif

  /*
   * GPRS functions
   */
  // No functions of this type supported

  /*
   * SIM card functions
   */
  // No functions of this type supported

  /*
   * Audio functions
   */
  // No functions of this type supported

  /*
   * Text messaging (SMS) functions
   */
  // No functions of this type supported

  /*
   * GSM Location functions
   */
  // No functions of this type supported

  /*
   * GPS/GNSS/GLONASS location functions
   */
  // No functions of this type supported

  /*
   * Time functions
   */
 public:
  void setTimeZone(int8_t timezone, bool enable_sync = true) {
    // configure the NTP settings for the modem
    sendAT(GF("+CIPSNTPCFG="), enable_sync ? 1 : 0, ',', timezone);
    waitResponse();
  }

  void setTimeSyncInterval(uint16_t seconds) {
    // configure the NTP settings for the modem
    sendAT(GF("+CIPSNTPINTV="), seconds);
    waitResponse();
  }

  bool waitForTimeSync(int timeout_s = 120) {
    // if we're not connected, we'll never get the time
    if (!isNetworkConnected()) { return false; }
    // if SNTP sync isn't enabled, we won't have the time
    // NOTE: We don't actually enable the time here, because doing so would
    // change any user settings for the timezone and time servers.
    sendAT(GF("+CIPSNTPCFG?"));
    int8_t is_enabled = streamGetIntBefore(',');
    waitResponse();  // returns OK
    if (!is_enabled) { return false; }
    // if we're sure we should be able to get the time, wait for it
    uint32_t start_millis = millis();
    while (millis() - start_millis < static_cast<uint32_t>(timeout_s) * 1000) {
      uint32_t modem_time = getNetworkEpoch();
      // If we get a time between January 1, 2020 and January 1, 2035, we're
      // (hopefully) good
      if (modem_time > 1577836800 && modem_time < 2051222400) { return true; }
      delay(500);
    }
    return false;
  }

 protected:
  // NOTE: Only returning full date time in Espressif's format
  String getGSMDateTimeImpl(TinyGSMDateTimeFormat) {
    sendAT(GF("+CIPSNTPTIME?"));
    if (waitResponse(2000L, GF("+CIPSNTPTIME:")) != 1) { return ""; }

    String res = stream.readStringUntil('\r');
    waitResponse();  // Ends with OK
    return res;
  }

  bool getNetworkTimeImpl(int* year, int* month, int* day, int* hour,
                          int* minute, int* second, float* timezone) {
    // get the time as a string
    sendAT(GF("+CIPSNTPTIME?"));
    if (waitResponse(2000L, GF("+CIPSNTPTIME:")) != 1) { return false; }

    int iyear     = 0;
    int imonth    = 0;
    int iday      = 0;
    int ihour     = 0;
    int imin      = 0;
    int isec      = 0;
    int itimezone = 0;

    // Date & Time
    streamSkipUntil(' ');  // skip the day of the week
    String mon_abbrev = stream.readStringUntil(' ');
    imonth            = getMonthFromAbbrev(mon_abbrev);
    iday              = streamGetIntBefore(' ');
    ihour             = streamGetIntBefore(':');
    imin              = streamGetIntBefore(':');
    isec              = streamGetIntBefore(' ');
    iyear             = streamGetIntLength(4);
    // Final OK
    waitResponse();

    // get the timezone
    sendAT(GF("+CIPSNTPCFG?"));
    if (waitResponse(2000L, GF("+CIPSNTPCFG:")) != 1) { return false; }

    streamSkipUntil(',');  // skip if sync is enabled
    itimezone = stream.parseFloat();
    // Final OK
    waitResponse();

    // Set pointers
    if (iyear < 2000) iyear += 2000;
    if (year != nullptr) *year = iyear;
    if (month != nullptr) *month = imonth;
    if (day != nullptr) *day = iday;
    if (hour != nullptr) *hour = ihour;
    if (minute != nullptr) *minute = imin;
    if (second != nullptr) *second = isec;
    if (timezone != nullptr) *timezone = itimezone;

    return true;
  }

  uint8_t getMonthFromAbbrev(String month_abbrev) {
    if (month_abbrev == "Jan") { return 1; }
    if (month_abbrev == "Feb") { return 2; }
    if (month_abbrev == "Mar") { return 3; }
    if (month_abbrev == "Apr") { return 4; }
    if (month_abbrev == "May") { return 5; }
    if (month_abbrev == "Jun") { return 6; }
    if (month_abbrev == "Jul") { return 7; }
    if (month_abbrev == "Aug") { return 8; }
    if (month_abbrev == "Sep") { return 9; }
    if (month_abbrev == "Oct") { return 10; }
    if (month_abbrev == "Nov") { return 11; }
    if (month_abbrev == "Dec") { return 12; }
    return 0;
  }

  bool getNetworkUTCTimeImpl(int* year, int* month, int* day, int* hour,
                             int* minute, int* second,
                             float* timezone) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  uint32_t getNetworkEpochImpl(TinyGSM_EpochStart epoch = UNIX) {
    // Returns unix timestamp.  Will match SNTP after SNTP syncs.
    sendAT(GF("+SYSTIMESTAMP?"));
    if (waitResponse(2000L, GF("+SYSTIMESTAMP:")) != 1) { return 0; }
    uint32_t start = millis();
    while (stream.available() < 9 && millis() - start < 10000L) {}
    uint32_t modem_time = 0;
    char     buf[12];
    size_t   bytesRead = stream.readBytesUntil('\n', buf,
                                               static_cast<size_t>(12));
    // if we read 12 or more bytes, it's an overflow
    if (bytesRead && bytesRead < 12) {
      buf[bytesRead] = '\0';
      modem_time     = atoi(buf);
    }
    waitResponse();

    if (modem_time != 0) {
      switch (epoch) {
        case UNIX: modem_time += 0; break;
        case Y2K: modem_time += 946684800; break;
        case GPS: modem_time += 315878400; break;
      }
    }

    return modem_time;
  }

  /*
   * NTP server functions
   */
 protected:
  // NOTE: I don't think this forces an immediate sync
  byte NTPServerSyncImpl(String server = "pool.ntp.org", int TimeZone = 0) {
    // configure the NTP settings for the modem
    sendAT(GF("+CIPSNTPCFG="), 1, ',', TimeZone, GF(",\""), server, '"');
    waitResponse();
    return 0;
  }

  /*
   * BLE functions
   */
  // No functions of this type implemented

  /*
   * Battery functions
   */
  // No functions of this type supported

  /*
   * Temperature functions
   */
  // No functions of this type implemented

  /*
   * Client related functions
   */
 protected:
  bool modemConnect(const char* host, uint16_t port, uint8_t* mux,
                    bool ssl = false, int timeout_s = 75) {
    uint32_t timeout_ms    = ((uint32_t)timeout_s) * 1000;
    uint8_t  requested_mux = *mux;
    if (ssl) {
      if (!(requested_mux < TINY_GSM_MUX_COUNT)) {
        // If we didn't get a valid mux - the user wants us to assign the mux
        // for them. If we're using SSL, in order to set the proper auth type
        // and certs before opening the socket, we need to open the socket with
        // a known mux rather than using CIPSTARTEX. Thus, we'l find the next
        // available unattached mux this way:
        bool got_next_mux = false;
        int  next_mux     = 0;
        for (; next_mux < TINY_GSM_MUX_COUNT; next_mux++) {
          if (sockets[next_mux] == nullptr) {
            got_next_mux = true;
            break;
          }
        }
        if (got_next_mux) {
          requested_mux = next_mux;
        } else {
          DBG("### WARNING: No empty mux sockets found, please select the mux "
              "you want in the client constructor.");
          return false;
        }
      }

      if (sslAuthModes[requested_mux] == PRE_SHARED_KEYS) {
        DBG("### The ESP32 does not support SSL using pre-shared keys with "
            "AT firmware.");
        return false;
      }

      // SSL certificate checking will not work without a valid timestamp!
      if (sockets[requested_mux] != nullptr &&
          (sslAuthModes[requested_mux] == CLIENT_VALIDATION ||
           sslAuthModes[requested_mux] == CA_VALIDATION ||
           sslAuthModes[requested_mux] == MUTUAL_AUTHENTICATION) &&
          !waitForTimeSync(timeout_s)) {
        DBG("### WARNING: The module timestamp must be valid for SSL auth. "
            "Please use setTimeZone(...) or NTPServerSync(...) to enable "
            "time syncing before attempting an SSL connection!");
        return false;
      }

      // configure SSL authentication type and in-use certificates
      // AT+CIPSSLCCONF=<link ID>,<auth_mode>[,<pki_number>][,<ca_number>]
      // <link ID>: ID of the connection (0 ~ max). For multiple connections,
      // if the value is max, it means all connections. By default, max is 5.
      // <auth_mode>:
      //     0: no authentication. In this case <pki_number> and <ca_number>
      //     are not required.
      //        - SRGD Note: You do not need to load any certificates onto
      //        your device for this. Not all servers will accept it.
      //     1: the client provides the client certificate for the server to
      //     verify.
      //        - SRGD Note: I do not believe this is commonly used. To use
      //        this, you must load a client certificate and a client key onto
      //        your device.
      //     2: the client loads CA certificate to verify the server’s
      //     certificate.
      //        - SRGD Note: This is a common authentication type sed by
      //        browsers, where the browser verifies the server's certificate.
      //        For this to work, you must load either the server's
      //        intermediate or parent certificate onto your device.
      //     3: mutual authentication.
      //        - SRGD Note: This is used by AWS IoT Core and other IoT
      //        services. In this case you must load 3 certs to your device:
      //        The servers CA cert, the client cert, and the client key.
      // <pki_number>: the index of certificate and private key. If there is
      // only one certificate and private key, the value should be 0.
      //    PKI - A public key infrastructure (PKI) is a set of roles,
      //    policies, hardware, software and procedures needed to create,
      //    manage, distribute, use, store and revoke digital certificates and
      //    manage public-key encryption.
      // <ca_number>: the index of CA (certificate authority certificate =
      // server's certificate). If there is only one CA, the value should be
      // 0.
      // The PKI number and CA number to use are based on what certificates
      // were (or were not) put into the customized certificate partitions.
      // The default firmware comes with espressif certificates in slots 0
      // and 1.
      if (sockets[requested_mux] == nullptr ||
          (sslAuthModes[requested_mux] == NO_VALIDATION)) {
        sendAT(GF("+CIPSSLCCONF="), requested_mux, GF(",0"));
      } else {
        uint8_t _pkiIndex = 0;
        uint8_t _caIndex  = 0;
        char    tempbuf[2];
        // extract the cert number from the name
        if (CAcerts[requested_mux] != nullptr) {
          memcpy(tempbuf,
                 CAcerts[requested_mux] + strlen(CAcerts[requested_mux]) - 1,
                 1);
          tempbuf[1] = '\0';
          _pkiIndex  = atoi(tempbuf);
        }
        // extract the cert number from the name
        if (clientCerts[requested_mux] != nullptr) {
          memcpy(tempbuf,
                 clientCerts[requested_mux] +
                     strlen(clientCerts[requested_mux]) - 1,
                 1);
          tempbuf[1] = '\0';
          _caIndex   = atoi(tempbuf);
        }
        sendAT(GF("+CIPSSLCCONF="), requested_mux, ',',
               static_cast<uint8_t>(sslAuthModes[requested_mux]), ',',
               _pkiIndex, ',', _caIndex);
      }
      waitResponse();

      // set the SSL SNI (server name indication)
      // Multiple connections: (AT+CIPMUX=1)
      // AT+CIPSSLCSNI=<link ID>,<"sni">
      sendAT(GF("+CIPSSLCSNI="), requested_mux, GF(",\""), host, GF("\""));
      waitResponse();
    }

    // Make the connection
    // If we know the mux number we want to use, use CIPSTART, if we want the
    // module to assign a mux number for us, use CIPSTARTEX
    if (requested_mux < TINY_GSM_MUX_COUNT) {
      sendAT(GF("+CIPSTART="), requested_mux, ',',
             ssl ? GF("\"SSL") : GF("\"TCP"), GF("\",\""), host, GF("\","),
             port);
    } else {
      sendAT(GF("+CIPSTARTEX="), ssl ? GF("\"SSL") : GF("\"TCP"), GF("\",\""),
             host, GF("\","), port);
    }

    String data;
    int8_t rsp = waitResponse(timeout_ms, data, GFP(GSM_OK), GFP(GSM_ERROR),
                              GF("ALREADY CONNECT"));
    if (rsp == 1 && data.length() > 8) {
      int8_t coma        = data.indexOf(',');
      int8_t connect_mux = data.substring(0, coma).toInt();
      *mux               = connect_mux;
    }
    return (1 == rsp);
  }

  bool modemGetConnected(uint8_t mux) {
    sendAT(GF("+CIPSTATE"));
    bool verified_connections[TINY_GSM_MUX_COUNT] = {0, 0, 0, 0, 0};
    for (int muxNo = 0; muxNo < TINY_GSM_MUX_COUNT; muxNo++) {
      uint8_t has_status = waitResponse(GF("+CIPSTATE:"), GFP(GSM_OK),
                                        GFP(GSM_ERROR));
      if (has_status == 1) {
        int8_t returned_mux = streamGetIntBefore(',');
        streamSkipUntil(',');   // Skip type
        streamSkipUntil(',');   // Skip remote IP
        streamSkipUntil(',');   // Skip remote port
        streamSkipUntil(',');   // Skip local port
        streamSkipUntil('\n');  // Skip client/server type
        verified_connections[returned_mux] = 1;
      }
      if (has_status == 2) break;  // once we get to the ok, stop
    }
    for (int muxNo = 0; muxNo < TINY_GSM_MUX_COUNT; muxNo++) {
      if (sockets[muxNo]) {
        sockets[muxNo]->sock_connected = verified_connections[muxNo];
      }
    }
    return verified_connections[mux];
  }

  /*
   * Utilities
   */
 public:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+IPD,"))) {
      int8_t  mux       = streamGetIntBefore(',');
      int16_t len       = streamGetIntBefore(':');
      size_t  len_orig  = len;
      size_t  prev_size = sockets[mux]->rx.size();
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        if (len > sockets[mux]->rx.free()) {
          DBG("### Buffer overflow: ", len, "->", sockets[mux]->rx.free());
          // reset the len to read to the amount free
          len = sockets[mux]->rx.free();
        }
        bool chars_remaining = true;
        while (len-- && chars_remaining) {
          chars_remaining = moveCharFromStreamToFifo(mux);
        }
        // TODO(SRGDamia1): deal with buffer overflow
        if (len_orig != sockets[mux]->rx.size() - prev_size) {
          DBG("### Different number of characters received than expected: ",
              sockets[mux]->rx.size() - prev_size, " vs ", len_orig);
        }
      }
      data = "";
      DBG("### Got Data: ", len_orig, "on", mux);
      return true;
    } else if (data.endsWith(GF("CLOSED"))) {
      int8_t muxStart = TinyGsmMax(0,
                                   data.lastIndexOf(AT_NL, data.length() - 8));
      int8_t coma     = data.indexOf(',', muxStart);
      int8_t mux      = data.substring(muxStart, coma).toInt();
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      streamSkipUntil('\n');  // throw away the new line
      data = "";
      DBG("### Closed: ", mux);
      return true;
    } else if (data.endsWith(GF("+TIME_UPDATED"))) {
      streamSkipUntil('\n');  // Refresh time and time zone by network
      data = "";
      DBG("### Network time updated.");
      return true;
    }
    return false;
  }

 protected:
  GsmClientESP32* sockets[TINY_GSM_MUX_COUNT];
};

#endif  // SRC_TINYGSMCLIENTESP32_H_
