/**
 * @file       TinyGsmClientBG96.h
 * @brief      BG96 modem client and modem-trait definitions.
 * @author     Volodymyr Shymanskyy and Aurelien BOUIN (SSL)
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Apr 2018, Aug 2023 (SSL)
 */
/* clang-format off */
/**
 * @defgroup quectel_bg96 Quectel BG95/BG96 Modem Family
 * @brief Manufacturer: Quectel. Models: BG95, BG96.
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
 * - Secure socket layer (SSL) certificate management functions (TinyGsmSSL.tpp)
 *     - @ref TinyGsmSSL<modemType>::loadCertificate "loadCertificate()"
 *     - @ref TinyGsmSSL<modemType>::deleteCertificate "deleteCertificate()"
 *     - @ref TinyGsmSSL<modemType>::printCertificate "printCertificate()"
 *     - @ref TinyGsmSSL<modemType>::convertCertificate "convertCertificate()"
 *     - @ref TinyGsmSSL<modemType>::convertCACertificate "convertCACertificate()"
 *     - @ref TinyGsmSSL<modemType>::convertClientCertificates "convertClientCertificates()"
 *     - @ref TinyGsmSSL<modemType>::convertPSKandID "convertPSKandID()"
 *     - @ref TinyGsmSSL<modemType>::convertPSKTable "convertPSKTable()"
 * - Phone call functions (TinyGsmCalling.tpp)
 *     - @ref TinyGsmCalling<modemType>::callAnswer "callAnswer()"
 *     - @ref TinyGsmCalling<modemType>::callNumber "callNumber()"
 *     - @ref TinyGsmCalling<modemType>::callHangup "callHangup()"
 *     - @ref TinyGsmCalling<modemType>::dtmfSend "dtmfSend()"
 * - Text messaging (SMS) functions (TinyGsmSMS.tpp)
 *     - @ref TinyGsmSMS<modemType>::sendUSSD "sendUSSD()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS "sendSMS()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS_UTF16 "sendSMS_UTF16()"
 * - GPS (GNSS, GLONASS) functions (TinyGsmGPS.tpp)
 *     - @ref TinyGsmGPS<modemType>::enableGPS "enableGPS()"
 *     - @ref TinyGsmGPS<modemType>::disableGPS "disableGPS()"
 *     - @ref TinyGsmGPS<modemType>::getGPSraw "getGPSraw()"
 *     - @ref TinyGsmGPS<modemType>::getGPS "getGPS()"
 *     - @ref TinyGsmGPS<modemType>::getGPSTime "getGPSTime()"
 * - Time functions (TinyGsmTime.tpp)
 *     - @ref TinyGsmTime<modemType>::getGSMDateTime "getGSMDateTime()"
 *     - @ref TinyGsmTime<modemType>::getNetworkTime "getNetworkTime()"
 *     - @ref TinyGsmTime<modemType>::getNetworkUTCTime "getNetworkUTCTime()"
 * - NTP server functions (TinyGsmNTP.tpp)
 *     - @ref TinyGsmNTP<modemType>::NTPServerSync "NTPServerSync()"
 *     - @ref TinyGsmNTP<modemType>::waitForTimeSync "waitForTimeSync()"
 * - NTP Utilities (TinyGsmNTP.tpp)
 *     - @ref TinyGsmNTP<modemType>::TinyGsmIsValidNumber "TinyGsmIsValidNumber()"
 * - Battery functions (TinyGsmBattery.tpp)
 *     - @ref TinyGsmBattery<modemType>::getBattVoltage "getBattVoltage()"
 *     - @ref TinyGsmBattery<modemType>::getBattPercent "getBattPercent()"
 *     - @ref TinyGsmBattery<modemType>::getBattChargeState "getBattChargeState()"
 *     - @ref TinyGsmBattery<modemType>::getBattStats "getBattStats()"
 * - Temperature functions (TinyGsmTemperature.tpp)
 *     - @ref TinyGsmTemperature<modemType>::getTemperature "getTemperature()"
 * - SSL client functions
 *     - @ref TinyGsmBG96::configureSSLContext "configureSSLContext()"
 *
 * # Connection Information
 *
 * - Combined TCP/SSL sockets:
 *   - 12
 * - SSL contexts:
 *   - 6
 * - Socket Buffering:
 *   - The modem has an internal buffer for incoming data.
 *   - This gives you leeway to pull data from the buffer as needed with less
 * risk of losing data.
 * - Socket Numbering:
 *   - The modem uses user-specified MUX channel numbers for socket connections.
 *   - If you attempt to create a new client with a channel number that is
 * already in use and other unused channels are available, this library will
 * select the next available one.
 *   - Use the getMux() function to get the assigned multiplexing channel number
 * after a successful connection.
 *
 * @todo In operator `GsmClientBG96::read()`: Read directly into user buffer?
 * @todo In `modemEndSendImpl()`: Wait for ACK? (AT+QISEND=id,0 or
 * AT+QSSLSEND=id,0)
 * @todo In `modemGetConnectedImpl()`: Verify mux
 */
/* clang-format on */

#ifndef SRC_TINYGSMCLIENTBG96_H_
#define SRC_TINYGSMCLIENTBG96_H_
#pragma message("TinyGSM:  TinyGsmClientBG96")

#include "TinyGsmModem.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmSSL.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmGPS.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"
#include "TinyGsmBattery.tpp"
#include "TinyGsmTemperature.tpp"

/// Registration status
/// @ingroup quectel_bg96
enum BG96RegStatus {
  REG_NO_RESULT    = -1,  ///< No registration result
  REG_UNREGISTERED = 0,   ///< Not registered on the network
  REG_SEARCHING    = 2,   ///< Searching for network
  REG_DENIED       = 3,   ///< Registration denied
  REG_OK_HOME      = 1,   ///< Registered on the home network
  REG_OK_ROAMING   = 5,   ///< Registered on a roaming network
  REG_UNKNOWN      = 4,   ///< Unknown registration status
};

