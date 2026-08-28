/**
 * @file       TinyGsmClientESP8266NonOS.h
 * @brief      ESP8266 Non-OS modem client and modem-trait definitions.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */
/* clang-format off */
/**
 * @defgroup espressif_esp8266_nonos Espressif ESP8266 Non-OS Modem Family
 * @ingroup espressif_at
 * @brief Manufacturer: Espressif. Models: ESP8266 (Non-OS AT firmware, version
 * ~1.7.4).
 *
 * @warning This is an outdated modem firmware.  Please update your module to
 * the latest version.
 *
 * # Supported Public Functions
 *
 * - Basic functions (TinyGsmModem.tpp)
 *     - @ref TinyGsmModem<modemType, modemConfig>::begin "begin()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::init "init()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::sendAT "sendAT()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::setBaud "setBaud()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::setDefaultBaud "setDefaultBaud()"
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
 * - WiFi functions (TinyGsmWifi.tpp)
 *     - @ref TinyGsmWifi<modemType>::networkConnect "networkConnect()"
 *     - @ref TinyGsmWifi<modemType>::networkDisconnect "networkDisconnect()"
 * - TCP functions (TinyGsmTCP.tpp)
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::maintain "maintain()"
 *
 * # Connection Information
 *
 * - Combined TCP/SSL sockets:
 *   - 5
 *   - Using more than 1 SSL socket at a time may cause the module to crash.
 * - SSL contexts: 2
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
 *
 * @todo In `modemConnectImpl()`: Check mux
 * @todo In `handleURCs()`: deal with buffer overflow
 */
/* clang-format on */

#ifndef SRC_TINYGSMCLIENTESP8266NONOS_H_
#define SRC_TINYGSMCLIENTESP8266NONOS_H_
#pragma message("TinyGSM:  TinyGsmClientESP8266NonOS")

#include "TinyGsmClientEspressif.h"
#include "TinyGsmTCP.tpp"

// NOTE: This module supports SSL, but we do not support any certificate
// management for the non-OS version of the ESP8266 firmware, so we define
// TINY_GSM_MODEM_HAS_SSL here and do not include the SSL module so as not to
// waste space.
#ifndef TINY_GSM_MODEM_HAS_SSL
/// flag to indicate that the modem has Secure Socket Layer (SSL) functions
#define TINY_GSM_MODEM_HAS_SSL
#endif

/// Status of ESP8266 station interface
/// @ingroup espressif_esp8266_nonos
enum class ESP8266NonOSRegStatus {
  /// ESP8266 station is not initialized.
  REG_UNINITIALIZED = 0,
  /// ESP8266 station is initialized, but not started a Wi-Fi connection yet.
  REG_UNREGISTERED = 1,
  /// ESP8266 station connected to an AP and has obtained IP
  REG_OK_IP = 2,
  /// ESP8266 station created a TCP or UDP transmission
  REG_OK_TCP = 3,
  /// the TCP or UDP transmission of ESP8266 station disconnected
  REG_OK_NO_TCP = 4,
  /// ESP8266 station did NOT connect to an AP
  REG_DENIED = 5,
  /// ESP8266 station is in an unknown state
  REG_UNKNOWN = 6,
};

/// Basic modem configurations for the ESP8266NonOS modem family
/// @ingroup espressif_esp8266_nonos
struct TinyGsmESP8266NonOSModemConfig
    : public TinyGsmModemConfigPreset<ESP8266NonOSRegStatus> {
  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "Espressif";
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "ESP8266";
};

constexpr char TinyGsmESP8266NonOSModemConfig::MODEM_MANUFACTURER[]
    __attribute__((weak));
constexpr char TinyGsmESP8266NonOSModemConfig::MODEM_MODEL[]
    __attribute__((weak));

/**
 * @brief TCP behavior and limits for the ESP8266 (non OS AT version) family.
 *
 * NOTE: There's a total limit of 5 sockets, any of them can be SSL. BUT the
 * manual warns that module may not be able to handle more than 1 SSL socket at
 * a time.
 *
 * These modules don't have "SSL Contexts" per-say, but they only support 2
 * certificate sets.  The certificates are loaded and referenced by number.
 *
 * The ESP8266 devices can receive 2048 bytes and send 1460 bytes at most in a
 * single transmission.
 * @ingroup espressif_esp8266_nonos
 */
