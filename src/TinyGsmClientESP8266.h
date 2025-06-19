/**
 * @file       TinyGsmClientESP8266.h
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCLIENTESP8266_H_
#define SRC_TINYGSMCLIENTESP8266_H_
// #pragma message("TinyGSM:  TinyGsmClientESP8266")

// #define TINY_GSM_DEBUG Serial

#ifdef TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
#undef TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
#endif
#ifdef TINY_GSM_BUFFER_READ_NO_CHECK
#undef TINY_GSM_BUFFER_READ_NO_CHECK
#endif
#ifndef TINY_GSM_NO_MODEM_BUFFER
#define TINY_GSM_NO_MODEM_BUFFER
#endif
#ifdef TINY_GSM_MUX_DYNAMIC
#undef TINY_GSM_MUX_DYNAMIC
#endif
#ifndef TINY_GSM_MUX_STATIC
#define TINY_GSM_MUX_STATIC
#endif

#include "TinyGsmClientEspressif.h"
#include "TinyGsmTCP.tpp"
#include "TinyGsmSSL.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"

// <state>: current Wi-Fi state.
//   0: ESP8266 station has not started any Wi-Fi connection.
//   1: ESP8266 station has connected to an AP, but does not get an IPv4 address
//   yet.
//   2: ESP8266 station has connected to an AP, and got an IPv4 address.
//   3: ESP8266 station is in Wi-Fi connecting or reconnecting state.
//   4: ESP8266 station is in Wi-Fi disconnected state.
enum ESP8266RegStatus {
  REG_UNINITIALIZED = 0,
  REG_UNREGISTERED  = 1,
  REG_OK            = 2,
  REG_CONNECTING    = 3,
  REG_DISCONNECTING = 4,
  REG_UNKNOWN       = 5,
};

class TinyGsmESP8266 : public TinyGsmEspressif<TinyGsmESP8266>,
                       public TinyGsmTCP<TinyGsmESP8266, TINY_GSM_MUX_COUNT>,
                       public TinyGsmSSL<TinyGsmESP8266, TINY_GSM_MUX_COUNT>,
                       public TinyGsmTime<TinyGsmESP8266>,
                       public TinyGsmNTP<TinyGsmESP8266> {
  friend class TinyGsmEspressif<TinyGsmESP8266>;
  friend class TinyGsmModem<TinyGsmESP8266>;
  friend class TinyGsmWifi<TinyGsmESP8266>;
  friend class TinyGsmTCP<TinyGsmESP8266, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmSSL<TinyGsmESP8266, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmTime<TinyGsmESP8266>;
  friend class TinyGsmNTP<TinyGsmESP8266>;

  /*
   * Inner Client
   */
 public:
  class GsmClientESP8266
      : public TinyGsmTCP<TinyGsmESP8266, TINY_GSM_MUX_COUNT>::GsmClient {
    friend class TinyGsmESP8266;

   public:
    GsmClientESP8266() {
      is_secure = false;
    }

    explicit GsmClientESP8266(TinyGsmESP8266& modem, uint8_t mux = 0) {
      init(&modem, mux);
      is_secure = false;
    }

    bool init(TinyGsmESP8266* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_connected = false;
      is_mid_send    = false;

      // The ESP8266 (as supported) generally lets you choose the mux number,
      // but we want to try to find an empty place in the socket array for it.

      // if it's a valid mux number, and that mux number isn't in use (or it's
      // already this), accept the mux number
      if (mux < TINY_GSM_MUX_COUNT &&
          (at->sockets[mux] == nullptr || at->sockets[mux] == this)) {
        this->mux = mux;
        // If the mux number is in use or out of range, find the next available
        // one
      } else if (at->findFirstUnassignedMux() != static_cast<uint8_t>(-1)) {
        this->mux = at->findFirstUnassignedMux();
      } else {
        // If we can't find anything available, overwrite something, useing mod
        // to make sure we're in range
        this->mux = (mux % TINY_GSM_MUX_COUNT);
      }
      at->sockets[this->mux] = this;

      return true;
    }

   public:
    virtual int connect(const char* host, uint16_t port, int timeout_s) {
      is_mid_send = false;
      if (mux < TINY_GSM_MUX_COUNT && at->sockets[mux] != nullptr) { stop(); }
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    virtual void stop(uint32_t maxWaitMs) {
      is_mid_send = false;
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
  class GsmClientSecureESP8266 : public GsmClientESP8266,
                                 public GsmSecureClient {
    friend class TinyGsmESP8266;

   public:
    TINY_GSM_SECURE_CLIENT_CTORS(ESP8266)

    // Because we have the same potetial range of mux numbers for secure and
    // insecure connections, we don't need to re-check for mux number
    // availability.

    void setCACertName(const char* CAcertName) override {
      this->CAcertName = CAcertName;
      // parse the certificate name into a number and namespace
      char*   cert_namespace = new char[14]();
      uint8_t certNumber     = 0;
      at->parseCertificateName(CAcertName, cert_namespace, certNumber);
      ca_number = certNumber;
    }
    virtual void setCACertName(String CAcertName) {
      setCACertName(CAcertName.c_str());
    }

    void setClientCertName(const char* clientCertName) override {
      this->clientCertName = clientCertName;
      // parse the certificate name into a number and namespace
      char*   cert_namespace = new char[14]();
      uint8_t certNumber     = 0;
      at->parseCertificateName(clientCertName, cert_namespace, certNumber);
      pki_number = certNumber;
    }
    virtual void setClientCertName(String clientCertName) {
      setClientCertName(clientCertName.c_str());
    }

    void setPrivateKeyName(const char* clientKeyName) override {
      this->clientKeyName = clientKeyName;
      // parse the certificate name into a number and namespace
      char*   cert_namespace = new char[14]();
      uint8_t certNumber     = 0;
      at->parseCertificateName(clientKeyName, cert_namespace, certNumber);
      pki_number = certNumber;
    }
    virtual void setPrivateKeyName(String clientKeyName) {
      setPrivateKeyName(clientKeyName.c_str());
    }

    void setCACertificateNumber(uint8_t certNumber) {
      ca_number = certNumber;
      // convert the certificate number and type into the proper certificate
      // names for the ESP32
      char* cert_name      = new char[16]();
      char* cert_namespace = new char[14]();
      at->getCertificateName(CertificateType::CA_CERTIFICATE, certNumber,
                             cert_name, cert_namespace);
      CAcertName = cert_name;
    }
    void setClientCertificateNumber(uint8_t certNumber) {
      pki_number           = certNumber;
      char* cert_name      = new char[16]();
      char* cert_namespace = new char[14]();
      at->getCertificateName(CertificateType::CLIENT_CERTIFICATE, certNumber,
                             cert_name, cert_namespace);
      CAcertName = cert_name;
    }
    void setPrivateKeyNumber(uint8_t keyNumber) {
      pki_number           = keyNumber;
      char* cert_name      = new char[16]();
      char* cert_namespace = new char[14]();
      at->getCertificateName(CertificateType::CLIENT_KEY, keyNumber, cert_name,
                             cert_namespace);
      CAcertName = cert_name;
    }

   protected:
    int8_t ca_number;
    int8_t pki_number;
  };

  /*
   * GSM Modem Constructor
   */
 public:
  explicit TinyGsmESP8266(Stream& stream)
      : TinyGsmEspressif<TinyGsmESP8266>(stream) {
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
  ESP8266RegStatus getRegistrationStatus() {
    sendAT(GF("+CWSTATE?"));
    if (waitResponse(3000, GF("+CWSTATE:")) != 1) return REG_UNKNOWN;
    // +CWSTATE:<state>,<"ssid">
    // followed by an OK
    int8_t status = streamGetIntBefore(',');
    streamSkipUntil('\n');  // throw away the ssid
    waitResponse();         // wait for trailing OK
    return (ESP8266RegStatus)status;
  }

 protected:
  bool isNetworkConnectedImpl() {
    ESP8266RegStatus s = getRegistrationStatus();
    return (s == REG_OK);
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // Uses the secure client inherited from TinyGsmSSL.tpp for setting the
  // certificate name and the SSL connection type so those can be called at
  // connection time, but this library does **NOT** currently support uploading,
  // deleting, or converting certificates on the modem.
  // On the ESP8266 the only way to update your certificates is to use a
  // combination of python scripts, AT commands, and flash tools or to fully
  // build your own version of the AT firmware.

#undef TINY_GSM_MODEM_CAN_LOAD_CERTS

 private:
  void parseCertificateName(const char* cert_name, char* parsed_namespace,
                            uint8_t& parsed_number) {
    // pull the namespace out of the name
    memcpy(parsed_namespace, cert_name, strlen(cert_name) - 2);
    parsed_namespace[strlen(cert_name) - 2] = '\0';
    // pull the number out of the name
    char certNumber[2];
    memcpy(certNumber, cert_name + strlen(cert_name) - 1, 1);
    parsed_number = atoi(certNumber);
  }

  void getCertificateName(CertificateType cert_type, uint8_t certNumber,
                          char* cert_name, char* cert_namespace) {
    char cert_number[2];
    itoa(certNumber, cert_number, 10);

    switch (cert_type) {
      case CertificateType::CLIENT_PSK_IDENTITY:
      case CertificateType::CLIENT_PSK: {
        // The ESP32 does not support SSL using pre-shared keys with AT
        // firmware.
        strcpy(cert_namespace, "\0");
        strcpy(cert_name, "\0");
        return;
      }
      case CertificateType::CLIENT_KEY: {
        const char* client_key_namespace = "client_key";
        strcpy(cert_namespace, client_key_namespace);
        strcpy(cert_name, client_key_namespace);
        break;
      }
      case CertificateType::CLIENT_CERTIFICATE: {
        const char* client_cert_namespace = "client_cert";
        strcpy(cert_namespace, client_cert_namespace);
        strcpy(cert_name, client_cert_namespace);
        break;
      }
      case CertificateType::CA_CERTIFICATE:
      default: {
        const char* ca_cert_namespace = "client_ca";
        strcpy(cert_namespace, ca_cert_namespace);
        strcpy(cert_name, ca_cert_namespace);
        break;
      }
    }
    strcpy(cert_name, ".");
    strcpy(cert_name, cert_number);
    return;
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

  bool waitForTimeSync(int timeout_s = 120) {
    // if we're not connected, we'll never get the time
    if (!isNetworkConnected()) { return false; }
    // if we're sure we should be able to get the time, wait for it
    uint32_t start_millis = millis();
    while (millis() - start_millis < static_cast<uint32_t>(timeout_s) * 1000) {
      uint32_t modem_time = getNetworkEpoch();
      // If we get a time between January 1, 2020 and January 1, 2035, we're
      // (hopefully) good
      if (modem_time > 1577836800 && modem_time < 2051222400) { return true; }
      // if SNTP sync isn't enabled, we won't have the time
      // NOTE: We don't actually enable the time here, because doing so would
      // change any user settings for the timezone and time servers.
      sendAT(GF("+CIPSNTPCFG?"));
      int8_t is_enabled = streamGetIntBefore(',');
      waitResponse();  // returns OK
      if (!is_enabled) { return false; }
      delay(250);
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

  uint32_t
  getNetworkEpochImpl(TinyGSM_EpochStart epoch = TinyGSM_EpochStart::UNIX) {
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
        case TinyGSM_EpochStart::UNIX: modem_time += 0; break;
        case TinyGSM_EpochStart::Y2K: modem_time += 946684800; break;
        case TinyGSM_EpochStart::GPS: modem_time += 315878400; break;
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
  // No functions of this type supported

  /*
   * Client related functions
   */
 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t mux,
                        int timeout_s) {
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    bool     ssl        = sockets[mux]->is_secure;

    // Blank holders for the SSL auth mode and certificates
    SSLAuthMode sslAuthMode = SSLAuthMode::NO_VALIDATION;
    uint8_t     ca_number   = 0;
    uint8_t     pki_number  = 0;
    // If we actually have a secure socket populate the above with real values
    if (ssl) {
      GsmClientSecureESP8266* thisClient =
          static_cast<GsmClientSecureESP8266*>(sockets[mux]);
      sslAuthMode = thisClient->sslAuthMode;
      ca_number   = thisClient->ca_number;
      pki_number  = thisClient->pki_number;
    }

    if (ssl) {
      if (sslAuthMode == SSLAuthMode::PRE_SHARED_KEYS) {
        // The ESP8266 does not support SSL using pre-shared keys with AT
        // firmware.
        return false;
      }
      // SSL certificate checking will not work without a valid timestamp!
      if (sockets[mux] != nullptr &&
          (sslAuthMode == SSLAuthMode::CLIENT_VALIDATION ||
           sslAuthMode == SSLAuthMode::CA_VALIDATION ||
           sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION) &&
          !waitForTimeSync(timeout_s)) {
        DBG("### WARNING: The module timestamp must be valid for SSL auth. "
            "Please use setTimeZone(...) or NTPServerSync(...) to enable "
            "time syncing before attempting an SSL connection!");
        return false;
      }

      // configure SSL authentication type and in-use certificates
      // AT+CIPSSLCCONF=<link ID>,<auth_mode>[,<pki_number>][,<ca_number>]
      // <link ID>: ID of the connection (0 ~ max). For multiple connections, if
      // the value is max, it means all connections. By default, max is 5.
      // <auth_mode>:
      //     0: no authentication. In this case <pki_number> and <ca_number> are
      //     not required.
      //     1: the client provides the client certificate for the server to
      //     verify.
      //     2: the client loads CA certificate to verify the server’s
      //     certificate.
      //     3: mutual authentication.
      // <pki_number>: the index of certificate and private key. If there is
      // only one certificate and private key, the value should be 0.
      //    PKI - A public key infrastructure (PKI) is a set of roles, policies,
      //    hardware, software and procedures needed to create, manage,
      //    distribute, use, store and revoke digital certificates and manage
      //    public-key encryption.
      // <ca_number>: the index of CA (certificate authority certificate =
      // server's certificate). If there is only one CA, the value should be 0.
      // The PKI number and CA number to use are based on what certificates were
      // (or were not) put into the customized certificate partitions.
      // The default firmware comes with espressif certificates in slots 0
      // and 1.
      if (sockets[mux] == nullptr ||
          (sslAuthMode == SSLAuthMode::NO_VALIDATION)) {
        sendAT(GF("+CIPSSLCCONF="), mux, GF(",0"));
      } else {
        sendAT(GF("+CIPSSLCCONF="), mux, ',', static_cast<uint8_t>(sslAuthMode),
               ',', pki_number, ',', ca_number);
      }
      waitResponse();

      // set the SSL SNI (server name indication)
      // Multiple connections: (AT+CIPMUX=1)
      // AT+CIPSSLCSNI=<link ID>,<"sni">
      sendAT(GF("+CIPSSLCSNI="), mux, GF(",\""), host, GF("\""));
      waitResponse();
    }

    // If you need to use a domain name and the length of the domain name
    // exceeds 64 bytes, use the AT+CIPDOMAIN command to obtain the IP address
    // corresponding to the domain name, and then use the IP address to
    // establish a connection.
    if (strlen(host) > 64) {
      // AT+CIPDOMAIN=<"domain name">[,<ip network>][,<timeout>]
      sendAT(GF("+CIPDOMAIN=\""), host, GF("\""));
      // +CIPDOMAIN:<"IP address"> then OK
      if (waitResponse(GF("+CIPDOMAIN:\"")) != 1) { return false; }
      String ip = stream.readStringUntil('"');
      streamSkipUntil('\n');  // skip the rest of the line
      waitResponse();         // ends with OK
      if (ip.length() > 0) {
        host = ip.c_str();
      } else {
        return false;
      }
    }

    // Make the connection
    sendAT(GF("+CIPSTART="), mux, ',', ssl ? GF("\"SSL") : GF("\"TCP"),
           GF("\",\""), host, GF("\","), port
#if defined(TINY_GSM_TCP_KEEP_ALIVE)
           ,
           ',', TINY_GSM_TCP_KEEP_ALIVE
#endif
    );

    String data;
    int8_t rsp = waitResponse(timeout_ms, data, GFP(GSM_OK), GFP(GSM_ERROR),
                              GF("ALREADY CONNECT"));
    if (rsp == 1 && data.length() > 8) {
      int8_t coma        = data.indexOf(',');
      int8_t connect_mux = data.substring(0, coma).toInt();
      if (mux != connect_mux) {
        DBG("WARNING:  Unexpected mux number returned:", connect_mux, "not",
            mux);
      }
    }
    return (1 == rsp);
  }

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    sendAT(GF("+CIPSEND="), mux, ',', len);
    return waitResponse(GF(">")) == 1;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t) {
    uint16_t received = 0;
    if (waitResponse(10000L, GF("Recv ")) == 1) {
      received = streamGetIntBefore(' ');  // check received length
    }
    if (waitResponse(30000L, GF(AT_NL "SEND OK" AT_NL),
                     GF(AT_NL "SEND FAIL" AT_NL), GFP(GSM_ERROR)) != 1) {
      return 0;
    }
    if (received != len) { DBG("### Sent:", received, "of", len); }
    return len;
  }

  bool modemGetConnectedImpl(uint8_t mux) {
    sendAT(GF("+CIPSTATE?"));
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
      } else {
        break;
      };  // once we get to the ok or error, stop
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
    if (data.endsWith(GF(AT_NL "+IPD,"))) {
      int8_t  mux          = streamGetIntBefore(',');
      int16_t len_reported = streamGetIntBefore(':');
      int16_t len          = len_reported;
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        if (len > sockets[mux]->rx.free()) {
          DBG("### Buffer overflow: ", len, "->", sockets[mux]->rx.free());
          // reset the len to read to the amount free
          len = sockets[mux]->rx.free();
        }
        moveCharsFromStreamToFifo(mux, len);
        // TODO(SRGDamia1): deal with buffer overflow
      }
      data = "";
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
    } else if (data.endsWith(GF("busy p..."))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Busy, please wait");
      return true;
    } else if (data.endsWith(GF(AT_NL "ready" AT_NL))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Module ready!");
      return true;
    } else if (data.endsWith(GF("WIFI GOT IP"))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Wifi got IP");
      return true;
    } else if (data.endsWith(GF("WIFI CONNECTED"))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Wifi connected");
      return true;
    } else if (data.endsWith(GF("WIFI DISCONNECT"))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Wifi disconnected");
      return true;
    }
    return false;
  }

 protected:
  GsmClientESP8266* sockets[TINY_GSM_MUX_COUNT];
};

#endif  // SRC_TINYGSMCLIENTESP8266_H_
