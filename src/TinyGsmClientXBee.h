/**
 * @file       TinyGsmClientXBee.h
 * @brief      XBee modem client and modem-trait definitions.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy, XBee module by Sara
 * Damiano
 * @date       Nov 2016
 *
 * @defgroup digi_xbee Digi XBee Modem Family
 * @brief Manufacturer: Digi. Models: XBee cellular modules supported by
 * TinyGSM.
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
 * - WiFi functions (TinyGsmWifi.tpp)
 *     - @ref TinyGsmWifi<modemType>::networkConnect "networkConnect()"
 *     - @ref TinyGsmWifi<modemType>::networkDisconnect "networkDisconnect()"
 * - TCP functions (TinyGsmTCP.tpp)
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::maintain "maintain()"
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::findFirstUnassignedMux "findFirstUnassignedMux()"
 * - Text messaging (SMS) functions (TinyGsmSMS.tpp)
 *     - @ref TinyGsmSMS<modemType>::sendSMS "sendSMS()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS_UTF16 "sendSMS_UTF16()"
 * - Battery functions (TinyGsmBattery.tpp)
 *     - @ref TinyGsmBattery<modemType>::getBattVoltage "getBattVoltage()"
 *     - @ref TinyGsmBattery<modemType>::getBattStats "getBattStats()"
 * - Temperature functions (TinyGsmTemperature.tpp)
 *     - @ref TinyGsmTemperature<modemType>::getTemperature "getTemperature()"
 * - XBee Type functions
 *     - @ref TinyGsmXBee::getBeeType "getBeeType()"
 *     - @ref TinyGsmXBee::getBeeName "getBeeName()"
 * - Utilities
 *     - @ref TinyGsmXBee::commandMode "commandMode()"
 *     - @ref TinyGsmXBee::writeChanges "writeChanges()"
 *     - @ref TinyGsmXBee::exitCommand "exitCommand()"
 *     - @ref TinyGsmXBee::exitAndFail "exitAndFail()"
 *     - @ref TinyGsmXBee::getSeries "getSeries()"
 *     - @ref TinyGsmXBee::readResponseString "readResponseString()"
 *     - @ref TinyGsmXBee::readResponseInt "readResponseInt()"
 *     - @ref TinyGsmXBee::sendATGetString "sendATGetString()"
 *     - @ref TinyGsmXBee::changeSettingIfNeeded "changeSettingIfNeeded()"
 *     - @ref TinyGsmXBee::gotIPforSavedHost "gotIPforSavedHost()"
 *
 * # Connection Information
 *
 * - Combined TCP/SSL sockets:
 *   - 1
 *   - The much more complicated (and unsupported) API mode is needed for
 * multiplexing.
 * - Socket Buffering:
 *   - The modem does **not** have an internal buffer for incoming data.
 *   - You must read all data from the modem as soon as it arrives, or you will
 * lose it.
 *   - You can reduce the risk of losing data by setting this library's buffer
 * to be as large as possible; this will increase the memory footprint of your
 * program.
 *   - Change the buffer size by defining TINY_GSM_RX_BUFFER_SIZE in your sketch
 * before including any TinyGSM header file.
 *   - Change the buffer size by defining TINY_GSM_RX_BUFFER_SIZE in your sketch
 * before including any TinyGSM header file.
 * - Socket Numbering:
 *   - The modem does not allow you to specify the multiplexing channel.
 *   - The modem will automatically assign a channel when the client connects to
 * a server.
 *   - Use the getMux() function to get the assigned multiplexing channel number
 * after a successful connection.
 */
#ifndef SRC_TINYGSMCLIENTXBEE_H_
#define SRC_TINYGSMCLIENTXBEE_H_
#pragma message("TinyGSM:  TinyGsmClientXBee")


#ifdef TINY_GSM_XBEE_GUARD_TIME
#undef TINY_GSM_XBEE_GUARD_TIME
#endif
/**
 * @brief "Guard time" for XBee modules, in milliseconds.
 * This is the required wait time before and after sending the '+++' to the
 * module to enter command mode.  XBee's have a default guard time of 1 second
 * (1000ms, 10 extra for safety here)
 */
#define TINY_GSM_XBEE_GUARD_TIME 1010

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmSSL.tpp"
#include "TinyGsmWifi.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmTemperature.tpp"
#include "TinyGsmBattery.tpp"

/**
 * @brief function decorator to enter command mode for XBee modules, if not
 * already in command mode, and exit command mode after the function call if it
 * was not already in command mode.
 *
 * Use this to avoid too many entrances and exits from command mode.  The
 * cellular Bee's often freeze up and won't respond when attempting to enter
 * command mode too many times.
 *
 */
#define XBEE_COMMAND_START_DECORATOR(nAttempts, failureReturn)                \
  bool wasInCommandMode = inCommandMode &&                                    \
      millis() - lastCommandModeMillis <=                                     \
          static_cast<uint32_t>(guardTime * 2);                               \
  if (!wasInCommandMode) { /* don't re-enter command mode if already in it */ \
    if (!commandMode(nAttempts))                                              \
      return failureReturn; /* Return immediately if fails */                 \
  }
/// Function decorator to exit command mode for XBee modules, if not already in
/// command mode, after the function call if it was not already in command mode.
#define XBEE_COMMAND_END_DECORATOR                                       \
  if (!wasInCommandMode) { /* only exit if we weren't in command mode */ \
    exitCommand();                                                       \
  }

/// Registration status
/// @ingroup digi_xbee
enum XBeeRegStatus {
  REG_OK           = 0,  ///< Registered on the network
  REG_UNREGISTERED = 1,  ///< Not registered on the network
  REG_SEARCHING    = 2,  ///< Searching for network
  REG_DENIED       = 3,  ///< Registration denied
  REG_UNKNOWN      = 4,  ///< Unknown registration status
};

/// Basic modem configurations for the XBee modem family
/// @ingroup digi_xbee
struct TinyGsmXBeeModemConfig : public TinyGsmModemConfigPreset<XBeeRegStatus> {
  /// The newline character(s) used in AT commands.
  static constexpr char GSM_NL[] TINY_GSM_PROGMEM = "\r";
  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "Digi";
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "XBee";
};

constexpr char TinyGsmXBeeModemConfig::GSM_NL[];
constexpr char TinyGsmXBeeModemConfig::MODEM_MANUFACTURER[];
constexpr char TinyGsmXBeeModemConfig::MODEM_MODEL[];

/**
 * @brief TCP behavior and limits for the XBee modem family.
 *
 * Bee's do not support multiplexing in transparent/command mode.  The much more
 * complicated API mode is needed for multiplexing.
 * @ingroup digi_xbee
 */
struct TinyGsmXBeeTcpConfig
    : public TinyGsmTcpConfigPreset<
          /*bufferMode*/ TinyGsmTcpBufferMode::NoModemBuffer,
          /*muxMode*/ TinyGsmTcpMuxMode::Static,
          /*muxCount*/ 1,
          /*sendMaxSize*/ 1500,    // default
          /*connectTimeoutS*/ 75,  // default
          /*stopTimeoutS*/ 5> {};

/// The known types of XBees
/// The values are responses to the HS command to get "hardware series"
/// @ingroup digi_xbee
enum XBeeType {
  XBEE_UNKNOWN   = 0,      ///< Unknown XBee type
  XBEE_S6B_WIFI  = 0x601,  ///< Digi XBee Wi-Fi
  XBEE_LTE1_VZN  = 0xB01,  ///< Digi XBee Cellular LTE Cat 1
  XBEE_3G        = 0xB02,  ///< Digi XBee Cellular 3G
  XBEE3_LTE1_ATT = 0xB06,  ///< Digi XBee3 Cellular LTE CAT 1
  XBEE3_LTEM_ATT = 0xB08,  ///< Digi XBee3 Cellular LTE-M
  XBEE3_LTEM3    = 0xB0E,  ///< Digi XBee3 Cellular LTE-M3
};