struct TinyGsmESP8266NonOSTcpConfig
    : public TinyGsmTcpConfigPreset<
          /*bufferMode*/ TinyGsmTcpBufferMode::NoModemBuffer,
          /*muxMode*/ TinyGsmTcpMuxMode::Static,
          /*muxCount*/ 5,
          /*sendMaxSize*/ 1460,
          /*connectTimeoutS*/ 75,  // default
          /*stopTimeoutS*/ 5> {};

/**
 * @brief Class for the Espressif ESP8266 modem, which is a Wi-Fi module with
 * SSL support.
 *
 * @warning This class is used to communicate with a module that has been
 * programmed with the non-OS based AT command firmware.  If you're using this,
 * please update your module.  It's quite outdated.
 * @ingroup espressif_esp8266_nonos
 */
class TinyGsmESP8266NonOS
    : public TinyGsmEspressif<TinyGsmESP8266NonOS,
                              TinyGsmESP8266NonOSModemConfig>,
      public TinyGsmTCP<TinyGsmESP8266NonOS, TinyGsmESP8266NonOSTcpConfig> {
  friend class TinyGsmEspressif<TinyGsmESP8266NonOS,
                                TinyGsmESP8266NonOSModemConfig>;
  friend class TinyGsmModem<TinyGsmESP8266NonOS,
                            TinyGsmESP8266NonOSModemConfig>;
  friend class TinyGsmWifi<TinyGsmESP8266NonOS>;
  friend class TinyGsmTCP<TinyGsmESP8266NonOS, TinyGsmESP8266NonOSTcpConfig>;
  friend class GsmClient<TinyGsmESP8266NonOS, TinyGsmESP8266NonOSTcpConfig>;

 public:
  using ModemConfig = TinyGsmESP8266NonOSModemConfig;
  using TcpConfig   = TinyGsmESP8266NonOSTcpConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  /// @ingroup espressif_esp8266_nonos
  class GsmClientESP8266NonOS
      : public GsmClient<TinyGsmESP8266NonOS, TinyGsmESP8266NonOSTcpConfig> {
    friend class TinyGsmESP8266NonOS;

   public:
    using GsmClient<TinyGsmESP8266NonOS, TinyGsmESP8266NonOSTcpConfig>::connect;
    using GsmClient<TinyGsmESP8266NonOS, TinyGsmESP8266NonOSTcpConfig>::stop;
    using TcpConfig = TinyGsmESP8266NonOSTcpConfig;

    /**
     * @brief Create a new TCP client.
     * @warning You must call the init() method before attempting to use a
     * client created with this constructor.
     */
    GsmClientESP8266NonOS() {
      is_secure = false;
    }
    /**
     * @brief Create a new TCP client and bind it to a modem and optionally a
     * multiplexing channel.
     * @param modem Modem instance used by this client.
     * @param mux Multiplexing channel to use.
     *
     * @note The ESP8266 allows you choose the multiplexing channel number, but
     * if the input mux channel number is already in use and other mux channels
     * are available, this library will select the next available one.  Use the
     * getMux() function to get the assigned multiplexing channel number after a
     * successful connection.
     */
    explicit GsmClientESP8266NonOS(TinyGsmESP8266NonOS& modem, uint8_t mux = 0)
        : GsmClient<TinyGsmESP8266NonOS, TinyGsmESP8266NonOSTcpConfig>(modem,
                                                                       mux) {
      is_secure = false;
      init(&modem, mux);
    }

    /**
     * @brief Initialize the TCP client with a modem and optionally a
     * multiplexing channel.
     * @return true if initialization was successful, false otherwise.
     * @copydetails GsmClientESP8266NonOS::GsmClientESP8266NonOS(
     * TinyGsmESP8266NonOS&, uint8_t)
     */
    bool init(TinyGsmESP8266NonOS* modem, uint8_t mux = 0) {
      if (modem == nullptr) { return false; }
      this->at       = modem;
      sock_connected = false;
      is_mid_send    = false;

      // The ESP8266 (as supported) generally lets you choose the mux number,
      // but we want to try to find an empty place in the socket array for it.

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
 public:
  /// Inner secure client
  /// @ingroup espressif_esp8266_nonos
  class GsmClientSecureESP8266NonOS : public GsmClientESP8266NonOS {
    friend class TinyGsmESP8266NonOS;

   public:
    using GsmClientESP8266NonOS::connect;
    using GsmClientESP8266NonOS::stop;
    using TcpConfig = TinyGsmESP8266NonOSTcpConfig;

    /**
     * @brief Create a new secured TCP (SSL) client.
     * @warning You must call the init() method before attempting to use a
     * client created with this constructor.
     */
    GsmClientSecureESP8266NonOS() {
      is_secure = true;
    }
    /**
     * @brief Create a new secured TCP (SSL) client and bind it to a modem and
     * optionally a multiplexing channel.
     * @copydetails GsmClientESP8266NonOS::GsmClientESP8266NonOS(
     * TinyGsmESP8266NonOS&, uint8_t)
     */
    explicit GsmClientSecureESP8266NonOS(TinyGsmESP8266NonOS& modem,
                                         uint8_t              mux = 0)
        : GsmClientESP8266NonOS(modem, mux) {
      is_secure = true;
    }
  };

  /*
   * GSM Modem Constructor
   */
 public:
  /**
   * @brief Construct a modem wrapper around a stream transport.
   * @param stream Stream used to communicate with the modem.
   */
  explicit TinyGsmESP8266NonOS(Stream& stream)
      : TinyGsmEspressif<TinyGsmESP8266NonOS, TinyGsmESP8266NonOSModemConfig>(
            stream) {
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
  // Light sleep does not work properly on the old ESP8266 non-OS AT firmware,
  // so we don't support it here.
  bool sleepEnableImpl(bool enable) TINY_GSM_ATTR_NOT_AVAILABLE;

  /*
   * Generic network functions
   */
 protected:
  ESP8266NonOSRegStatus getRegistrationStatusImpl() {
    sendAT(GF("+CIPSTATUS"));
    if (waitResponse(3000, GF("STATUS:")) != 1)
      return ESP8266NonOSRegStatus::REG_UNKNOWN;
    // after "STATUS:" it should return the status number (0,1,2,3,4,5),
    // followed by an OK
    // Since there are more possible status number codes than the arguments for
    // waitResponse, we'll capture the response in a string and then parse it.
    String res;
    if (waitResponse(3000L, res) != 1) {
      return ESP8266NonOSRegStatus::REG_UNKNOWN;
    }
    res.trim();
    int8_t status = res.toInt();
    return static_cast<ESP8266NonOSRegStatus>(status);
  }

  bool isNetworkConnectedImpl() {
    ESP8266NonOSRegStatus s = this->getRegistrationStatus();
    if (s == ESP8266NonOSRegStatus::REG_OK_IP ||
        s == ESP8266NonOSRegStatus::REG_OK_TCP) {
      // with these, we're definitely connected
      return true;
    } else if (s == ESP8266NonOSRegStatus::REG_OK_NO_TCP) {
      // with this, we may or may not be connected
      if (getLocalIP() == "") {
        return false;
      } else {
        return true;
      }
    } else {
      return false;
    }
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // No SSL functions are supported on the ESP8266 using the non-OS AT firmware.


  /*
   * Wifi functions
   */
 protected:
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
   * Client-related functions
   */
 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t /*static*/ mux,
                        int timeout_s) {
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    bool     ssl        = sockets[mux]->is_secure;
    if (ssl) {
      sendAT(GF("+CIPSSLCCONF="), mux, ',', '0');
      if (waitResponse() != 1) {
        sendAT(GF("+CIPSSLSIZE=4096"));
        waitResponse();
      }
    }
    sendAT(GF("+CIPSTART="), mux, ',', ssl ? GF("\"SSL") : GF("\"TCP"),
           GF("\",\""), host, GF("\","), port
#if defined(TINY_GSM_TCP_KEEP_ALIVE)
           ,
           ',', TINY_GSM_TCP_KEEP_ALIVE
#endif
    );
    // TODO(?): Check mux
    int8_t rsp = waitResponse(timeout_ms, GFP(ModemConfig::GSM_OK),
                              GFP(ModemConfig::GSM_ERROR),
                              GF("ALREADY CONNECT"));
    if (rsp == 3) waitResponse();
    // May return "ERROR" after the "ALREADY CONNECT"
    return (1 == rsp);
  }

  bool modemGetConnectedImpl(uint8_t mux) {
    sendAT(GF("+CIPSTATUS"));
    if (waitResponse(3000, GF("STATUS:")) != 1) { return false; }
    // after "STATUS:" it should return the status number (0,1,2,3,4,5),
    // followed by an OK
    // Hopefully we'll catch the "3" here, but fall back to the OK or Error
    int8_t status = waitResponse(GF("3"), GFP(ModemConfig::GSM_OK),
                                 GFP(ModemConfig::GSM_ERROR));
    // if the status is anything but 3, there are no connections open
    if (status != 1) {
      for (uint8_t muxNo = 0; muxNo < TcpConfig::kMuxCount; muxNo++) {
        if (sockets[muxNo]) { sockets[muxNo]->sock_connected = false; }
      }
      return false;
    }
    // initialize the connection array assuming no connections are active
    bool verified_connections[TcpConfig::kMuxCount] = {0};
    for (uint8_t muxNo = 0; muxNo < TcpConfig::kMuxCount; muxNo++) {
      uint8_t has_status = waitResponse(GF("+CIPSTATUS:"),
                                        GFP(ModemConfig::GSM_OK),
                                        GFP(ModemConfig::GSM_ERROR));
      if (has_status == 1) {
        int16_t returned_mux = streamGetIntBefore(',');
        streamSkipUntil(',');   // Skip mux (?)
        streamSkipUntil(',');   // Skip type
        streamSkipUntil(',');   // Skip remote IP
        streamSkipUntil(',');   // Skip remote port
        streamSkipUntil(',');   // Skip local port
        streamSkipUntil('\n');  // Skip client/server type
        if (returned_mux >= 0 && returned_mux < TcpConfig::kMuxCount) {
          verified_connections[returned_mux] = 1;
        }
      }
      if (has_status == 2) break;  // once we get to the ok, stop
    }
    for (uint8_t muxNo = 0; muxNo < TcpConfig::kMuxCount; muxNo++) {
      if (sockets[muxNo]) {
        sockets[muxNo]->sock_connected = verified_connections[muxNo];
      }
    }
    return verified_connections[mux];
  }

  // Disambiguate modemStopImpl by using the Espressif implementation
  using TinyGsmEspressif<TinyGsmESP8266NonOS,
                         TinyGsmESP8266NonOSModemConfig>::modemStopImpl;

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    sendAT(GF("+CIPSEND="), mux, ',', len);
    return waitResponse(GF(">")) == 1;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t) {
    if (waitResponse(30000L, GF("SEND OK\r\n"), GF("SEND FAIL\r\n"),
                     GFP(ModemConfig::GSM_ERROR)) != 1) {
      return 0;
    }
    return len;
  }

  /*
   * Utilities
   */
 protected:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+IPD,"))) {
      int16_t mux          = streamGetIntBefore(',');
      int16_t len_reported = streamGetIntBefore(':');
      int16_t len          = len_reported;
      if (isValidMux(mux)) {
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
      int16_t muxStart =
          TinyGsmMax(0,
                     data.lastIndexOf(String(GFP(ModemConfig::GSM_NL)),
                                      data.length() - 8));
      int16_t coma = data.indexOf(',', muxStart);
      int16_t mux  = data.substring(muxStart, coma).toInt();
      if (isValidMux(mux)) { sockets[mux]->sock_connected = false; }
      streamSkipUntil('\n');  // throw away the new line
      data = "";
      DBG("### Closed: ", mux);
      return true;
    } else if (data.endsWith(GF("busy p..."))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Busy, please wait");
      return true;
    } else if (data.endsWith(GF("ready\r\n"))) {
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
  GsmClientESP8266NonOS* sockets[TcpConfig::kMuxCount];
};

#endif  // SRC_TINYGSMCLIENTESP8266NONOS_H_
