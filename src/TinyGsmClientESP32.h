/**
 * @file       TinyGsmClientESP32.h
 * @brief      ESP32 AT modem client and modem-trait definitions.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */
/* clang-format off */
/**
 * @defgroup espressif_esp32 Espressif ESP32 AT Modem Family
 * @ingroup espressif_at
 * @brief Manufacturer: Espressif. Models: ESP32 (AT firmware, version
 * >= 3.2.0.0).
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
 * - WiFi functions (TinyGsmWifi.tpp)
 *     - @ref TinyGsmWifi<modemType>::networkConnect "networkConnect()"
 *     - @ref TinyGsmWifi<modemType>::networkDisconnect "networkDisconnect()"
 * - TCP functions (TinyGsmTCP.tpp)
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::maintain "maintain()"
 * - Secure socket layer (SSL) certificate management functions (TinyGsmSSL.tpp)
 *     - @ref TinyGsmSSL<modemType>::loadCertificate "loadCertificate()"
 *     - @ref TinyGsmSSL<modemType>::deleteCertificate "deleteCertificate()"
 *     - @ref TinyGsmSSL<modemType>::printCertificate "printCertificate()"
 *     - @ref TinyGsmSSL<modemType>::convertCertificate "convertCertificate()"
 *     - @ref TinyGsmSSL<modemType>::convertCACertificate "convertCACertificate()"
 *     - @ref TinyGsmSSL<modemType>::convertClientCertificates "convertClientCertificates()"
 *     - @ref TinyGsmSSL<modemType>::convertPSKandID "convertPSKandID()"
 *     - @ref TinyGsmSSL<modemType>::convertPSKTable "convertPSKTable()"
 * - Time functions (TinyGsmTime.tpp)
 *     - @ref TinyGsmTime<modemType>::getGSMDateTime "getGSMDateTime()"
 *     - @ref TinyGsmTime<modemType>::getNetworkTime "getNetworkTime()"
 *     - @ref TinyGsmTime<modemType>::getNetworkEpoch "getNetworkEpoch()"
 * - NTP server functions (TinyGsmNTP.tpp)
 *     - @ref TinyGsmNTP<modemType>::NTPServerSync "NTPServerSync()"
 *     - @ref TinyGsmNTP<modemType>::waitForTimeSync "waitForTimeSync()"
 *     - @ref TinyGsmNTP<modemType>::ShowNTPError "ShowNTPError()"
 * - NTP Utilities (TinyGsmNTP.tpp)
 *     - @ref TinyGsmNTP<modemType>::TinyGsmIsValidNumber "TinyGsmIsValidNumber()"
 * - Secure socket layer (SSL) certificate management functions
 *     - @ref TinyGsmESP32::loadCACert "loadCACert()"
 *     - @ref TinyGsmESP32::loadClientCert "loadClientCert()"
 *     - @ref TinyGsmESP32::loadPrivateKey "loadPrivateKey()"
 *     - @ref TinyGsmESP32::loadCertificateByNumber "loadCertificateByNumber()"
 *     - @ref TinyGsmESP32::deleteCertificateByNumber "deleteCertificateByNumber()"
 *     - @ref TinyGsmESP32::printCertificateByNumber "printCertificateByNumber()"
 * - Time functions
 *     - @ref TinyGsmESP32::setTimeZone "setTimeZone()"
 *     - @ref TinyGsmESP32::setTimeSyncInterval "setTimeSyncInterval()"
 *
 * # Connection Information
 *
 * - Combined TCP/SSL sockets:
 *   - 5
 *   - Using more than 1 SSL socket at a time may cause the module to crash.
 * - SSL contexts: 2
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
 *
 * @todo In `GsmClientSecureESP32::connect()`: Implement PSK and PSK Identity as
 * they're now supported by newer firmware.
 * @todo In `handleURCs()`: I'm not sure if each +IPD URC reports the amount
 * newly received or the total now in the buffer. It appears to be the latter.
 */
/* clang-format on */

#ifndef SRC_TINYGSMCLIENTESP32_H_
#define SRC_TINYGSMCLIENTESP32_H_
#pragma message("TinyGSM:  TinyGsmClientESP32")

#include "TinyGsmClientEspressif.h"
#include "TinyGsmTCP.tpp"
#include "TinyGsmSSL.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"

/// State: current Wi-Fi state.
/// @ingroup espressif_esp32
enum class ESP32RegStatus {
  /// ESP32 station has not started any Wi-Fi connection.
  REG_UNINITIALIZED = 0,
  /// ESP32 station has connected to an AP, but does not get an IPv4 address
  /// yet.
  REG_UNREGISTERED = 1,
  /// ESP32 station has connected to an AP, and got an IPv4 address.
  REG_OK = 2,
  /// ESP32 station is in Wi-Fi connecting or reconnecting state.
  REG_CONNECTING = 3,
  /// ESP32 station is in Wi-Fi disconnected state.
  REG_DISCONNECTING = 4,
  /// ESP32 station is in an unknown state.
  REG_UNKNOWN = 5,

};

/// Basic modem configurations for the ESP32 modem family
/// @ingroup espressif_esp32
struct TinyGsmESP32ModemConfig
    : public TinyGsmModemConfigPreset<ESP32RegStatus> {
  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "Espressif";
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "ESP32";
};

constexpr char TinyGsmESP32ModemConfig::MODEM_MANUFACTURER[]
    __attribute__((weak));
constexpr char TinyGsmESP32ModemConfig::MODEM_MODEL[] __attribute__((weak));

/**
 * @brief TCP behavior and limits for the ESP32 family.
 *
 * NOTE: There's a total limit of 5 sockets, any of them can be SSL. BUT the
 * manual warns that module may not be able to handle more than 1 SSL socket at
 * a time.
 *
 * These modules don't have "SSL Contexts" per-say, but they only support 2
 * certificate sets.  The certificates are loaded and referenced by number.
 *
 * The ESP32 devices can receive 8192 bytes and send 2920 bytes at most in one
 * single transmission.
 * @ingroup espressif_esp32
 */
struct TinyGsmESP32TcpConfig
    : public TinyGsmTcpConfigPreset<
          /*bufferMode*/ TinyGsmTcpBufferMode::BufferReadAndCheckSize,
          /*muxMode*/ TinyGsmTcpMuxMode::Dynamic,
          /*muxCount*/ 5,
          /*sendMaxSize*/ 2920,
          /*connectTimeoutS*/ 75,  // default
          /*stopTimeoutS*/ 5> {};

