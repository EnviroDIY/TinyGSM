/**
 * @file       TinyGsmClientM95.h
 * @brief      M95 modem client and modem-trait definitions.
 * @author     Volodymyr Shymanskyy, Pacman Pereira, and Replicade Ltd.
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy, (c)2017 Replicade Ltd.
 * <http://www.replicade.com>
 * @date       Nov 2016
 */
/* clang-format off */
/**
 * @defgroup quectel_m95 Quectel M95 Modem Family
 * @brief Manufacturer: Quectel. Models: M95.
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
 * - Temperature functions (TinyGsmTemperature.tpp)
 *     - @ref TinyGsmTemperature<modemType>::getTemperature "getTemperature()"
 * - Text messaging (SMS) functions
 *     - @ref TinyGsmM95::deleteAllSMS "deleteAllSMS()"
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
 *   - The Quectel M95 uses static mux selection.
 *   - The caller chooses the multiplexing channel number via the constructor or
 * init() function.
 *   - init() honors the requested channel when available; if unavailable, it
 * will select the next available channel or use modulo assignment.
 *
 * @todo In `modemEndSendImpl()`: get len/ack properly
 * @todo In `modemReadImpl()`: Does this even work????
 */
/* clang-format on */

#ifndef SRC_TINYGSMCLIENTM95_H_
#define SRC_TINYGSMCLIENTM95_H_
#pragma message("TinyGSM:  TinyGsmClientM95")

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmBattery.tpp"
#include "TinyGsmTemperature.tpp"

/// Registration status
/// @ingroup quectel_m95
enum class M95RegStatus {
  REG_NO_RESULT    = -1,  ///< No registration result
  REG_UNREGISTERED = 0,   ///< Not registered on the network
  REG_SEARCHING    = 2,   ///< Searching for network
  REG_DENIED       = 3,   ///< Registration denied
  REG_OK_HOME      = 1,   ///< Registered on the home network
  REG_OK_ROAMING   = 5,   ///< Registered on a roaming network
  REG_UNKNOWN      = 4,   ///< Unknown registration status
};

/// Basic modem configurations for the M95 modem family
/// @ingroup quectel_m95
struct TinyGsmM95ModemConfig : public TinyGsmModemConfigPreset<M95RegStatus> {
  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "Quectel";
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "M95";
};

constexpr char TinyGsmM95ModemConfig::MODEM_MANUFACTURER[]
    __attribute__((weak));
constexpr char TinyGsmM95ModemConfig::MODEM_MODEL[] __attribute__((weak));

/**
 * @brief TCP behavior and limits for the M95 modem family.
 *
 * The send data commands, QISEND and QSSLSEND, both accept up to 1460 bytes of
 * input.
 * @ingroup quectel_m95
 */
struct TinyGsmM95TcpConfig
    : public TinyGsmTcpConfigPreset<
          /*bufferMode*/ TinyGsmTcpBufferMode::BufferReadNoCheck,
          /*muxMode*/ TinyGsmTcpMuxMode::Static,
          /*muxCount*/ 6,
          /*sendMaxSize*/ 1460,
          /*connectTimeoutS*/ 75,  // default
          /*stopTimeoutS*/ 75> {};

