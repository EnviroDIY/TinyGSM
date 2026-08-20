/**
 * @file       TinyGsmClientMC60.h
 * @brief      MC60 modem client and modem-trait definitions.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Oct 2017
 *
 * MC60 support added by Tamas Dajka 2017.10.15 - with fixes by Sara Damiano
 */
/* clang-format off */
/**
 * @defgroup quectel_mc60 Quectel MC60 Modem Family
 * @brief Manufacturer: Quectel. Models: MC60, MC60E.
 *
 * # Supported Public Functions
 *
 * - Basic functions (TinyGsmModem.tpp)
 *     - @ref TinyGsmModem<modemType, modemConfig>::begin "begin()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::init "init()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::sendAT "sendAT()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::setBaud "setBaud()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::forceModemBaud "forceModemBaud()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::testAT "testAT()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::waitResponse "waitResponse()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getConfiguredModem "getConfiguredModem()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemInfo "getModemInfo()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemName "getModemName()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemManufacturer "getModemManufacturer()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemModel "getModemModel()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemRevision "getModemRevision()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemSerialNumber "getModemSerialNumber()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::factoryDefault "factoryDefault()"
 * - Power functions (TinyGsmModem.tpp)
 *     - @ref TinyGsmModem<modemType, modemConfig>::restart "restart()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::powerOff "powerOff()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::radioOff "radioOff()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::sleepEnable "sleepEnable()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::setPhoneFunctionality "setPhoneFunctionality()"
 * - Generic network functions (TinyGsmModem.tpp)
 *     - @ref TinyGsmModem<modemType, modemConfig>::getRegistrationStatus "getRegistrationStatus()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::isNetworkConnected "isNetworkConnected()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::waitForNetwork "waitForNetwork()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getSignalQuality "getSignalQuality()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getLocalIP "getLocalIP()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::localIP "localIP()"
 * - Utilities (TinyGsmModem.tpp)
 *     - @ref TinyGsmModem<modemType, modemConfig>::streamWrite "streamWrite()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::streamClear "streamClear()"
 * - SIM card functions (TinyGsmGPRS.tpp)
 *     - @ref TinyGsmGPRS<modemType>::simUnlock "simUnlock()"
 *     - @ref TinyGsmGPRS<modemType>::getSimCCID "getSimCCID()"
 *     - @ref TinyGsmGPRS<modemType>::getIMEI "getIMEI()"
 *     - @ref TinyGsmGPRS<modemType>::getIMSI "getIMSI()"
 *     - @ref TinyGsmGPRS<modemType>::getSimStatus "getSimStatus()"
 * - GPRS functions (TinyGsmGPRS.tpp)
 *     - @ref TinyGsmGPRS<modemType>::gprsConnect "gprsConnect()"
 *     - @ref TinyGsmGPRS<modemType>::gprsDisconnect "gprsDisconnect()"
 *     - @ref TinyGsmGPRS<modemType>::isGprsConnected "isGprsConnected()"
 *     - @ref TinyGsmGPRS<modemType>::getOperator "getOperator()"
 *     - @ref TinyGsmGPRS<modemType>::getProvider "getProvider()"
 * - TCP functions (TinyGsmTCP.tpp)
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::maintain "maintain()"
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::findFirstUnassignedMux "findFirstUnassignedMux()"
 * - Phone call functions (TinyGsmCalling.tpp)
 *     - @ref TinyGsmCalling<modemType>::callAnswer "callAnswer()"
 *     - @ref TinyGsmCalling<modemType>::callNumber "callNumber()"
 *     - @ref TinyGsmCalling<modemType>::callHangup "callHangup()"
 *     - @ref TinyGsmCalling<modemType>::dtmfSend "dtmfSend()"
 * - Text messaging (SMS) functions (TinyGsmSMS.tpp)
 *     - @ref TinyGsmSMS<modemType>::sendUSSD "sendUSSD()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS "sendSMS()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS_UTF16 "sendSMS_UTF16()"
 * - Time functions (TinyGsmTime.tpp)
 *     - @ref TinyGsmTime<modemType>::getGSMDateTime "getGSMDateTime()"
 *     - @ref TinyGsmTime<modemType>::getNetworkTime "getNetworkTime()"
 * - Battery functions (TinyGsmBattery.tpp)
 *     - @ref TinyGsmBattery<modemType>::getBattVoltage "getBattVoltage()"
 *     - @ref TinyGsmBattery<modemType>::getBattPercent "getBattPercent()"
 *     - @ref TinyGsmBattery<modemType>::getBattChargeState "getBattChargeState()"
 *     - @ref TinyGsmBattery<modemType>::getBattStats "getBattStats()"
 * - Text messaging (SMS) functions
 *     - @ref TinyGsmMC60::deleteAllSMS "deleteAllSMS()"
 *
 * # Connection Information
 *
 * - TCP sockets:
 *   - 6
 *   - SSL is not supported
 * - Socket Buffering:
 *   - The modem has an internal buffer for incoming data.
 *   - This gives you some leeway to pull data from the buffer as needed with
 * less risk of losing data.
 *   - Because this module doesn't supply a way to check how much data is stored
 * in the buffer, you should try to avoid reading it out in small chunks.
 * - Socket Numbering:
 *   - The modem does not allow you to specify the multiplexing channel.
 *   - The modem will automatically assign a channel when the client connects to
 * a server.
 *   - Use the getMux() function to get the assigned multiplexing channel number
 * after a successful connection.
 *
 * @todo In `modemEndSendImpl()`: verify len/ack
 * @todo In `modemReadImpl()`: Does this even work????
 * @todo In `handleURCs()`: QIRD? or QIRDI?
 */
