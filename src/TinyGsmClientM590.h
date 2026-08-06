/**
 * @file       TinyGsmClientM590.h
 * @brief      M590 modem client and modem-trait definitions.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */
/* clang-format off */
/**
 * @defgroup neoway_m590 Neoway M590 Modem Family
 * @brief Manufacturer: Neoway. Models: M590.
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
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemInfo "getModemInfo()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemName "getModemName()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemManufacturer "getModemManufacturer()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemModel "getModemModel()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemRevision "getModemRevision()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::getModemSerialNumber "getModemSerialNumber()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::factoryDefault "factoryDefault()"
 * - Power functions (TinyGsmModem.tpp)
 *     - @ref TinyGsmModem<modemType, modemConfig>::restart "restart()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::poweroff "poweroff()"
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
 * - TCP functions (TinyGsmTCP.tpp)
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::maintain "maintain()"
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::findFirstUnassignedMux "findFirstUnassignedMux()"
 * - Text messaging (SMS) functions (TinyGsmSMS.tpp)
 *     - @ref TinyGsmSMS<modemType>::sendUSSD "sendUSSD()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS "sendSMS()"
 * - Time functions (TinyGsmTime.tpp)
 *     - @ref TinyGsmTime<modemType>::getGSMDateTime "getGSMDateTime()"
 *     - @ref TinyGsmTime<modemType>::getNetworkTime "getNetworkTime()"
 *
 * # Connection Information
 *
 * - TCP sockets:
 *   - 2
 *   - SSL is not supported
 * - Socket Buffering:
 *   - The modem does **not** have an internal buffer for incoming data.
 *   - You must read all data from the modem as soon as it arrives, or you will
 * lose it.
 *   - You can reduce the risk of losing data by setting this library's buffer
 * to be as large as possible; this will increase the memory footprint of your
 * program.
 *   - Change the buffer size by defining TINY_GSM_RX_BUFFER in your sketch
 * before including any TinyGSM header file.
 * - Socket Numbering:
 *   - The modem does not allow you to specify the multiplexing channel.
 *   - The modem will automatically assign a channel when the client connects to
 * a server.
 *   - Use the getMux() function to get the assigned multiplexing channel number
 * after a successful connection.
 */
/* clang-format on */

#ifndef SRC_TINYGSMCLIENTM590_H_
#define SRC_TINYGSMCLIENTM590_H_
#pragma message("TinyGSM:  TinyGsmClientM590")

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmTime.tpp"

/// Registration status
/// @ingroup neoway_m590
enum M590RegStatus {
  REG_NO_RESULT    = -1,  ///< No result yet
  REG_UNREGISTERED = 0,   ///< Not registered
  REG_SEARCHING    = 3,   ///< Searching for network
  REG_DENIED       = 2,   ///< Registration denied
  REG_OK_HOME      = 1,   ///< Registered, home network
  REG_OK_ROAMING   = 5,   ///< Registered, roaming
  REG_UNKNOWN      = 4,   ///< Unknown registration status
};

/// Basic modem configurations for the M590 modem family
/// @ingroup neoway_m590
struct TinyGsmM590ModemConfig : public TinyGsmModemConfigPreset<M590RegStatus> {
  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "Neoway";
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "M590";
};

constexpr char TinyGsmM590ModemConfig::MODEM_MANUFACTURER[];
constexpr char TinyGsmM590ModemConfig::MODEM_MODEL[];

/**
 * @brief TCP behavior and limits for the M590 modem family.
 *
 * The M590 can send up to 2000 bytes at a time with TCPSEND
 * @ingroup neoway_m590
 */
struct TinyGsmM590TcpConfig
    : public TinyGsmTcpConfigPreset<
          /*bufferMode*/ TinyGsmTcpBufferMode::NoModemBuffer,
          /*muxMode*/ TinyGsmTcpMuxMode::Static,
          /*muxCount*/ 2,
          /*sendMaxSize*/ 2000,
          /*connectTimeoutS*/ 75,  // default
          /*stopTimeoutS*/ 1> {};

