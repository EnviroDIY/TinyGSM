/**
 * @file       TinyGsmClientUBLOX.h
 * @brief      u-blox modem client and modem-trait definitions.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 *
 * @defgroup ublox_cellular u-blox Cellular Modem Family
 * @brief Manufacturer: u-blox. Models: cellular modules supported by
 * TinyGsmUBLOX.
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
 * - Phone call functions (TinyGsmCalling.tpp)
 *     - @ref TinyGsmCalling<modemType>::callAnswer "callAnswer()"
 *     - @ref TinyGsmCalling<modemType>::callNumber "callNumber()"
 *     - @ref TinyGsmCalling<modemType>::callHangup "callHangup()"
 *     - @ref TinyGsmCalling<modemType>::dtmfSend "dtmfSend()"
 * - Text messaging (SMS) functions (TinyGsmSMS.tpp)
 *     - @ref TinyGsmSMS<modemType>::sendUSSD "sendUSSD()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS "sendSMS()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS_UTF16 "sendSMS_UTF16()"
 * - GSM location functions (TinyGsmGSMLocation.tpp)
 *     - @ref TinyGsmGSMLocation<modemType>::getGsmLocationRaw "getGsmLocationRaw()"
 *     - @ref TinyGsmGSMLocation<modemType>::getGsmLocation "getGsmLocation()"
 *     - @ref TinyGsmGSMLocation<modemType>::getGsmLocationTime "getGsmLocationTime()"
 * - GPS (GNSS, GLONASS) functions (TinyGsmGPS.tpp)
 *     - @ref TinyGsmGPS<modemType>::enableGPS "enableGPS()"
 *     - @ref TinyGsmGPS<modemType>::disableGPS "disableGPS()"
 *     - @ref TinyGsmGPS<modemType>::getGPSraw "getGPSraw()"
 *     - @ref TinyGsmGPS<modemType>::getGPS "getGPS()"
 *     - @ref TinyGsmGPS<modemType>::getGPSTime "getGPSTime()"
 * - Time functions (TinyGsmTime.tpp)
 *     - @ref TinyGsmTime<modemType>::getGSMDateTime "getGSMDateTime()"
 *     - @ref TinyGsmTime<modemType>::getNetworkTime "getNetworkTime()"
 * - Battery functions (TinyGsmBattery.tpp)
 *     - @ref TinyGsmBattery<modemType>::getBattPercent "getBattPercent()"
 *     - @ref TinyGsmBattery<modemType>::getBattStats "getBattStats()"
 * - Generic network functions
 *     - @ref TinyGsmUBLOX::setRadioAccessTechnology "setRadioAccessTechnology()"
 *     - @ref TinyGsmUBLOX::getCurrentRadioAccessTechnology "getCurrentRadioAccessTechnology()"
 *
 * # Connection Information
 *
 * - Combined TCP/SSL sockets:
 *   - 7
 *   - a few u-blox modules support more, but this library does not
 * - Socket Buffering:
 *   - The modem has an internal buffer for incoming data.
 *   - This gives you leeway to pull data from the buffer as needed with less
 * risk of losing data.
 * - Socket Numbering:
 *   - The modem does not allow you to specify the multiplexing channel.
 *   - The modem will automatically assign a channel when the client connects to
 * a server.
 *   - Use the getMux() function to get the assigned multiplexing channel number
 * after a successful connection.
 */
#ifndef SRC_TINYGSMCLIENTUBLOX_H_
#define SRC_TINYGSMCLIENTUBLOX_H_
#pragma message("TinyGSM:  TinyGsmClientUBLOX")

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"

// NOTE: This module supports SSL, but we do not support any certificate
// management yet. TINY_GSM_MODEM_HAS_SSL here and do no include the SSL module
// so as not to waste space.
#ifndef TINY_GSM_MODEM_HAS_SSL
/// flag to indicate that the modem has Secure Socket Layer (SSL) functions
#define TINY_GSM_MODEM_HAS_SSL
#endif

#include "TinyGsmGPRS.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmGSMLocation.tpp"
#include "TinyGsmGPS.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmBattery.tpp"

/// Registration status
/// @ingroup ublox_cellular
enum UBLOXRegStatus {
  REG_NO_RESULT    = -1,  ///< No registration result
  REG_UNREGISTERED = 0,   ///< Not registered on the network
  REG_SEARCHING    = 2,   ///< Searching for network
  REG_DENIED       = 3,   ///< Registration denied
  REG_OK_HOME      = 1,   ///< Registered on the home network
  REG_OK_ROAMING   = 5,   ///< Registered on a roaming network
  REG_UNKNOWN      = 4,   ///< Unknown registration status
};