/// Class for the Digi XBee family of modems
/// @ingroup digi_xbee
class TinyGsmXBee : public TinyGsmModem<TinyGsmXBee, TinyGsmXBeeModemConfig>,
                    public TinyGsmGPRS<TinyGsmXBee>,
                    public TinyGsmWifi<TinyGsmXBee>,
                    public TinyGsmTCP<TinyGsmXBee, TinyGsmXBeeTcpConfig>,
                    public TinyGsmSMS<TinyGsmXBee>,
                    public TinyGsmBattery<TinyGsmXBee>,
                    public TinyGsmTemperature<TinyGsmXBee> {
  friend class TinyGsmModem<TinyGsmXBee, TinyGsmXBeeModemConfig>;
  friend class TinyGsmGPRS<TinyGsmXBee>;
  friend class TinyGsmWifi<TinyGsmXBee>;
  friend class TinyGsmTCP<TinyGsmXBee, TinyGsmXBeeTcpConfig>;
  friend class GsmClient<TinyGsmXBee, TinyGsmXBeeTcpConfig>;
  friend class TinyGsmSMS<TinyGsmXBee>;
  friend class TinyGsmBattery<TinyGsmXBee>;
  friend class TinyGsmTemperature<TinyGsmXBee>;

  using ModemConfig = TinyGsmXBeeModemConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  /// @ingroup digi_xbee
  class GsmClientXBee : public GsmClient<TinyGsmXBee, TinyGsmXBeeTcpConfig> {
    friend class TinyGsmXBee;

   public:
    using GsmClient<TinyGsmXBee, TinyGsmXBeeTcpConfig>::connect;
    using GsmClient<TinyGsmXBee, TinyGsmXBeeTcpConfig>::stop;

    /**
     * @brief Create a new TCP client.  This must be initialized with a modem
     * before it can be used.
     */
    GsmClientXBee() {
      is_secure = false;
    }
    /**
     * @brief Create a new TCP client and bind it to a modem.
     * @param modem Modem instance used by this client.
     *
     * @note The XBee does not support multiplexing in transparent/command mode.
     * The much more complicated API mode is needed for multiplexing.
     */
    explicit GsmClientXBee(TinyGsmXBee& modem, uint8_t /*mux*/ = 0) {
      init(&modem);
      is_secure = false;
    }

    /**
     * @brief Initialize the TCP client with a modem.
     * @return true if initialization was successful, false otherwise.
     * @copydetails GsmClientXBee::GsmClientXBee(TinyGsmXBee&, uint8_t)
     */
    bool init(TinyGsmXBee* modem, uint8_t /*mux*/ = 0) {
      this->at       = modem;
      this->mux      = 0;
      sock_connected = false;
      is_mid_send    = false;

      at->sockets[0] = this;

      return true;
    }

   public:
    /**
     * @copydoc GsmClient::connect(const char*, uint16_t, int)
     *
     * @note The XBee saves all connection information (ssid/pwd etc) except IP
     * address and port number, in flash (NVM). The NVM is be updated only when
     * it is initialized. The TCP connection itself is not opened until you
     * attempt to send data. Because all settings are saved to flash, it is
     * possible (or likely) that you could send data even if you haven't "made"
     * any connection.
     */
    int connect(const char* host, uint16_t port, int timeout_s) override {
      // NOTE:  Not calling stop() or yield() here
      at->streamClear();  // Empty anything in the buffer before starting
      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }
    /// @copydoc GsmClient::connect(const char*, uint16_t)
    int connect(const char* host, uint16_t port) override {
      return connect(host, port, 75);
    }

    /// @copydoc GsmClient::connect(IPAddress, uint16_t, int)
    virtual int connect(IPAddress ip, uint16_t port, int timeout_s) {
      if (timeout_s != 0) {
        DBG("Timeout [", timeout_s, "] doesn't apply here.");
      }
      // NOTE:  Not calling stop() or yield() here
      at->streamClear();  // Empty anything in the buffer before starting
      sock_connected = at->modemConnect(ip, port, mux);
      return sock_connected;
    }
    /// @copydoc GsmClient::connect(IPAddress, uint16_t)
    int connect(IPAddress ip, uint16_t port) override {
      return connect(ip, port, 0);
    }

    /**
     * @copydoc GsmClient::stop(uint32_t)
     *
     * @note Because settings are saved in flash, the XBEE will attempt to
     * reconnect to the previous socket if it receives any outgoing data.
     * Setting sock_connected to false after the stop ensures that connected()
     * will return false after a stop has been ordered.  This makes it play much
     * more nicely with libraries like PubSubClient.
     */
    void stop(uint32_t maxWaitMs) override {
      at->streamClear();  // Empty anything in the buffer
      // empty the saved currently-in-use destination address
      at->modemStop(maxWaitMs);
      at->streamClear();  // Empty anything in the buffer
      is_mid_send    = false;
      sock_connected = false;
    }

    /// @copydoc GsmClient::write(const uint8_t*, size_t)
    size_t write(const uint8_t* buf, size_t size) override {
      TINY_GSM_YIELD();
      return at->modemSend(buf, size, mux);
    }

    /// @copydoc GsmClient::write(uint8_t)
    size_t write(uint8_t c) override {
      return write(&c, 1);
    }

    /// @copydoc GsmClient::write(const char*)
    size_t write(const char* str) {
      if (str == nullptr) return 0;
      return write(reinterpret_cast<const uint8_t*>(str), strlen(str));
    }

    /// @copydoc GsmClient::available()
    int available() override {
      TINY_GSM_YIELD();
      return at->stream.available();
      /*
      if (!rx.size() || at->stream.available()) {
        at->maintain();
      }
      return at->stream.available() + rx.size();
      */
    }

    /// @copydoc GsmClient::read(uint8_t*, size_t)
    int read(uint8_t* buf, size_t size) override {
      TINY_GSM_YIELD();
      return at->stream.readBytes(reinterpret_cast<char*>(buf), size);
      /*
      size_t cnt = 0;
      uint32_t _startMillis = millis();
      while (cnt < size && millis() - _startMillis < _timeout) {
        size_t chunk = TinyGsmMin(size-cnt, rx.size());
        if (chunk > 0) {
          rx.get(buf, chunk);
          buf += chunk;
          cnt += chunk;
          continue;
        }
        // TODO(vshymanskyy): Read directly into user buffer?
        if (!rx.size() || at->stream.available()) {
          at->maintain();
        }
      }
      return cnt;
      */
    }

    /// @copydoc GsmClient::read()
    int read() override {
      TINY_GSM_YIELD();
      return at->stream.read();
      /*
      uint8_t c;
      if (read(&c, 1) == 1) {
        return c;
      }
      return -1;
      */
    }

    /// @copydoc GsmClient::peek()
    int peek() override {
      return at->stream.peek();
    }
    /// @copydoc GsmClient::flush()
    void flush() override {
      at->stream.flush();
    }

    /// @copydoc GsmClient::connected()
    uint8_t connected() override {
      if (available()) {
        return true;
        // if we never got an IP, it can't be connected
      } else if (at->savedIP == IPAddress(0, 0, 0, 0)) {
        return false;
      }
      return sock_connected;
      // NOTE:  We don't check or return
      // modemGetConnected() because we don't
      // want to go into command mode.
      // return at->modemGetConnected();
    }
    /// @copydoc GsmClient::operator bool()
    operator bool() override {
      return connected();
    }

    /*
     * Extended API
     */

    /// @copydoc GsmClient::remoteIP()
    String remoteIP() {
      IPAddress atLastIP = at->savedIP;
      return TinyGsmStringFromIp(atLastIP);
    }
  };

  /*
   * Inner Secure Client
   */
 public:
  /// Inner secure client
  /// @ingroup digi_xbee
  class GsmClientSecureXBee : public GsmClientXBee, public GsmSecureClient {
    friend class TinyGsmXBee;

   public:
    using GsmClientXBee::connect;
    using GsmClientXBee::stop;

    TINY_GSM_SECURE_CLIENT_CTORS(XBee)
  };

  /*
   * GSM Modem Constructor
   */
 public:
  /**
   * @brief Construct a modem wrapper around a stream transport.
   * @param stream Stream used to communicate with the modem.
   */
  explicit TinyGsmXBee(Stream& stream) : TinyGsmXBee(stream, -1) {
    // Delegate to main constructor
  }

  /**
   * @brief Construct a modem wrapper around a stream transport and a reset pin.
   * @param stream Stream used to communicate with the modem.
   * @param resetPin Physical pin on the MCU that is connected to the XBee reset
   * pin .
   */
  TinyGsmXBee(Stream& stream, int8_t resetPin)
      : stream(stream),
        guardTime(TINY_GSM_XBEE_GUARD_TIME),
        beeType(XBEE_UNKNOWN),
        resetPin(resetPin),
        savedIP(IPAddress(0, 0, 0, 0)),
        savedHost(""),
        savedHostIP(IPAddress(0, 0, 0, 0)),
        savedOperatingIP(IPAddress(0, 0, 0, 0)),
        inCommandMode(false),
        lastCommandModeMillis(0),
        lastHostLookupMillis(0) {
    // Start not knowing what kind of bee it is
    // Start with the default guard time of 1 second
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientXBee"));

    if (resetPin >= 0) {
      pinMode(resetPin, OUTPUT);
      digitalWrite(resetPin, HIGH);
    }

    XBEE_COMMAND_START_DECORATOR(10, false)

    bool changesMade = false;
    bool ret_val     = true;

    // if there's a pin, we need to re-write to flash each time
    if (pin && strlen(pin) > 0) {
      sendAT(GF("PN"), pin);
      if (waitResponse() != 1) {
        ret_val = false;
      } else {
        changesMade = true;
      }
    }

    // Put in transparent mode, if it isn't already
    changesMade |= changeSettingIfNeeded(GF("AP"), 0x0);

    // shorten the guard time to 100ms, if it was anything else
    sendAT(GF("GT"));
    if (readResponseInt() != 0x64) {
      sendAT(GF("GT"), 64);
      ret_val &= waitResponse() == 1;
      if (ret_val) {
        guardTime   = 110;
        changesMade = true;
      }
    } else {
      guardTime = 110;
    }

    // Make sure the command mode drop-out time is long enough that we won't
    // fall out of command mode without intentionally leaving it.  This is the
    // default drop out time of 0x64 x 100ms (10 seconds)
    changesMade |= changeSettingIfNeeded(GF("CT"), 0x64);

    if (changesMade) { ret_val &= writeChanges(); }

    getSeries();  // Get the "Hardware Series";

    XBEE_COMMAND_END_DECORATOR

    return ret_val;
  }

  String getModemNameImpl() {
    String result;
    result.reserve(26);
    result =
        (const __FlashStringHelper*)TinyGsmXBeeModemConfig::MODEM_MANUFACTURER;
    result += ' ';
    result += getModemModelImpl();
    return result;
  }

  String getModemModelImpl() {
    PGM_P suffix;
    switch (beeType) {
      case XBEE_S6B_WIFI: suffix = PSTR(" Wi-Fi"); break;
      case XBEE_LTE1_VZN: suffix = PSTR(" Cellular LTE Cat 1"); break;
      case XBEE_3G: suffix = PSTR(" Cellular 3G"); break;
      case XBEE3_LTE1_ATT: suffix = PSTR("3 Cellular LTE CAT 1"); break;
      case XBEE3_LTEM_ATT: suffix = PSTR("3 Cellular LTE-M"); break;
      case XBEE3_LTEM3: suffix = PSTR("3 Cellular LTE-M3"); break;
      default: suffix = PSTR(" Unknown");
    }
    String result;
    result.reserve(30);
    result = (const __FlashStringHelper*)TinyGsmXBeeModemConfig::MODEM_MODEL;
    result += (const __FlashStringHelper*)suffix;
    return result;
  }

  // Gets the modem serial number
  String getModemSerialNumberImpl() {
    String result;
    result.reserve(24);
    result = sendATGetString(GF("SL"));  // Request Module MAC/Serial Number Low
    result += ' ';
    result +=
        sendATGetString(GF("SH"));  // Request Module MAC/Serial Number High
    return result;
  }

  // Gets the modem hardware version
  String getModemHardwareVersion() {
    return sendATGetString(GF("HV"));
  }

  // Gets the modem firmware version
  String getModemFirmwareVersion() {
    return sendATGetString(GF("VR"));
  }

  // Gets the modem combined version
  String getModemRevisionImpl() {
    String result;
    result.reserve(24);
    result = getModemHardwareVersion();
    result += ' ';
    result += getModemFirmwareVersion();
    return result;
  }

  bool setBaudImpl(uint32_t baud) {
    XBEE_COMMAND_START_DECORATOR(5, false)
    bool changesMade = false;
    switch (baud) {
      case 2400: changesMade |= changeSettingIfNeeded(GF("BD"), 0x1); break;
      case 4800: changesMade |= changeSettingIfNeeded(GF("BD"), 0x2); break;
      case 9600: changesMade |= changeSettingIfNeeded(GF("BD"), 0x3); break;
      case 19200: changesMade |= changeSettingIfNeeded(GF("BD"), 0x4); break;
      case 38400: changesMade |= changeSettingIfNeeded(GF("BD"), 0x5); break;
      case 57600: changesMade |= changeSettingIfNeeded(GF("BD"), 0x6); break;
      case 115200: changesMade |= changeSettingIfNeeded(GF("BD"), 0x7); break;
      case 230400: changesMade |= changeSettingIfNeeded(GF("BD"), 0x8); break;
      case 460800: changesMade |= changeSettingIfNeeded(GF("BD"), 0x9); break;
      case 921600: changesMade |= changeSettingIfNeeded(GF("BD"), 0xA); break;
      default: {
        DBG(GF("Specified baud rate is unsupported! Setting to 9600 baud."));
        changesMade |= changeSettingIfNeeded(GF("BD"),
                                             0x3);  // Set to default of 9600
        break;
      }
    }
    if (changesMade) { writeChanges(); }
    XBEE_COMMAND_END_DECORATOR
    return true;
  }

  bool testATImpl(uint32_t timeout_ms) {
    uint32_t start   = millis();
    bool     success = false;
    while (!success && millis() - start < timeout_ms) {
      if (!inCommandMode) {
        success = commandMode();
        if (success) exitCommand();
      } else {
        sendAT();
        if (waitResponse(200) == 1) {
          success = true;
        } else {
          // if we didn't respond to the AT, assume we're not in command mode
          inCommandMode = false;
        }
      }
      delay(250);
    }
    return success;
  }

  void maintainImpl() {
    // this only happens OUTSIDE command mode, so if we're getting characters
    // they should be data received from the TCP connection
    // TINY_GSM_YIELD();
    // if (!inCommandMode) {
    //   while (stream.available()) {
    //     char c = stream.read();
    //     if (c > 0) sockets[0]->rx.put(c);
    //   }
    // }
  }

  bool factoryDefaultImpl() {
    XBEE_COMMAND_START_DECORATOR(5, false)
    sendAT(GF("RE"));
    bool ret_val = waitResponse() == 1;
    ret_val &= writeChanges();
    XBEE_COMMAND_END_DECORATOR
    // Make sure the guard time for the modem object is set back to default
    // otherwise communication would fail after the reset
    guardTime = 1010;
    return ret_val;
  }

  String getModemInfoImpl() {
    return sendATGetString(GF("HS"));
  }

  /*
  bool thisHasSSL() {
    if (beeType == XBEE_S6B_WIFI)
      return false;
    else
      return true;
  }

  bool thisHasWifi() {
    if (beeType == XBEE_S6B_WIFI)
      return true;
    else
      return false;
  }

  bool thisHasGPRS() {
    if (beeType == XBEE_S6B_WIFI)
      return false;
    else
      return true;
  }
  */

  /*
   * XBee Type functions
   */
 public:
  /**
   * @brief Get the type of XBee module currently in use.
   * @return The current Bee Type.
   */
  XBeeType getBeeType() {
    return beeType;
  }

  /**
   * @brief Get the name of the XBee module currently in use.
   * @return The name of the current XBee module.
   */
  String getBeeName() {
    return getModemNameImpl();
  }

  /*
   * Power functions
   */
 protected:
  // The XBee's have a bad habit of getting into an unresponsive funk
  // This uses the board's hardware reset pin to force it to reset
  void pinReset(const char* pin = nullptr) {
    if (resetPin >= 0) {
      DBG("### Forcing a modem reset!\r\n");
      digitalWrite(resetPin, LOW);
      delay(1);
      digitalWrite(resetPin, HIGH);
    } else {
      DBG("### Attempting a modem software restart");
      restart(pin);
    }
  }

  bool restartImpl(const char* pin) {
    if (!commandMode()) {
      DBG("### XBee not in command mode for restart; Exit");
      return false;
    }  // Return immediately

    if (beeType == XBEE_UNKNOWN) getSeries();  // how we restart depends on this

    if (beeType != XBEE_S6B_WIFI) {
      sendAT(GF("AM1"));  // Digi suggests putting cellular modules into
                          // airplane mode before restarting This allows the
                          // sockets and connections to close cleanly
      if (waitResponse() != 1) return exitAndFail();
      if (!writeChanges()) return exitAndFail();
    }

    sendAT(GF("FR"));
    if (waitResponse() != 1)
      return exitAndFail();
    else
      inCommandMode = false;  // Reset effectively exits command mode

    if (beeType == XBEE_S6B_WIFI)
      delay(2000);  // Wifi module actually resets about 2 seconds later
    else
      delay(100);  // cellular modules wait 100ms before reset happens

    // Wait until reboot completes and XBee responds to command mode call again
    for (uint32_t start = millis(); millis() - start < 60000L;) {
      if (commandMode(1)) break;
      delay(250);  // wait a litle before trying again
    }

    if (beeType != XBEE_S6B_WIFI) {
      sendAT(GF("AM0"));  // Turn off airplane mode
      if (waitResponse() != 1) return exitAndFail();
      if (!writeChanges()) return exitAndFail();
    }

    exitCommand();

    return init(pin);
  }

  void setupPinSleep(bool maintainAssociation = false) {
    XBEE_COMMAND_START_DECORATOR(5, )

    if (beeType == XBEE_UNKNOWN) getSeries();  // Command depends on series

    bool changesMade = false;

    // Pin sleep
    changesMade |= changeSettingIfNeeded(GF("SM"), 0x1);

    if (beeType == XBEE_S6B_WIFI && !maintainAssociation) {
      // For lowest power, dissassociated deep sleep
      changesMade |= changeSettingIfNeeded(GF("SO"), 0x200);
    } else if (!maintainAssociation) {
      // For supported cellular modules, maintain association
      // Not supported by all modules, will return "ERROR"
      changesMade |= changeSettingIfNeeded(GF("SO"), 0x1);
    }

    if (changesMade) { writeChanges(); }
    XBEE_COMMAND_END_DECORATOR
  }

  bool
  powerOffImpl() {  // NOTE:  Not supported for WiFi or older cellular firmware
    XBEE_COMMAND_START_DECORATOR(5, false)
    sendAT(GF("SD"));
    bool ret_val = waitResponse(120000L) == 1;
    // make sure we're really shut down
    if (ret_val) { ret_val &= (sendATGetString(GF("AI")) == "2D"); }
    XBEE_COMMAND_END_DECORATOR
    return ret_val;
  }

  // Enable airplane mode
  bool radioOffImpl() {
    bool success     = true;
    bool changesMade = false;
    XBEE_COMMAND_START_DECORATOR(5, false)
    changesMade = changeSettingIfNeeded(GF("AM"), 0x1, 5000L);
    if (changesMade) { success = writeChanges(); }
    XBEE_COMMAND_END_DECORATOR
    return success;
  }

  bool sleepEnableImpl(bool enable) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  bool setPhoneFunctionalityImpl(uint8_t fun,
                                 bool    reset) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  /*
   * Generic network functions
   */
 protected:
  XBeeRegStatus parseWifiRegStatus(int16_t code) {
    // 0x00 Successfully joined an access point, established IP addresses and
    // IP listening sockets
    if (code == 0x00) return REG_OK;
    // 0x01 Wi-Fi transceiver initialization in progress.
    // 0x02 Wi-Fi transceiver initialized, but not yet scanning for access
    // point.
    // 0x40 Waiting for WPA or WPA2 Authentication.
    // 0x41 Device joined a network and is waiting for IP configuration to
    // complete
    // 0x42 Device is joined, IP is configured, and listening sockets are being
    // set up.
    // 0xFF Device is currently scanning for the configured SSID.
    if (code == 0x01 || code == 0x02 || code == 0x40 || code == 0x41 ||
        code == 0x42 || code == 0xFF)
      return REG_SEARCHING;
    // 0x13 Disconnecting from access point.
    if (code == 0x13) {
      restart();  // S6B tends to get stuck "disconnecting"
      return REG_UNREGISTERED;
    }
    // 0x23 SSID not configured.
    if (code == 0x23) return REG_UNREGISTERED;
    // 0x24 Encryption key invalid (either NULL or invalid length for WEP).
    // 0x27 SSID was found, but join failed.
    if (code == 0x24 || code == 0x27) return REG_DENIED;
    return REG_UNKNOWN;
  }

  XBeeRegStatus parseCellularRegStatus(int16_t code) {
    // 0x00 Connected to the Internet.
    if (code == 0x00) return REG_OK;
    // 0x22 Registering to cellular network.
    // 0x23 Connecting to the Internet.
    // 0xFF Initializing.
    if (code == 0x22 || code == 0x23 || code == 0xFF) return REG_SEARCHING;
    // 0x25 Cellular network registration denied.
    if (code == 0x25) return REG_DENIED;
    // 0x2A Airplane mode.
    if (code == 0x2A) {
      sendAT(GF("AM0"));  // Turn off airplane mode
      waitResponse();
      writeChanges();
      return REG_UNKNOWN;
    }
    // 0x2F Bypass mode active.
    if (code == 0x2F) {
      sendAT(GF("AP0"));  // Set back to transparent mode
      waitResponse();
      writeChanges();
      return REG_UNKNOWN;
    }
    // 0x24 The cellular component is missing, corrupt, or otherwise in error.
    // 0x2B USB Direct active.
    // 0x2C Cellular component is in PSM (power save mode).
    // All other codes default to REG_UNKNOWN
    return REG_UNKNOWN;
  }

  XBeeRegStatus getRegistrationStatusImpl() {
    XBEE_COMMAND_START_DECORATOR(5, REG_UNKNOWN)

    if (!inCommandMode) return REG_UNKNOWN;
    if (beeType == XBEE_UNKNOWN) getSeries();

    sendAT(GF("AI"));
    int16_t       intRes = readResponseInt(10000L);
    XBeeRegStatus stat   = (beeType == XBEE_S6B_WIFI)
          ? parseWifiRegStatus(intRes)
          : parseCellularRegStatus(intRes);

    XBEE_COMMAND_END_DECORATOR
    return stat;
  }

  int8_t getSignalQualityImpl() {
    XBEE_COMMAND_START_DECORATOR(5, 0);

    if (beeType == XBEE_UNKNOWN)
      getSeries();  // Need to know what type of bee so we know how to ask

    if (beeType == XBEE_S6B_WIFI)
      sendAT(GF("LM"));  // ask for the "link margin" - the dB above sensitivity
    else
      sendAT(GF("DB"));  // ask for the cell strength in dBm
    int16_t intRes = readResponseInt();

    XBEE_COMMAND_END_DECORATOR

    if (beeType == XBEE3_LTEM_ATT && intRes == 105)
      intRes = 0;  // tends to reply with "69" when signal is unknown

    if (beeType == XBEE_S6B_WIFI) {
      if (intRes == 0xFF) {
        return 0;  // 0xFF returned for unknown
      } else {
        return -93 + intRes;  // the maximum sensitivity is -93dBm
      }
    } else {
      return -1 * intRes;  // need to convert to negative number
    }
  }

  bool isNetworkConnectedImpl() {
    // first check for association indicator
    XBeeRegStatus s = this->getRegistrationStatus();
    if (s == REG_OK) {
      if (beeType == XBEE_S6B_WIFI) {
        // For wifi bees, if the association indicator is ok, check that a both
        // a local IP and DNS have been allocated
        IPAddress ip  = localIP();
        IPAddress dns = getDNSAddress();
        if (ip != IPAddress(0, 0, 0, 0) && dns != IPAddress(0, 0, 0, 0)) {
          return true;
        } else {
          return false;
        }
      } else {
        return true;
      }
    } else {
      return false;
    }
  }

  bool waitForNetworkImpl(uint32_t timeout_ms, bool check_signal) {
    bool retVal = false;
    XBEE_COMMAND_START_DECORATOR(5, false)
    for (uint32_t start = millis(); millis() - start < timeout_ms;) {
      if (check_signal) { getSignalQuality(); }
      if (isNetworkConnected()) {
        retVal = true;
        break;
      }
      delay(250);  // per Neil H. - more stable with delay
    }
    XBEE_COMMAND_END_DECORATOR
    return retVal;
  }

  String getLocalIPImpl() {
    XBEE_COMMAND_START_DECORATOR(5, "")
    sendAT(GF("MY"));
    String IPaddr;
    IPaddr.reserve(16);
    // wait for the response - this response can be very slow
    IPaddr = readResponseString(30000);
    XBEE_COMMAND_END_DECORATOR
    IPaddr.trim();
    return IPaddr;
  }

  String getDNS() {
    XBEE_COMMAND_START_DECORATOR(5, "")
    switch (beeType) {
      case XBEE_S6B_WIFI: {
        sendAT(GF("NS"));
        break;
      }
      default: {
        sendAT(GF("N1"));
        break;
      }
    }
    String DNSaddr;
    DNSaddr.reserve(16);
    // wait for the response - this response can be very slow
    DNSaddr = readResponseString(30000);
    XBEE_COMMAND_END_DECORATOR
    DNSaddr.trim();
    return DNSaddr;
  }

  IPAddress getDNSAddress() {
    return TinyGsmIpFromString(getDNS());
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // Uses the secure client inherited from TinyGsmSSL.tpp for setting the
  // certificate name and the SSL connection type so those can be called at
  // connection time, but this library does **NOT** currently support uploading,
  // deleting, or converting certificates on the modem.
  // Uploading the certificates is easy using the Digi XBee Studio program.

#undef TINY_GSM_MODEM_CAN_LOAD_CERTS

  /*
   * Wifi functions
   */
 protected:
  bool networkConnectImpl(const char* ssid, const char* pwd) {
    bool changesMade = false;
    bool retVal      = true;

    XBEE_COMMAND_START_DECORATOR(5, false)

    if (ssid == nullptr) retVal = false;

    changesMade |= changeSettingIfNeeded(GF("ID"), ssid);

    if (pwd && strlen(pwd) > 0) {
      // Set security to WPA2
      changesMade |= changeSettingIfNeeded(GF("EE"), 0x2);
      // set the password
      // the wifi bee will NOT return the previously set password,
      // so we have no way of knowing if the passwords has changed
      // and must re-write to flash each time
      sendAT(GF("PK"), pwd);
      if (waitResponse() != 1) {
        retVal = false;
      } else {
        changesMade = true;
      }
    } else {
      changesMade |= changeSettingIfNeeded(GF("EE"), 0x0);  // Set No security
    }

    if (changesMade) { retVal &= writeChanges(); }

    XBEE_COMMAND_END_DECORATOR

    return retVal;
  }

  bool networkDisconnectImpl() {
    XBEE_COMMAND_START_DECORATOR(5, false)
    sendAT(GF("NR0"));  // Do a network reset in order to disconnect
    // WARNING:  On wifi modules, using a network reset will not
    // allow the same ssid to re-join without rebooting the module.
    int8_t res = (1 == waitResponse(5000));
    writeChanges();
    XBEE_COMMAND_END_DECORATOR
    return res;
  }

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* user, const char* pwd) {
    bool success     = true;
    bool changesMade = false;
    XBEE_COMMAND_START_DECORATOR(5, false)

    // the cellular bees will NOT return the previously set username or
    // password, so we have no way of knowing if they have changed
    // and must re-write to flash each time
    if (user && strlen(user) > 0) {
      sendAT(GF("CU"), user);  // Set the user for the APN
      if (waitResponse() != 1) {
        success = false;
      } else {
        changesMade = true;
      }
    }
    if (pwd && strlen(pwd) > 0) {
      sendAT(GF("CW"), pwd);  // Set the password for the APN
      if (waitResponse() != 1) {
        success = false;
      } else {
        changesMade = true;
      }
    }
    changesMade |= changeSettingIfNeeded(GF("AN"), String(apn));  // Set the APN

    changesMade |= changeSettingIfNeeded(GF("AM"), 0x0,
                                         5000L);  // Airplane mode off

    if (changesMade) { success = writeChanges(); }
    XBEE_COMMAND_END_DECORATOR
    return success;
  }

  bool gprsDisconnectImpl() {
    bool success = true;
    XBEE_COMMAND_START_DECORATOR(5, false)
    // Cheating and disconnecting by turning on airplane mode
    bool changesMade = changeSettingIfNeeded(GF("AM"), 0x1, 5000L);

    if (changesMade) { success = writeChanges(); }
    XBEE_COMMAND_END_DECORATOR
    return success;
  }

  bool isGprsConnectedImpl() {
    return isNetworkConnected();
  }

  String getOperatorImpl() {
    return sendATGetString(GF("MN"));
  }

  /*
   * SIM card functions
   */
 protected:
  bool simUnlockImpl(const char* pin) {
    if (pin && strlen(pin) > 0) {
      sendAT(GF("PN"), pin);
      return waitResponse() == 1;
    }
    return false;
  }

  String getSimCCIDImpl() {
    return sendATGetString(GF("S#"));
  }

  String getIMEIImpl() {
    return sendATGetString(GF("IM"));
  }

  String getIMSIImpl() {
    return sendATGetString(GF("II"));
  }

  SimStatus getSimStatusImpl(uint32_t) {
    return SIM_READY;  // unsupported
  }

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
  String sendUSSDImpl(const String& code) TINY_GSM_ATTR_NOT_AVAILABLE;

  bool sendSMSImpl(const String& number, const String& text) {
    bool changesMade = false;
    if (!commandMode()) { return false; }  // Return immediately

    sendAT(GF("IP"));  // check mode
    if (readResponseInt() != 2) {
      sendAT(GF("IP"), 2);  // Put in text messaging mode
      if (waitResponse() != 1) {
        return exitAndFail();
      } else {
        changesMade = true;
      }
    }

    sendAT(GF("PH"));  // check last number
    if (readResponseString() != String(number)) {
      sendAT(GF("PH"), number);  // Set the phone number
      if (waitResponse() != 1) {
        return exitAndFail();
      } else {
        changesMade = true;
      }
    }

    sendAT(GF("TD"));  // check the text delimiter
    if (readResponseString() != String("D")) {
      sendAT(GF("TDD"));  // Set the text delimiter to the standard 0x0D
                          //(carriage return)
      if (waitResponse() != 1) {
        return exitAndFail();
      } else {
        changesMade = true;
      }
    }

    if (changesMade) {
      if (!writeChanges()) return exitAndFail();
    }
    // Get out of command mode to actually send the text
    exitCommand();

    stream.print(text);
    stream.write(
        static_cast<char>(0x0D));  // close off with the carriage return

    return true;
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
  // No functions of this type supported
  // ATDT0 - as secs since 1/1/2000
  // ATDO1 - as ISO8601 format

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
  // Use: float vBatt = modem.getBattVoltage() / 1000.0;
  int16_t getBattVoltageImpl() {
    int16_t intRes = 0;
    XBEE_COMMAND_START_DECORATOR(5, false)
    if (beeType == XBEE_UNKNOWN) getSeries();
    if (beeType == XBEE_S6B_WIFI) {
      sendAT(GF("%V"));
      intRes = readResponseInt();
    }
    XBEE_COMMAND_END_DECORATOR
    return intRes;
  }

  int8_t getBattPercentImpl() TINY_GSM_ATTR_NOT_AVAILABLE;
  int8_t getBattChargeStateImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool getBattStatsImpl(int8_t& chargeState, int8_t& percent,
                        int16_t& milliVolts) {
    chargeState = 0;
    percent     = 0;
    milliVolts  = getBattVoltage();
    return true;
  }

  /*
   * Temperature functions
   */
 protected:
  float getTemperatureImpl() {
    XBEE_COMMAND_START_DECORATOR(5, static_cast<float>(-9999))
    String res = sendATGetString(GF("TP"));
    if (res == "") { return static_cast<float>(-9999); }
    char buf[5] = {
        0,
    };
    res.toCharArray(buf, 5);
    int8_t intRes = (int8_t)strtol(
        buf, 0,
        16);  // degrees Celsius displayed in 8-bit two's complement format.
    XBEE_COMMAND_END_DECORATOR
    return static_cast<float>(intRes);
  }

  /*
   * Client related functions
   */
 protected:
  int16_t getConnectionIndicator() {
    XBEE_COMMAND_START_DECORATOR(5, false)
    sendAT(GF("CI"));
    int16_t intRes = readResponseInt();
    XBEE_COMMAND_END_DECORATOR
    return intRes;
  }

  IPAddress getOperatingIP() {
    String strIP;
    strIP.reserve(16);

    XBEE_COMMAND_START_DECORATOR(5, IPAddress(0, 0, 0, 0))
    sendAT(GF("OD"));
    strIP = stream.readStringUntil('\r');  // read result
    strIP.trim();
    XBEE_COMMAND_END_DECORATOR

    if (strIP != "" && strIP != GF("ERROR")) {
      return TinyGsmIpFromString(strIP);
    } else {
      return IPAddress(0, 0, 0, 0);
    }
  }

  IPAddress lookupHostIP(const char* host, int timeout_s = 45) {
    String strIP;
    strIP.reserve(16);
    uint32_t startMillis = millis();
    uint32_t timeout_ms  = ((uint32_t)timeout_s) * 1000;
    bool     gotIP       = false;
    XBEE_COMMAND_START_DECORATOR(5, IPAddress(0, 0, 0, 0))
    // XBee's require a numeric IP address for connection, but do provide the
    // functionality to look up the IP address from a fully qualified domain
    // name
    // NOTE: the lookup can take a while
    while ((millis() - startMillis) < timeout_ms) {
      DBG("Looking up IP address for host:", host);
      sendAT(GF("LA"), host);
      while (stream.available() < 4 && (millis() - startMillis < timeout_ms)) {
        TINY_GSM_YIELD()
      }
      strIP = stream.readStringUntil('\r');  // read result
      strIP.trim();
      if (strIP != "" && strIP != GF("ERROR")) {
        gotIP                = true;
        lastHostLookupMillis = millis();
        break;
      }
      delay(2500);  // wait a bit before trying again
    }

    XBEE_COMMAND_END_DECORATOR

    if (gotIP) {
      return TinyGsmIpFromString(strIP);
    } else {
      return IPAddress(0, 0, 0, 0);
    }
  }

  bool configureConnection(const char* host, uint16_t port, bool ssl) {
    XBEE_COMMAND_START_DECORATOR(5, false)
    bool success     = true;
    bool changesMade = false;

    if (ssl) {
      // If we have a secure socket, use a static cast to get the
      // authentication mode and certificate names. This isn't really "safe"
      // but since we've already checked that the socket is a secure one,
      // we're pretty sure of the type and it should work.
      const GsmClientSecureXBee* thisClient =
          static_cast<const GsmClientSecureXBee*>(sockets[0]);
      SSLAuthMode sslAuthMode    = thisClient->sslAuthMode;
      SSLVersion  sslVersion     = thisClient->sslVersion;
      const char* CAcertName     = thisClient->CAcertName;
      const char* clientCertName = thisClient->clientCertName;
      const char* clientKeyName  = thisClient->clientKeyName;

      // Put in SSL over TCP communication mode
      changesMade |= changeSettingIfNeeded(GF("IP"), 0x4);

      // NOTE: We will always configure TLS profile 0 ($0)

      // set the ssl version
      // The XBee only supports TLS 1.2 (0x3) and 1.3 (0x4, set as default)
      changesMade |= changeSettingIfNeeded(
          GF("TL"), sslVersion == SSLVersion::TLS1_2 ? 0x3 : 0x4);

      // apply the correct certificates to the connection
      // NOTE: We supply the number of certs we expect to use!
      if (sslAuthMode == SSLAuthMode::NO_VALIDATION) {
        changesMade |= changeSettingIfNeeded(GF("$0"), GF(";;"));
      } else if (sslAuthMode == SSLAuthMode::CA_VALIDATION &&
                 CAcertName != nullptr) {
        String newTLSProfile;
        newTLSProfile.reserve(strlen(CAcertName) + 3);
        newTLSProfile = CAcertName;
        newTLSProfile += GF(";;");
        changesMade |= changeSettingIfNeeded(GF("$0"), newTLSProfile);
      } else if (sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION &&
                 CAcertName != nullptr && clientCertName != nullptr &&
                 clientKeyName != nullptr) {
        String newTLSProfile;
        newTLSProfile.reserve(strlen(CAcertName) + strlen(clientCertName) +
                              strlen(clientKeyName) + 3);
        newTLSProfile = CAcertName;
        newTLSProfile += ';';
        newTLSProfile += clientCertName;
        newTLSProfile += ';';
        newTLSProfile += clientKeyName;
        changesMade |= changeSettingIfNeeded(GF("$0"), newTLSProfile);
      } else {
        success = false;
      }
    } else {
      // Put in unsecured TCP mode
      changesMade |= changeSettingIfNeeded(GF("IP"), 0x1);
    }
    bool changesMadeSSL = changesMade;

    // Ignore these changes for the WiFi Bee (why?)
    changesMade |= changeSettingIfNeeded(
        GF("DL"), String(host), 2500L);  // Set the "Destination Address Low"
    String newPort = String(port, HEX);
    newPort.toUpperCase();  // port is returned in uppercase
    changesMade |= changeSettingIfNeeded(GF("DE"),
                                         newPort);  // Set the destination port

    // WiFi Bee is different
    if (beeType == XBEE_S6B_WIFI) { changesMade = changesMadeSSL; }

    if (changesMade) { success &= writeChanges(); }

    XBEE_COMMAND_END_DECORATOR
    return success;
  }

  bool modemConnect(const char* host, uint16_t port, uint8_t mux = 0,
                    int timeout_s = TinyGsmXBeeTcpConfig::kConnectTimeoutS) {
    // check if the host is an IP address already - if so, we can skip the DNS
    // lookup and just connect
    IPAddress hostIP = TinyGsmIpFromString(String(host));
    if (hostIP != IPAddress(0, 0, 0, 0)) {
      DBG("Host is already an IP address; connecting directly");
      return modemConnect(hostIP, port, mux);
    }

    bool retVal = false;

    // If this is a new host name, replace the saved host and wipe out the saved
    // host IP
    if (this->savedHost != String(host)) {
      this->savedHost = String(host);
      savedHostIP     = IPAddress(0, 0, 0, 0);
    }

    XBEE_COMMAND_START_DECORATOR(5, false)

    // confirm the XBee type if needed so we know if we can know if connected
    if (beeType == XBEE_UNKNOWN) { getSeries(); }

    if (beeType != XBEE_S6B_WIFI && beeType != XBEE_LTE1_VZN &&
        beeType != XBEE_3G) {
      // the newer cellular modules can look up the address on the fly
      // this is definitely the better option
      bool ssl     = sockets[mux]->is_secure;
      bool success = configureConnection(host, port, ssl);
      DBG("Attempting to ping the host");
      sendAT(GF("PG"), host);
      readResponseString(2500L);
      uint16_t ci = getConnectionIndicator();
      // if (ci == 0xFF || ci == 0x28) {
      //   DBG("Checking rejection status");
      //   sendAT(GF("RJ"));
      //   readResponseString(10000L);
      // }
      // we'll accept either unknown or connected
      success &= (ci == 0x00 || ci == 0xFF || ci == 0x28);
      XBEE_COMMAND_END_DECORATOR
      return success;
    }

    // For the pre XBee3 Bees, we need to look up the IP address manually

    // If we don't have a good IP for the host, we need to do a DNS search
    if (savedHostIP == IPAddress(0, 0, 0, 0) || lastHostLookupMillis == 0 ||
        millis() - lastHostLookupMillis > 43200000LL) {
      // This will return 0.0.0.0 if lookup fails
      savedHostIP = lookupHostIP(host, timeout_s);
    }

    // If we now have a valid IP address, use it to connect
    if (savedHostIP != IPAddress(0, 0, 0, 0)) {
      // Only re-set connection information if we have an IP address
      retVal = modemConnect(savedHostIP, port, mux);
    }

    XBEE_COMMAND_END_DECORATOR

    return retVal;
  }

  bool modemConnect(IPAddress ip, uint16_t port, uint8_t mux = 0) {
    bool success = true;

    if (mux != 0) {
      DBG("XBee only supports 1 IP channel in transparent mode!");
    }

    // empty the saved currenty-in-use destination address
    savedOperatingIP = IPAddress(0, 0, 0, 0);

    XBEE_COMMAND_START_DECORATOR(5, false)

    if (ip != savedIP) {  // Can skip almost everything if there's no
                          // change in the IP address
      DBG("IP address has changed; reconnecting");
      savedIP = ip;  // Set the newly requested IP address
      String host;
      host.reserve(16);
      host += ip[0];
      host += '.';
      host += ip[1];
      host += '.';
      host += ip[2];
      host += '.';
      host += ip[3];
      bool ssl = sockets[mux]->is_secure;
      success &= configureConnection(host.c_str(), port, ssl);
      DBG("Attempting to ping the host");
      sendAT(GF("PG"), host);
      readResponseString(2500L);
    }

    // confirm the XBee type if needed so we know if we can know if connected
    if (beeType == XBEE_UNKNOWN) { getSeries(); }
    // we'll accept either unknown or connected
    if (beeType != XBEE_S6B_WIFI) {
      uint16_t ci = getConnectionIndicator();
      // if (ci == 0xFF || ci == 0x28) {
      //   DBG("Checking rejection status");
      //   sendAT(GF("RJ"));
      //   readResponseString(10000L);
      // }
      success &= (ci == 0x00 || ci == 0xFF || ci == 0x28);
    }

    if (success) { sockets[mux]->sock_connected = true; }

    XBEE_COMMAND_END_DECORATOR

    return success;
  }

  bool modemStop(uint32_t maxWaitMs) {
    streamClear();  // Empty anything in the buffer
    // empty the saved currently-in-use destination address
    savedOperatingIP = IPAddress(0, 0, 0, 0);

    XBEE_COMMAND_START_DECORATOR(5, false)

    // For WiFi models, there's no direct way to close the socket;
    // use DigiXBeeWifi::disconnectInternet(void)

    if (beeType != XBEE_S6B_WIFI) {
      // Get the current socket timeout
      sendAT(GF("TM"));
      String timeoutUsed = readResponseString(5000L);

      // For cellular models, per documentation: If you write the TM (socket
      // timeout) value while in Transparent Mode, the current connection is
      // immediately closed - this works even if the TM values is unchanged
      sendAT(GF("TM"), timeoutUsed);  // Re-set socket timeout
      waitResponse(maxWaitMs);        // This response can be slow
    }

    XBEE_COMMAND_END_DECORATOR
    return true;
  }

  bool modemBeginSendImpl(size_t, uint8_t mux) {
    if (mux != 0) {
      DBG("XBee only supports 1 IP channel in transparent mode!");
      return false;
    }
    return true;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t) {
    if (beeType != XBEE_S6B_WIFI) {
      // After a send, verify the outgoing ip if it isn't set
      if (savedOperatingIP == IPAddress(0, 0, 0, 0)) {
        modemGetConnected(0);
      } else if (len > 5) {
        // After sending several characters, also re-check
        // NOTE:  I'm intentionally not checking after every single character!
        modemGetConnected(0);
      }
    }

    return len;
  }

  // Helper function to handle cellular connection status for 0x28/0xFF cases
  bool handleUnknownCellularStatus(IPAddress od) {
    IPAddress nullIP = IPAddress(0, 0, 0, 0);
    // If we previously had an operating destination and we no longer
    // do, the socket must have closed
    if (od == nullIP && savedOperatingIP != nullIP) {
      savedOperatingIP           = od;
      sockets[0]->sock_connected = false;
      DBG("Got no operating IP, we're not connected");
      return false;
    }
    // else if the operating destination exists, but is wrong
    // we need to close and re-open
    if (od != nullIP && od != savedIP) {
      DBG("We're connected to the wrong endpoint", od, "not", savedIP);
      sockets[0]->stop();
      return false;
    }
    // else if the operating destination exists and matches, we're good to go
    if (od != nullIP && od == savedIP) {
      DBG("Got new IP of", od, "we should be good");
      savedOperatingIP = od;
      return true;
    }
    // If we never had an operating destination, then sock may be
    // open but data never sent - this is the dreaded "we don't know"
    DBG("We have no idea if we're connected");
    savedOperatingIP = od;
    return true;
  }

  // NOTE:  The CI command returns the status of the TCP connection as open only
  // after data has been sent on the socket.  If it returns 0xFF the socket may
  // really be open, but no data has yet been sent.  We return this unknown
  // value as true so there's a possibility it's wrong.
  bool modemGetConnected(uint8_t) {
    // If the IP address is 0, it's not valid so we can't be connected
    if (savedIP == IPAddress(0, 0, 0, 0)) { return false; }

    XBEE_COMMAND_START_DECORATOR(5, false)

    if (beeType == XBEE_UNKNOWN)
      getSeries();  // Need to know the bee type to interpret response

    switch (beeType) {
      // The wifi be can only say if it's connected to the netowrk
      case XBEE_S6B_WIFI: {
        XBeeRegStatus s = this->getRegistrationStatus();
        XBEE_COMMAND_END_DECORATOR
        if (s != REG_OK) {
          sockets[0]->sock_connected = false;  // no multiplex
        }
        return (s == REG_OK);  // if it's connected, we hope the sockets are too
      }

      // Cellular XBee's
      default: {
        int16_t ci = getConnectionIndicator();
        // Get the operating destination address
        IPAddress od = getOperatingIP();

        // Ask for information about any open sockets
        sendAT(GF("SI"));
        String open_socks = stream.readStringUntil('\r');
        open_socks.replace(String(GFP(ModemConfig::GSM_NL)), "");
        open_socks.trim();
        if (open_socks != "") {
          // In transparent mode, only socket 0 should be possible
          sendAT(GF("SI0"));
          // read socket it
          String sock_id = stream.readStringUntil('\r');
          // read socket state
          String sock_state = stream.readStringUntil('\r');
          // read socket protocol (TCP/UDP)
          String sock_protocol = stream.readStringUntil('\r');
          // read local port number
          String local_port = stream.readStringUntil('\r');
          // read remote port number
          String remote_port = stream.readStringUntil('\r');
          // read remote ip address
          String remoted_address = stream.readStringUntil('\r');  // read result
          streamSkipUntil('\r');  // final carriage return
        }

        XBEE_COMMAND_END_DECORATOR

        // 0x00 = The socket is definitely open
        if (ci == 0x00) {
          savedOperatingIP = od;
          // but it's possible the socket is set to the wrong place
          if (od != IPAddress(0, 0, 0, 0) && od != savedIP) {
            sockets[0]->stop();
            return false;
          }
          return true;
        }

        // 0x28 = "Unknown."
        // 0xFF = No known status - always returned prior to sending data
        if (ci == 0x28 || ci == 0xFF) {
          DBG("Got 0x28 or 0xFF, we don't know if we're connected");
          return handleUnknownCellularStatus(od);
        }

        // 0x21 = User closed
        // 0x27 = Connection lost
        // If the connection is lost or timed out on our side,
        // we force close so it can reopen
        if (ci == 0x21 || ci == 0x27) {
          sendAT(GF("TM"));  // Get socket timeout
          String timeoutUsed = readResponseString(5000L);
          sendAT(GF("TM"), timeoutUsed);  // Re-set socket timeout
          waitResponse(5000L);            // This response can be slow
        }

        // 0x02 = Invalid parameters (bad IP/host)
        // 0x12 = DNS query lookup failure
        // 0x25 = Unknown server - DNS lookup failed (0x22 for UDP socket!)
        if (ci == 0x02 || ci == 0x12 || ci == 0x25) {
          savedIP = IPAddress(0, 0, 0, 0);  // force a lookup next time!
        }

        // If it's anything else (inc 0x02, 0x12, and 0x25)...
        // it's definitely NOT connected
        sockets[0]->sock_connected = false;
        savedOperatingIP           = od;
        return false;
      }
    }
  }

  /*
   * Utilities
   */
 protected:
  /**
   * @brief Handle unsolicited responses (URCs) from the XBee module.
   * @return Always false, as the XBee does not have unsolicited responses in
   * command mode.
   */
  bool handleURCs(String&) {
    return false;
  }

 public:
  /// Clear out the stream buffer
  void streamClear(void) {
    while (stream.available()) {
      stream.read();
      TINY_GSM_YIELD();
    }
  }

  /**
   * @brief Put the XBee into command mode.
   *
   * If it is already in command mode, this function will return true.  If it is
   * not in command mode, it will attempt to enter command mode.
   *
   * @param retries The number of times to retry entering command mode.
   * @return True if the XBee is in command mode, false otherwise.
   */
  bool commandMode(uint8_t retries = 5) {
    // If we're already in command mode, move on
    if (inCommandMode && (millis() - lastCommandModeMillis) < 10000L)
      return true;

    uint8_t triesMade = 0;
    int8_t  res;
    bool    success         = false;
    uint8_t triesUntilReset = 4;  // reset after number of tries
    if (beeType == XBEE_S6B_WIFI || beeType == XBEE3_LTEM3) {
      triesUntilReset = 9;
    }
    streamClear();  // Empty everything in the buffer before starting

    while (!success && triesMade < retries) {
      // Cannot send anything for 1 "guard time" before entering command mode
      // Default guard time is 1s, but the init fxn decreases it to 100 ms
      delay(guardTime + 10);
      stream.print(GF("+++"));  // enter command mode

      if (beeType != XBEE_S6B_WIFI && beeType != XBEE3_LTEM3) {
        res = waitResponse(guardTime * 2);
      } else {
        // S6B wait a full second for OK
        res = waitResponse();
      }

      success = (1 == res);
      if (0 == res) {
        triesUntilReset--;
        if (triesUntilReset == 0) {
          triesUntilReset = 4;
          pinReset();  // if it's unresponsive, reset
          delay(250);  // a short delay to allow it to come back up
          // TODO(SRGDamia1) optimize this
        }
        if (beeType == XBEE_S6B_WIFI) {
          delay(5000);  // WiFi module frozen, wait longer
        }
      }
      triesMade++;
    }

    if (success) {
      inCommandMode         = true;
      lastCommandModeMillis = millis();
    }
    return success;
  }

  /**
   * @brief Write changes to flash and apply them.
   * @return True if the changes were written and applied successfully, false
   * otherwise.
   */
  bool writeChanges(void) {
    sendAT(GF("WR"));  // Write changes to flash
    if (1 != waitResponse()) { return false; }
    sendAT(GF("AC"));  // Apply changes
    if (1 != waitResponse()) { return false; }
    return true;
  }

  /// Exit command mode
  void exitCommand(void) {
    // NOTE:  Here we explicitely try to exit command mode
    // even if the internal flag inCommandMode was already false
    sendAT(GF("CN"));  // Exit command mode
    waitResponse();
    inCommandMode = false;
  }

  /**
   * @brief Exit command mode and return false.
   *
   * This is a convenience function for use in functions that need to exit
   * command mode and return false on failure.
   *
   * @return False, always.
   */
  bool exitAndFail(void) {
    exitCommand();  // Exit command mode
    return false;
  }

  /// Get the series of the XBee module and store it internally
  void getSeries(void) {
    sendAT(GF("HS"));  // Get the "Hardware Series";
    int16_t intRes = readResponseInt();
    // if no response from module, then try again
    if (0xff == intRes) {
      sendAT(GF("HS"));  // Get the "Hardware Series";
      intRes = readResponseInt();
      if (0xff == intRes) {
        // Still no response, leave a known value - should reset
        intRes = XBEE_UNKNOWN;
      }
    }
    beeType = (XBeeType)intRes;
    DBG(GF("### Modem: "), getModemName(), beeType);
  }

  /**
   * @brief Reads a response from the XBee module as a string, waiting up to
   * timeout_ms milliseconds for data to become available.
   *
   * @param timeout_ms The maximum time to wait for a response, in milliseconds.
   * @return The response from the XBee module as a string.
   */
  String readResponseString(uint32_t timeout_ms = 1000) {
    TINY_GSM_YIELD();
    uint32_t startMillis = millis();
    while (!stream.available() && millis() - startMillis < timeout_ms) {}
    String res =
        stream.readStringUntil('\r');  // lines end with carriage returns
    res.trim();
    return res;
  }

  /**
   * @brief Reads a response from the XBee module as an integer, waiting up to
   * timeout_ms milliseconds for data to become available.
   *
   * @param timeout_ms The maximum time to wait for a response, in milliseconds.
   * @return The response from the XBee module as an integer.
   */
  int16_t readResponseInt(uint32_t timeout_ms = 1000) {
    String res = readResponseString(
        timeout_ms);  // it just works better reading a string first
    if (res == "") res = "FF";
    char buf[5] = {
        0,
    };
    res.toCharArray(buf, 5);
    int16_t intRes = strtol(buf, 0, 16);
    return intRes;
  }

  /**
   * @brief Send an AT command to the XBee module and read the response as a
   * string.
   *
   * @param cmd The AT command to send.
   * @return The response from the XBee module as a string.
   */
  String sendATGetString(GsmConstStr cmd) {
    XBEE_COMMAND_START_DECORATOR(5, "")
    sendAT(cmd);
    String res = readResponseString();
    XBEE_COMMAND_END_DECORATOR
    return res;
  }

  /**
   * @brief Sends an AT command to the XBee module and changes the setting if
   * needed.
   *
   * @param cmd The AT command to send.
   * @param newValue The new value to set.
   * @param timeout_ms The maximum time to wait for a response, in milliseconds.
   * @return True if the setting was changed successfully, false otherwise.
   */
  bool changeSettingIfNeeded(GsmConstStr cmd, int16_t newValue,
                             uint32_t timeout_ms = 1000L) {
    sendAT(cmd);
    if (readResponseInt() != newValue) {
      sendAT(cmd, newValue);
      // return false if we attempted to change but failed
      if (waitResponse(timeout_ms) != 1) { return false; }
      // check if we succeeded in staging a change and retry once
      sendAT(cmd);
      if (readResponseInt() != newValue) {
        sendAT(cmd, newValue);
        if (waitResponse(timeout_ms) != 1) { return false; }
      }
      // return true if we succeeded in staging a change
      return true;
    }
    // return false if no change is needed
    return false;
  }

  /// @copydoc changeSettingIfNeeded(GsmConstStr, int16_t, uint32_t)
  bool changeSettingIfNeeded(GsmConstStr cmd, String newValue,
                             uint32_t timeout_ms = 1000L) {
    sendAT(cmd);
    if (readResponseString() != newValue) {
      sendAT(cmd, newValue);
      // return false if we attempted to change but failed
      if (waitResponse(timeout_ms) != 1) { return false; }
      // check if we succeeded in staging a change and retry once
      sendAT(cmd);
      if (readResponseString() != newValue) {
        sendAT(cmd, newValue);
        if (waitResponse(timeout_ms) != 1) { return false; }
      }
      return true;
    }
    // return false if no change is needed
    return false;
  }

  /**
   * @brief Verifies if the saved host has a valid IP address.
   * @return True if the saved host has a valid IP address, false otherwise.
   */
  bool gotIPforSavedHost() {
    if (savedHost != "" && savedHostIP != IPAddress(0, 0, 0, 0))
      return true;
    else
      return false;
  }

 public:
  /// Stream used to communicate with the modem.
  Stream& stream;

 protected:
  GsmClientXBee* sockets[TinyGsmXBeeTcpConfig::kMuxCount];
  int16_t        guardTime;
  /// The type of XBee we're working with
  XBeeType beeType;
  int8_t   resetPin;
  /// The IP address we last requested
  IPAddress savedIP;
  /// The text name of the host we last requested a connection to
  String savedHost;
  /// The IP address of the saved host the last time we did an IP lookup
  IPAddress savedHostIP;
  // The last IP address we actually were connected to
  IPAddress savedOperatingIP;
  bool      inCommandMode;
  uint32_t  lastCommandModeMillis;
  uint32_t  lastHostLookupMillis;
};

// cspell:words LTEM

#endif  // SRC_TINYGSMCLIENTXBEE_H_