/// Class for the Quectel M95
/// @ingroup quectel_m95
class TinyGsmM95 : public TinyGsmModem<TinyGsmM95, TinyGsmM95ModemConfig>,
                   public TinyGsmGPRS<TinyGsmM95>,
                   public TinyGsmTCP<TinyGsmM95, TinyGsmM95TcpConfig>,
                   public TinyGsmCalling<TinyGsmM95>,
                   public TinyGsmSMS<TinyGsmM95>,
                   public TinyGsmTime<TinyGsmM95>,
                   public TinyGsmBattery<TinyGsmM95>,
                   public TinyGsmTemperature<TinyGsmM95> {
  friend class TinyGsmModem<TinyGsmM95, TinyGsmM95ModemConfig>;
  friend class TinyGsmGPRS<TinyGsmM95>;
  friend class TinyGsmTCP<TinyGsmM95, TinyGsmM95TcpConfig>;
  friend class GsmClient<TinyGsmM95, TinyGsmM95TcpConfig>;
  friend class TinyGsmCalling<TinyGsmM95>;
  friend class TinyGsmSMS<TinyGsmM95>;
  friend class TinyGsmTime<TinyGsmM95>;
  friend class TinyGsmBattery<TinyGsmM95>;
  friend class TinyGsmTemperature<TinyGsmM95>;

 public:
  using ModemConfig = TinyGsmM95ModemConfig;
  using TcpConfig   = TinyGsmM95TcpConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  /// @ingroup quectel_m95
  class GsmClientM95 : public GsmClient<TinyGsmM95, TinyGsmM95TcpConfig> {
    friend class TinyGsmM95;

   public:
    using GsmClient<TinyGsmM95, TinyGsmM95TcpConfig>::connect;
    using GsmClient<TinyGsmM95, TinyGsmM95TcpConfig>::stop;
    using TcpConfig = TinyGsmM95TcpConfig;

    /**
     * @brief Create a new TCP client.
     * @warning You must call the init() method before attempting to use a
     * client created with this constructor.
     */
    GsmClientM95() {
      is_secure = false;
    }
    /**
     * @brief Create a new TCP client and bind it to a modem and optionally a
     * multiplexing channel.
     * @param modem Modem instance used by this client.
     * @param mux Multiplexing channel to use.
     *
     * @note The M95 allows you choose the multiplexing channel number, but if
     * the input mux channel number is already in use and other mux channels are
     * available, this library will select the next available one.  Use the
     * getMux() function to get the assigned multiplexing channel number after a
     * successful connection.
     */
    explicit GsmClientM95(TinyGsmM95& modem, uint8_t mux = 0)
        : GsmClient<TinyGsmM95, TinyGsmM95TcpConfig>(modem, mux) {
      is_secure = false;
      init(&modem, mux);
    }

    /**
     * @brief Initialize the TCP client with a modem and optionally a
     * multiplexing channel.
     * @return true if initialization was successful, false otherwise.
     * @copydetails GsmClientM95::GsmClientM95(TinyGsmM95&, uint8_t)
     */
    bool init(TinyGsmM95* modem, uint8_t mux = 0) {
      if (modem == nullptr) { return false; }
      this->at       = modem;
      sock_available = 0;
      sock_connected = false;
      is_mid_send    = false;

      // The M95 generally lets you choose the mux number, but we want to try
      // to find an empty place in the socket array for it.

      // if it's a valid mux number, and that mux number isn't in use (or it's
      // already this), accept the mux number
      if (mux < TcpConfig::kMuxCount &&
          (at->sockets[mux] == nullptr || at->sockets[mux] == this)) {
        this->mux = mux;
        // If the mux number is in use or out of range, find the next available
        // one
      } else {
        uint8_t nextMux = at->findFirstUnassignedMux();
        if (nextMux != static_cast<uint8_t>(-1)) {
          this->mux = nextMux;
        } else {
          // If we can't find anything available, overwrite something, using mod
          // to make sure we're in range
          this->mux = (mux % TcpConfig::kMuxCount);
        }
      }
      at->sockets[this->mux] = this;

      return true;
    }

    /*
     * Client API
     */
   public:
    TINY_GSM_STATIC_TCP_CONNECT

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
  explicit TinyGsmM95(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientM95"));

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
    sendAT(GF("+CFUN=0"));
    if (waitResponse(10000L, GF("NORMAL POWER DOWN"), GF("OK"), GF("FAIL")) ==
        3) {
      return false;
    }
    sendAT(GF("+CFUN=1"));
    if (waitResponse(10000L, GF("Call Ready"), GF("OK"), GF("FAIL")) == 3) {
      return false;
    }
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+QPOWD=1"));
    return waitResponse(300, GF("NORMAL POWER DOWN")) == 1;
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

  bool setPhoneFunctionalityImpl(uint8_t fun,
                                 bool    reset) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  /*
   * Generic network functions
   */
 protected:
  M95RegStatus getRegistrationStatusImpl() {
    return static_cast<M95RegStatus>(getRegistrationStatusXREG("CREG"));
  }

  bool isNetworkConnectedImpl() {
    M95RegStatus s = this->getRegistrationStatus();
    return (s == M95RegStatus::REG_OK_HOME ||
            s == M95RegStatus::REG_OK_ROAMING);
  }

  void setHostFormat(bool useDottedQuad) {
    if (useDottedQuad) {
      sendAT(GF("+QIDNSIP=0"));
    } else {
      sendAT(GF("+QIDNSIP=1"));
    }
    waitResponse();
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

    // Select TCP/IP transfer mode - NOT transparent mode
    sendAT(GF("+QIMODE=0"));
    if (waitResponse() != 1) { return false; }

    // Enable multiple TCP/IP connections
    sendAT(GF("+QIMUX=1"));
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
    // Mode = 1 - Output a notification when data is received
    // +QIRDI: <id>,<sc>,<sid>
    sendAT(GF("+QINDI=1"));
    if (waitResponse() != 1) { return false; }

    // // Request an IP header for received data
    // // "IPD(data length):"
    // sendAT(GF("+QIHEAD=1"));
    // if (waitResponse() != 1) {
    //   return false;
    // }
    //
    // // Do NOT show the IP address of the sender when receiving data
    // // The format to show the address is: RECV FROM: <IP ADDRESS>:<PORT>
    // sendAT(GF("+QISHOWRA=0"));
    // if (waitResponse() != 1) {
    //   return false;
    // }
    //
    // // Do NOT show the protocol type at the end of the header for received
    // data
    // // IPD(data length)(TCP/UDP):
    // sendAT(GF("+QISHOWPT=0"));
    // if (waitResponse() != 1) {
    //   return false;
    // }
    //
    // // Do NOT show the destination address before receiving data
    // // The format to show the address is: TO:<IP ADDRESS>
    // sendAT(GF("+QISHOWLA=0"));
    // if (waitResponse() != 1) {
    //   return false;
    // }

    return true;
  }

  bool gprsDisconnectImpl() {
    sendAT(GF("+QIDEACT"));  // Deactivate the bearer context
    return waitResponse(60000L, GF("DEACT OK"), GF("ERROR")) == 1;
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
  String getSimCCIDImpl() {
    sendAT(GF("+QCCID"));
    if (waitResponse(GF("+QCCID:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
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
    if (waitResponse(waitResponse(60000L, GF("OK"), GF("ERROR")) == 1)) {
      return true;
    }
    return false;
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
  // Follows all NTP server functions as inherited from TinyGsmNTP.tpp

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
 protected:
  float getTemperatureImpl() {
    sendAT(GF("+QTEMP?"));
    if (waitResponse(GF("+QTEMP:")) != 1) { return static_cast<float>(-9999); }
    streamSkipUntil(',');  // Skip mode
    // Read charge of thermistor
    // milliVolts = streamGetIntBefore(',');
    streamSkipUntil(',');  // Skip thermistor charge
    float temp = streamGetFloatBefore('\n');
    // Wait for final OK
    waitResponse();
    return temp;
  }

  /*
   * Client-related functions
   */
 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t /*static*/ mux,
                        int timeout_s) {
    if (mux >= TcpConfig::kMuxCount || !sockets[mux]) { return false; }
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    sendAT(GF("+QIOPEN="), mux, GF(",\""), GF("TCP"), GF("\",\""), host,
           GF("\","), port);
    int8_t rsp = waitResponse(timeout_ms, GF("CONNECT OK\r\n"),
                              GF("CONNECT FAIL\r\n"),
                              GF("ALREADY CONNECT\r\n"));
    return (1 == rsp || 3 == rsp);  // OK or ALREADY CONNECT
  }

  bool modemStopImpl(uint8_t mux, uint32_t maxWaitMs) {
    if (mux >= TcpConfig::kMuxCount || !sockets[mux]) { return false; }
    sendAT(GF("+QICLOSE="), mux);
    int8_t rsp = waitResponse(maxWaitMs, GF("CLOSED"), GF("CLOSE OK"),
                              GF("ERROR"));
    return rsp == 1 || rsp == 2;
  }

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    if (mux >= TcpConfig::kMuxCount || !sockets[mux]) { return false; }
    sendAT(GF("+QISEND="), mux, ',', (uint16_t)len);
    return waitResponse(GF(">")) == 1;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t) {
    if (waitResponse(GF("SEND OK")) != 1) { return 0; }
    return len;
    // TODO(?): get len/ack properly
    // bool allAcknowledged = false;
    // // bool failed = false;
    // while ( !allAcknowledged ) {
    //   sendAT( GF("+QISACK"));
    //   if (waitResponse(5000L, GF("+QISACK:")) != 1) {
    //     return -1;
    //   } else {
    //     streamSkipUntil(',');  // Skip total length sent on connection
    //     streamSkipUntil(',');  // Skip length already acknowledged by remote
    //     // Make sure the total length un-acknowledged is 0
    //     if ( streamGetIntBefore('\n') == 0 ) {
    //       allAcknowledged = true;
    //     }
    //   }
    // }
    // waitResponse(5000L);
  }

  size_t modemReadImpl(size_t size, uint8_t mux) {
    if (mux >= TcpConfig::kMuxCount || !sockets[mux]) { return 0; }
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
      int16_t len_reported = streamGetIntBefore('\n');
      if (len_reported <= 0) {
        sockets[mux]->sock_available = 0;
        waitResponse();  // ends with an OK
        return 0;
      }
      // We have no way of knowing in advance how much data will be in the
      // buffer so when data is received we always assume the buffer is
      // completely full. Chances are, this is not true and there's really not
      // that much there. In that case, make sure we make sure we re-set the
      // amount of data available.
      if (static_cast<size_t>(len_reported) < size) {
        sockets[mux]->sock_available = len_reported;
      }
      size_t len_read = moveCharsFromStreamToFifo(mux, len_reported);
      // Never wrap the unsigned available counter
      sockets[mux]->sock_available -= TinyGsmMin(
          static_cast<size_t>(sockets[mux]->sock_available), len_read);
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
    if (mux >= TcpConfig::kMuxCount || !sockets[mux]) { return false; }
    sendAT(GF("+QISTATE=1,"), mux);
    // +QISTATE: 0,"TCP","151.139.237.11",80,5087,4,1,0,0,"uart1"

    if (waitResponse(GF("+QISTATE:")) != 1) { return false; }

    int16_t ret_mux = streamGetIntBefore(',');  // mux
    streamSkipUntil(',');                       // Skip socket type
    streamSkipUntil(',');                       // Skip remote ip
    streamSkipUntil(',');                       // Skip remote port
    streamSkipUntil(',');                       // Skip local port
    int8_t res = streamGetIntBefore(',');       // socket state

    waitResponse();

    // 0 Initial, 1 Opening, 2 Connected, 3 Listening, 4 Closing
    return 2 == res && isExpectedMux(ret_mux, mux);
  }

  /*
   * Utilities
   */
 protected:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+QIRDI:"))) {
      streamSkipUntil(',');  // Skip the context
      streamSkipUntil(',');  // Skip the role
      int16_t mux = streamGetIntBefore('\n');
      // DBG("### Got Data:", mux);
      if (isValidMux(mux)) {
        // We have no way of knowing how much data actually came in, so
        // we set the value to 1500, the maximum possible size.
        sockets[mux]->sock_available = 1500;
      }
      data = "";
      return true;
    } else if (data.endsWith(GF("CLOSED\r\n"))) {
      int16_t nl = TinyGsmMax(0,
                              data.lastIndexOf(String(GFP(ModemConfig::GSM_NL)),
                                               data.length() - 8));
      int16_t coma = data.indexOf(',', nl + 2);
      if (coma > nl + 2) {
        int16_t mux = data.substring(nl + 2, coma).toInt();
        if (isValidMux(mux)) { sockets[mux]->sock_connected = false; }
        DBG("### Closed: ", mux);
      }
      data = "";
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
  GsmClientM95* sockets[TcpConfig::kMuxCount];
};

#endif  // SRC_TINYGSMCLIENTM95_H_