/// Basic modem configurations for the BG96 modem family
/// @ingroup quectel_bg96
struct TinyGsmBG96ModemConfig : public TinyGsmModemConfigPreset<BG96RegStatus> {
  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "Quectel";
#if defined(TINY_GSM_MODEM_BG95) || defined(TINY_GSM_MODEM_BG95SSL)
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "BG95";
#else
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "BG96";
#endif
};

constexpr char TinyGsmBG96ModemConfig::MODEM_MANUFACTURER[];
constexpr char TinyGsmBG96ModemConfig::MODEM_MODEL[];

/**
 * @brief TCP behavior and limits for the BG95/BG96 modem family.
 *
 * The BG95/BG96 supports 12 sockets (0-11); any of them can be SSL
 *
 * The BG95/BG96 also supports 6 SSL contexts (0-5)
 * The SSL context is a collection of SSL settings, not the connection
 * identifier. This library always uses SSL context 0.
 *
 * When sending  data QISEND and QSSLSEND both accept up to 1460 bytes of input.
 *
 * @ingroup quectel_bg96
 */
struct TinyGsmBG96TcpConfig
    : public TinyGsmTcpConfigPreset<
          /*bufferMode*/ TinyGsmTcpBufferMode::BufferReadAndCheckSize,
          /*muxMode*/ TinyGsmTcpMuxMode::Static,
          /*muxCount*/ 12,
          /*sendMaxSize*/ 1460,
          /*connectTimeoutS*/ 150> {};