/* clang-format on */

#ifndef SRC_TINYGSMCLIENTMC60_H_
#define SRC_TINYGSMCLIENTMC60_H_
#pragma message("TinyGSM:  TinyGsmClientMC60")

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmBattery.tpp"

/// Registration status
/// @ingroup quectel_mc60
enum class MC60RegStatus {
  REG_NO_RESULT    = -1,  ///< No registration result
  REG_UNREGISTERED = 0,   ///< Not registered on the network
  REG_SEARCHING    = 2,   ///< Searching for network
  REG_DENIED       = 3,   ///< Registration denied
  REG_OK_HOME      = 1,   ///< Registered on the home network
  REG_OK_ROAMING   = 5,   ///< Registered on a roaming network
  REG_UNKNOWN      = 4,   ///< Unknown registration status
};

/// Basic modem configurations for the MC60 modem family
/// @ingroup quectel_mc60
struct TinyGsmMC60ModemConfig : public TinyGsmModemConfigPreset<MC60RegStatus> {
  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "Quectel";
#if defined(TINY_GSM_MODEM_MC60E)
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "MC60E";
#else
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "MC60";
#endif
};

constexpr char TinyGsmMC60ModemConfig::MODEM_MANUFACTURER[]
    __attribute__((weak));
constexpr char TinyGsmMC60ModemConfig::MODEM_MODEL[] __attribute__((weak));

/**
 * @brief TCP behavior and limits for the MC60 modem family.
 *
 * The send data commands, QISEND and QSSLSEND, both accept up to 1460 bytes of
 * input.
 * @ingroup quectel_mc60
 */
struct TinyGsmMC60TcpConfig
    : public TinyGsmTcpConfigPreset<
          /*bufferMode*/ TinyGsmTcpBufferMode::BufferReadNoCheck,
          /*muxMode*/ TinyGsmTcpMuxMode::Static,
          /*muxCount*/ 6,
          /*sendMaxSize*/ 1460,
          /*connectTimeoutS*/ 75,  // default
          /*stopTimeoutS*/ 75> {};