/**
 * @brief Class for the Espressif ESP32 modem, which is a Wi-Fi module with SSL
 * support.
 *
 * @warning This class is used to communicate with a module that has been
 * programmed with the AT command firmware.
 * @ingroup espressif_esp32
 */
class TinyGsmESP32
    : public TinyGsmEspressif<TinyGsmESP32, TinyGsmESP32ModemConfig>,
      public TinyGsmTCP<TinyGsmESP32, TinyGsmESP32TcpConfig>,
      public TinyGsmSSL<TinyGsmESP32>,
      public TinyGsmTime<TinyGsmESP32>,
      public TinyGsmNTP<TinyGsmESP32> {
  friend class TinyGsmEspressif<TinyGsmESP32, TinyGsmESP32ModemConfig>;
  friend class TinyGsmModem<TinyGsmESP32, TinyGsmESP32ModemConfig>;
  friend class TinyGsmWifi<TinyGsmESP32>;
  friend class TinyGsmTCP<TinyGsmESP32, TinyGsmESP32TcpConfig>;
  friend class GsmClient<TinyGsmESP32, TinyGsmESP32TcpConfig>;
  friend class TinyGsmSSL<TinyGsmESP32>;
  friend class TinyGsmTime<TinyGsmESP32>;
  friend class TinyGsmNTP<TinyGsmESP32>;

 public:
  using ModemConfig = TinyGsmESP32ModemConfig;
  using TcpConfig   = TinyGsmESP32TcpConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  /// @ingroup espressif_esp32
  class GsmClientESP32 : public GsmClient<TinyGsmESP32, TinyGsmESP32TcpConfig> {
    friend class TinyGsmESP32;

   public:
    using GsmClient<TinyGsmESP32, TinyGsmESP32TcpConfig>::connect;
    using GsmClient<TinyGsmESP32, TinyGsmESP32TcpConfig>::stop;
    using TcpConfig = TinyGsmESP32TcpConfig;

    /**
     * @brief Create a new TCP client.
     * @warning You must call the init() method before attempting to use a
     * client created with this constructor.
     */
    GsmClientESP32() {
      is_secure = false;
    }
    /**
     * @brief Create a new TCP client and bind it to a modem and optionally a
     * multiplexing channel.
     * @param modem Modem instance used by this client.
     * @param mux Multiplexing channel to use.
     *
     * @note The ESP32 allows you choose the multiplexing channel number, but if
     * the input mux channel number is already in use and other mux channels are
     * available, this library will select the next available one.  Use the
     * getMux() function to get the assigned multiplexing channel number after a
     * successful connection.
     */
    explicit GsmClientESP32(TinyGsmESP32& modem, uint8_t mux = 0)
        : GsmClient<TinyGsmESP32, TinyGsmESP32TcpConfig>(modem, mux) {
      is_secure = false;
      init(&modem, mux);
    }

    /**
     * @brief Initialize the TCP client with a modem and optionally a
     * multiplexing channel.
     * @return true if initialization was successful, false otherwise.
     * @copydetails GsmClientESP32::GsmClientESP32(TinyGsmESP32&, uint8_t)
     */
    bool init(TinyGsmESP32* modem, uint8_t mux = 0) {
      if (modem == nullptr) { return false; }
      this->at       = modem;
      sock_connected = false;
      is_mid_send    = false;

      // NOTE: Although the ESP32 would be happy to give us a mux number, we
      // need to assign a mux number here first so that we can assign the
      // pointer for the client in the socket array and in-turn allow the modem
      // to look back at the properties of the client to check if the client
      // needs SSL and, if so, what the SSL specs are.
      // If the mux number returned at the end of the connection process is
      // different from the one we assigned here, we update the position of the
      // pointer to this in the socket array after the connection finishes.

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
    int connect(const char* host, uint16_t port, int timeout_s) override {
      if (at == nullptr) { return 0; }
      is_mid_send = false;
#if 1
      // stop if and only if the mux number is valid, the socket pointer is not
      // null, and the socket is connected
      if (mux < TcpConfig::kMuxCount && at->sockets[mux] != nullptr &&
          sock_connected) {
        stop(TcpConfig::kStopTimeoutS * 1000L);
      }
#endif
      TINY_GSM_YIELD();
      rx.clear();
      // attempt to use the requested mux number first
      uint8_t assignedMux = mux;
      // modemConnect will validate the mux number returned by the modem and
      // return false and set the assignedMux to -1 if the mux number is invalid
      // or the connection fails
      sock_connected = at->modemConnect(host, port, &assignedMux, timeout_s);
      if (sock_connected) {
        // move any existing client at the assigned mux number to the next
        // available slot
        // set the requested mux to -1 to get the  next available mux number
        at->moveSocket(mux, static_cast<uint8_t>(-1));
        // set the client's internal mux number and insert it into the array
        at->sockets[assignedMux] = this;
        mux                      = assignedMux;
      }
      // NOTE: If the sock didn't connect, DO NOT assign an invalid mux number
      // or move the pointer to this client in the modem's sockets array.  The
      // modem still needs to be able to access this client via its mux number
      // in the socket array to check if it's expected to be an SSL connection
      // and, if so, what the SSL specs are.  If we set an invalid mux number or
      // break the alignment between the mux number and the position of the
      // pointer in the array client in the sockets array when the connection
      // fails, the modem loses access to the client.
      return sock_connected;
    }

    void stop(uint32_t maxWaitMs) override {
      if (at == nullptr) { return; }
      is_mid_send = false;
      TINY_GSM_YIELD();
      if (sock_connected || sock_available) {
        // Update available data first, because if the socket was closed
        // externally, the module may have thrown away the data
        at->modemGetAvailable(mux);
        // Now we throw away any remaining data in the modem buffer
        // We explicitly toss it here because the socket will appear open in
        // response to connected() even after it closes until all data is read
        // to give the user a chance to recover the data if they want it.
        dumpModemBuffer(/*maxWaitMs*/);
      }
      // NOTE: It should be safe to only send the close here if sock_connected
      // reads true because the above will have updated sock_connected
      // (dumpModemBuffer calls modemRead until sock_available=0, modemRead
      // calls modemGetAvailable on every read to update sock_available, once
      // sock_available=0 modemGetAvailable calls modemGetConnected, and
      // modemGetConnected updates sock_connected for all sockets.)
      if (sock_connected) { at->modemStop(mux, maxWaitMs); }
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
  /// @ingroup espressif_esp32
  class GsmClientSecureESP32 : public GsmClientESP32, public GsmSecureClient {
    friend class TinyGsmESP32;

   public:
    using GsmClientESP32::connect;
    using GsmClientESP32::stop;
    using TcpConfig = TinyGsmESP32TcpConfig;

    TINY_GSM_SECURE_CLIENT_CTORS(ESP32)

    // Because we have the same potetial range of mux numbers for secure and
    // insecure connections, we don't need to re-check for mux number
    // availability.

    /// @copydoc GsmSecureClient::setCACertName(const char*)
    /// @warning The CA certificate name must be either "client_ca.0" or
    /// "client_ca.1".
    void setCACertName(const char* CAcertName) override {
      if (at == nullptr) { return; }
      if (CAcertName == nullptr || strlen(CAcertName) == 0) { return; }
      // copy the certificate name into owned buffer
      strncpy(CAcertNameBuf, CAcertName, sizeof(CAcertNameBuf) - 1);
      CAcertNameBuf[sizeof(CAcertNameBuf) - 1] = '\0';
      this->CAcertName                         = CAcertNameBuf;
      // parse the certificate name into a number and namespace
      char    cert_namespace[14] = {};
      uint8_t certNumber         = 0;
      at->parseCertificateName(CAcertName, cert_namespace, certNumber);
      ca_number = certNumber;
    }
    /// @copydoc GsmClientSecureESP32::setCACertName(const char*)
    void setCACertName(String CAcertName) override {
      setCACertName(CAcertName.c_str());
    }

    /**
     * @copydoc GsmSecureClient::setClientCertName(const char*)
     * @warning The client certificate name must be either "client_cert.0" or
     * "client_cert.1".
     * @note The client certificate number and the client private key number
     * must be the same for a given connection.  Calling either
     * setClientCertName() or setPrivateKeyName() will set the other
     * to the equivalent name with the same number.
     */
    void setClientCertName(const char* clientCertName) override {
      if (at == nullptr) { return; }
      if (clientCertName == nullptr || strlen(clientCertName) == 0) { return; }
      // copy the certificate name into owned buffer
      strncpy(clientCertNameBuf, clientCertName, sizeof(clientCertNameBuf) - 1);
      clientCertNameBuf[sizeof(clientCertNameBuf) - 1] = '\0';
      this->clientCertName                             = clientCertNameBuf;
      // parse the certificate name into a number and namespace
      char    cert_namespace[14] = {};
      uint8_t certNumber         = 0;
      at->parseCertificateName(clientCertName, cert_namespace, certNumber);
      // set the private key number
      pki_number = certNumber;
      // generate the matching client private key name from the certificate
      // number and type
      char cert_name[16] = {};
      at->getCertificateName(CertificateType::CLIENT_KEY, certNumber, cert_name,
                             cert_namespace);
      strncpy(clientKeyNameBuf, cert_name, sizeof(clientKeyNameBuf) - 1);
      clientKeyNameBuf[sizeof(clientKeyNameBuf) - 1] = '\0';
      clientKeyName                                  = clientKeyNameBuf;
    }
    /// @copydoc GsmClientSecureESP32::setClientCertName(const char*)
    void setClientCertName(String clientCertName) override {
      setClientCertName(clientCertName.c_str());
    }

    /**
     * @copydoc GsmSecureClient::setPrivateKeyName(const char*)
     * @warning The private key name must be either "client_key.0" or
     * "client_key.1".
     * @note The client certificate number and the client private key number
     * must be the same for a given connection.  Calling either
     * setClientCertName() or setPrivateKeyName() will set the other
     * to the equivalent name with the same number.
     */
    void setPrivateKeyName(const char* clientKeyName) override {
      if (at == nullptr) { return; }
      if (clientKeyName == nullptr || strlen(clientKeyName) == 0) { return; }
      // copy the key name into owned buffer
      strncpy(clientKeyNameBuf, clientKeyName, sizeof(clientKeyNameBuf) - 1);
      clientKeyNameBuf[sizeof(clientKeyNameBuf) - 1] = '\0';
      this->clientKeyName                            = clientKeyNameBuf;
      // parse the certificate name into a number and namespace
      char    cert_namespace[14] = {};
      uint8_t certNumber         = 0;
      at->parseCertificateName(clientKeyName, cert_namespace, certNumber);
      // set the private key number
      pki_number = certNumber;
      // generate the matching client certificate name from the private key
      // number and type
      char cert_name[16] = {};
      at->getCertificateName(CertificateType::CLIENT_CERTIFICATE, certNumber,
                             cert_name, cert_namespace);
      // set the client certificate name
      strncpy(clientCertNameBuf, cert_name, sizeof(clientCertNameBuf) - 1);
      clientCertNameBuf[sizeof(clientCertNameBuf) - 1] = '\0';
      clientCertName                                   = clientCertNameBuf;
    }
    /// @copydoc GsmClientSecureESP32::setPrivateKeyName(const char*)
    void setPrivateKeyName(String clientKeyName) override {
      setPrivateKeyName(clientKeyName.c_str());
    }

    /**
     * @brief Set the CA certificate number to use for this connection
     * @param certNumber The CA certificate number, must be 0 or 1.
     */
    void setCACertificateNumber(uint8_t certNumber) {
      if (at == nullptr) { return; }
      ca_number = certNumber;
      // convert the certificate number and type into the proper certificate
      // names for the ESP32
      char cert_name[16]      = {};
      char cert_namespace[14] = {};
      at->getCertificateName(CertificateType::CA_CERTIFICATE, certNumber,
                             cert_name, cert_namespace);
      memcpy(CAcertNameBuf, cert_name, sizeof(CAcertNameBuf));
      CAcertNameBuf[sizeof(CAcertNameBuf) - 1] = '\0';
      CAcertName                               = CAcertNameBuf;
    }
    /**
     * @brief Set the client certificate number to use for this connection
     * @param certNumber The client certificate number, must be 0 or 1.
     * @note The client certificate number and the client private key number
     * must be the same for a given connection.  Calling either
     * setClientCertificateNumber() or setPrivateKeyNumber() will set the other
     * to the same number.
     */
    void setClientCertificateNumber(uint8_t certNumber) {
      if (at == nullptr) { return; }
      pki_number = certNumber;
      // generate and set the name for the client certificate from the number
      char cert_name[16]      = {};
      char cert_namespace[14] = {};
      at->getCertificateName(CertificateType::CLIENT_CERTIFICATE, certNumber,
                             cert_name, cert_namespace);
      memcpy(clientCertNameBuf, cert_name, sizeof(clientCertNameBuf));
      clientCertNameBuf[sizeof(clientCertNameBuf) - 1] = '\0';
      clientCertName                                   = clientCertNameBuf;
      // generate and set the name for the client private key from the number
      at->getCertificateName(CertificateType::CLIENT_KEY, certNumber, cert_name,
                             cert_namespace);
      memcpy(clientKeyNameBuf, cert_name, sizeof(clientKeyNameBuf));
      clientKeyNameBuf[sizeof(clientKeyNameBuf) - 1] = '\0';
      clientKeyName                                  = clientKeyNameBuf;
    }
    /**
     * @brief Set the client private key number to use for this connection
     * @param keyNumber The client private key number, must be 0 or 1.
     * @note The client certificate number and the client private key number
     * must be the same for a given connection.  Calling either
     * setClientCertificateNumber() or setPrivateKeyNumber() will set the other
     * to the same number.
     */
    void setPrivateKeyNumber(uint8_t keyNumber) {
      setClientCertificateNumber(keyNumber);
    }

   protected:
    int8_t ca_number;
    int8_t pki_number;
    char   CAcertNameBuf[16];
    char   clientCertNameBuf[16];
    char   clientKeyNameBuf[16];
  };

  /*
   * GSM Modem Constructor
   */
 public:
  /**
   * @brief Construct a modem wrapper around a stream transport.
   * @param stream Stream used to communicate with the modem.
   */
  explicit TinyGsmESP32(Stream& stream)
      : TinyGsmEspressif<TinyGsmESP32, TinyGsmESP32ModemConfig>(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientEspressif"));
    bool success = true;

    if (!testAT()) { return false; }
    if (pin && strlen(pin) > 0) {
      DBG("Espressif modules do not use an unlock pin!");
    }
    sendAT(GF("E0"));  // Echo Off
    success &= waitResponse() == 1;

#ifdef TINY_GSM_DEBUG
    sendAT(GF("+SYSLOG=1"));  // turn on verbose error codes
#else
    sendAT(GF("+SYSLOG=0"));  // turn off error codes
#endif
    waitResponse();

    sendAT(GF("+CIPMUX=1"));  // Enable Multiple Connections
    success &= waitResponse() == 1;
    sendAT(GF("+CWMODE=1"));  // Put into "station" mode
    if (waitResponse() != 1) {
      sendAT(GF("+CWMODE_CUR=1"));  // Attempt "current" station mode command
                                    // for some firmware variants if needed
      success &= waitResponse() == 1;
    }
    sendAT(GF("+CIPDINFO=0"));  // do not show the remote host and port in
                                // “+IPD” and “+CIPRECVDATA” messages.
    success &= waitResponse() == 1;

    // Set the data receive mode to have the module buffer data for all
    // connections AT+CIPRECVTYPE=<link ID>,<mode>
    // <link ID>: ID of the connection (0 ~ max). For a single connection, <link
    //   ID> is 0. For multiple connections, if the value is max, it means all
    //   connections. Max is 5 by default.
    // <mode>: the receive mode of socket data. Default: 0.
    //  0: active mode. ESP-AT will send all the received socket data instantly
    //    to the host MCU with the header “+IPD”. (The socket receive window is
    //    5760 bytes by default. The maximum valid bytes sent to MCU is 2920
    //    bytes each time.)
    //  1: passive mode. ESP-AT will keep the received socket data in an
    //    internal buffer (socket receive window, 5760 bytes by default), and
    //    wait for the host MCU to read. If the buffer is full, the socket
    //    transmission will be blocked for TCP/SSL connections, or data will be
    //    lost for UDP connections.
    sendAT(GF("+CIPRECVTYPE=5,1"));
    success &= waitResponse() == 1;

    DBG(GF("### Modem:"), getModemName());
    return success;
  }

  void maintainImpl() {
    // Keep listening for modem URC's and proactively iterate through
    // sockets asking if any data is available
    bool check_socks = false;
    for (int mux = 0; mux < TcpConfig::kMuxCount; mux++) {
      GsmClientESP32* sock = sockets[mux];
      if (sock && sock->got_data) {
        sock->got_data = false;
        if (sock->sock_available == 0) {
          // NOTE: Only check the socket if says it got new data by the amount
          // available is 0. This avoids extra un-needed checks.
          check_socks = true;
        }
      }
    }
    // modemGetAvailable checks all socks, so we only want to do it once
    // modemGetAvailable calls modemGetConnected(), which also checks all socks
    if (check_socks) { modemGetAvailable(0); }
    while (stream.available()) { waitResponse(15, nullptr, nullptr); }
  }

  /*
   * Power functions
   */
 protected:
  // NOTE: The ESP32 series only supports deep sleep with a wake-up timer.  It
  // cannot be put to deep sleep indefinitely. This function is not implemented.
  bool powerOffImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;

  bool radioOffImpl() {
    bool success = true;

    // AT+CWINIT=<init>
    // <init>:
    //   0: Deinitialize Wi-Fi driver of ESP32 device.
    //   1: Initialize Wi-Fi driver of ESP32 device. (Default value)
    sendAT(GF("+CWINIT=0"));
    success &= waitResponse() == 1;

    // AT+BTINIT=<init>
    //  <init>:
    //    0: Deinitialize Classic Bluetooth.
    //    1: Initialize Classic Bluetooth.
    sendAT(GF("+BTINIT=0"));
    success &= waitResponse() == 1;

    // AT+BLEINIT=<init>
    // <init>:
    //   0: deinit Bluetooth LE
    //   1: client role
    //   2: server role
    //   3: dual role (client and server)
    sendAT(GF("+BLEINIT=0"));
    success &= waitResponse() == 1;

    return success;
  };

  /*
   * Generic network functions
   */
 protected:
  ESP32RegStatus getRegistrationStatusImpl() {
    sendAT(GF("+CWSTATE?"));
    if (waitResponse(3000, GF("+CWSTATE:")) != 1)
      return ESP32RegStatus::REG_UNKNOWN;
    // +CWSTATE:{state},{"ssid"}
    // followed by an OK
    int8_t status = streamGetIntBefore(',');
    streamSkipUntil('\n');  // throw away the ssid
    waitResponse();         // wait for trailing OK
    return static_cast<ESP32RegStatus>(status);
  }

  bool isNetworkConnectedImpl() {
    ESP32RegStatus s = this->getRegistrationStatus();
    return (s == ESP32RegStatus::REG_OK);
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // NOTE: You must be running AT firmware >= 3.2.0.0 for these functions to
  // work. If you are running a lower level firmware, you must update. You
  // almost certainly will need to flash your board with the new firmware using
  // esptools/ESP flash download tools instead of using the AT+CIUPDATE function
  // because the structure of the NVM space changed and the newer structure is
  // needed for the SYSMFG command used here. The CIUPDATE function does not
  // update the NVM.

  // NOTE: In firmware release notes, it says that more than 5 sets of CA
  // certificates are supported, but all other command examples and descriptions
  // of the flash memory storage for the certificates mention only 2 possible
  // sets (0 and 1).

 public:
  // This adds the server's CA certificate that the client connects to, used
  // in auth mode 2 and 3
  // This is the value client_ca_0x.crt in the AT firmware
  /**
   * @brief Load a CA certificate into the modem's flash memory for SSL
   * connections.
   *
   * @param certNumber The certificate number to load.
   * @param cert The certificate data.
   * @param len The length of the certificate data.
   * @return True if the certificate was successfully loaded, false otherwise.
   */
  bool loadCACert(uint8_t certNumber, const char* cert, const uint16_t len) {
    return loadCertificateByNumber(CertificateType::CA_CERTIFICATE, certNumber,
                                   cert, len);
  }

  /**
   * @brief Load a client certificate into the modem's flash memory for SSL
   * connections.
   *
   * @param certNumber The certificate number to load.
   * @param cert The certificate data.
   * @param len The length of the certificate data.
   * @return True if the certificate was successfully loaded, false otherwise.
   */
  bool loadClientCert(uint8_t certNumber, const char* cert,
                      const uint16_t len) {
    return loadCertificateByNumber(CertificateType::CLIENT_CERTIFICATE,
                                   certNumber, cert, len);
  }

  /**
   * @brief Load a private key into the modem's flash memory for SSL
   * connections.
   *
   * @param keyNumber The key number to load.
   * @param key The key data.
   * @param len The length of the key data.
   * @return True if the key was successfully loaded, false otherwise.
   */
  bool loadPrivateKey(uint8_t keyNumber, const char* key, const uint16_t len) {
    return loadCertificateByNumber(CertificateType::CLIENT_KEY, keyNumber, key,
                                   len);
  }

  /**
   * @brief Load a certificate into the modem's flash memory for SSL connections
   * by its number.
   *
   * @param cert_type The type of certificate to load.
   * @param certNumber The certificate number to load.
   * @param cert The certificate data.
   * @param len The length of the certificate data.
   * @return True if the certificate was successfully loaded, false otherwise.
   */
  bool loadCertificateByNumber(CertificateType cert_type, uint8_t certNumber,
                               const char* cert, const uint16_t len) {
    if (cert_type == CertificateType::CLIENT_PSK ||
        cert_type == CertificateType::CLIENT_PSK_IDENTITY) {
      return false;
    }
    // convert the certificate number and type into the proper certificate names
    // for the ESP32
    char* cert_name      = new char[16]();
    char* cert_namespace = new char[14]();
    getCertificateName(cert_type, certNumber, cert_name, cert_namespace);
    // add the certificate by name/namespace
    return loadCertificateWithNamespace(cert_namespace, cert_name, cert, len);
  }

  /**
   * @brief Delete a certificate from the modem's flash memory for SSL
   * connections by its number.
   *
   * @param cert_type The type of certificate to delete.
   * @param certNumber The certificate number to delete.
   * @return True if the certificate was successfully deleted, false otherwise.
   */
  bool deleteCertificateByNumber(CertificateType cert_type,
                                 uint8_t         certNumber) {
    if (cert_type == CertificateType::CLIENT_PSK ||
        cert_type == CertificateType::CLIENT_PSK_IDENTITY) {
      return false;
    }
    // convert the certificate number and type into the proper certificate names
    // for the ESP32
    char* cert_name      = new char[16]();
    char* cert_namespace = new char[14]();
    getCertificateName(cert_type, certNumber, cert_name, cert_namespace);
    // delete the certificate by name/namespace
    return deleteCertificateWithNamespace(cert_namespace, cert_name);
  }

  /**
   * @brief Print a certificate from the modem's flash memory for SSL
   * connections by its number to a stream.
   *
   * @param cert_type The type of certificate to print.
   * @param certNumber The certificate number to print.
   * @param print_stream The stream to print the certificate to.
   * @return True if the certificate was successfully printed, false otherwise.
   */
  bool printCertificateByNumber(CertificateType cert_type, uint8_t certNumber,
                                Stream& print_stream) {
    if (cert_type == CertificateType::CLIENT_PSK ||
        cert_type == CertificateType::CLIENT_PSK_IDENTITY) {
      return false;
    }
    // convert the certificate number and type into the proper certificate names
    // for the ESP32
    char* cert_name      = new char[16]();
    char* cert_namespace = new char[14]();
    getCertificateName(cert_type, certNumber, cert_name, cert_namespace);
    // delete the certificate by name/namespace
    return printCertificateWithNamespace(cert_namespace, cert_name,
                                         print_stream);
  }

 protected:
  void parseCertificateName(const char* cert_name, char* parsed_namespace,
                            uint8_t& parsed_number) {
    // pull the namespace out of the name
    memcpy(parsed_namespace, cert_name, strlen(cert_name) - 2);
    parsed_namespace[strlen(cert_name) - 2] = '\0';
    // pull the number out of the name
    char certNumber[2];
    memcpy(certNumber, cert_name + strlen(cert_name) - 1, 1);
    // Null terminate
    memset(certNumber + 1, '\0', 1);
    parsed_number = atoi(certNumber);
  }

  void getCertificateName(CertificateType cert_type, uint8_t certNumber,
                          char* cert_name, char* cert_namespace) {
    char cert_number[2];  // Must be '0' or '1', so 2 bytes is enough
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
    strcat(cert_name, ".");
    strcat(cert_name, cert_number);
    return;
  }

  bool loadCertificateWithNamespace(char* certNamespace, char* certificateName,
                                    const char* cert, const uint16_t len) {
    // delete any old text in the cert first
    deleteCertificateWithNamespace(certNamespace, certificateName);
    // AT+SYSMFG=<operation>,<"namespace">,<"key">,<type>,<value>
    // operation = 2 for write
    // type = 8 for binary (ie, the certificates must be stored in binary,
    // though you can enter them as strings)
    // Write a new value for client_cert.0 key into client_cert namespace
    // (That is, update the 0th client certificate)
    // AT+SYSMFG=2,"client_cert","client_cert.0",8,1164
    // Wait until AT command port returns ``>``, and then write 1164 bytes

    // data = at_update_param('client_ca.0', 'B', args.client_ca0, data)
    //        at_update_param(key,          type, value,           data):

    sendAT(GF("+SYSMFG=2,\""), certNamespace, GF("\",\""), certificateName,
           GF("\",8,"), len);
    if (waitResponse(GF(">")) != 1) { return false; }
    stream.write(reinterpret_cast<const uint8_t*>(cert), len);
    stream.flush();
    if (waitResponse(10000L) != 1) { return false; }
    return true;
  }


  bool deleteCertificateWithNamespace(char* certNamespace,
                                      char* certificateName) {
    // AT+SYSMFG=<operation>,<"namespace">[,<"key">]
    // operation = 0 for erase
    // AT+SYSMFG=0,"client_cert","client_cert.0"
    sendAT(GF("+SYSMFG=0,\""), certNamespace, GF("\",\""), certificateName,
           '"');
    return waitResponse() == 1;
  }


  bool printCertificateWithNamespace(char* certNamespace, char* certificateName,
                                     Stream& print_stream) {
    // AT+SYSMFG=<operation>,<"namespace">,<"key">,<type>,<value>
    // operation = 1 for read
    // type = 8 for binary (ie, the certificates must be stored in binary,
    // though you can enter them as strings)
    // Read the value of client_cert.0 key from client_cert namespace
    // (That is, read the 0th client certificate)
    // AT+SYSMFG=1,"client_cert","client_cert.0",8
    sendAT(GF("+SYSMFG=1,\""), certNamespace, GF("\",\""), certificateName,
           "\"");
    if (waitResponse(GF("+SYSMFG:")) != 1) { return false; }
    streamSkipUntil(',');  // skip the returned namespace
    streamSkipUntil(',');  // skip the returned name
    streamSkipUntil(',');  // skip the returned filetype (should be 8)
    // read the length of the certificate
    int print_len = streamGetIntBefore(',');

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
    // Final OK
    return waitResponse() == 1;
  }


  bool loadCertificateImpl(const char* certificateName, const char* cert,
                           const uint16_t len) {
    // parse the certificate name into a number and namespace
    char*   cert_namespace = new char[14]();
    uint8_t certNumber     = 0;
    parseCertificateName(certificateName, cert_namespace, certNumber);
    // add the certificate by name
    return loadCertificateWithNamespace(
        cert_namespace, const_cast<char*>(certificateName), cert, len);
  }

  bool deleteCertificateImpl(const char* certificateName) {
    // parse the certificate name into a number and namespace
    char*   cert_namespace = new char[14]();
    uint8_t certNumber     = 0;
    parseCertificateName(certificateName, cert_namespace, certNumber);
    // add the certificate by name
    return deleteCertificateWithNamespace(cert_namespace,
                                          const_cast<char*>(certificateName));
  }


  bool printCertificateImpl(const char* filename, Stream& print_stream) {
    // parse the certificate name into a number and namespace
    char*   cert_namespace = new char[14]();
    uint8_t certNumber     = 0;
    parseCertificateName(filename, cert_namespace, certNumber);
    // add the certificate by name
    return printCertificateWithNamespace(
        cert_namespace, const_cast<char*>(filename), print_stream);
  }

  bool convertCertificateImpl(CertificateType cert_type, const char*) {
    if (cert_type == CertificateType::CLIENT_PSK ||
        cert_type == CertificateType::CLIENT_PSK_IDENTITY) {
      // The ESP32 does not support SSL using pre-shared keys with AT firmware.
      return false;
    }
    return true;  // no conversion needed on the ESP32
  }
  bool convertCACertificateImpl(const char*) {
    return true;
  }
  bool convertClientCertificatesImpl(const char*, const char*) {
    return true;  // no conversion needed on the ESP32
  }
  bool convertPSKandIDImpl(const char*, const char*) {
    // The ESP32 does not support SSL using pre-shared keys with AT firmware.
    return false;
  }
  bool convertPSKTableImpl(const char*) {
    return true;
  }

  /*
   * Wifi functions
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
 public:
  /**
   * @brief Set the time zone for the modem and optionally enable SNTP time
   * synchronization.
   * @param timezone The time zone offset in hours from UTC (e.g., -5 for EST, 1
   * for CET).
   * @param enable_sync Optional parameter to enable or disable SNTP time
   * synchronization. Defaults to true (enabled).
   */
  void setTimeZone(int8_t timezone, bool enable_sync = true) {
    // configure the NTP settings for the modem
    sendAT(GF("+CIPSNTPCFG="), enable_sync ? 1 : 0, ',', timezone);
    waitResponse();
  }

  /**
   * @brief Set the time synchronization interval for the modem.
   * @param seconds The number of seconds between time synchronization attempts.
   */
  void setTimeSyncInterval(uint16_t seconds) {
    // configure the NTP settings for the modem
    sendAT(GF("+CIPSNTPINTV="), seconds);
    waitResponse();
  }

 protected:
  bool waitForTimeSyncImpl(int timeout_s) {
    // if we're not connected, we'll never get the time
    if (!isNetworkConnected()) {
      DBG(GF("### Not connected to network; cannot sync time!"));
      return false;
    }
    // if SNTP sync isn't enabled, we won't have the time
    // NOTE: We don't actually enable the time here, because doing so would
    // change any user settings for the timezone and time servers.
    sendAT(GF("+CIPSNTPCFG?"));
    waitResponse(2000L, GF("+CIPSNTPCFG:"));
    int8_t is_enabled = streamGetIntBefore(',');
    waitResponse(5000L);  // returns OK at the end
    if (!is_enabled) {
      DBG(GF("### SNTP sync not enabled; cannot sync time!"));
      return false;
    }
    // if we're sure we should be able to get the time, wait for it
    uint32_t start_millis = millis();
    while (millis() - start_millis < static_cast<uint32_t>(timeout_s) * 1000) {
      uint32_t modem_time = getNetworkEpoch();
      // If we get a time between January 1, 2020 and January 1, 2035, we're
      // (hopefully) good
      if (modem_time > 1577836800 && modem_time < 2051222400) { return true; }
      delay(250);
    }
    return false;
  }

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
    itimezone = streamGetFloatBefore(',');
    // Final OK
    waitResponse(5000L);

    // Set pointers
    if (iyear < 2000) iyear += 2000;
    if (year != nullptr) *year = iyear;
    if (month != nullptr) *month = imonth;
    if (day != nullptr) *day = iday;
    if (hour != nullptr) *hour = ihour;
    if (minute != nullptr) *minute = imin;
    if (second != nullptr) *second = isec;
    if (timezone != nullptr) *timezone = itimezone;

    // Validate parsed values
    // NOTE: This is a basic validation and does not account for leap years or
    // the number of days in each month.
    if (iyear < 2000 || imonth < 1 || imonth > 12 || iday < 1 || iday > 31 ||
        ihour < 0 || ihour > 23 || imin < 0 || imin > 59 || isec < 0 ||
        isec > 59 || itimezone < -48 || itimezone > 56) {
      return false;
    }

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

  uint32_t getNetworkEpochImpl(TinyGSM_EpochStart epoch) {
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
      modem_time     = atol(buf);
    }
    waitResponse();
    DBG(GF("### Modem Raw Time:"), buf, '(', modem_time, ')');

    if (modem_time != 0) {
      switch (epoch) {
        case TinyGSM_EpochStart::UNIX: modem_time += 0; break;
        case TinyGSM_EpochStart::Y2K: modem_time += 946684800; break;
        case TinyGSM_EpochStart::GPS: modem_time += 315878400; break;
      }
    }
    DBG(GF("### Modem Epoch Time:"), modem_time);

    return modem_time;
  }

  /*
   * NTP server functions
   */
 protected:
  // NOTE: I don't think this forces an immediate sync
  bool NTPServerSyncImpl(const char* server, int TimeZone) {
    // configure the NTP settings for the modem
    sendAT(GF("+CIPSNTPCFG="), 1, ',', TimeZone, GF(",\""), server, '"');
    // returns a simple OK or ERROR
    return waitResponse() == 1;
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
   * Client-related functions
   */
 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t* dynamicMux,
                        int timeout_s) {
    uint32_t timeout_ms    = ((uint32_t)timeout_s) * 1000;
    uint8_t  requested_mux = *dynamicMux;
    bool     ssl           = sockets[requested_mux]->is_secure;

    // Blank holders for the SSL auth mode and certificates
    SSLAuthMode sslAuthMode = SSLAuthMode::NO_VALIDATION;
    uint8_t     ca_number   = 0;
    uint8_t     pki_number  = 0;
    const char* pskIdent    = nullptr;
    const char* psKey       = nullptr;
    // If we actually have a secure socket populate the above with real values
    if (ssl) {
      const GsmClientSecureESP32* thisClient =
          static_cast<const GsmClientSecureESP32*>(sockets[requested_mux]);
      sslAuthMode = thisClient->sslAuthMode;
      ca_number   = thisClient->ca_number;
      pki_number  = thisClient->pki_number;
      pskIdent    = thisClient->pskIdent;
      psKey       = thisClient->psKey;
    }

    if (ssl) {
      if (sslAuthMode == SSLAuthMode::PRE_SHARED_KEYS) { return false; }
      // TODO: Implement PSK and PSK Identity


      // SSL certificate checking will not work without a valid timestamp!
      if (sockets[requested_mux] != nullptr &&
          (sslAuthMode != SSLAuthMode::NO_VALIDATION) &&
          !waitForTimeSync(timeout_s)) {
        DBG("### WARNING: The module timestamp must be valid for SSL auth. "
            "Please use setTimeZone(...) or NTPServerSync(...) to enable "
            "time syncing before attempting an SSL connection!");
        return false;
      }

      // configure SSL authentication type and in-use certificates
      // AT+CIPSSLCCONF=<link ID>,{auth_mode}[,{pki_number}][,{ca_number}]
      // <link ID>: ID of the connection (0 ~ max). For multiple connections, if
      // the value is max, it means all connections. By default, max is 5.
      // auth_mode:
      //     0: no authentication. In this case <pki_number> and <ca_number> are
      //     not required.
      //     1: the client provides the client certificate for the server to
      //     verify.
      //     2: the client loads CA certificate to verify the server’s
      //     certificate.
      //     3: mutual authentication.
      // pki_number: the index of certificate and private key. If there is
      // only one certificate and private key, the value should be 0.
      //    PKI - A public key infrastructure (PKI) is a set of roles, policies,
      //    hardware, software and procedures needed to create, manage,
      //    distribute, use, store and revoke digital certificates and manage
      //    public-key encryption.
      // ca_number: the index of CA (certificate authority certificate =
      // server's certificate). If there is only one CA, the value should be 0.
      // The PKI number and CA number to use are based on what certificates were
      // (or were not) put into the customized certificate partitions.
      // The default firmware comes with espressif certificates in slots 0
      // and 1.
      if (sockets[requested_mux] == nullptr ||
          (sslAuthMode == SSLAuthMode::NO_VALIDATION)) {
        sendAT(GF("+CIPSSLCCONF="), requested_mux, GF(",0"));
      } else if (sslAuthMode != SSLAuthMode::PRE_SHARED_KEYS) {
        // For auth modes 1, 2, and 3, we need to specify the PKI and CA numbers
        sendAT(GF("+CIPSSLCCONF="), requested_mux, ',',
               static_cast<uint8_t>(sslAuthMode), ',', pki_number, ',',
               ca_number);
      } else {
        // NOTE: Support for this is firmware dependent!
        // AT+CIPSSLCPSK=<link ID>,<"psk">,<"hint">
        if (psKey == nullptr || pskIdent == nullptr) {
          DBG("### PSK authentication requires both a PSK and a PSK identity!");
          return false;
        }
        sendAT(GF("+CIPSSLCPSK="), requested_mux, GF(",\""), psKey, GF("\",\""),
               pskIdent, '"');
      }
      waitResponse();

      // set the SSL SNI (server name indication)
      // Multiple connections: (AT+CIPMUX=1)
      // AT+CIPSSLCSNI=<link ID>,<"sni">
      // NOTE: On firmware versions above 0.4.2 this happens automatically, but
      // on older versions it must be done manually.
      sendAT(GF("+CIPSSLCSNI="), requested_mux, GF(",\""), host, '"');
      waitResponse();
    }

    // If you need to use a domain name and the length of the domain name
    // exceeds 64 bytes, use the AT+CIPDOMAIN command to obtain the IP address
    // corresponding to the domain name, and then use the IP address to
    // establish a connection.
    if (strlen(host) > 64) {
      // AT+CIPDOMAIN=<"domain name">[,<ip network>][,<timeout>]
      sendAT(GF("+CIPDOMAIN=\""), host, '"');
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

    // Select TCP/IP transmission mode (normal mode)
    sendAT(GF("+CIPMODE=0"));
    waitResponse();

    // Make the connection
    sendAT(GF("+CIPSTART="), requested_mux, ',',
           ssl ? GF("\"SSL") : GF("\"TCP"), GF("\",\""), host, GF("\","), port
#if defined(TINY_GSM_TCP_KEEP_ALIVE)
           ,
           ',', TINY_GSM_TCP_KEEP_ALIVE
#endif
    );

    String data;
    bool   success = waitResponse(timeout_ms, data, GFP(ModemConfig::GSM_OK),
                                  GFP(ModemConfig::GSM_ERROR),
                                  GF("ALREADY CONNECT")) == 1;
    if (success && data.length() > 8) {
      int coma          = data.indexOf(',');
      int connected_mux = data.substring(0, coma).toInt();

      // Validate the returned mux
      if (coma < 0 || connected_mux < 0 ||
          connected_mux >= TcpConfig::kMuxCount) {
        DBG(GF("ERROR: Modem returned invalid mux or connection failed"));
        *dynamicMux = static_cast<uint8_t>(-1);  // Set mux to invalid value
        return false;  // Return failure when mux is out of range
      }
      *dynamicMux = connected_mux;
    } else {
      *dynamicMux =
          static_cast<uint8_t>(-1);  // Set mux to invalid value on failure
    }
    return success;
  }

  // Disambiguate modemStopImpl by using the Espressif implementation
  using TinyGsmEspressif<TinyGsmESP32, TinyGsmESP32ModemConfig>::modemStopImpl;

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
    if (waitResponse(30000L, GF("SEND OK\r\n"), GF("SEND FAIL\r\n"),
                     GFP(ModemConfig::GSM_ERROR)) != 1) {
      return 0;
    }
    if (received != len) { DBG("### Sent:", received, "of", len); }
    return len;
  }

  size_t modemReadImpl(size_t size, uint8_t mux) {
    if (!sockets[mux]) return 0;

    // AT+CIPRECVDATA=<link_id>,<len>
    sendAT(GF("+CIPRECVDATA="), mux, ',', (uint16_t)size);
    // +CIPRECVDATA:<actual_len>,<"remote IP">,<remote port>,<data>
    if (waitResponse(GF("+CIPRECVDATA:")) != 1) { return 0; }
    size_t len_reported = streamGetIntBefore(',');
    size_t len_read     = moveCharsFromStreamToFifo(mux, len_reported);
    waitResponse();  // final ok

    // Check how much is left in the buffer after reading.
    sockets[mux]->sock_available = modemGetAvailable(mux);
    return len_read;
  }

  size_t modemGetAvailableImpl(uint8_t mux) {
    size_t result = 0;
    sendAT(GF("+CIPRECVLEN?"));
    if (waitResponse(GF("+CIPRECVLEN:")) != 1) { return result; }
    for (int muxNo = 0; muxNo < TcpConfig::kMuxCount; muxNo++) {
      long mux_avail = stream.parseInt();
      if (sockets[muxNo]) { sockets[muxNo]->sock_available = mux_avail; }
    }
    waitResponse();  // ends with OK
    result = sockets[mux]->sock_available;
    if (!result) { sockets[mux]->sock_connected = modemGetConnected(mux); }
    return result;
  }

  bool modemGetConnectedImpl(uint8_t mux) {
    sendAT(GF("+CIPSTATE?"));
    // initialize the connection array assuming no connections are active
    bool verified_connections[TcpConfig::kMuxCount] = {0};
    for (int muxNo = 0; muxNo < TcpConfig::kMuxCount; muxNo++) {
      uint8_t has_status = waitResponse(GF("+CIPSTATE:"),
                                        GFP(ModemConfig::GSM_OK),
                                        GFP(ModemConfig::GSM_ERROR));
      if (has_status == 1) {
        int8_t returned_mux = streamGetIntBefore(',');
        streamSkipUntil(',');   // Skip type
        streamSkipUntil(',');   // Skip remote IP
        streamSkipUntil(',');   // Skip remote port
        streamSkipUntil(',');   // Skip local port
        streamSkipUntil('\n');  // Skip client/server type
        if (returned_mux >= 0 && returned_mux < TcpConfig::kMuxCount) {
          verified_connections[returned_mux] = 1;
        }
      } else {
        break;
      };  // once we get to the ok or error, stop
    }
    for (int muxNo = 0; muxNo < TcpConfig::kMuxCount; muxNo++) {
      if (sockets[muxNo]) {
        sockets[muxNo]->sock_connected = verified_connections[muxNo];
      }
    }
    return verified_connections[mux];
  }

  /*
   * Utilities
   */
 protected:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+IPD,"))) {
      int8_t   mux = streamGetIntBefore(',');
      uint16_t len = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TcpConfig::kMuxCount && sockets[mux]) {
        sockets[mux]->got_data = true;
        // TODO: I'm not sure if each +IPD URC reports the amount newly received
        // or the total now in the buffer. It appears to be the latter.
        // sockets[mux]->sock_available = sockets[mux]->sock_available + len;
        sockets[mux]->sock_available = len;
      }
      data = "";
      DBG("### Got Data:", len, "on", mux);
      return true;
    } else if (data.endsWith(GF("CLOSED"))) {
      int muxStart =
          TinyGsmMax(0,
                     data.lastIndexOf(String(GFP(ModemConfig::GSM_NL)),
                                      data.length() - 8));
      int coma = data.indexOf(',', muxStart);
      int mux  = data.substring(muxStart, coma).toInt();
      if (mux >= 0 && mux < TcpConfig::kMuxCount && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      streamSkipUntil('\n');  // throw away the new line
      data = "";
      DBG("### Closed: ", mux);
      return true;
    } else if (data.endsWith(GF("ERR CODE:"))) {
#if defined(TINY_GSM_DEBUG) && !defined(DUMP_AT_COMMANDS)
      DBG("### ERR CODE: ", stream.readStringUntil('\n'));
#else
      streamSkipUntil('\n');  // Read out the ERR CODE
#endif
      data = "";
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
    } else if (data.endsWith(GF("ready\r\n"))) {
      streamSkipUntil('\n');
      data = "";
      // DBG("### Module ready!");
      return true;
    } else if (data.endsWith(GF("WIFI GOT IP"))) {
      // WIFI GOT IP; WIFI GOT IPv6 LL; WIFI GOT IPv6 GL
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
  GsmClientESP32* sockets[TcpConfig::kMuxCount];
};

#endif  // SRC_TINYGSMCLIENTESP32_H_