/// Class for the Quectel BG96 and BG95
/// @ingroup quectel_bg96
class TinyGsmBG96 : public TinyGsmModem<TinyGsmBG96, TinyGsmBG96ModemConfig>,
                    public TinyGsmGPRS<TinyGsmBG96>,
                    public TinyGsmTCP<TinyGsmBG96, TinyGsmBG96TcpConfig>,
                    public TinyGsmSSL<TinyGsmBG96>,
                    public TinyGsmCalling<TinyGsmBG96>,
                    public TinyGsmSMS<TinyGsmBG96>,
                    public TinyGsmGPS<TinyGsmBG96>,
                    public TinyGsmTime<TinyGsmBG96>,
                    public TinyGsmNTP<TinyGsmBG96>,
                    public TinyGsmBattery<TinyGsmBG96>,
                    public TinyGsmTemperature<TinyGsmBG96> {
  friend class TinyGsmModem<TinyGsmBG96, TinyGsmBG96ModemConfig>;
  friend class TinyGsmGPRS<TinyGsmBG96>;
  friend class TinyGsmTCP<TinyGsmBG96, TinyGsmBG96TcpConfig>;
  friend class GsmClient<TinyGsmBG96, TinyGsmBG96TcpConfig>;
  friend class TinyGsmSSL<TinyGsmBG96>;
  friend class TinyGsmCalling<TinyGsmBG96>;
  friend class TinyGsmSMS<TinyGsmBG96>;
  friend class TinyGsmGPS<TinyGsmBG96>;
  friend class TinyGsmTime<TinyGsmBG96>;
  friend class TinyGsmNTP<TinyGsmBG96>;
  friend class TinyGsmBattery<TinyGsmBG96>;
  friend class TinyGsmTemperature<TinyGsmBG96>;

  using ModemConfig = TinyGsmBG96ModemConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  /// @ingroup quectel_bg96
  class GsmClientBG96 : public GsmClient<TinyGsmBG96, TinyGsmBG96TcpConfig> {
    friend class TinyGsmBG96;

   public:
    using GsmClient<TinyGsmBG96, TinyGsmBG96TcpConfig>::connect;
    using GsmClient<TinyGsmBG96, TinyGsmBG96TcpConfig>::stop;

    /**
     * @brief Create a new TCP client.  This must be initialized with a modem
     * before it can be used.
     */
    GsmClientBG96() {
      is_secure = false;
    }
    /**
     * @brief Create a new TCP client and bind it to a modem and optionally a
     * multiplexing channel.
     * @param modem Modem instance used by this client.
     * @param mux Multiplexing channel to use.
     *
     * @note The BG96 allows you choose the multiplexing channel number, but if
     * the input mux channel number is already in use and other mux channels are
     * available, this library will select the next available one.  Use the
     * getMux() function to get the assigned multiplexing channel number after a
     * successful connection.
     */
    explicit GsmClientBG96(TinyGsmBG96& modem, uint8_t mux = 0) {
      init(&modem, mux);
      is_secure = false;
    }

    /**
     * @brief Initialize the TCP client with a modem and optionally a
     * multiplexing channel.
     * @return true if initialization was successful, false otherwise.
     * @copydetails GsmClientBG96::GsmClientBG96(TinyGsmBG96&, uint8_t)
     */
    bool init(TinyGsmBG96* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_available = 0;
      prev_check     = 0;
      sock_connected = false;
      got_data       = false;
      is_mid_send    = false;

      // The BG96 generally lets you choose the mux number, but we want to try
      // to find an empty place in the socket array for it.

      // if it's a valid mux number, and that mux number isn't in use (or it's
      // already this), accept the mux number
      if (mux < TinyGsmBG96TcpConfig::kMuxCount &&
          (at->sockets[mux] == nullptr || at->sockets[mux] == this)) {
        this->mux = mux;
        // If the mux number is in use or out of range, find the next available
        // one
      } else if (at->findFirstUnassignedMux() != static_cast<uint8_t>(-1)) {
        this->mux = at->findFirstUnassignedMux();
      } else {
        // If we can't find anything available, overwrite something, using mod
        // to make sure we're in range
        this->mux = (mux % TinyGsmBG96TcpConfig::kMuxCount);
      }
      at->sockets[this->mux] = this;

      return true;
    }

   public:
    int connect(const char* host, uint16_t port, int timeout_s) override {
      stop(TinyGsmBG96TcpConfig::kStopTimeoutS * 1000L);
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }

    void stop(uint32_t maxWaitMs) override {
      is_mid_send          = false;
      uint32_t startMillis = millis();
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+QICLOSE="), mux);
      sock_connected = false;
      at->waitResponse((maxWaitMs - (millis() - startMillis)));
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
  /// @ingroup quectel_bg96
  class GsmClientSecureBG96 : public GsmClientBG96, public GsmSecureClient {
    friend class TinyGsmBG96;

   public:
    using GsmClientBG96::connect;
    using GsmClientBG96::stop;

    TINY_GSM_SECURE_CLIENT_CTORS(BG96)

    // Because we have the same potetial range of mux numbers for secure and
    // insecure connections, we don't need to re-check for mux number
    // availability.
    int connect(const char* host, uint16_t port, int timeout_s) override {
      stop(TinyGsmBG96TcpConfig::kStopTimeoutS * 1000L);
      TINY_GSM_YIELD();
      rx.clear();
      if (!sslCtxConfigured) {
        if (sslAuthMode == SSLAuthMode::PRE_SHARED_KEYS) {
          DBG("### The BG96 does not support SSL using pre-shared keys.");
          sslCtxConfigured = false;
        } else {
          sslCtxConfigured = at->configureSSLContext(
              sslCtxIndex, sslAuthMode, sslVersion, CAcertName, clientCertName,
              clientKeyName);
        }
      }
      sock_connected = at->modemConnect(host, port, mux, timeout_s);
      return sock_connected;
    }

    void stop(uint32_t maxWaitMs) override {
      is_mid_send          = false;
      uint32_t startMillis = millis();
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+QSSLCLOSE="), mux);
      sock_connected = false;
      at->waitResponse((maxWaitMs - (millis() - startMillis)));
    }


    // NOTE: Unlike the unsecured client, we can't check the size of the buffer
    // for an SSL socket. This means we have to overwrite all of the
    // buffer-checking versions of functions with the read-without-size-check
    // versions.
    /// @copydoc GsmClient::available()
    int available() override {
      is_mid_send = false;  // Any calls to the AT when mid-send will cause the
                            // send to fail
      TINY_GSM_YIELD();
      // Returns the combined number of characters available in the TinyGSM
      // fifo and the modem chips internal fifo.
      if (!rx.size()) { at->maintain(); }
      return static_cast<uint16_t>(rx.size()) + sock_available;
    }

    /// @copydoc GsmClient::read(uint8_t*, size_t)
    int read(uint8_t* buf, size_t size) override {
      is_mid_send = false;  // Any calls to the AT when mid-send will cause the
                            // send to fail
      TINY_GSM_YIELD();
      size_t cnt = 0;

      // Reads characters out of the TinyGSM fifo, and from the modem chip's
      // internal fifo if available.
      at->maintain();
      while (cnt < size) {
        size_t chunk = TinyGsmMin(size - cnt, rx.size());
        if (chunk > 0) {
          rx.get(buf, chunk);
          buf += chunk;
          cnt += chunk;
          continue;
        } /* TODO: Read directly into user buffer? */
        at->maintain();
        if (sock_available > 0) {
          int n = at->modemRead(TinyGsmMin((uint16_t)rx.free(), sock_available),
                                mux);
          if (n == 0) break;
        } else {
          break;
        }
      }
      return cnt;
    }

    /// @copydoc GsmClient::read()
    int read() override {
      uint8_t c;
      if (read(&c, 1) == 1) { return c; }
      return -1;
    }

    /// @copydoc GsmClient::connected()
    uint8_t connected() override {
      if (is_mid_send) { return true; }  // Don't interrupt a send
      if (available()) { return true; }
      // If the modem doesn't have an internal buffer, or if we can't check how
      // many characters are in the buffer then the cascade won't happen.
      // We need to call modemGetConnected to check the sock state.
      return at->modemGetConnected(mux);
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
  explicit TinyGsmBG96(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientBG96"));

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

    // Disable time and time zone URC's
    sendAT(GF("+CTZR=0"));
    if (waitResponse(10000L) != 1) { return false; }

    // Enable automatic time zone update
    sendAT(GF("+CTZU=1"));
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
    if (!setPhoneFunctionality(1, true)) { return false; }
    waitResponse(10000L, GF("APP RDY"));
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+QPOWD=1"));
    waitResponse(300);  // returns OK first
    return waitResponse(300, GF("POWERED DOWN")) == 1;
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
    return waitResponse(15500L, GF("OK")) == 1;
  }

  /*
   * Generic network functions
   */
 protected:
  BG96RegStatus getRegistrationStatusImpl() {
    // Check first for EPS registration
    BG96RegStatus epsStatus = (BG96RegStatus)getRegistrationStatusXREG("CEREG");

    // If we're connected on EPS, great!
    if (epsStatus == REG_OK_HOME || epsStatus == REG_OK_ROAMING) {
      return epsStatus;
    } else {
      // Otherwise, check generic network status
      return (BG96RegStatus)getRegistrationStatusXREG("CREG");
    }
  }

  bool isNetworkConnectedImpl() {
    BG96RegStatus s = this->getRegistrationStatus();
    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
 protected:
// NOTE: Some of the documentation suggests this set of commands for
// uploading certs, but the BG96 I have doesn't accept them.
#if 0
  // AT+QSECWRITE=<filename>,<filesize> [,<timeout>]
  // <filename> - The name of the certificate/key/password file. The file name
  // has to be prefixed with "RAM:" or "NVRAM:".
  bool loadCertificateImpl(const char* certificateName, const char* cert,
                           const uint16_t len) {
    sendAT(GF("+QSECWRITE=\"RAM:"), certificateName, GF("\","), len, GF(",10"));
    if (waitResponse(GF("CONNECT")) != 1) {
      return false;
    }
    stream.write(cert, len);
    stream.flush();
    // After module switches to data mode, and the certificate or key data can
    // be inputted. When the size of the inputted data reaches <filesize> (unit:
    // byte) or module receives "+++" sequence from UART, module will return to
    // command mode and reply the following codes.
    // +QSECWRITE:<uploadsize>,<checksum>
    bool success = true;
    success &= waitResponse(GF("+QSECWRITE:")) == 1;
    if (success) {
      uint16_t len_confirmed = stream.parseInt();
      streamSkipUntil('\n');  // skip the checksum
      success &= len_confirmed == len;
    }
    return success && waitResponse() == 1;
  }

  // NOTE: You cannot print/view the content of a certificate after uploading it
  // to the modem This only prints the checksum
  bool printCertificateImpl(const char* filename, Stream& print_stream) {
    bool success = true;

    // Read the file's checksum and checksum match
    // AT+QSECREAD=<filename>
    // Responds with +QSECREAD=<good>,<checksum>
    sendAT(GF("+QSECREAD=\"RAM:"), filename, '"');
    success &= waitResponse(GF("+QSECREAD:")) == 1;
    if (success) { success = stream.parseInt() == 1; }
    if (!success) { return false; }

    // wait for some characters to be available
    uint32_t start = millis();
    while (!stream.available() && millis() - start < 10000) {}

    while (stream.available()) {
      int      c;
      uint32_t _startMillis = millis();
      do {
        c = stream.read();
        if (c >= 0) break;
      } while (millis() - _startMillis < 50);
      // Print the file to the buffer
#ifndef DUMP_AT_COMMANDS
      // NOTE: Only do this if we're not dumping the all AT, or we'll double
      // print
      print_stream.write(c);
#endif
      if (c < 0) { break; }  // if we run out of characters, stop
    }
    print_stream.flush();

    // wait for the ending OK
    success &= waitResponse() == 1;
    return success;
  }

  // NOTE: Deleting the certificate only happens by filename, the type of
  // certificate does not matter here
  // AT+QSECDEL=<filename>
  bool deleteCertificateImpl(const char* certificateName) {
    sendAT(GF("+QSECDEL=\"RAM:"), certificateName, '"');
    return waitResponse() == 1;
  }
#endif

  // AT+QFUPL=<filename>[,<file_size>[,<timeout>[,<ackmode>]]]
  bool loadCertificateImpl(const char* certificateName, const char* cert,
                           const uint16_t len) {
    sendAT(GF("+QFUPL=\""), certificateName, GF("\","), len, GF(",10"));
    if (waitResponse(GF("CONNECT")) != 1) { return false; }
    stream.write(cert, len);
    stream.flush();
    // TA switches to the data mode (transparent access mode), and the binary
    // data of file can be inputted. When the total size of the inputted data
    // reaches <file_size> (unit: byte), TA will return to command mode and
    // reply the following codes:
    // +QFUPL: <upload_size>,<checksum>
    bool success = true;
    success &= waitResponse(GF("+QFUPL:")) == 1;
    if (success) {
      uint16_t len_confirmed = stream.parseInt();
      streamSkipUntil('\n');  // skip the checksum
      success &= len_confirmed == len;
    }
    return success && waitResponse() == 1;
  }

  // NOTE: You cannot print/view the content of a certificate after uploading it
  // to the modem This only prints the checksum
  bool printCertificateImpl(const char* filename, Stream& print_stream) {
    bool    success   = true;
    int16_t print_len = 0;

    // Download the file
    // AT+QFDWL=<filename>
    sendAT(GF("+QFDWL=\""), filename, '"');
    success &= waitResponse(GF("+QFDWL")) == 1;
    if (success) {
      print_len = streamGetIntBefore(',');
      streamSkipUntil('\n');  // skip the checksum
    }

    // wait for some characters to be available
    uint32_t start = millis();
    while (!stream.available() && millis() - start < 10000) {}

    for (int i = 0; i < print_len; i++) {
      int      c;
      uint32_t _startMillis = millis();
      do {
        c = stream.read();
        if (c >= 0) break;
      } while (millis() - _startMillis < 50);
      // Print the file to the buffer
#ifndef DUMP_AT_COMMANDS
      // NOTE: Only do this if we're not dumping the all AT, or we'll double
      // print
      print_stream.write(c);
#endif
      if (c < 0) { break; }  // if we run out of characters, stop
    }
    print_stream.flush();

    // wait for the ending OK
    success &= waitResponse() == 1;
    return success;
  }

  // NOTE: Deleting the certificate only happens by filename, the type of
  // certificate does not matter here
  // AT+QFDEL=<filename>
  bool deleteCertificateImpl(const char* certificateName) {
    sendAT(GF("+QFDEL=\""), certificateName, '"');
    return waitResponse() == 1;
  }

  // no certificate conversion needed
  bool convertCertificateImpl(CertificateType, const char*) {
    return true;
  }
  bool convertCACertificateImpl(const char*) {
    return true;
  }
  bool convertClientCertificatesImpl(const char*, const char*) {
    return true;
  }
  bool convertPSKandIDImpl(const char*, const char*) {
    return true;
  }
  bool convertPSKTableImpl(const char*) {
    return true;
  }

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

    // Configure the TCPIP Context
    sendAT(GF("+QICSGP=1,1,\""), apn, GF("\",\""), user, GF("\",\""), pwd, '"');
    if (waitResponse() != 1) { return false; }

    // Activate GPRS/CSD Context
    sendAT(GF("+QIACT=1"));
    if (waitResponse(150000L) != 1) { return false; }

    // Attach to Packet Domain service - is this necessary?
    sendAT(GF("+CGATT=1"));
    if (waitResponse(60000L) != 1) { return false; }

    return true;
  }

  bool gprsDisconnectImpl() {
    sendAT(GF("+QIDEACT=1"));  // Deactivate the bearer context
    if (waitResponse(40000L) != 1) { return false; }

    return true;
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

  /*
   * GSM location functions
   */
 protected:
  // NOTE:  As of application firmware version 01.016.01.016 triangulated
  // locations can be obtained via the QuecLocator service and accompanying AT
  // commands.  As this is a separate paid service which I do not have access
  // to, I am not implementing it here.

  /*
   * GPS/GNSS/GLONASS location functions
   */
 protected:
  // enable GPS
  bool enableGPSImpl() {
    sendAT(GF("+QGPS=1"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  bool disableGPSImpl() {
    sendAT(GF("+QGPSEND"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  // get the RAW GPS output
  String getGPSrawImpl() {
    sendAT(GF("+QGPSLOC=2"));
    if (waitResponse(10000L, GF("+QGPSLOC: ")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  // get GPS informations
  bool getGPSImpl(float* lat, float* lon, float* speed, float* alt, int* vsat,
                  int* usat, float* accuracy, int* year, int* month, int* day,
                  int* hour, int* minute, int* second) {
    sendAT(GF("+QGPSLOC=2"));
    if (waitResponse(10000L, GF("+QGPSLOC: ")) != 1) {
      // NOTE:  Will return an error if the position isn't fixed
      return false;
    }

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

    // UTC date & Time
    ihour        = streamGetIntLength(2);      // Two digit hour
    imin         = streamGetIntLength(2);      // Two digit minute
    secondWithSS = streamGetFloatBefore(',');  // 6 digit second with subseconds

    ilat      = streamGetFloatBefore(',');  // Latitude
    ilon      = streamGetFloatBefore(',');  // Longitude
    iaccuracy = streamGetFloatBefore(',');  // Horizontal precision
    ialt      = streamGetFloatBefore(',');  // Altitude from sea level
    streamSkipUntil(',');                   // GNSS positioning mode
    streamSkipUntil(',');  // Course Over Ground based on true north
    streamSkipUntil(',');  // Speed Over Ground in Km/h
    ispeed = streamGetFloatBefore(',');  // Speed Over Ground in knots

    iday   = streamGetIntLength(2);    // Two digit day
    imonth = streamGetIntLength(2);    // Two digit month
    iyear  = streamGetIntBefore(',');  // Two digit year

    iusat = streamGetIntBefore(',');  // Number of satellites,
    streamSkipUntil('\n');  // The error code of the operation. If it is not
                            // 0, it is the type of error.

    // Set pointers
    if (lat != nullptr) *lat = ilat;
    if (lon != nullptr) *lon = ilon;
    if (speed != nullptr) *speed = ispeed;
    if (alt != nullptr) *alt = ialt;
    if (vsat != nullptr) *vsat = 0;
    if (usat != nullptr) *usat = iusat;
    if (accuracy != nullptr) *accuracy = iaccuracy;
    if (iyear < 2000) iyear += 2000;
    if (year != nullptr) *year = iyear;
    if (month != nullptr) *month = imonth;
    if (day != nullptr) *day = iday;
    if (hour != nullptr) *hour = ihour;
    if (minute != nullptr) *minute = imin;
    if (second != nullptr) *second = static_cast<int>(secondWithSS);

    waitResponse();  // Final OK
    return true;
  }

  /*
   * Time functions
   */
 protected:
  String getGSMDateTimeImpl(TinyGSMDateTimeFormat format) {
    sendAT(GF("+QLTS=2"));
    if (waitResponse(2000L, GF("+QLTS: \"")) != 1) { return ""; }

    String res;

    switch (format) {
      case TinyGSMDateTimeFormat::DATE_FULL:
        res = stream.readStringUntil('"');
        break;
      case TinyGSMDateTimeFormat::DATE_TIME:
        streamSkipUntil(',');
        res = stream.readStringUntil('"');
        break;
      case TinyGSMDateTimeFormat::DATE_DATE:
        res = stream.readStringUntil(',');
        break;
    }
    waitResponse();  // Ends with OK
    return res;
  }

  // The BG96 returns UTC time instead of local time as other modules do in
  // response to CCLK, so we're using QLTS where we can specifically request
  // local time.
  bool getNetworkUTCTimeImpl(int* year, int* month, int* day, int* hour,
                             int* minute, int* second, float* timezone) {
    sendAT(GF("+QLTS=1"));
    if (waitResponse(2000L, GF("+QLTS: \"")) != 1) { return false; }

    int iyear     = 0;
    int imonth    = 0;
    int iday      = 0;
    int ihour     = 0;
    int imin      = 0;
    int isec      = 0;
    int itimezone = 0;

    // Date & Time
    iyear       = streamGetIntBefore('/');
    imonth      = streamGetIntBefore('/');
    iday        = streamGetIntBefore(',');
    ihour       = streamGetIntBefore(':');
    imin        = streamGetIntBefore(':');
    isec        = streamGetIntLength(2);
    char tzSign = stream.read();
    itimezone   = streamGetIntBefore(',');
    if (tzSign == '-') { itimezone = itimezone * -1; }
    streamSkipUntil('\n');  // DST flag

    // Set pointers
    if (iyear < 2000) iyear += 2000;
    if (year != nullptr) *year = iyear;
    if (month != nullptr) *month = imonth;
    if (day != nullptr) *day = iday;
    if (hour != nullptr) *hour = ihour;
    if (minute != nullptr) *minute = imin;
    if (second != nullptr) *second = isec;
    if (timezone != nullptr) *timezone = static_cast<float>(itimezone) / 4.0;

    // Final OK
    waitResponse();  // Ends with OK
    return true;
  }

  // The BG96 returns UTC time instead of local time as other modules do in
  // response to CCLK, so we're using QLTS where we can specifically request
  // local time.
  bool getNetworkTimeImpl(int* year, int* month, int* day, int* hour,
                          int* minute, int* second, float* timezone) {
    sendAT(GF("+QLTS=2"));
    if (waitResponse(2000L, GF("+QLTS: \"")) != 1) { return false; }

    int iyear     = 0;
    int imonth    = 0;
    int iday      = 0;
    int ihour     = 0;
    int imin      = 0;
    int isec      = 0;
    int itimezone = 0;

    // Date & Time
    iyear       = streamGetIntBefore('/');
    imonth      = streamGetIntBefore('/');
    iday        = streamGetIntBefore(',');
    ihour       = streamGetIntBefore(':');
    imin        = streamGetIntBefore(':');
    isec        = streamGetIntLength(2);
    char tzSign = stream.read();
    itimezone   = streamGetIntBefore(',');
    if (tzSign == '-') { itimezone = itimezone * -1; }
    streamSkipUntil('\n');  // DST flag

    // Set pointers
    if (iyear < 2000) iyear += 2000;
    if (year != nullptr) *year = iyear;
    if (month != nullptr) *month = imonth;
    if (day != nullptr) *day = iday;
    if (hour != nullptr) *hour = ihour;
    if (minute != nullptr) *minute = imin;
    if (second != nullptr) *second = isec;
    if (timezone != nullptr) *timezone = static_cast<float>(itimezone) / 4.0;

    // Final OK
    waitResponse();  // Ends with OK
    return true;
  }

  /*
   * NTP server functions
   */

  byte NTPServerSyncImpl(const char* server, byte) {
    // Request network synchronization
    // AT+QNTP=<contextID>,<server>[,<port>][,<autosettime>]
    sendAT(GF("+QNTP=1,\""), server, '"');
    if (waitResponse(10000L, GF("+QNTP:"))) {
      String result = stream.readStringUntil(',');
      streamSkipUntil('\n');
      result.trim();
      if (TinyGsmIsValidNumber(result)) { return result.toInt(); }
    } else {
      return -1;
    }
    return -1;
  }

  bool waitForTimeSyncImpl(int timeout_s) {
    // if we're not connected, we'll never get the time
    if (!isNetworkConnected()) { return false; }
    // if we're sure we should be able to get the time, wait for it
    uint32_t start_millis = millis();
    while (millis() - start_millis < static_cast<uint32_t>(timeout_s) * 1000) {
      // Request network synchronization
      sendAT(GF("+QNTP?"));
      // +QNTP: <server>,<port>]\r\nOK\r\n
      if (waitResponse(10000L) == 1) { return true; }
      delay(250);
    }
    return false;
  }

  String ShowNTPErrorImpl(byte error) TINY_GSM_ATTR_NOT_IMPLEMENTED;

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
  // get temperature in degree celsius
  uint16_t getTemperatureImpl() {
    sendAT(GF("+QTEMP"));
    if (waitResponse(GF("+QTEMP:")) != 1) { return 0; }
    // return temperature in C
    uint16_t res =
        streamGetIntBefore(',');  // read PMIC (primary ic) temperature
    streamSkipUntil(',');         // skip XO temperature ??
    streamSkipUntil('\n');        // skip PA temperature ??
    // Wait for final OK
    waitResponse();
    return res;
  }

  /*
   * SSL client functions
   */
 public:
  /**
   * @brief Configure the SSL context for the modem.
   * @param context_id The SSL context ID.
   * @param sslAuthMode The SSL authentication mode.
   * @param sslVersion The SSL version.
   * @param CAcertName The CA certificate name.
   * @param clientCertName The client certificate name.
   * @param clientKeyName The client key name.
   * @return True if the operation was successful, false otherwise.
   */
  bool configureSSLContext(uint8_t context_id, SSLAuthMode sslAuthMode,
                           SSLVersion sslVersion, const char* CAcertName,
                           const char* clientCertName,
                           const char* clientKeyName) {
    bool success = true;

    // NOTE: The SSL context (<sslctxID>) is not the same as the connection
    // identifier.  The SSL context is the grouping of SSL settings, the
    // connection identifier is the mux/socket number.
    // For this, we will *always* configure SSL context 0, just as we always
    // configured PDP context 1.

    // apply the correct certificates to the connection
    if (CAcertName != nullptr &&
        (sslAuthMode == SSLAuthMode::CA_VALIDATION ||
         sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION)) {
      // AT+QSSLCFG="cacert",<sslctxID>,<cacertpath>
      // <sslctxID> SSL Context ID, range 0-5
      // <cacertpath> certificate file path
      sendAT(GF("+QSSLCFG=\"cacert\","), context_id, GF(",\""), CAcertName,
             '"');
      success &= waitResponse(5000L) == 1;
    }
    // SRGD WARNING: UNTESTED!!
    if (clientCertName != nullptr &&
        (sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION)) {
      // AT+QSSLCFG="clientcert",<sslctxID>,<client_cert_path>
      sendAT(GF("+QSSLCFG=\"clientcert\","), context_id, GF(",\""),
             clientCertName, '"');
      success &= waitResponse(5000L) == 1;
    }
    // SRGD WARNING: UNTESTED!!
    if (clientKeyName != nullptr &&
        (sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION)) {
      // AT+QSSLCFG="clientkey",<sslctxID>[,<client_key_path>]
      sendAT(GF("+QSSLCFG=\"clientkey\","), context_id, GF(",\""),
             clientKeyName, '"');
      success &= waitResponse(5000L) == 1;
    }

    // set the ssl cipher_suite
    // AT+QSSLCFG="ciphersuite",<sslctxID>,<cipher_suite>
    // <sslctxID> SSL Context ID, range 0-5
    // <cipher_suite> 0: TODO
    //              1: TODO
    //              0X0035: TLS_RSA_WITH_AES_256_CBC_SHA
    //              0XFFFF: ALL
    sendAT(GF("+QSSLCFG=\"ciphersuite\","), context_id,
           GF(",0XFFFF"));  // All available
    success &= waitResponse(5000L) == 1;

    // set the ssl version
    // AT+QSSLCFG="sslversion",<sslctxID>,<sslversion>
    // <sslctxID> SSL Context ID, range 0-5
    // <sslversion> 0: QAPI_NET_SSL_3.0
    //              1: QAPI_NET_SSL_PROTOCOL_TLS_1_0
    //              2: QAPI_NET_SSL_PROTOCOL_TLS_1_1
    //              3: QAPI_NET_SSL_PROTOCOL_TLS_1_2
    //              4: ALL
    int8_t q_ssl_version = 3;
    // convert the ssl version into the format for this command
    switch (sslVersion) {
      case SSLVersion::SSL3_0: {
        q_ssl_version = 0;
        break;
      }
      case SSLVersion::TLS1_0: {
        q_ssl_version = 1;
        break;
      }
      case SSLVersion::TLS1_1: {
        q_ssl_version = 2;
        break;
      }
      case SSLVersion::TLS1_2: {
        q_ssl_version = 3;
        break;
      }
      default: {
        q_ssl_version = 4;
        break;
      }
    }
    sendAT(GF("+CSSLCFG=\"sslversion\","), context_id, ',', q_ssl_version);
    success &= waitResponse(5000L) == 1;

    // set the ssl sec level
    // AT+QSSLCFG="seclevel",<sslctxID>,<sec_level>
    // <sslctxID> SSL Context ID, range 0-5
    // <sec_level> 0: No authentication (SSLAuthMode::NO_VALIDATION)
    //             1: Manage server authentication
    //             (SSLAuthMode::CA_VALIDATION)
    //             2: Manage server and client authentication if requested
    //             by the remote server (SSLAuthMode::MUTUAL_AUTHENTICATION)
    switch (sslAuthMode) {
      case SSLAuthMode::CA_VALIDATION: {
        sendAT(GF("+QSSLCFG=\"seclevel\","), context_id, GF(",1"));
        break;
      }
      case SSLAuthMode::MUTUAL_AUTHENTICATION: {
        sendAT(GF("+QSSLCFG=\"seclevel\","), context_id, GF(",2"));
        break;
      }
      default: {
        sendAT(GF("+QSSLCFG=\"seclevel\","), context_id, GF(",0"));
        break;
      }
    }
    success &= waitResponse(5000L) == 1;

    // Don't ignore the time - make sure you sync it first!
    // AT+QSSLCFG="ignorelocaltime",<SSL_ctxID>[,<ignore_ltime>]
    // <sslctxID> SSL Context ID, range 0-5
    // <ignore_ltime> 0 to use the time, 1 to ignore it (default 1)
    sendAT(GF("+QSSLCFG=\"ignorelocaltime\","), context_id, GF(",0"));
    success &= waitResponse() == 1;

    return success;
  }

  /*
   * Client related functions
   */

 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t mux,
                        int timeout_s) {
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    bool     ssl        = sockets[mux]->is_secure;

    if (ssl) {
      // If we have a secure socket, use a static cast to get the authentication
      // mode and certificate names. This isn't really "safe" but since we've
      // already checked that the socket is a secure one, we're pretty sure of
      // the type and it should work.
      const GsmClientSecureBG96* thisClient =
          static_cast<const GsmClientSecureBG96*>(sockets[mux]);
      uint8_t sslCtxIndex = thisClient->sslCtxIndex;

      // AT+QSSLOPEN=<pdpctxID>,<sslctxID>,<clientID>,<serveraddr>,<server_port>[,<access_mode>]
      // <PDPcontextID>(1-16) - we always use 1, which we configured above
      // <sslctxID> SSL Context ID, range 0-5
      // <connectID>(0-11), - socket index (mux)
      // may need previous AT+QSSLCFG
      sendAT(GF("+QSSLOPEN=1,"), sslCtxIndex, ',', mux, GF(",\""), host,
             GF("\","), port, GF(",0"));
      if (waitResponse() != 1) return false;

      if (waitResponse(timeout_ms, GF("+QSSLOPEN:")) != 1) { return false; }
      // 20230629 -> +QSSLOPEN: <clientID>,<err>
      // clientID is mux
      // err must be 0
      if (streamGetIntBefore(',') != mux) { return false; }
      // Read status
      return (0 == streamGetIntBefore('\n'));
    } else {
      // AT+QIOPEN=1,0,"TCP","220.180.239.212",8009,0,0
      // <PDPcontextID>(1-16), <connectID>(0-11),
      // "TCP/UDP/TCP LISTENER/UDPSERVICE", "<IP_address>/<domain_name>",
      // <remote_port>,<local_port>,<access_mode>(0-2; 0=buffer)
      sendAT(GF("+QIOPEN=1,"), mux, GF(",\""), GF("TCP"), GF("\",\""), host,
             GF("\","), port, GF(",0,0"));
      waitResponse();

      if (waitResponse(timeout_ms, GF("+QIOPEN:")) != 1) { return false; }

      if (streamGetIntBefore(',') != mux) { return false; }
    }
    // Read status
    return (0 == streamGetIntBefore('\n'));
  }

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    bool ssl = sockets[mux]->is_secure;
    if (ssl) {
      sendAT(GF("+QSSLSEND="), mux, ',', (uint16_t)len);
    } else {
      sendAT(GF("+QISEND="), mux, ',', (uint16_t)len);
    }
    return waitResponse(GF(">")) == 1;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t) {
    if (waitResponse(GF("SEND OK")) != 1) { return 0; }
    // TODO(?): Wait for ACK? (AT+QISEND=id,0 or AT+QSSLSEND=id,0)
    return len;
  }

  size_t modemReadImpl(size_t size, uint8_t mux) {
    if (!sockets[mux]) return 0;
    size_t len_reported = 0;
    size_t len_read     = 0;
    bool   ssl          = sockets[mux]->is_secure;
    if (ssl) {
      sendAT(GF("+QSSLRECV="), mux, ',', (uint16_t)size);
      if (waitResponse(GF("+QSSLRECV:")) != 1) {
        DBG("### READ: For unknown reason close");
        return 0;
      }
      len_reported = streamGetIntBefore('\n');
      // We have no way of knowing in advance how much data will be in the
      // buffer so when data is received we always assume the buffer is
      // completely full. Chances are, this is not true and there's really not
      // that much there. In that case, make sure we make sure we re-set the
      // amount of data available.
      if (len_reported < size) { sockets[mux]->sock_available = len_reported; }
      len_read = moveCharsFromStreamToFifo(mux, len_reported);
      sockets[mux]->sock_available -= len_read;
      // ^^ Decrease the characters available after moving from modem's FIFO to
      // our FIFO
      waitResponse();  // ends with an OK
    } else {
      sendAT(GF("+QIRD="), mux, ',', (uint16_t)size);
      if (waitResponse(GF("+QIRD:")) != 1) { return 0; }
      len_reported = streamGetIntBefore('\n');
      len_read     = moveCharsFromStreamToFifo(mux, len_reported);
      waitResponse();
      // For unsecured sockets, we can check how much is left in the buffer
      // after reading.
      sockets[mux]->sock_available = modemGetAvailable(mux);
    }
    return len_read;
  }

  size_t modemGetAvailableImpl(uint8_t mux) {
    if (!sockets[mux]) return 0;
    bool   ssl    = sockets[mux]->is_secure;
    size_t result = 0;
    if (ssl) {
      return 0;  // Sadly, we can't read the amount of buffered data for a
                 // secure socket
    } else {
      sendAT(GF("+QIRD="), mux, GF(",0"));
      if (waitResponse(GF("+QIRD:")) == 1) {
        streamSkipUntil(',');  // Skip total received
        streamSkipUntil(',');  // Skip have read
        result = streamGetIntBefore('\n');
        if (result) { DBG("### DATA AVAILABLE:", result, "on", mux); }
        waitResponse();
      }
    }
    if (!result) { sockets[mux]->sock_connected = modemGetConnected(mux); }
    return result;
  }

  bool modemGetConnectedImpl(uint8_t mux) {
    bool ssl = sockets[mux]->is_secure;
    if (ssl) {
      sendAT(GF("+QSSLSTATE="), mux);
      // +QSSLSTATE:<clientID>,"SSLClient",<IP_address>,<remote_port>,<local_port>,<socket_state>,<pdpctxID>,<serverID>,<access_mode>,<AT_port>,<sslctxID>)

      if (waitResponse(GF("+QSSLSTATE:")) != 1) {
        waitResponse();  // just returns "OK" if the sock is closed
        return false;
      }

      streamSkipUntil(',');  // Skip clientID
      // TODO: Verify mux
      streamSkipUntil(',');                  // Skip "SSLClient"
      streamSkipUntil(',');                  // Skip remote ip
      streamSkipUntil(',');                  // Skip remote port
      streamSkipUntil(',');                  // Skip local port
      int8_t res = streamGetIntBefore(',');  // socket state

      waitResponse();

      // 0 Initial, 1 Opening, 2 Connected, 3 Listening, 4 Closing
      return 2 == res;
    } else {
      sendAT(GF("+QISTATE=1,"), mux);
      // +QISTATE: 0,"TCP","151.139.237.11",80,5087,4,1,0,0,"uart1"

      if (waitResponse(GF("+QISTATE:")) != 1) {
        waitResponse();  // just returns "OK" if the sock is closed
        return false;
      }

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
  }

  /*
   * Utilities
   */
 protected:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+QIURC:"))) {
      streamSkipUntil('\"');
      String urc = stream.readStringUntil('\"');
      streamSkipUntil(',');
      if (urc == "recv") {
        int8_t mux = streamGetIntBefore('\n');
        DBG("### URC RECV:", mux);
        if (mux >= 0 && mux < TinyGsmBG96TcpConfig::kMuxCount && sockets[mux]) {
          sockets[mux]->got_data = true;
        }
      } else if (urc == "closed") {
        int8_t mux = streamGetIntBefore('\n');
        DBG("### URC CLOSE:", mux);
        if (mux >= 0 && mux < TinyGsmBG96TcpConfig::kMuxCount && sockets[mux]) {
          sockets[mux]->sock_connected = false;
        }
      } else {
        streamSkipUntil('\n');
      }
      data = "";
      return true;
    }
    if (data.endsWith(GF("+QSSLURC:"))) {
      streamSkipUntil('\"');
      String urc = stream.readStringUntil('\"');
      streamSkipUntil(',');
      if (urc == "recv") {
        int8_t mux = streamGetIntBefore('\n');
        DBG("### URC SSL RECV:", mux);
        if (mux >= 0 && mux < TinyGsmBG96TcpConfig::kMuxCount && sockets[mux]) {
          // We have no way of knowing how much data actually came in, so
          // we set the value to 1500, the maximum transmission unit for TCP.
          sockets[mux]->sock_available = 1500;
        }
      } else if (urc == "closed") {
        int8_t mux = streamGetIntBefore('\n');
        DBG("### URC CLOSE:", mux);
        if (mux >= 0 && mux < TinyGsmBG96TcpConfig::kMuxCount && sockets[mux]) {
          sockets[mux]->sock_connected = false;
        }
      } else {
        streamSkipUntil('\n');
      }
      data = "";
      return true;
    }
    return false;
  }

 public:
  /// Stream used to communicate with the modem.
  Stream& stream;

 protected:
  GsmClientBG96* sockets[TinyGsmBG96TcpConfig::kMuxCount];
};

#endif  // SRC_TINYGSMCLIENTBG96_H_
