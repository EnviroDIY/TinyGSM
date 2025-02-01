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
      _sslAuthMode = -1;
      _caIndex     = 0;
      _pkiIndex    = 0;
      init(&modem, mux);
    }

    bool init(TinyGsmESP32* modem, uint8_t mux = static_cast<uint8_t>(-1)) {
      this->at       = modem;
      sock_connected = false;

      // if it's a valid mux number,
      // and that mux number isn't in use,
      // accept the mux number
      if (mux < TINY_GSM_MUX_COUNT && at->sockets[mux] != nullptr) {
        this->mux              = mux;
        at->sockets[this->mux] = this;
      } else {
        this->mux = static_cast<uint>(-1);
      }

      return true;
    }

   public:
    virtual int connect(const char* host, uint16_t port, int timeout_s) {
      if (mux < TINY_GSM_MUX_COUNT && at->sockets[this->mux] != nullptr) {
        stop();
      }
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

    void stop(uint32_t maxWaitMs) {
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

    // needed for SSL client, thus for both
    int8_t _sslAuthMode;
    bool   _caIndex;
    bool   _pkiIndex;
  };

  /*
   * Inner Secure Client
   */
 public:
  class GsmClientSecureESP32 : public GsmClientESP32 {
   public:
    GsmClientSecureESP32() {}

    explicit GsmClientSecureESP32(TinyGsmESP32& modem, uint8_t mux = -1)
        : GsmClientESP32(modem, mux) {}

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
    void setSSLAuthMode(int8_t mode) {
      _sslAuthMode = mode;
    }
    // <ca_number>: the index of CA (certificate authority certificate =
    // server's certificate). There are only two client-CA certificate slots: 0
    // and 1.
    void setCAIndex(bool index) {
      _caIndex = index;
    }
    // <pki_number>: the index of certificate and private key. There are only
    // two client-PKI slots: 0 and 1. You must put both the certificate and the
    // matching private key into the same slot number.
    //    PKI - A public key infrastructure (PKI) is a set of roles,
    //    policies, hardware, software and procedures needed to create,
    //    manage, distribute, use, store and revoke digital certificates and
    //    manage public-key encryption.
    void setPKIIndex(bool index) {
      _pkiIndex = index;
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
  // Follows functions as inherited from TinyGsmSSL.tpp

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
    if (!isNetworkConnected()) { return false; }
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
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    if (ssl) {
      if (!*mux < TINY_GSM_MUX_COUNT) {
        // If we didn't get a valid mux - the user wants us to assign the mux
        // for them. If we're using SSL, in order to set the proper auth type
        // and certs before opening the socket, we need to open the socket with
        // a known mux rather than using CIPSTARTEX. Thus, we'l find the next
        // available unattached mux this way:
        bool got_next_mux = false;
        int  next_mux     = 0;
        for (; next_mux < TINY_GSM_MUX_COUNT; next_mux++) {
          if (sockets[next_mux] == nullptr) {
            DBG(GF("### Socket mux"), next_mux, GF("will be used"));
            got_next_mux = true;
            break;
          }
        }
        if (got_next_mux) {
          *mux = next_mux;
        } else {
          DBG("### WARNING: No empty mux sockets found, please select the mux "
              "you want in the client constructor.");
          return false;
        }

        // SSL certificate checking will not work without a valid timestamp!
        if (!waitForTimeSync(timeout_s) && sockets[*mux]->_sslAuthMode >= 1 &&
            sockets[*mux]->_sslAuthMode <= 3) {
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
        if (sockets[*mux]->_sslAuthMode == static_cast<uint8_t>(-1) ||
            sockets[*mux]->_sslAuthMode == 0) {
          sendAT(GF("+CIPSSLCCONF="), *mux, GF(",0"));
        } else {
          sendAT(GF("+CIPSSLCCONF="), *mux, ',', sockets[*mux]->_sslAuthMode,
                 ',', sockets[*mux]->_pkiIndex, ',', sockets[*mux]->_caIndex);
        }
        waitResponse();

        // set the SSL SNI (server name indication)
        // Multiple connections: (AT+CIPMUX=1)
        // AT+CIPSSLCSNI=<link ID>,<"sni">
        sendAT(GF("+CIPSSLCSNI="), *mux, GF(",\""), host, GF("\""));
        waitResponse();
      }
    }

    // Make the connection
    // If we know the mux number we want to use, use CIPSTART, if we want the
    // module to assign a mux number for us, use CIPSTARTEX
    if (*mux < TINY_GSM_MUX_COUNT) {
      sendAT(GF("+CIPSTART="), *mux, ',', ssl ? GF("\"SSL") : GF("\"TCP"),
             GF("\",\""), host, GF("\","), port);
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
      int8_t  mux            = streamGetIntBefore(',');
      int16_t len            = streamGetIntBefore(':');
      int16_t len_orig       = len;
      int16_t prev_available = sockets[mux]->available();
      if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
        if (len > sockets[mux]->rx.free()) {
          DBG("### Buffer overflow: ", len, "->", sockets[mux]->rx.free());
          // reset the len to read to the amount free
          len = sockets[mux]->rx.free();
        }
        while (len--) { moveCharFromStreamToFifo(mux); }
        // TODO(SRGDamia1): deal with buffer overflow/missed characters
        if (len_orig != sockets[mux]->available() - prev_available) {
          DBG("### Different number of characters received than expected: ",
              sockets[mux]->available() - prev_available, " vs ", len_orig);
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