/// Class for the Quectel MC60
/// @ingroup quectel_mc60
class TinyGsmMC60 : public TinyGsmModem<TinyGsmMC60, TinyGsmMC60ModemConfig>,
                    public TinyGsmGPRS<TinyGsmMC60>,
                    public TinyGsmTCP<TinyGsmMC60, TinyGsmMC60TcpConfig>,
                    public TinyGsmCalling<TinyGsmMC60>,
                    public TinyGsmSMS<TinyGsmMC60>,
                    public TinyGsmTime<TinyGsmMC60>,
                    public TinyGsmBattery<TinyGsmMC60> {
  friend class TinyGsmModem<TinyGsmMC60, TinyGsmMC60ModemConfig>;
  friend class TinyGsmGPRS<TinyGsmMC60>;
  friend class TinyGsmTCP<TinyGsmMC60, TinyGsmMC60TcpConfig>;
  friend class GsmClient<TinyGsmMC60, TinyGsmMC60TcpConfig>;
  friend class TinyGsmCalling<TinyGsmMC60>;
  friend class TinyGsmSMS<TinyGsmMC60>;
  friend class TinyGsmTime<TinyGsmMC60>;
  friend class TinyGsmBattery<TinyGsmMC60>;

 public:
  using ModemConfig = TinyGsmMC60ModemConfig;
  using TcpConfig   = TinyGsmMC60TcpConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  /// @ingroup quectel_mc60
  class GsmClientMC60 : public GsmClient<TinyGsmMC60, TinyGsmMC60TcpConfig> {
    friend class TinyGsmMC60;

   public:
    using GsmClient<TinyGsmMC60, TinyGsmMC60TcpConfig>::connect;
    using GsmClient<TinyGsmMC60, TinyGsmMC60TcpConfig>::stop;
    using TcpConfig = TinyGsmMC60TcpConfig;

    /**
     * @brief Create a new TCP client.
     * @warning You must call the init() method before attempting to use a
     * client created with this constructor.
     */
    GsmClientMC60() {
      is_secure = false;
    }
    /**
     * @brief Create a new TCP client and bind it to a modem and optionally a
     * multiplexing channel.
     * @param modem Modem instance used by this client.
     * @param mux Multiplexing channel to use.
     *
     * @note The MC60 allows you choose the multiplexing channel number, but if
     * the input mux channel number is already in use and other mux channels are
     * available, this library will select the next available one.  Use the
     * getMux() function to get the assigned multiplexing channel number after a
     * successful connection.
     */
    explicit GsmClientMC60(TinyGsmMC60& modem, uint8_t mux = 0) {
      init(&modem, mux);
      is_secure = false;
    }

    /**
     * @brief Initialize the TCP client with a modem and optionally a
     * multiplexing channel.
     * @return true if initialization was successful, false otherwise.
     * @copydetails GsmClientMC60::GsmClientMC60(TinyGsmMC60&, uint8_t)
     */
    bool init(TinyGsmMC60* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_available = 0;
      sock_connected = false;
      is_mid_send    = false;

      // The MC60 generally lets you choose the mux number, but we want to try
      // to find an empty place in the socket array for it.

      // if it's a valid mux number, and that mux number isn't in use (or it's
      // already this), accept the mux number
      if (mux < TcpConfig::kMuxCount &&
          (at->sockets[mux] == nullptr || at->sockets[mux] == this)) {
        this->mux = mux;
        // If the mux number is in use or out of range, find the next available
        // one
      } else if (at->findFirstUnassignedMux() != static_cast<uint8_t>(-1)) {
        this->mux = at->findFirstUnassignedMux();
      } else {
        // If we can't find anything available, overwrite something, using mod
        // to make sure we're in range
        this->mux = (mux % TcpConfig::kMuxCount);
      }
      at->sockets[this->mux] = this;

      return true;
    }

   public:
    int connect(const char* host, uint16_t port, int timeout_s) override {
      if (at == nullptr) { return 0; }
      stop(TcpConfig::kStopTimeoutS * 1000L);
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }

    void stop(uint32_t maxWaitMs) override {
      if (at == nullptr) { return; }
      is_mid_send          = false;
      uint32_t startMillis = millis();
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+QICLOSE="), mux);
      sock_connected = false;
      at->waitResponse((maxWaitMs - (millis() - startMillis)), GF("CLOSED"),
                       GF("CLOSE OK"), GF("ERROR"));
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
  // NOT SUPPORTED

  /*
   * GSM Modem Constructor
   */
 public:
  /**
   * @brief Construct a modem wrapper around a stream transport.
   * @param stream Stream used to communicate with the modem.
   */
  explicit TinyGsmMC60(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientMC60"));

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

    DBG(GF("### Modem:"), getModemName());

    // Enable network time synchronization
    sendAT(GF("+QNITZ=1"));
    if (waitResponse(10000L) != 1) { return false; }

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

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin) {
    if (!testAT()) { return false; }
    if (!setPhoneFunctionality(0)) { return false; }
    if (!setPhoneFunctionality(1, true)) { return false; }
    delay(3000);
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+QPOWD=1"));
    return waitResponse(GF("NORMAL POWER DOWN")) == 1;
  }

  // When entering into sleep mode is enabled, DTR is pulled up, and WAKEUP_IN
  // is pulled up, the module can directly enter into sleep mode.If entering
  // into sleep mode is enabled, DTR is pulled down, and WAKEUP_IN is pulled
  // down, there is a need to pull the DTR pin and the WAKEUP_IN pin up first,
  // and then the module can enter into sleep mode.
  bool sleepEnableImpl(bool enable) {
    sendAT(GF("+QSCLK="), enable);
    return waitResponse() == 1;
  }

  bool setPhoneFunctionalityImpl(uint8_t fun, bool reset) {
    sendAT(GF("+CFUN="), fun, reset ? ",1" : "");
    return waitResponse(10000L) == 1;
  }

  /*
   * Generic network functions
   */
 protected:
  MC60RegStatus getRegistrationStatusImpl() {
    return static_cast<MC60RegStatus>(getRegistrationStatusXREG("CREG"));
  }

  bool isNetworkConnectedImpl() {
    MC60RegStatus s = this->getRegistrationStatus();
    return (s == MC60RegStatus::REG_OK_HOME ||
            s == MC60RegStatus::REG_OK_ROAMING);
  }

  String getLocalIPImpl() {
    sendAT(GF("+QILOCIP"));
    streamSkipUntil('\n');
    String res = stream.readStringUntil('\n');
    res.trim();
    return res;
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // No functions of this type supported

  /*
   * Wifi functions
   */
  // No functions of this type supported

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* user, const char* pwd) {
    gprsDisconnect();

    // select foreground context 0 = VIRTUAL_UART_1
    sendAT(GF("+QIFGCNT=0"));
    if (waitResponse() != 1) { return false; }

    // Select GPRS (=1) as the Bearer
    sendAT(GF("+QICSGP=1,\""), apn, GF("\",\""), user, GF("\",\""), pwd, '"');
    if (waitResponse() != 1) { return false; }

    // Define PDP context - is this necessary?
    sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, '"');
    waitResponse();

    // Activate PDP context - is this necessary?
    sendAT(GF("+CGACT=1,1"));
    waitResponse(60000L);

    // Select TCP/IP transfer mode - NOT transparent mode
    sendAT(GF("+QIMODE=0"));
    if (waitResponse() != 1) { return false; }

    // Enable multiple TCP/IP connections
    sendAT(GF("+QIMUX=1"));
    if (waitResponse() != 1) { return false; }

    // Modem is used as a client
    sendAT(GF("+QISRVC=1"));
    if (waitResponse() != 1) { return false; }

    // Start TCPIP Task and Set APN, User Name and Password
    sendAT(GF("+QIREGAPP=\""), apn, GF("\",\""), user, GF("\",\""), pwd, '"');
    if (waitResponse() != 1) { return false; }

    // Activate GPRS/CSD Context
    sendAT(GF("+QIACT"));
    if (waitResponse(60000L) != 1) { return false; }

    // Check that we have a local IP address
    if (localIP() == IPAddress(0, 0, 0, 0)) { return false; }

    // Set Method to Handle Received TCP/IP Data
    // Mode=2 - Output a notification statement:
    // +QIRDI: <id>,<sc>,<sid>,<num>,<len>,< tlen>
    sendAT(GF("+QINDI=2"));
    if (waitResponse() != 1) { return false; }

    return true;
  }

  bool gprsDisconnectImpl() {
    sendAT(GF("+QIDEACT"));  // Deactivate the bearer context
    return waitResponse(60000L, GF("DEACT OK")) == 1;
  }

  String getProviderImpl() {
    sendAT(GF("+QSPN?"));
    if (waitResponse(GF("+QSPN:")) != 1) { return ""; }
    streamSkipUntil('"');                      // Skip mode and format
    String res = stream.readStringUntil('"');  // read the provider
    waitResponse();                            // skip anything else
    return res;
  }

  /*
   * SIM card functions
   */
 protected:
  SimStatus getSimStatusImpl(uint32_t timeout_ms) {
    for (uint32_t start = millis(); millis() - start < timeout_ms;) {
      sendAT(GF("+CPIN?"));
      if (waitResponse(GF("+CPIN:")) != 1) {
        delay(1000);
        continue;
      }
      int8_t status = waitResponse(GF("READY"), GF("SIM PIN"), GF("SIM PUK"),
                                   GF("NOT INSERTED"), GF("PH_SIM PIN"),
                                   GF("PH_SIM PUK"));
      waitResponse();
      switch (status) {
        case 2:
        case 3: return SIM_LOCKED;
        case 5:
        case 6: return SIM_ANTITHEFT_LOCKED;
        case 1: return SIM_READY;
        default: return SIM_ERROR;
      }
    }
    return SIM_ERROR;
  }

  /*
   * Phone call functions
   */
  // Follows all Phone call functions as inherited from TinyGsmCalling.tpp

  /*
   * Audio functions
   */
  // No functions of this type supported

  /*
   * Text messaging (SMS) functions
   */
  // Follows all text messaging (SMS) functions as inherited from TinyGsmSMS.tpp

 public:
  /**
   * @brief Delete all SMS messages from the modem's memory.
   * @return True if the deletion was successful, false otherwise.
   */
  bool deleteAllSMS() {
    sendAT(GF("+QMGDA=6"));
    return waitResponse(60000L) == 1;
  }

  /*
   * GSM location functions
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

  /*
   * NTP server functions
   */
  // No functions of this type supported

  /*
   * BLE functions
   */
  // No functions of this type supported

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
  // Follows all battery functions as inherited from TinyGsmBattery.tpp

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
    // By default, MC60 expects IP address as 'host' parameter.
    // If it is a domain name, "AT+QIDNSIP=1" should be executed.
    // "AT+QIDNSIP=0" is for dotted decimal IP address.
    IPAddress addr;
    sendAT(GF("+QIDNSIP="),
           (TinyGsmIpFromString(host) == IPAddress(0, 0, 0, 0) ? 1 : 0));
    if (waitResponse() != 1) { return false; }

    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    sendAT(GF("+QIOPEN="), mux, GF(",\""), GF("TCP"), GF("\",\""), host,
           GF("\","), port);
    int8_t rsp = waitResponse(timeout_ms, GF("CONNECT OK\r\n"),
                              GF("CONNECT FAIL\r\n"),
                              GF("ALREADY CONNECT\r\n"));
    return (1 == rsp);
  }

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    sendAT(GF("+QISEND="), mux, ',', (uint16_t)len);
    return waitResponse(GF(">")) == 1;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t mux) {
    if (waitResponse(GF("SEND OK")) != 1) { return 0; }

    bool allAcknowledged = false;
    // bool failed = false;
    while (!allAcknowledged) {
      sendAT(GF("+QISACK="), mux);  // If 'mux' is not specified, MC60 returns
                                    // 'ERRROR' (for QIMUX == 1)
      if (waitResponse(5000L, GF("+QISACK:")) != 1) {
        return -1;
      } else {
        streamSkipUntil(','); /** Skip total */
        streamSkipUntil(','); /** Skip acknowledged data size */
        if (streamGetIntBefore('\n') == 0) { allAcknowledged = true; }
      }
    }
    waitResponse(5000L);

    // streamSkipUntil(','); // Skip mux
    // return streamGetIntBefore('\n');
    if (!allAcknowledged) { return 0; }
    return len;  // TODO(?): verify len/ack
  }

  size_t modemReadImpl(size_t size, uint8_t mux) {
    if (!sockets[mux]) return 0;
    // TODO(?):  Does this even work????
    // AT+QIRD=<id>,<sc>,<sid>,<len>
    // id = GPRS context number = 0, set in GPRS connect
    // sc = role in connection = 1, client of connection
    // sid = index of connection = mux
    // len = maximum length of data to retrieve
    sendAT(GF("+QIRD=0,1,"), mux, ',', (uint16_t)size);
    // If it replies only OK for the write command, it means there is no
    // received data in the buffer of the connection.
    int8_t res = waitResponse(GF("+QIRD:"), GFP(ModemConfig::GSM_OK),
                              GFP(ModemConfig::GSM_ERROR));
    if (res == 1) {
      streamSkipUntil(':');  // skip IP address
      streamSkipUntil(',');  // skip port
      streamSkipUntil(',');  // skip connection type (TCP/UDP)
      // read the real length of the retrieved data
      uint16_t len_reported = streamGetIntBefore('\n');
      // It's possible that the real length available is less than expected
      // This is quite likely if the buffer is broken into packets - which may
      // be different sizes.
      // If so, make sure we make sure we re-set the amount of data available.
      if (len_reported < size) { sockets[mux]->sock_available = len_reported; }
      size_t len_read = moveCharsFromStreamToFifo(mux, len_reported);
      sockets[mux]->sock_available -= len_read;
      // ^^ Decrease the characters available after moving from modem's FIFO to
      // our FIFO
      waitResponse();  // ends with an OK
      return len_read;
    } else {
      sockets[mux]->sock_available = 0;
      return 0;
    }
  }

  // Not possible to check the number of characters remaining in buffer
  // This doesn't even need to be implemented
  // size_t modemGetAvailableImpl(uint8_t) {
  //   return 0;
  // }

  bool modemGetConnectedImpl(uint8_t mux) {
    sendAT(GF("+QISTATE=1,"), mux);
    // +QISTATE: 0,"TCP","151.139.237.11",80,5087,4,1,0,0,"uart1"

    if (waitResponse(GF("+QISTATE:")) != 1) { return false; }

    streamSkipUntil(',');                  // Skip mux
    streamSkipUntil(',');                  // Skip socket type
    streamSkipUntil(',');                  // Skip remote ip
    streamSkipUntil(',');                  // Skip remote port
    streamSkipUntil(',');                  // Skip local port
    int8_t res = streamGetIntBefore(',');  // socket state

    waitResponse();

    // 0 Initial, 1 Opening, 2 Connected, 3 Listening, 4 Closing
    return 2 == res;
  }

  /*
   * Utilities
   */
 protected:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+QIRDI:"))) {  // TODO(?):
                                         // QIRD? or QIRDI?
      // +QIRDI: <id>,<sc>,<sid>,<num>,<len>,< tlen>
      streamSkipUntil(',');  // Skip the context
      streamSkipUntil(',');  // Skip the role
      // read the connection id
      int8_t mux = streamGetIntBefore(',');
      // read the number of packets in the buffer
      int8_t num_packets = streamGetIntBefore(',');
      // read the length of the current packet
      streamSkipUntil(
          ',');  // Skip the length of the current package in the buffer
      int16_t len_total =
          streamGetIntBefore('\n');  // Total length of all packages
      if (mux >= 0 && mux < TcpConfig::kMuxCount && sockets[mux] &&
          num_packets >= 0 && len_total >= 0) {
        sockets[mux]->sock_available = len_total;
      }
      data = "";
      // DBG("### Got Data:", len_total, "on", mux);
      return true;
    } else if (data.endsWith(GF("CLOSED\r\n"))) {
      int8_t nl   = data.lastIndexOf(String(GFP(ModemConfig::GSM_NL)),
                                     data.length() - 8);
      int8_t coma = data.indexOf(',', nl + 2);
      int8_t mux  = data.substring(nl + 2, coma).toInt();
      if (mux >= 0 && mux < TcpConfig::kMuxCount && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      data = "";
      DBG("### Closed: ", mux);
      return true;
    } else if (data.endsWith(GF("+QNITZ:"))) {
      streamSkipUntil('\n');  // URC for time sync
      data = "";
      DBG("### Network time updated.");
      return true;
    }
    return false;
  }

 public:
  /// Stream used to communicate with the modem.
  Stream& stream;

 protected:
  GsmClientMC60* sockets[TcpConfig::kMuxCount];
};

#endif  // SRC_TINYGSMCLIENTMC60_H_