/// Basic modem configurations for the UBLOX modem family
/// @ingroup ublox_cellular
struct TinyGsmUBLOXModemConfig
    : public TinyGsmModemConfigPreset<UBLOXRegStatus> {
  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "u-blox";
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "unknown";
};

constexpr char TinyGsmUBLOXModemConfig::MODEM_MANUFACTURER[];
constexpr char TinyGsmUBLOXModemConfig::MODEM_MODEL[];

/**
 * @brief TCP behavior and limits for the u-blox family.
 *
 * Most modules using this protocol support 7 sockets, the LEON-G1 supports 16.
 * I **think** all sockets can be SSL, but the manual is not clear (to me).
 *
 * Also supports 5 SSL contexts (0-4).
 * The SSL context is collection of SSL settings, not the connection identifier.
 *
 * @warning You cannot control the socket mux number on this module! The module
 * opens the connection and returns the connection number it opened.
 *
 * The send data command, USOWR, accepts up to 1024 bytes in "normal" and
 * "binary extended" modes and up to 512 bytes in "HEX" mode.
 * @ingroup ublox_cellular
 */
struct TinyGsmUBLOXTcpConfig
    : public TinyGsmTcpConfigPreset<
          /*bufferMode*/ TinyGsmTcpBufferMode::BufferReadAndCheckSize,
          /*muxMode*/ TinyGsmTcpMuxMode::Dynamic,
          /*muxCount*/ 7,
          /*sendMaxSize*/ 1024,
          /*connectTimeoutS*/ 120> {};