/// Class for the Neoway M590
/// @ingroup neoway_m590
class TinyGsmM590 : public TinyGsmModem<TinyGsmM590, TinyGsmM590ModemConfig>,
                    public TinyGsmGPRS<TinyGsmM590>,
                    public TinyGsmTCP<TinyGsmM590, TinyGsmM590TcpConfig>,
                    public TinyGsmSMS<TinyGsmM590>,
                    public TinyGsmTime<TinyGsmM590> {
  friend class TinyGsmModem<TinyGsmM590, TinyGsmM590ModemConfig>;
  friend class TinyGsmGPRS<TinyGsmM590>;
  friend class TinyGsmTCP<TinyGsmM590, TinyGsmM590TcpConfig>;
  friend class GsmClient<TinyGsmM590, TinyGsmM590TcpConfig>;
  friend class TinyGsmSMS<TinyGsmM590>;
  friend class TinyGsmTime<TinyGsmM590>;

  using ModemConfig = TinyGsmM590ModemConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  /// @ingroup neoway_m590
  class GsmClientM590 : public GsmClient<TinyGsmM590, TinyGsmM590TcpConfig> {
    friend class TinyGsmM590;

   public:
    using GsmClient<TinyGsmM590, TinyGsmM590TcpConfig>::connect;
    using GsmClient<TinyGsmM590, TinyGsmM590TcpConfig>::stop;

    /**
     * @brief Create a new GsmClientM590 object.  This must be initialized with
     * a TinyGsmM590 modem before it can be used.
     */
    GsmClientM590() {
      is_secure = false;
    }
    /**
     * @brief Create a new TCP client and bind it to a modem and optionally a
     * multiplexing channel.
     * @param modem Modem instance used by this client.
     * @param mux Multiplexing channel to use.
     *
     * @note The M590 allows you choose the multiplexing channel number, but if
     * the input mux channel number is already in use and other mux channels are
     * available, this library will select the next available one.  Use the
     * getMux() function to get the assigned multiplexing channel number after a
     * successful connection.
     */
    explicit GsmClientM590(TinyGsmM590& modem, uint8_t mux = 0) {
      init(&modem, mux);
      is_secure = false;
    }

    /**
     * @brief Initialize the TCP client with a modem and optionally a
     * multiplexing channel.
     * @return true if initialization was successful, false otherwise.
     * @copydetails GsmClientM590::GsmClientM590(TinyGsmM590&, uint8_t)
     */
    bool init(TinyGsmM590* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_connected = false;
      is_mid_send    = false;

      // The M590 generally lets you choose the mux number, but we want to try
      // to find an empty place in the socket array for it.

      // if it's a valid mux number, and that mux number isn't in use (or it's
      // already this), accept the mux number
      if (mux < TinyGsmM590TcpConfig::kMuxCount &&
          (at->sockets[mux] == nullptr || at->sockets[mux] == this)) {
        this->mux = mux;
        // If the mux number is in use or out of range, find the next available
        // one
      } else if (at->findFirstUnassignedMux() != static_cast<uint8_t>(-1)) {
        this->mux = at->findFirstUnassignedMux();
      } else {
        // If we can't find anything available, overwrite something, using mod
        // to make sure we're in range
        this->mux = (mux % TinyGsmM590TcpConfig::kMuxCount);
      }
      at->sockets[this->mux] = this;

      return true;
    }

   public:
    int connect(const char* host, uint16_t port, int timeout_s) override {
      stop(TinyGsmM590TcpConfig::kStopTimeoutS * 1000L);
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }

    void stop(uint32_t maxWaitMs) override {
      is_mid_send = false;
      TINY_GSM_YIELD();
      at->sendAT(GF("+TCPCLOSE="), mux);
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
  // NOT SUPPORTED

  /*
   * GSM Modem Constructor
   */
 public:
  /**
   * @brief Construct a modem wrapper around a stream transport.
   *
   * @param stream Stream used to communicate with the modem.
   */
  explicit TinyGsmM590(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientM590"));

    if (!testAT()) { return false; }

    sendAT(GF("E0"));  // Echo Off
    if (waitResponse() != 1) { return false; }

#ifdef TINY_GSM_DEBUG
    sendAT(GF("+CMEE=2"));  // turn on verbose error codes
#else
    sendAT(GF("+CMEE=0"));  // turn off error codes
#endif
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

  // This is extracted from the modem info
  String getModemNameImpl() {
    sendAT('I');
    String factory = stream.readStringUntil('\n');  // read the factory
    factory.trim();
    String model = stream.readStringUntil('\n');  // read the model
    model.trim();
    streamSkipUntil('\n');  // skip the revision
    waitResponse();         // wait for the OK
    return factory + ' ' + model;
  }

  // This is extracted from the modem info
  String getModemManufacturerImpl() {
    sendAT('I');
    String factory = stream.readStringUntil('\n');  // read the factory
    factory.trim();
    streamSkipUntil('\n');  // skip the model
    streamSkipUntil('\n');  // skip the revision
    if (waitResponse() == 1) { return factory; }
    return String(GFP(ModemConfig::MODEM_MANUFACTURER));
  }

  // This is extracted from the modem info
  String getModemModelImpl() {
    sendAT('I');
    streamSkipUntil('\n');                        // skip the factory
    String model = stream.readStringUntil('\n');  // read the model
    model.trim();
    streamSkipUntil('\n');  // skip the revision
    if (waitResponse() == 1) { return model; }
    return String(GFP(ModemConfig::MODEM_MODEL));
  }

  // Gets the modem firmware version
  // This is extracted from the modem info
  String getModemRevisionImpl() {
    sendAT('I');
    streamSkipUntil('\n');                      // skip the factory
    streamSkipUntil('\n');                      // skip the model
    String res = stream.readStringUntil('\n');  // read the revision
    res.trim();
    waitResponse();  // wait for the OK
    return res;
  }

  // Extra stuff here - pwr save, internal stack
  bool factoryDefaultImpl() {
    sendAT(GF("&FZE0&W"));  // Factory + Reset + Echo Off + Write
    waitResponse();
    sendAT(GF("+ICF=3,1"));  // 8 data 0 parity 1 stop
    waitResponse();
    sendAT(GF("+ENPWRSAVE=0"));  // Disable PWR save
    waitResponse();
    sendAT(GF("+XISP=0"));  // Use internal stack
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
    if (!setPhoneFunctionality(15)) { return false; }
    // MODEM:STARTUP
    waitResponse(60000L, GF("+PBREADY\r\n"));
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+CPWROFF"));
    return waitResponse(3000L) == 1;
  }

  bool sleepEnableImpl(bool enable) {
    sendAT(GF("+ENPWRSAVE="), enable);
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
  M590RegStatus getRegistrationStatusImpl() {
    return (M590RegStatus)getRegistrationStatusXREG("CREG");
  }

  bool isNetworkConnectedImpl() {
    M590RegStatus s = this->getRegistrationStatus();
    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
  }

  String getLocalIPImpl() {
    sendAT(GF("+XIIC?"));
    if (waitResponse(GF("+XIIC:")) != 1) { return ""; }
    streamSkipUntil(',');
    String res = stream.readStringUntil('\n');
    waitResponse();
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

    sendAT(GF("+XISP=0"));
    waitResponse();

    sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, '"');
    waitResponse();

    if (!user) user = "";
    if (!pwd) pwd = "";
    sendAT(GF("+XGAUTH=1,1,\""), user, GF("\",\""), pwd, '"');
    waitResponse();

    sendAT(GF("+XIIC=1"));
    waitResponse();

    const uint32_t timeout_ms = 60000L;
    for (uint32_t start = millis(); millis() - start < timeout_ms;) {
      if (isGprsConnected()) {
        // goto set_dns; // TODO
        return true;
      }
      delay(500);
    }
    return false;

    // set_dns:  // TODO
    //     sendAT(GF("+DNSSERVER=1,8.8.8.8"));
    //     waitResponse();
    //
    //     sendAT(GF("+DNSSERVER=2,8.8.4.4"));
    //     waitResponse();

    return true;
  }

  bool gprsDisconnectImpl() {
    // TODO(?): There is no command in AT command set
    // XIIC=0 does not work
    return true;
  }

  bool isGprsConnectedImpl() {
    sendAT(GF("+XIIC?"));
    if (waitResponse(GF("+XIIC:")) != 1) { return false; }
    int8_t res = streamGetIntBefore(',');
    waitResponse();
    return res == 1;
  }

  /*
   * SIM card functions
   */
 protected:
  // Able to follow all SIM card functions as inherited from TinyGsmGPRS.tpp


  /*
   * Phone call functions
   */
  // No functions of this type supported

  /*
   * Audio functions
   */
  // No functions of this type supported

  /*
   * Text messaging (SMS) functions
   */
 protected:
  bool sendSMS_UTF16Impl(const String& number, const void* text,
                         size_t len) TINY_GSM_ATTR_NOT_AVAILABLE;

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
    for (int i = 0; i < 3; i++) {  // TODO(?): no need for loop?
      String ip = dnsIpQuery(host);

      sendAT(GF("+TCPSETUP="), mux, ',', ip, ',', port);
      int8_t rsp = waitResponse(timeout_ms, GF(",OK\r\n"), GF(",FAIL\r\n"),
                                GF("+TCPSETUP:Error\r\n"));
      if (1 == rsp) {
        return true;
      } else if (3 == rsp) {
        sendAT(GF("+TCPCLOSE="), mux);
        waitResponse();
      }
      delay(1000);
    }
    return false;
  }

  // re-implement so we don't have an extra flush
  size_t modemSendImpl(const uint8_t* buff, size_t len, uint8_t mux) {
    // Pointer to where in the buffer we're up to
    // A const cast is need to cast-away the constant-ness of the buffer (ie,
    // modify it).
    uint8_t* txPtr     = const_cast<uint8_t*>(buff);
    size_t   bytesSent = 0;

    do {
      // make no more than 3 attempts at the single send command
      int8_t send_attempts = 0;
      bool   send_success  = false;
      while (send_attempts < 3 && !send_success) {
        // Number of bytes to send from buffer in this command
        size_t sendLength = TinyGsmM590TcpConfig::kSendMaxSize;
        // Ensure the program doesn't read past the allocated memory
        if (txPtr + TinyGsmM590TcpConfig::kSendMaxSize >
            const_cast<uint8_t*>(buff) + len) {
          sendLength = const_cast<uint8_t*>(buff) + len - txPtr;
        }
        // start up a send command
        send_success = modemBeginSend(sendLength, mux);
        if (!send_success) {
          send_attempts++;
          continue;
        }
        // write out the number of bytes for this chunk
        int16_t attempted =
            stream.write(reinterpret_cast<const uint8_t*>(txPtr), sendLength);
        // NOTE: Don't flush here! Differ that to the modemEndSend() function
        // End this send command and check its responses
        // NOTE: In many cases, confirmed is just a passthrough of len
        int16_t confirmed = modemEndSend(len, mux);
        bytesSent += min(attempted,
                         confirmed);         // bump up number of bytes sent
        txPtr += min(attempted, confirmed);  // bump up the pointer
        send_success &= min(attempted, confirmed) > 0;
        send_attempts++;
      }
      // if we failed after 3 attempts at the same chunk, bail from the whole
      // thing
      if (!send_success) { break; }
    } while (bytesSent < len && sockets[mux]->sock_connected);
    return bytesSent;
  }

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    sendAT(GF("+TCPSEND="), mux, ',', (uint16_t)len);
    return waitResponse(GF(">")) == 1;
  }
  size_t modemEndSendImpl(size_t len, uint8_t) {
    stream.write(static_cast<char>(0x0D));
    stream.flush();
    if (waitResponse(30000L, GF("+TCPSEND:")) != 1) { return 0; }
    streamSkipUntil('\n');
    return len;
  }

  bool modemGetConnectedImpl(uint8_t mux) {
    sendAT(GF("+CIPSTATUS="), mux);
    int8_t res = waitResponse(GF(",\"CONNECTED\""), GF(",\"CLOSED\""),
                              GF(",\"CLOSING\""), GF(",\"INITIAL\""));
    waitResponse();
    return 1 == res;
  }

  String dnsIpQuery(const char* host) {
    sendAT(GF("+DNS=\""), host, '"');
    if (waitResponse(10000L, GF("+DNS:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse(GF("+DNS:OK\r\n"));
    res.trim();
    return res;
  }

  /*
   * Utilities
   */
 protected:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+TCPRECV:"))) {
      int8_t  mux          = streamGetIntBefore(',');
      int16_t len_reported = streamGetIntBefore(',');
      int16_t len          = len_reported;
      if (mux >= 0 && mux < TinyGsmM590TcpConfig::kMuxCount && sockets[mux]) {
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
    } else if (data.endsWith(GF("+TCPCLOSE:"))) {
      int8_t mux = streamGetIntBefore(',');
      streamSkipUntil('\n');
      if (mux >= 0 && mux < TinyGsmM590TcpConfig::kMuxCount && sockets[mux]) {
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
  GsmClientM590* sockets[TinyGsmM590TcpConfig::kMuxCount];
};

#endif  // SRC_TINYGSMCLIENTM590_H_
