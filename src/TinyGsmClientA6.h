/**
 * @file       TinyGsmClientA6.h
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCLIENTA6_H_
#define SRC_TINYGSMCLIENTA6_H_
#pragma message("TinyGSM:  TinyGsmClientA6")

#if defined(TINY_GSM_MAX_RESPONSE_CHECKS)
#if TINY_GSM_MAX_RESPONSE_CHECKS < 5
#undef TINY_GSM_MAX_RESPONSE_CHECKS
#endif
#endif
#if !defined(TINY_GSM_MAX_RESPONSE_CHECKS)
#define TINY_GSM_MAX_RESPONSE_CHECKS 5
#endif

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmBattery.tpp"

#ifdef AT_NL
#undef AT_NL
#endif
#define AT_NL "\r\n"

/// Registration status
enum A6RegStatus {
  REG_NO_RESULT    = -1,  ///< No registration result
  REG_UNREGISTERED = 0,   ///< Not registered on the network
  REG_SEARCHING    = 2,   ///< Searching for network
  REG_DENIED       = 3,   ///< Registration denied
  REG_OK_HOME      = 1,   ///< Registered on the home network
  REG_OK_ROAMING   = 5,   ///< Registered on a roaming network
  REG_UNKNOWN      = 4,   ///< Unknown registration status
};

/// Basic modem configurations for the A6 modem family
struct TinyGsmA6ModemConfig : public TinyGsmModemConfigPreset<A6RegStatus> {
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "Ai-Thinker";
#if defined(TINY_GSM_MODEM_A7)
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "A7";
#else
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "A6";
#endif
};

constexpr char TinyGsmA6ModemConfig::MODEM_MANUFACTURER[];
constexpr char TinyGsmA6ModemConfig::MODEM_MODEL[];

/**
 * @brief TCP behavior and limits for the A6 modem family.
 */
struct TinyGsmA6TcpConfig
    : public TinyGsmTcpConfigPreset<
          /*bufferMode*/ TinyGsmTcpBufferMode::NoModemBuffer,
          /*muxMode*/ TinyGsmTcpMuxMode::Dynamic,
          /*muxCount*/ 8,
          /*sendMaxSize*/ 1024,
          /*connectTimeoutS*/ 75,  // default
          /*stopTimeoutS*/ 1> {};

/**
 * @brief TinyGsmA6 is a class for controlling the Ai-Thinker A6 and A7 GSM/GPRS
 * module.
 */