/// Class for the u-blox family of modems
/// @ingroup ublox_cellular
class TinyGsmUBLOX : public TinyGsmModem<TinyGsmUBLOX, TinyGsmUBLOXModemConfig>,
                     public TinyGsmGPRS<TinyGsmUBLOX>,
                     public TinyGsmTCP<TinyGsmUBLOX, TinyGsmUBLOXTcpConfig>,
                     public TinyGsmCalling<TinyGsmUBLOX>,
                     public TinyGsmSMS<TinyGsmUBLOX>,
                     public TinyGsmGSMLocation<TinyGsmUBLOX>,
                     public TinyGsmGPS<TinyGsmUBLOX>,
                     public TinyGsmTime<TinyGsmUBLOX>,
                     public TinyGsmBattery<TinyGsmUBLOX> {
  friend class TinyGsmModem<TinyGsmUBLOX, TinyGsmUBLOXModemConfig>;
  friend class TinyGsmGPRS<TinyGsmUBLOX>;
  friend class TinyGsmTCP<TinyGsmUBLOX, TinyGsmUBLOXTcpConfig>;
  friend class GsmClient<TinyGsmUBLOX, TinyGsmUBLOXTcpConfig>;
  friend class TinyGsmCalling<TinyGsmUBLOX>;
  friend class TinyGsmSMS<TinyGsmUBLOX>;
  friend class TinyGsmGSMLocation<TinyGsmUBLOX>;
  friend class TinyGsmGPS<TinyGsmUBLOX>;
  friend class TinyGsmTime<TinyGsmUBLOX>;
  friend class TinyGsmBattery<TinyGsmUBLOX>;

  using ModemConfig = TinyGsmUBLOXModemConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  /// @ingroup ublox_cellular
  class GsmClientUBLOX : public GsmClient<TinyGsmUBLOX, TinyGsmUBLOXTcpConfig> {
    friend class TinyGsmUBLOX;

   public:
    using GsmClient<TinyGsmUBLOX, TinyGsmUBLOXTcpConfig>::connect;
    using GsmClient<TinyGsmUBLOX, TinyGsmUBLOXTcpConfig>::stop;

    /**
     * @brief Create a new TCP client.  This must be initialized with a modem
     * before it can be used.
     */
    GsmClientUBLOX() {
      is_secure = false;
    }
    /**
     * @brief Create a new TCP client and bind it to a modem.
     * @param modem Modem instance used by this client.
     *
     * @note The u-blox modules do not allow you to specify the multiplexing
     * channel.  The modem will automatically assign a channel when the client
     * connects to a server.  Use the getMux() function to get the assigned
     * multiplexing channel number after a successful connection.
     */
    explicit GsmClientUBLOX(TinyGsmUBLOX& modem, uint8_t /*mux*/ = 0) {
      init(&modem, -1);
      is_secure = false;
    }

    /**
     * @brief Initialize the TCP client with a modem.
     * @return true if initialization was successful, false otherwise.
     * @copydetails GsmClientUBLOX::GsmClientUBLOX(TinyGsmUBLOX&, uint8_t)
     */
    bool init(TinyGsmUBLOX* modem, uint8_t /*mux*/ = 0) {
      this->at       = modem;
      sock_available = 0;
      prev_check     = 0;
      sock_connected = false;
      got_data       = false;
      is_mid_send    = false;
      uint8_t mux    = 0;  // only a placeholder!

      // These ublox modules do NOT allow you to choose the mux number; this is
      // an initial place holder for before connection. We need to assign a mux
      // number here first so that we can assign the pointer for the client in
      // the socket array and in-turn allow the modem to look back at the
      // properties of the client to check if the client needs SSL and, if so,
      // what the SSL specs are. If the mux number returned at the end of the
      // connection process is different from the one we assigned here, we
      // update the position of the pointer to this in the socket array after
      // the connection finishes.

      // NOTE: Because we can't control the mux number, we don't need to worry
      // about attempting to assign a mux number for a secure socket that is out
      // of range of because the number of secure sockets is smaller than the
      // number of standard sockets.

      // if it's a valid mux number, and that mux number isn't in use (or it's
      // already this), accept the mux number
      if (mux < TinyGsmUBLOXTcpConfig::kMuxCount &&
          (at->sockets[mux] == nullptr || at->sockets[mux] == this)) {
        this->mux = mux;
        // If the mux number is in use or out of range, find the next available
        // one
      } else if (at->findFirstUnassignedMux() != static_cast<uint8_t>(-1)) {
        this->mux = at->findFirstUnassignedMux();
      } else {
        // If we can't find anything available, overwrite something, using mod
        // to make sure we're in range
        this->mux = (mux % TinyGsmUBLOXTcpConfig::kMuxCount);
      }
      at->sockets[this->mux] = this;

      return true;
    }

   public:
    int connect(const char* host, uint16_t port, int timeout_s) override {
      is_mid_send = false;
      // stop();  // DON'T stop! We don't know our actual mux yet!
      TINY_GSM_YIELD();
      rx.clear();

      uint8_t oldMux = mux;
      sock_connected = at->modemConnect(host, port, &mux, timeout_s);
      if (mux != oldMux) {
        DBG(GF("###  Mux number changed from"), oldMux, GF("to"), mux);
        if (!(mux < TinyGsmUBLOXTcpConfig::kMuxCount &&
              (at->sockets[mux] == nullptr || at->sockets[mux] == this))) {
          DBG(GF("WARNING: This new mux number had already been assigned to a "
                 "different client, attempting to move it!"));
          uint8_t next_empty_mux = at->findFirstUnassignedMux();
          if (next_empty_mux != static_cast<uint8_t>(-1)) {
            DBG(GF("### Socket previously assigned as"), mux, GF("moved to"),
                next_empty_mux);
            at->sockets[next_empty_mux] = at->sockets[mux];
          } else {
            DBG(GF("WARNING: Failed to move socket, it will be overwritten!"));
          }
        }
        at->sockets[oldMux] = nullptr;
      }
      at->sockets[mux] = this;
      at->maintain();

      return sock_connected;
    }

    void stop(uint32_t maxWaitMs) override {
      is_mid_send = false;
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+USOCL="), mux);
      at->waitResponse();  // should return within 1s
      sock_connected = false;
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
 public:
  /// Inner secure client
  /// @ingroup ublox_cellular
  class GsmClientSecureUBLOX : public GsmClientUBLOX {
    friend class TinyGsmUBLOX;

   public:
    using GsmClientUBLOX::connect;
    using GsmClientUBLOX::stop;

    /**
     * @brief Create a new secured TCP (SSL) client.  This must be initialized
     * with a modem before it can be used.
     */
    GsmClientSecureUBLOX() {
      is_secure = true;
    }
    /**
     * @brief Create a new secured TCP (SSL) client and bind it to a modem.
     * @copydetails GsmClientUBLOX::GsmClientUBLOX(TinyGsmUBLOX&, uint8_t)
     */
    explicit GsmClientSecureUBLOX(TinyGsmUBLOX& modem, uint8_t /*mux*/ = 0)
        : GsmClientUBLOX(modem) {
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
  explicit TinyGsmUBLOX(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientUBLOX"));

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

    // Enable automatic time zome update
    sendAT(GF("+CTZU=1"));
    waitResponse(10000L);
    // Ignore the response, in case the network doesn't support it.
    // if (waitResponse(10000L) != 1) { return false; }

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

  // only difference in implementation is the warning on the wrong type
  String getModemNameImpl() {
    String manufacturer = getModemManufacturer();
    String model        = getModemModel();
    String name         = manufacturer + String(" ") + model;
    if (name.startsWith("u-blox SARA-R4") ||
        name.startsWith("u-blox SARA-N4")) {
      DBG("### WARNING:  You are using the wrong TinyGSM modem!");
    } else if (name.startsWith("u-blox SARA-N2")) {
      DBG("### SARA N2 NB-IoT modems not supported!");
    }
    return name;
  }

  bool factoryDefaultImpl() {
    sendAT(GF("+UFACTORY=0,1"));  // No factory restore, erase NVM
    waitResponse();
    return setPhoneFunctionality(16);  // Reset
  }

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin) {
    if (!testAT()) { return false; }
    if (!setPhoneFunctionality(16)) { return false; }
    delay(3000);  // TODO(?):  Verify delay timing here
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+CPWROFF"));
    return waitResponse(40000L) == 1;
  }

  bool sleepEnableImpl(bool enable) TINY_GSM_ATTR_NOT_AVAILABLE;

  bool setPhoneFunctionalityImpl(uint8_t fun, bool reset) {
    sendAT(GF("+CFUN="), fun, reset ? ",1" : "");
    return waitResponse(10000L) == 1;
  }

  /*
   * Generic network functions
   */
 public:
  /**
   * @brief Set the radio access technology (RAT) for the modem.
   *
   * Possible values for selected are:
   * - 0: GSM / GPRS / eGPRS (single mode)
   * - 1: GSM / UMTS (dual mode)
   * - 2: UMTS (single mode)
   * - 3: LTE (single mode)
   * - 4: GSM / UMTS / LTE (tri mode)
   * - 5: GSM / LTE (dual mode)
   * - 6: UMTS / LTE (dual mode)
   *
   * Possible values for preferred are:
   * - 0: GSM / GPRS / eGPRS
   * - 2: UTRAN
   * - 3: LTE
   *
   * @param selected The selected RAT mode.
   * @param preferred The preferred RAT mode.
   * @return True if the command was successful, false otherwise.
   */
  bool setRadioAccessTechnology(int selected, int preferred) {
    sendAT(GF("+URAT="), selected, ',', preferred);
    if (waitResponse() != 1) { return false; }
    return true;
  }

  /**
   * @brief Get the current radio access technology (RAT) of the modem.
   * @param rat A reference to an integer to store the current RAT mode.
   * @return True if the command was successful, false otherwise.
   */
  bool getCurrentRadioAccessTechnology(int& rat) {
    sendAT(GF("+URAT?"));
    if (waitResponse(10000L, GF("+URAT:")) != 1) { return false; }

    int16_t parsedRat = streamGetIntBefore('\n');
    if (waitResponse() != 1 || parsedRat == -9999) { return false; }

    rat = parsedRat;
    return true;
  }

 protected:
  UBLOXRegStatus getRegistrationStatusImpl() {
    return (UBLOXRegStatus)getRegistrationStatusXREG("CGREG");
  }

  bool isNetworkConnectedImpl() {
    UBLOXRegStatus s = this->getRegistrationStatus();
    if (s == REG_OK_HOME || s == REG_OK_ROAMING)
      return true;
    else if (s == REG_UNKNOWN)  // for some reason, it can hang at unknown..
      return isGprsConnected();
    else
      return false;
  }

  String getLocalIPImpl() {
    sendAT(GF("+UPSND=0,0"));
    if (waitResponse(GF("+UPSND:")) != 1) { return ""; }
    streamSkipUntil(',');   // Skip PSD profile
    streamSkipUntil('\"');  // Skip request type
    String res = stream.readStringUntil('\"');
    if (waitResponse() != 1) { return ""; }
    return res;
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // This module supports SSL, but there are no certificate management functions
  // are supported yet.
  // If you wish to add certificate management for this module you must (in
  // addition to adding the functions here):
  //  - Add `#include "TinyGsmSSL.tpp` to the top of the file
  //  - Remove `#define TINY_GSM_MODEM_HAS_SSL` from the top of the file
  //  - Add `public TinyGsmSSL<TinyGsmUBLOX>,` to the constructor's initializer
  //  list
  //  - Add `friend class TinyGsmSSL<TinyGsmUBLOX>;` to the friend list
  //  - Make the secure client inherit from the secure client class in the SSL
  //  template.

  /*
   * Wifi functions
   */
  // No functions of this type supported

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* user, const char* pwd) {
    // gprsDisconnect();

    sendAT(GF("+CGATT=1"));  // attach to GPRS
    if (waitResponse(360000L) != 1) { return false; }

    // Setting up the PSD profile/PDP context with the UPSD commands sets up an
    // "internal" PDP context, i.e. a data connection using the internal IP
    // stack and related AT commands for sockets.

    // Packet switched data configuration
    // AT+UPSD=<profile_id>,<param_tag>,<param_val>
    // profile_id = 0 - PSD profile identifier, in range 0-6 (NOT PDP context)
    // param_tag = 1: APN
    // param_tag = 2: username
    // param_tag = 3: password
    // param_tag = 7: IP address Note: IP address set as "0.0.0.0" means
    //    dynamic IP address assigned during PDP context activation
    sendAT(GF("+UPSD=0,1,\""), apn, '"');  // Set APN for PSD profile 0
    waitResponse();

    if (user && strlen(user) > 0) {
      sendAT(GF("+UPSD=0,2,\""), user, '"');  // Set user for PSD profile 0
      waitResponse();
    }
    if (pwd && strlen(pwd) > 0) {
      sendAT(GF("+UPSD=0,3,\""), pwd, '"');  // Set password for PSD profile 0
      waitResponse();
    }

    sendAT(GF("+UPSD=0,7,\"0.0.0.0\""));  // Dynamic IP on PSD profile 0
    waitResponse();

    // Packet switched data action
    // AT+UPSDA=<profile_id>,<action>
    // profile_id = 0: PSD profile identifier, in range 0-6 (NOT PDP context)
    // action = 3: activate; it activates a PDP context with the specified
    // profile, using the current parameters
    sendAT(GF(
        "+UPSDA=0,3"));  // Activate the PDP context associated with profile 0
    if (waitResponse(360000L) != 1) {  // Should return ok
      return false;
    }

    // Packet switched network-assigned data - Returns the current (dynamic)
    // network-assigned or network-negotiated value of the specified parameter
    // for the active PDP context associated with the specified PSD profile.
    // AT+UPSND=<profile_id>,<param_tag>
    // profile_id = 0: PSD profile identifier, in range 0-6 (NOT PDP context)
    // param_tag = 8: PSD profile status: if the profile is active the return
    // value is 1, 0 otherwise
    sendAT(GF("+UPSND=0,8"));  // Check if PSD profile 0 is now active
    int8_t res = waitResponse(GF(",8,1"), GF(",8,0"));
    waitResponse();  // Should return another OK
    if (res == 1) {
      return true;          // It's now active
    } else if (res == 2) {  // If it's not active yet, wait for the +UUPSDA URC
      if (waitResponse(180000L, GF("+UUPSDA: 0")) != 1) {  // 0=successful
        return false;
      }
      streamSkipUntil('\n');  // Ignore the IP address, if returned
    } else {
      return false;
    }

    return true;
  }

  bool gprsDisconnectImpl() {
    sendAT(GF(
        "+UPSDA=0,4"));  // Deactivate the PDP context associated with profile 0
    if (waitResponse(360000L) != 1) { return false; }

    sendAT(GF("+CGATT=0"));  // detach from GPRS
    if (waitResponse(360000L) != 1) { return false; }

    return true;
  }

  /*
   * SIM card functions
   */
 protected:
  // This uses "CGSN" instead of "GSN"
  String getIMEIImpl() {
    sendAT(GF("+CGSN"));
    if (waitResponse(GFP(ModemConfig::GSM_NL)) != 1) { return ""; }
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

  /*
   * GSM/GPS/GNSS/GLONASS Location functions
   * NOTE:  u-blox modules use the same function to get location data from both
   * GSM tower triangulation and from dedicated GPS/GNSS/GLONASS receivers.  The
   * only difference in which sensor the data is requested from.  If a GNSS
   * location is requested from a modem without a GNSS receiver installed on the
   * I2C port, the GSM-based "Cell Locate" location will be returned instead.
   */
 protected:
  bool enableGPSImpl() {
    // AT+UGPS=<mode>[,<aid_mode>[,<GNSS_systems>]]
    // <mode> - 0: GNSS receiver powered off, 1: on
    // <aid_mode> - 0: no aiding (default)
    // <GNSS_systems> - 3: GPS + SBAS (default)
    sendAT(GF("+UGPS=1,0,3"));
    if (waitResponse(10000L, GF("+UGPS:")) != 1) { return false; }
    return waitResponse(10000L) == 1;
  }
  bool disableGPSImpl() {
    sendAT(GF("+UGPS=0"));
    if (waitResponse(10000L, GF("+UGPS:")) != 1) { return false; }
    return waitResponse(10000L) == 1;
  }
  String inline getUbloxLocationRaw(int8_t sensor) {
    // AT+ULOC=<mode>,<sensor>,<response_type>,<timeout>,<accuracy>
    // <mode> - 2: single shot position
    // <sensor> - 0: use the last fix in the internal database and stop the GNSS
    //          receiver
    //          - 1: use the GNSS receiver for localization
    //          - 2: use cellular CellLocate location information
    //          - 3: ?? use the combined GNSS receiver and CellLocate service
    //          information ?? - Docs show using sensor 3 and it's
    //          documented for the +UTIME command but not for +ULOC
    // <response_type> - 0: standard (single-hypothesis) response
    // <timeout> - Timeout period in seconds
    // <accuracy> - Target accuracy in meters (1 - 999999)
    sendAT(GF("+ULOC=2,"), sensor, GF(",0,120,1"));
    // wait for first "OK"
    if (waitResponse(10000L) != 1) { return ""; }
    // wait for the final result - wait full timeout time
    if (waitResponse(120000L, GF("+UULOC:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }
  String getGsmLocationRawImpl() {
    return getUbloxLocationRaw(2);
  }
  String getGPSrawImpl() {
    return getUbloxLocationRaw(1);
  }

  inline bool getUbloxLocation(int8_t sensor, float* lat, float* lon,
                               float* speed = 0, float* alt = 0, int* vsat = 0,
                               int* usat = 0, float* accuracy = 0,
                               int* year = 0, int* month = 0, int* day = 0,
                               int* hour = 0, int* minute = 0,
                               int* second = 0) {
    // AT+ULOC=<mode>,<sensor>,<response_type>,<timeout>,<accuracy>
    // <mode> - 2: single shot position
    // <sensor> - 2: use cellular CellLocate location information
    //          - 0: use the last fix in the internal database and stop the GNSS
    //          receiver
    //          - 1: use the GNSS receiver for localization
    //          - 3: ?? use the combined GNSS receiver and CellLocate service
    //          information ?? - Docs show using sensor 3 and it's documented
    //          for the +UTIME command but not for +ULOC
    // <response_type> - 0: standard (single-hypothesis) response
    // <timeout> - Timeout period in seconds
    // <accuracy> - Target accuracy in meters (1 - 999999)
    sendAT(GF("+ULOC=2,"), sensor, GF(",0,120,1"));
    // wait for first "OK"
    if (waitResponse(10000L) != 1) { return false; }
    // wait for the final result - wait full timeout time
    if (waitResponse(120000L, GF("+UULOC: ")) != 1) { return false; }

    // +UULOC: <date>, <time>, <lat>, <long>, <alt>, <uncertainty>, <speed>,
    // <direction>, <vertical_acc>, <sensor_used>, <SV_used>, <antenna_status>,
    // <jamming_status>

    // init variables
    float ilat         = 0;
    float ilon         = 0;
    float ispeed       = 0;
    float ialt         = 0;
    int   iusat        = 0;
    float iaccuracy    = 0;
    int   iyear        = 0;
    int   imonth       = 0;
    int   iday         = 0;
    int   ihour        = 0;
    int   imin         = 0;
    float secondWithSS = 0;

    // Date & Time
    iday         = streamGetIntBefore('/');    // Two digit day
    imonth       = streamGetIntBefore('/');    // Two digit month
    iyear        = streamGetIntBefore(',');    // Four digit year
    ihour        = streamGetIntBefore(':');    // Two digit hour
    imin         = streamGetIntBefore(':');    // Two digit minute
    secondWithSS = streamGetFloatBefore(',');  // 6 digit second with subseconds

    ilat = streamGetFloatBefore(',');  // Estimated latitude, in degrees
    ilon = streamGetFloatBefore(',');  // Estimated longitude, in degrees
    ialt = streamGetFloatBefore(
        ',');         // Estimated altitude, in meters - only forGNSS
                      // positioning, 0 in case of CellLocate
    if (ialt != 0) {  // values not returned for CellLocate
      iaccuracy =
          streamGetFloatBefore(',');       // Maximum possible error, in meters
      ispeed = streamGetFloatBefore(',');  // Speed over ground m/s3
      streamSkipUntil(',');  // Course over ground in degree (0 deg - 360 deg)
      streamSkipUntil(',');  // Vertical accuracy, in meters
      streamSkipUntil(',');  // Sensor used for the position calculation
      iusat = streamGetIntBefore(',');  // Number of satellite used
      streamSkipUntil(',');             // Antenna status
      streamSkipUntil('\n');            // Jamming status
    } else {
      iaccuracy =
          streamGetFloatBefore('\n');  // Maximum possible error, in meters
    }

    // Set pointers
    if (lat != nullptr) *lat = ilat;
    if (lon != nullptr) *lon = ilon;
    if (speed != nullptr) *speed = ispeed;
    if (alt != nullptr) *alt = ialt;
    if (vsat != nullptr)
      *vsat = 0;  // Number of satellites viewed not reported;
    if (usat != nullptr) *usat = iusat;
    if (accuracy != nullptr) *accuracy = iaccuracy;
    if (iyear < 2000) iyear += 2000;
    if (year != nullptr) *year = iyear;
    if (month != nullptr) *month = imonth;
    if (day != nullptr) *day = iday;
    if (hour != nullptr) *hour = ihour;
    if (minute != nullptr) *minute = imin;
    if (second != nullptr) *second = static_cast<int>(secondWithSS);

    // final ok
    waitResponse();
    return true;
  }
  bool getGsmLocationImpl(float* lat, float* lon, float* accuracy, int* year,
                          int* month, int* day, int* hour, int* minute,
                          int* second) {
    return getUbloxLocation(2, lat, lon, 0, 0, 0, 0, accuracy, year, month, day,
                            hour, minute, second);
  }
  bool getGPSImpl(float* lat, float* lon, float* speed, float* alt, int* vsat,
                  int* usat, float* accuracy, int* year, int* month, int* day,
                  int* hour, int* minute, int* second) {
    return getUbloxLocation(1, lat, lon, speed, alt, vsat, usat, accuracy, year,
                            month, day, hour, minute, second);
  }

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
   * Battery functions
   */
 protected:
  int16_t getBattVoltageImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  int8_t getBattPercentImpl() {
    sendAT(GF("+CIND?"));
    if (waitResponse(GF("+CIND:")) != 1) { return 0; }

    int8_t res     = streamGetIntBefore(',');
    int8_t percent = res * 20;  // return is 0-5
    // Wait for final OK
    waitResponse();
    return percent;
  }

  int8_t getBattChargeStateImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool getBattStatsImpl(int8_t& chargeState, int8_t& percent,
                        int16_t& milliVolts) {
    chargeState = 0;
    percent     = getBattPercent();
    milliVolts  = 0;
    return true;
  }

  /*
   * Temperature functions
   */
  // This would only available for a small number of modules in this group
  // (TOBY-L)
  float getTemperatureImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;

  /*
   * Client related functions
   */
 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t* mux,
                        int timeout_s) {
    uint32_t timeout_ms  = ((uint32_t)timeout_s) * 1000;
    bool     ssl         = sockets[*mux]->is_secure;
    uint32_t startMillis = millis();

    // create a socket
    sendAT(GF("+USOCR=6"));
    // reply is +USOCR: ## of socket created
    if (waitResponse(GF("+USOCR:")) != 1) { return false; }
    *mux = streamGetIntBefore('\n');
    waitResponse();

    if (ssl) {
      sendAT(GF("+USOSEC="), *mux, ",1");
      waitResponse();
    }

    // Enable NODELAY
    // AT+USOSO=<socket>,<level>,<opt_name>,<opt_val>[,<opt_val2>]
    // <level> - 0 for IP, 6 for TCP, 65535 for socket level options
    // <opt_name> TCP/1 = no delay (do not delay send to coalesce packets)
    // NOTE:  Enabling this may increase data plan usage
    // sendAT(GF("+USOSO="), *mux, GF(",6,1,1"));
    // waitResponse();

    // Enable KEEPALIVE, 30 sec
    // sendAT(GF("+USOSO="), *mux, GF(",6,2,30000"));
    // waitResponse();

    // connect on the allocated socket
    sendAT(GF("+USOCO="), *mux, GF(",\""), host, GF("\","), port);
    int8_t rsp = waitResponse(timeout_ms - (millis() - startMillis));
    return (1 == rsp);
  }

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    sendAT(GF("+USOWR="), mux, ',', (uint16_t)len);
    if (waitResponse(GF("@")) != 1) { return 0; }
    // 50ms delay, see AT manual section 25.10.4
    delay(50);
    return true;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t mux) {
    if (waitResponse(GF("+USOWR:")) != 1) { return 0; }
    uint8_t  ret_mux = streamGetIntBefore(',');   // check mux
    uint16_t sent    = streamGetIntBefore('\n');  // check send length
    bool     success = waitResponse() ==
        1;  // sends back OK after the confirmation of number sent
    if (sent != len) { DBG("### Sent:", sent, "of", len, "on", mux); }
    if (mux == ret_mux && success) return sent;
    return 0;
  }

  size_t modemReadImpl(size_t size, uint8_t mux) {
    if (!sockets[mux]) return 0;
    sendAT(GF("+USORD="), mux, ',', (uint16_t)size);
    if (waitResponse(GF("+USORD:")) != 1) { return 0; }
    streamSkipUntil(',');  // Skip mux
    int16_t len_reported = streamGetIntBefore(',');
    streamSkipUntil('\"');
    size_t len_read = moveCharsFromStreamToFifo(mux, len_reported);
    streamSkipUntil('\"');
    waitResponse();
    sockets[mux]->sock_available = modemGetAvailable(mux);
    return len_read;
  }

  size_t modemGetAvailableImpl(uint8_t mux) {
    if (!sockets[mux]) return 0;
    // NOTE:  Querying a closed socket gives an error "operation not allowed"
    sendAT(GF("+USORD="), mux, ",0");
    size_t  result = 0;
    uint8_t res    = waitResponse(GF("+USORD:"));
    // Will give error "operation not allowed" when attempting to read a socket
    // that you have already told to close
    if (res == 1) {
      streamSkipUntil(',');  // Skip mux
      result = streamGetIntBefore('\n');
      // if (result) DBG("### DATA AVAILABLE:", result, "on", mux);
      waitResponse();
    }
    if (!result) { sockets[mux]->sock_connected = modemGetConnected(mux); }
    // DBG("### AvailablE:", result, "on", mux);
    return result;
  }

  bool modemGetConnectedImpl(uint8_t mux) {
    // NOTE:  Querying a closed socket gives an error "operation not allowed"
    sendAT(GF("+USOCTL="), mux, ",10");
    uint8_t res = waitResponse(GF("+USOCTL:"));
    if (res != 1) { return false; }

    streamSkipUntil(',');  // Skip mux
    streamSkipUntil(',');  // Skip type
    int8_t result = streamGetIntBefore('\n');
    // 0: the socket is in INACTIVE status (it corresponds to CLOSED status
    // defined in RFC793 "TCP Protocol Specification" [112])
    // 1: the socket is in LISTEN status
    // 2: the socket is in SYN_SENT status
    // 3: the socket is in SYN_RCVD status
    // 4: the socket is in ESTABILISHED status
    // 5: the socket is in FIN_WAIT_1 status
    // 6: the socket is in FIN_WAIT_2 status
    // 7: the sokcet is in CLOSE_WAIT status
    // 8: the socket is in CLOSING status
    // 9: the socket is in LAST_ACK status
    // 10: the socket is in TIME_WAIT status
    waitResponse();
    return (result != 0);
  }

  /*
   * Utilities
   */
 protected:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+UUSORD:"))) {
      int8_t  mux = streamGetIntBefore(',');
      int16_t len = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TinyGsmUBLOXTcpConfig::kMuxCount && sockets[mux]) {
        sockets[mux]->got_data = true;
        // max size is 1024
        if (len >= 0 && len <= 1024) { sockets[mux]->sock_available = len; }
      }
      data = "";
      // DBG("### URC Data Received:", len, "on", mux);
      return true;
    } else if (data.endsWith(GF("+UUSOCL:"))) {
      int8_t mux = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TinyGsmUBLOXTcpConfig::kMuxCount && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      data = "";
      DBG("### URC Sock Closed: ", mux);
      return true;
    }
    return false;
  }

 public:
  /// Stream used to communicate with the modem.
  Stream& stream;

 protected:
  GsmClientUBLOX* sockets[TinyGsmUBLOXTcpConfig::kMuxCount];
};
// cspell:words USOWR

#endif  // SRC_TINYGSMCLIENTUBLOX_H_