class TinyGsmA6 : public TinyGsmModem<TinyGsmA6, TinyGsmA6ModemConfig>,
                  public TinyGsmGPRS<TinyGsmA6>,
                  public TinyGsmTCP<TinyGsmA6, TinyGsmA6TcpConfig>,
                  public TinyGsmCalling<TinyGsmA6>,
                  public TinyGsmSMS<TinyGsmA6>,
                  public TinyGsmTime<TinyGsmA6>,
                  public TinyGsmBattery<TinyGsmA6> {
  friend class TinyGsmModem<TinyGsmA6, TinyGsmA6ModemConfig>;
  friend class TinyGsmGPRS<TinyGsmA6>;
  friend class TinyGsmTCP<TinyGsmA6, TinyGsmA6TcpConfig>;
  friend class TinyGsmCalling<TinyGsmA6>;
  friend class TinyGsmSMS<TinyGsmA6>;
  friend class TinyGsmTime<TinyGsmA6>;
  friend class TinyGsmBattery<TinyGsmA6>;

  using ModemConfig = TinyGsmA6ModemConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  class GsmClientA6
      : public TinyGsmTCP<TinyGsmA6, TinyGsmA6TcpConfig>::GsmClient {
    friend class TinyGsmA6;

   public:
    using TinyGsmTCP<TinyGsmA6, TinyGsmA6TcpConfig>::GsmClient::connect;
    using TinyGsmTCP<TinyGsmA6, TinyGsmA6TcpConfig>::GsmClient::stop;

    /**
     * @brief Create a new TCP client.  This must be initialized with a modem
     * before it can be used.
     */
    GsmClientA6() {
      is_secure = false;
    }
    /**
     * @brief Create a new TCP client and bind it to a modem.
     * @param modem Modem instance used by this client.
     *
     * @note The A6 and A7 do not allow you to specify the multiplexing channel.
     * The modem will automatically assign a channel when the client connects to
     * a server.  Use the getMux() function to get the assigned multiplexing
     * channel number after a successful connection.
     */
    explicit GsmClientA6(TinyGsmA6& modem, uint8_t /*mux*/ = 0) {
      init(&modem, -1);
      is_secure = false;
    }

    /**
     * @brief Initialize the TCP client with a modem.
     * @return true if initialization was successful, false otherwise.
     * @copydetails GsmClientA6::GsmClientA6(TinyGsmA6&, uint8_t)
     */
    bool init(TinyGsmA6* modem, uint8_t /*mux*/ = 0) {
      this->at       = modem;
      this->mux      = -1;
      sock_connected = false;
      is_mid_send    = false;

      return true;
    }

   public:
    int connect(const char* host, uint16_t port, int timeout_s) override {
      stop(TinyGsmA6TcpConfig::kStopTimeoutS * 1000L);
      TINY_GSM_YIELD();
      rx.clear();
      uint8_t newMux = -1;
      sock_connected = at->modemConnect(host, port, &newMux, timeout_s);
      if (sock_connected) {
        mux              = newMux;
        at->sockets[mux] = this;
      }
      return sock_connected;
    }

    void stop(uint32_t maxWaitMs) override {
      is_mid_send = false;
      TINY_GSM_YIELD();
      at->sendAT(GF("+CIPCLOSE="), mux);
      sock_connected = false;
      at->waitResponse(maxWaitMs);
      rx.clear();
    }

    /*
     * Extended API
     */

    String remoteIP() override TINY_GSM_ATTR_NOT_IMPLEMENTED {
      return "0.0.0.0";
    }
  };

  /*
   * Inner Secure Client
   */

  // Doesn't support SSL

  /*
   * GSM Modem Constructor
   */
 public:
  /**
   * @brief Construct a modem wrapper around a stream transport.
   * @param stream Stream used to communicate with the modem.
   */
  explicit TinyGsmA6(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientA6"));

    if (!testAT()) { return false; }

    // sendAT(GF("&FZ"));  // Factory + Reset
    // waitResponse();

    sendAT(GF("E0"));  // Echo Off
    if (waitResponse() != 1) { return false; }

#ifdef TINY_GSM_DEBUG
    sendAT(GF("+CMEE=2"));  // turn on verbose error codes
#else
    sendAT(GF("+CMEE=0"));  // turn off error codes
#endif
    waitResponse();
    sendAT(
        GF("+CMER=3,0,0,2"));  // Set unsolicited result code output destination
    waitResponse();

    DBG(GF("### Modem:"), getModemName());

    SimStatus ret = getSimStatus();
    // if the sim isn't ready and a pin has been provided, try to unlock the sim
    if (ret != SIM_READY && pin != nullptr && strlen(pin) > 0) {
      simUnlock(pin);
      return (getSimStatus() == SIM_READY);
    } else {
      // if the sim is ready, or it's locked but no pin has been provided,
      // return true
      return (ret == SIM_READY || ret == SIM_LOCKED);
    }
  }

  // Gets the modem serial number
  String getModemSerialNumberImpl() {
    sendAT(GF("GSN"));  // Not CGSN
    String res;
    if (waitResponse(1000L, res) != 1) { return ""; }
    cleanResponseString(res);
    return res;
  }

  bool factoryDefaultImpl() {
    sendAT(GF("&FZE0&W"));  // Factory + Reset + Echo Off + Write
    waitResponse();
    sendAT(GF("&W"));  // Write configuration
    return waitResponse() == 1;
  }

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin) {
    if (!testAT()) { return false; }
    sendAT(GF("+RST=1"));
    delay(3000);
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+CPOF"));
    // +CPOF: MS OFF OK
    return waitResponse() == 1;
  }

  bool sleepEnableImpl(bool enable) TINY_GSM_ATTR_NOT_AVAILABLE;

  bool setPhoneFunctionalityImpl(uint8_t fun,
                                 bool    reset) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  /*
   * Generic network functions
   */
 protected:
  A6RegStatus getRegistrationStatusImpl() {
    return (A6RegStatus)getRegistrationStatusXREG("CREG");
  }

  bool isNetworkConnectedImpl() {
    A6RegStatus s = this->getRegistrationStatus();
    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
  }

  String getLocalIPImpl() {
    sendAT(GF("+CIFSR"));
    String res;
    if (waitResponse(10000L, res) != 1) { return ""; }
    cleanResponseString(res);
    return res;
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
 protected:
  // No functions of this type supported

  /*
   * WiFi functions
   */
  // No functions of this type supported

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* user, const char* pwd) {
    gprsDisconnect();

    sendAT(GF("+CGATT=1"));
    if (waitResponse(60000L) != 1) { return false; }

    // TODO(?): wait AT+CGATT?

    sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, '"');
    waitResponse();

    if (!user) user = "";
    if (!pwd) pwd = "";
    sendAT(GF("+CSTT=\""), apn, GF("\",\""), user, GF("\",\""), pwd, '"');
    if (waitResponse(60000L) != 1) { return false; }

    sendAT(GF("+CGACT=1,1"));
    waitResponse(60000L);

    sendAT(GF("+CIPMUX=1"));
    if (waitResponse() != 1) { return false; }

    return true;
  }

  bool gprsDisconnectImpl() {
    // Shut the TCP/IP connection
    sendAT(GF("+CIPSHUT"));
    if (waitResponse(60000L) != 1) { return false; }

    for (int i = 0; i < 3; i++) {
      sendAT(GF("+CGATT=0"));
      if (waitResponse(5000L) == 1) { return true; }
    }

    return false;
  }

  String getOperatorImpl() {
    sendAT(GF("+COPS=3,0"));  // Set format
    waitResponse();

    sendAT(GF("+COPS?"));
    if (waitResponse(GF(AT_NL "+COPS:")) != 1) { return ""; }
    streamSkipUntil('"');  // Skip mode and format
    String res = stream.readStringUntil('"');
    waitResponse();
    return res;
  }

  /*
   * SIM card functions
   */
 protected:
  String getSimCCIDImpl() {
    sendAT(GF("+CCID"));
    if (waitResponse(GF(AT_NL "+SCID: SIM Card ID:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  /*
   * Phone Call functions
   */
 protected:
  // Returns true on pick-up, false on error/busy
  bool callNumberImpl(const String& number) {
    if (number == GF("last")) {
      sendAT(GF("DLST"));
    } else {
      sendAT(GF("D\""), number, GF("\";"));
    }

    if (waitResponse(5000L) != 1) { return false; }

    if (waitResponse(60000L, GF(AT_NL "+CIEV: \"CALL\",1"),
                     GF(AT_NL "+CIEV: \"CALL\",0"),
                     GFP(ModemConfig::GSM_ERROR)) != 1) {
      return false;
    }

    int8_t rsp = waitResponse(60000L, GF(AT_NL "+CIEV: \"SOUNDER\",0"),
                              GF(AT_NL "+CIEV: \"CALL\",0"));

    int8_t rsp2 = waitResponse(300L, GF(AT_NL "BUSY" AT_NL),
                               GF(AT_NL "NO ANSWER" AT_NL));

    return rsp == 1 && rsp2 == 0;
  }

  // 0-9,*,#,A,B,C,D
  bool dtmfSendImpl(char cmd, int duration_ms) {
    duration_ms = constrain(duration_ms, 100, 1000);

    // The duration parameter is not working, so we simulate it using delay..
    // TODO(?): Maybe there's another way...

    // sendAT(GF("+VTD="), duration_ms / 100);
    // waitResponse();

    sendAT(GF("+VTS="), cmd);
    if (waitResponse(10000L) == 1) {
      delay(duration_ms);
      return true;
    }
    return false;
  }

  /*
   * Audio functions
   */
 public:
  /**
   * @brief Set the audio output to headphones.
   * @return True if the command was successful, false otherwise.
   */
  bool audioSetHeadphones() {
    sendAT(GF("+SNFS=0"));
    return waitResponse() == 1;
  }
  /**
   * @brief Set the audio output to speaker.
   * @return True if the command was successful, false otherwise.
   */
  bool audioSetSpeaker() {
    sendAT(GF("+SNFS=1"));
    return waitResponse() == 1;
  }
  /**
   * @brief Mute or unmute the microphone.
   * @param mute True to mute, false to unmute.
   * @return True if the command was successful, false otherwise.
   */
  bool audioMuteMic(bool mute) {
    sendAT(GF("+CMUT="), mute);
    return waitResponse() == 1;
  }

  /*
   * Text messaging (SMS) functions
   */
 protected:
  String sendUSSDImpl(const String& code) {
    sendAT(GF("+CMGF=1"));
    waitResponse();
    sendAT(GF("+CSCS=\"HEX\""));
    waitResponse();
    sendAT(GF("+CUSD=1,\""), code, GF("\",15"));
    if (waitResponse(10000L) != 1) { return ""; }
    if (waitResponse(GF(AT_NL "+CUSD:")) != 1) { return ""; }
    streamSkipUntil('"');
    String hex = stream.readStringUntil('"');
    streamSkipUntil(',');
    int8_t dcs = streamGetIntBefore('\n');

    if (dcs == 15) {
      return TinyGsmDecodeHex7bit(hex);
    } else if (dcs == 72) {
      return TinyGsmDecodeHex16bit(hex);
    } else {
      return hex;
    }
  }

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
  // Follows all clock functions as inherited from TinyGsmTime.tpp
  // Note - the clock probably has to be set manaually first

  /*
   * NTP server functions
   */
  // No functions of this type supported

  /*
   * BLE functions
   */
  // No functions of this type supported

  /*
   * Battery functions
   */
 protected:
  int16_t getBattVoltageImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  // Needs a '?' after CBC, unlike most
  int8_t getBattPercentImpl() {
    sendAT(GF("+CBC?"));
    if (waitResponse(GF(AT_NL "+CBC:")) != 1) { return false; }
    streamSkipUntil(',');  // Skip battery charge status
    // Read battery charge level
    int8_t res = streamGetIntBefore('\n');
    // Wait for final OK
    waitResponse();
    return res;
  }

  // Needs a '?' after CBC, unlike most
  bool getBattStatsImpl(int8_t& chargeState, int8_t& percent,
                        int16_t& milliVolts) {
    sendAT(GF("+CBC?"));
    if (waitResponse(GF(AT_NL "+CBC:")) != 1) { return false; }
    chargeState = streamGetIntBefore(',');
    percent     = streamGetIntBefore('\n');
    milliVolts  = 0;
    // Wait for final OK
    waitResponse();
    return true;
  }

  /*
   * Temperature functions
   */
  // No functions of this type supported

  /*
   * Client related functions
   */
 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t* mux,
                        int timeout_s) {
    uint32_t startMillis = millis();
    uint32_t timeout_ms  = ((uint32_t)timeout_s) * 1000;

    sendAT(GF("+CIPSTART="), GF("\"TCP"), GF("\",\""), host, GF("\","), port);
    if (waitResponse(timeout_ms, GF(AT_NL "+CIPNUM:")) != 1) { return false; }
    int8_t newMux = streamGetIntBefore('\n');

    int8_t rsp = waitResponse((timeout_ms - (millis() - startMillis)),
                              GF("CONNECT OK" AT_NL), GF("CONNECT FAIL" AT_NL),
                              GF("ALREADY CONNECT" AT_NL));
    if (waitResponse() != 1) { return false; }
    *mux = newMux;

    return (1 == rsp);
  }

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    sendAT(GF("+CIPSEND="), mux, ',', (uint16_t)len);
    return waitResponse(2000L, GF(AT_NL ">")) == 1;
  }
  // Between the begin and end, modem send calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t) {
    if (waitResponse(10000L, GFP(ModemConfig::GSM_OK), GF(AT_NL "FAIL")) != 1) {
      return 0;
    }
    return len;
  }

  bool modemGetConnectedImpl(uint8_t) {
    sendAT(GF("+CIPSTATUS"));  // TODO(?) mux?
    int8_t res = waitResponse(GF(",\"CONNECTED\""), GF(",\"CLOSED\""),
                              GF(",\"CLOSING\""), GF(",\"INITIAL\""));
    waitResponse();
    return 1 == res;
  }

  /*
   * Utilities
   */
 protected:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+CIPRCV:"))) {
      int8_t  mux          = streamGetIntBefore(',');
      int16_t len_reported = streamGetIntBefore(',');
      int16_t len          = len_reported;
      if (mux >= 0 && mux < TinyGsmA6TcpConfig::kMuxCount && sockets[mux]) {
        if (len > sockets[mux]->rx.free()) {
          DBG("### Buffer overflow: ", len_reported, "->",
              sockets[mux]->rx.free());
          // reset the len to read to the amount free
          len = sockets[mux]->rx.free();
        }
        moveCharsFromStreamToFifo(mux, len);
      }
      data = "";
      DBG("### Got Data: ", len_reported, "on", mux);
      return true;
    } else if (data.endsWith(GF("+TCPCLOSED:"))) {
      int8_t mux = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TinyGsmA6TcpConfig::kMuxCount && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      data = "";
      DBG("### Closed: ", mux);
      return true;
    }
    return false;
  }

 public:
  /// Stream used to communicate with the modem.
  Stream& stream;

 protected:
  GsmClientA6* sockets[TinyGsmA6TcpConfig::kMuxCount];
};

#undef AT_NL

#endif  // SRC_TINYGSMCLIENTA6_H_
