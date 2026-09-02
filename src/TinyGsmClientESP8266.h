/**
 * @file       TinyGsmClientESP8266.h
 * @brief      ESP8266 AT modem client and modem-trait definitions.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */
/* clang-format off */
/**
 * @defgroup espressif_esp8266 Espressif ESP8266 AT Modem Family
 * @ingroup espressif_at
 * @brief Manufacturer: Espressif. Models: ESP8266 (AT firmware version
 * ~2.3.0.0).
 *
 * Documentation for the ESP8266 AT firmware can be found here:
 * https://docs.espressif.com/projects/esp-at/en/release-v2.3.0.0_esp8266/Get_Started/index.html
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
 *     - @ref TinyGsmModem<modemType, modemConfig>::streamDump "streamDump()"
 * - WiFi functions (TinyGsmWifi.tpp)
 *     - @ref TinyGsmWifi<modemType>::networkConnect "networkConnect()"
 *     - @ref TinyGsmWifi<modemType>::networkDisconnect "networkDisconnect()"
 * - TCP functions (TinyGsmTCP.tpp)
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::maintain "maintain()"
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
 * - Time functions
 *     - @ref TinyGsmESP8266::setTimeZone "setTimeZone()"
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
 *   - Change the buffer size by defining TINY_GSM_RX_BUFFER in your sketch before including any TinyGSM header file.
 * - Socket Numbering:
 *   - The modem uses user-specified MUX channel numbers for socket connections.
 *   - If you attempt to create a new client with a channel number that is
 * already in use and other unused channels are available, this library will
 * select the next available one.
 *   - Use the getMux() function to get the assigned multiplexing channel number
 * after a successful connection.
 */
/* clang-format on */

#ifndef SRC_TINYGSMCLIENTESP8266_H_
#define SRC_TINYGSMCLIENTESP8266_H_
#pragma message("TinyGSM:  TinyGsmClientESP8266")

#include "TinyGsmClientEspressif.h"
#include "TinyGsmTCP.tpp"
#include "TinyGsmSSL.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"

/// State: current Wi-Fi state.
/// @ingroup espressif_esp8266
enum class ESP8266RegStatus {
  /// ESP8266 station has not started any Wi-Fi connection.
  REG_UNINITIALIZED = 0,
  /// ESP8266 station has connected to an AP, but does not get an IPv4 address
  /// yet.
  REG_UNREGISTERED = 1,
  /// ESP8266 station has connected to an AP, and got an IPv4 address.
  REG_OK = 2,
  /// ESP8266 station is in Wi-Fi connecting or reconnecting state.
  REG_CONNECTING = 3,
  /// ESP8266 station is in Wi-Fi disconnected state.
  REG_DISCONNECTING = 4,
  /// ESP8266 station is in an unknown state.
  REG_UNKNOWN = 5,
};

/// Basic modem configurations for the ESP8266 modem family
/// @ingroup espressif_esp8266
struct TinyGsmESP8266ModemConfig
    : public TinyGsmModemConfigPreset<ESP8266RegStatus> {
  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "Espressif";
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "ESP8266";

  /// The set namespace for the certificate authority (CA) certificate
  static constexpr char CA_CERT_NAMESPACE[] TINY_GSM_PROGMEM = "client_ca";
  /// The set namespace for the client certificate
  static constexpr char CLIENT_CERT_NAMESPACE[] TINY_GSM_PROGMEM =
      "client_cert";
  /// The set namespace for the client key
  static constexpr char CLIENT_KEY_NAMESPACE[] TINY_GSM_PROGMEM = "client_key";
};

constexpr char TinyGsmESP8266ModemConfig::MODEM_MANUFACTURER[]
    __attribute__((weak));
constexpr char TinyGsmESP8266ModemConfig::MODEM_MODEL[] __attribute__((weak));
constexpr char TinyGsmESP8266ModemConfig::CA_CERT_NAMESPACE[]
    __attribute__((weak));
constexpr char TinyGsmESP8266ModemConfig::CLIENT_CERT_NAMESPACE[]
    __attribute__((weak));
constexpr char TinyGsmESP8266ModemConfig::CLIENT_KEY_NAMESPACE[]
    __attribute__((weak));

/**
 * @brief TCP behavior and limits for the ESP8266 family.
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
 * @ingroup espressif_esp8266
 */
struct TinyGsmESP8266TcpConfig
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
 * programmed with the AT command firmware.
 * @ingroup espressif_esp8266
 */
class TinyGsmESP8266
    : public TinyGsmEspressif<TinyGsmESP8266, TinyGsmESP8266ModemConfig>,
      public TinyGsmTCP<TinyGsmESP8266, TinyGsmESP8266TcpConfig>,
      public TinyGsmSSL<TinyGsmESP8266>,
      public TinyGsmTime<TinyGsmESP8266>,
      public TinyGsmNTP<TinyGsmESP8266> {
  friend class TinyGsmEspressif<TinyGsmESP8266, TinyGsmESP8266ModemConfig>;
  friend class TinyGsmModem<TinyGsmESP8266, TinyGsmESP8266ModemConfig>;
  friend class TinyGsmWifi<TinyGsmESP8266>;
  friend class TinyGsmTCP<TinyGsmESP8266, TinyGsmESP8266TcpConfig>;
  friend class GsmClient<TinyGsmESP8266, TinyGsmESP8266TcpConfig>;
  friend class TinyGsmSSL<TinyGsmESP8266>;
  friend class TinyGsmTime<TinyGsmESP8266>;
  friend class TinyGsmNTP<TinyGsmESP8266>;

 public:
  using ModemConfig = TinyGsmESP8266ModemConfig;
  using TcpConfig   = TinyGsmESP8266TcpConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  /// @ingroup espressif_esp8266
  class GsmClientESP8266
      : public GsmClient<TinyGsmESP8266, TinyGsmESP8266TcpConfig> {
    friend class TinyGsmESP8266;

   public:
    using GsmClient<TinyGsmESP8266, TinyGsmESP8266TcpConfig>::connect;
    using GsmClient<TinyGsmESP8266, TinyGsmESP8266TcpConfig>::stop;
    using TcpConfig = TinyGsmESP8266TcpConfig;

    /**
     * @brief Create a new TCP client.
     * @warning You must call the init() method before attempting to use a
     * client created with this constructor.
     */
    GsmClientESP8266() {
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
    explicit GsmClientESP8266(TinyGsmESP8266& modem, uint8_t mux = 0)
        : GsmClient<TinyGsmESP8266, TinyGsmESP8266TcpConfig>(modem, mux) {
      is_secure = false;
      init(&modem, mux);
    }

    /**
     * @brief Initialize the TCP client with a modem and optionally a
     * multiplexing channel.
     * @return true if initialization was successful, false otherwise.
     * @copydetails GsmClientESP8266::GsmClientESP8266(TinyGsmESP8266&, uint8_t)
     */
    bool init(TinyGsmESP8266* modem, uint8_t mux = 0) {
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
 public:
  /// Inner secure client
  /// @ingroup espressif_esp8266
  class GsmClientSecureESP8266 : public GsmClientESP8266,
                                 public GsmSecureClient {
    friend class TinyGsmESP8266;

   public:
    using GsmClientESP8266::connect;
    using GsmClientESP8266::stop;
    using TcpConfig = TinyGsmESP8266TcpConfig;

    TINY_GSM_SECURE_CLIENT_CTORS(ESP8266)

    // Because we have the same potetial range of mux numbers for secure and
    // insecure connections, we don't need to re-check for mux number
    // availability.

    /// @copydoc GsmSecureClient::setCACertName(const char*)
    /// @warning The CA certificate name must be either "client_ca.0" or
    /// "client_ca.1".
    void setCACertName(const char* CAcertName) override {
      if (at == nullptr || CAcertName == nullptr) { return; }
      // parse the certificate name
      CertificateType parsed_type = CertificateType::UNKNOWN;
      uint8_t         certNumber  = 0;
      at->parseCertificateName(CAcertName, parsed_type, certNumber);
      if (parsed_type != CertificateType::CA_CERTIFICATE || certNumber > 1) {
        return;
      }
      // copy the certificate name into owned buffer
      strncpy(this->CAcertName, CAcertName, sizeof(this->CAcertName) - 1);
      this->CAcertName[sizeof(this->CAcertName) - 1] = '\0';
      // set the number for the CA certificate
      this->ca_number = certNumber;
    }
    /// @copydoc GsmClientSecureESP8266::setCACertName(const char*)
    void setCACertName(const String& CAcertName) override {
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
      if (at == nullptr || clientCertName == nullptr) { return; }
      // parse the certificate name
      CertificateType parsed_type = CertificateType::UNKNOWN;
      uint8_t         certNumber  = 0;
      at->parseCertificateName(clientCertName, parsed_type, certNumber);
      if (parsed_type != CertificateType::CLIENT_CERTIFICATE ||
          certNumber > 1) {
        return;
      }
      // copy the certificate name into owned buffer
      strncpy(this->clientCertName, clientCertName,
              sizeof(this->clientCertName) - 1);
      this->clientCertName[sizeof(this->clientCertName) - 1] = '\0';
      // set thenumber for the client certificate and private key (only 1 var,
      // they must be the same)
      pki_number = certNumber;
      // generate the matching client private key name from the certificate
      // number and type
      char cert_name[16] = {};
      at->getCertificateName(CertificateType::CLIENT_KEY, certNumber,
                             cert_name);
      // set the client key name in the owned buffer
      strncpy(this->clientKeyName, cert_name, sizeof(this->clientKeyName) - 1);
      this->clientKeyName[sizeof(this->clientKeyName) - 1] = '\0';
    }
    /// @copydoc GsmClientSecureESP8266::setClientCertName(const char*)
    void setClientCertName(const String& clientCertName) override {
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
      if (at == nullptr || clientKeyName == nullptr) { return; }
      // parse the certificate name
      CertificateType parsed_type = CertificateType::UNKNOWN;
      uint8_t         certNumber  = 0;
      at->parseCertificateName(clientKeyName, parsed_type, certNumber);
      if (parsed_type != CertificateType::CLIENT_KEY || certNumber > 1) {
        return;
      }
      // copy the key name into owned buffer
      strncpy(this->clientKeyName, clientKeyName,
              sizeof(this->clientKeyName) - 1);
      this->clientKeyName[sizeof(this->clientKeyName) - 1] = '\0';
      // set thenumber for the client certificate and private key (only 1 var,
      // they must be the same)
      pki_number = certNumber;
      // generate the matching client certificate name from the private key
      // number and type
      char cert_name[16] = {};
      at->getCertificateName(CertificateType::CLIENT_CERTIFICATE, certNumber,
                             cert_name);
      // set the client certificate name in the owned buffer
      strncpy(this->clientCertName, cert_name,
              sizeof(this->clientCertName) - 1);
      this->clientCertName[sizeof(this->clientCertName) - 1] = '\0';
    }
    /// @copydoc GsmClientSecureESP8266::setPrivateKeyName(const char*)
    void setPrivateKeyName(const String& clientKeyName) override {
      setPrivateKeyName(clientKeyName.c_str());
    }

    /**
     * @brief Set the CA certificate number to use for this connection
     * @param certNumber The CA certificate number, must be 0 or 1.
     */
    void setCACertificateNumber(uint8_t certNumber) {
      if (at == nullptr) { return; }
      if (certNumber > 1) { return; }
      ca_number = certNumber;
      // convert the certificate number and type into the proper certificate
      // names for the ESP32
      char cert_name[16] = {};
      at->getCertificateName(CertificateType::CA_CERTIFICATE, certNumber,
                             cert_name);
      memcpy(this->CAcertName, cert_name,
             TinyGsmMin(sizeof(this->CAcertName), sizeof(cert_name)));
      this->CAcertName[TinyGsmMin(sizeof(this->CAcertName), sizeof(cert_name)) -
                       1] = '\0';
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
      if (certNumber > 1) { return; }
      pki_number = certNumber;
      // generate and set the name for the client certificate from the number
      char cert_name[16] = {};
      at->getCertificateName(CertificateType::CLIENT_CERTIFICATE, certNumber,
                             cert_name);
      memcpy(this->clientCertName, cert_name,
             TinyGsmMin(sizeof(this->clientCertName), sizeof(cert_name)));
      this->clientCertName[TinyGsmMin(sizeof(this->clientCertName),
                                      sizeof(cert_name)) -
                           1] = '\0';
      // generate and set the name for the client private key from the number
      at->getCertificateName(CertificateType::CLIENT_KEY, certNumber,
                             cert_name);
      memcpy(this->clientKeyName, cert_name,
             TinyGsmMin(sizeof(this->clientKeyName), sizeof(cert_name)));
      this->clientKeyName[TinyGsmMin(sizeof(this->clientKeyName),
                                     sizeof(cert_name)) -
                          1] = '\0';
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
    int8_t ca_number  = 0;
    int8_t pki_number = 0;
  };

  /*
   * GSM Modem Constructor
   */
 public:
  /**
   * @brief Construct a modem wrapper around a stream transport.
   * @param stream Stream used to communicate with the modem.
   */
  explicit TinyGsmESP8266(Stream& stream)
      : TinyGsmEspressif<TinyGsmESP8266, TinyGsmESP8266ModemConfig>(stream) {
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
 protected:
  ESP8266RegStatus getRegistrationStatusImpl() {
    sendAT(GF("+CWSTATE?"));
    if (waitResponse(3000, GF("+CWSTATE:")) != 1)
      return ESP8266RegStatus::REG_UNKNOWN;
    // +CWSTATE:{state},{"ssid"}
    // followed by an OK
    int8_t status = streamGetIntBefore(',');
    streamSkipUntil('\n');  // throw away the ssid
    waitResponse();         // wait for trailing OK
    return static_cast<ESP8266RegStatus>(status);
  }

  bool isNetworkConnectedImpl() {
    ESP8266RegStatus s = this->getRegistrationStatus();
    return (s == ESP8266RegStatus::REG_OK);
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // Uses the secure client inherited from TinyGsmSSL.tpp for setting the
  // certificate name and the SSL connection type so those can be called at
  // connection time, but this library does **NOT** currently support uploading,
  // deleting, or converting certificates on the modem.
  // On the ESP8266 the only way to update your certificates is to use a
  // combination of python scripts, AT commands, and flash tools or to fully
  // build your own version of the AT firmware.

#undef TINY_GSM_MODEM_CAN_LOAD_CERTS

 protected:
  void parseCertificateName(const char* cert_name, CertificateType& parsed_type,
                            uint8_t& parsed_number) {
    uint8_t ns_length = 0;
    // look for the namespace in the name and determine the certificate type
    if (strncmp(cert_name, ModemConfig::CA_CERT_NAMESPACE,
                strlen(ModemConfig::CA_CERT_NAMESPACE)) == 0) {
      parsed_type = CertificateType::CA_CERTIFICATE;
      ns_length   = strlen(ModemConfig::CA_CERT_NAMESPACE);
    } else if (strncmp(cert_name, ModemConfig::CLIENT_CERT_NAMESPACE,
                       strlen(ModemConfig::CLIENT_CERT_NAMESPACE)) == 0) {
      parsed_type = CertificateType::CLIENT_CERTIFICATE;
      ns_length   = strlen(ModemConfig::CLIENT_CERT_NAMESPACE);
    } else if (strncmp(cert_name, ModemConfig::CLIENT_KEY_NAMESPACE,
                       strlen(ModemConfig::CLIENT_KEY_NAMESPACE)) == 0) {
      parsed_type = CertificateType::CLIENT_KEY;
      ns_length   = strlen(ModemConfig::CLIENT_KEY_NAMESPACE);
    } else {
      goto parsing_error;
    }

    if (cert_name[ns_length] != '.') { goto parsing_error; }
    if (cert_name[ns_length + 1] == '0') {
      parsed_number = 0;
      return;
    } else if (cert_name[ns_length + 1] == '1') {
      parsed_number = 1;
      return;
    } else {
      goto parsing_error;
    }
    return;

  parsing_error:
    parsed_type   = CertificateType::UNKNOWN;
    parsed_number = static_cast<uint8_t>(-1);
    return;
  }

  void getCertificateName(CertificateType cert_type, uint8_t certNumber,
                          char* cert_name) {
    // Validate certNumber is 0 or 1 to prevent buffer overflow in itoa()
    if (certNumber > 1) {
      if (cert_name != nullptr) { cert_name[0] = '\0'; }
      return;
    }
    char cert_number[2];  // Must be '0' or '1', so 2 bytes is enough
    itoa(certNumber, cert_number, 10);

    // put the set certificate namespace and name into the provided buffers
    // based on the certificate type
    switch (cert_type) {
      case CertificateType::CLIENT_PSK_IDENTITY:
      case CertificateType::CLIENT_PSK: {
        // The ESP32 does not support SSL using pre-shared keys with AT
        // firmware.
        strcpy(cert_name, "\0");
        return;
      }
      case CertificateType::CLIENT_KEY: {
        strcpy(cert_name, ModemConfig::CLIENT_KEY_NAMESPACE);
        break;
      }
      case CertificateType::CLIENT_CERTIFICATE: {
        strcpy(cert_name, ModemConfig::CLIENT_CERT_NAMESPACE);
        break;
      }
      case CertificateType::CA_CERTIFICATE:
      default: {
        strcpy(cert_name, ModemConfig::CA_CERT_NAMESPACE);
        break;
      }
    }
    // append the certificate number to the name to create the full certificate
    // name (e.g., "client_cert.0")
    strcat(cert_name, ".");
    strcat(cert_name, cert_number);
    return;
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

 protected:
  bool waitForTimeSyncImpl(uint16_t timeout_s) {
    // if we're not connected, we'll never get the time
    if (!isNetworkConnected()) {
      DBG(GF("### Not connected to network; cannot sync time!"));
      return false;
    }
    // if SNTP sync isn't enabled, we won't have the time
    // NOTE: We don't actually enable the time here, because doing so would
    // change any user settings for the timezone and time servers.
    sendAT(GF("+CIPSNTPCFG?"));
    int8_t is_enabled = waitResponse(2000L, GF("+CIPSNTPCFG:")) == 1;
    is_enabled &= streamGetIntBefore(',');
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

    int16_t iyear     = 0;
    int16_t imonth    = 0;
    int16_t iday      = 0;
    int16_t ihour     = 0;
    int16_t imin      = 0;
    int16_t isec      = 0;
    float   itimezone = 0;

    // Date & Time
    streamSkipUntil(' ');  // skip the day of the week
    String mon_abbrev = stream.readStringUntil(' ');
    imonth            = getMonthFromAbbrev(mon_abbrev);
    iday              = streamGetIntBefore(' ');
    if (iday == -9999) {
      // there may be multiple spaces between the month and day, so try a second
      // time
      iday = streamGetIntBefore(' ');
    }
    ihour = streamGetIntBefore(':');
    imin  = streamGetIntBefore(':');
    isec  = streamGetIntBefore(' ');
    iyear = streamGetIntLength(4);

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

#if 0
    DBG(GF("Year:"), iyear, GF("	Month:"), imonth, GF("	Day:"), iday);
    DBG(GF("Hour:"), ihour, GF("	Minute:"), imin, GF("	Second:"), isec);
    DBG(GF("Timezone:"), static_cast<float>(itimezone) / 4.0);
#endif

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
    // Returns unix timestamp. Will match SNTP after SNTP syncs.
    sendAT(GF("+SYSTIMESTAMP?"));
    if (waitResponse(2000L, GF("+SYSTIMESTAMP:")) != 1) { return 0; }

    uint32_t start = millis();
    while (stream.available() < 9 && millis() - start < 10000L) {}

    uint32_t modem_time = streamGetULBefore('\r');

    waitResponse();

    if (modem_time != static_cast<uint32_t>(-1)) {
      switch (epoch) {
        case TinyGSM_EpochStart::UNIX: modem_time += 0; break;
        case TinyGSM_EpochStart::Y2K: modem_time -= 946684800; break;
        case TinyGSM_EpochStart::GPS: modem_time -= 315964800; break;
      }
    }

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
  // No functions of this type supported

  /*
   * Client-related functions
   */
 protected:
  bool modemConnectImpl(const char* host, uint16_t port, uint8_t /*static*/ mux,
                        int timeout_s) {
    if (!isValidMux(mux)) { return false; }
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    bool     ssl        = sockets[mux]->is_secure;

    // Blank holders for the SSL auth mode and certificates
    SSLAuthMode sslAuthMode = SSLAuthMode::NO_VALIDATION;
    uint8_t     ca_number   = 0;
    uint8_t     pki_number  = 0;
    // If we actually have a secure socket populate the above with real values
    if (ssl) {
      const GsmClientSecureESP8266* thisClient =
          static_cast<const GsmClientSecureESP8266*>(sockets[mux]);
      sslAuthMode = thisClient->sslAuthMode;
      ca_number   = thisClient->ca_number;
      pki_number  = thisClient->pki_number;
    }

    if (ssl) {
      if (sslAuthMode == SSLAuthMode::PRE_SHARED_KEYS) {
        DBG("### The ESP8266 does not support SSL using pre-shared keys.");
        // The ESP8266 does not support SSL using pre-shared keys with AT
        // firmware.
        return false;
      }
      // SSL certificate checking will not work without a valid timestamp!
      if (sockets[mux] != nullptr &&
          (sslAuthMode == SSLAuthMode::CLIENT_VALIDATION ||
           sslAuthMode == SSLAuthMode::CA_VALIDATION ||
           sslAuthMode == SSLAuthMode::MUTUAL_AUTHENTICATION) &&
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
      if (sockets[mux] == nullptr ||
          (sslAuthMode == SSLAuthMode::NO_VALIDATION)) {
        sendAT(GF("+CIPSSLCCONF="), mux, GF(",0"));
      } else {
        sendAT(GF("+CIPSSLCCONF="), mux, ',', static_cast<uint8_t>(sslAuthMode),
               ',', pki_number, ',', ca_number);
      }
      waitResponse();

      // set the SSL SNI (server name indication)
      // Multiple connections: (AT+CIPMUX=1)
      // AT+CIPSSLCSNI=<link ID>,<"sni">
      sendAT(GF("+CIPSSLCSNI="), mux, GF(",\""), host, '"');
      waitResponse();
    }

    String resolved_ip;
    // If you need to use a domain name and the length of the domain name
    // exceeds 64 bytes, use the AT+CIPDOMAIN command to obtain the IP address
    // corresponding to the domain name, and then use the IP address to
    // establish a connection.
    if (strlen(host) > 64) {
      // AT+CIPDOMAIN=<"domain name">[,<ip network>][,<timeout>]
      sendAT(GF("+CIPDOMAIN=\""), host, '"');
      // +CIPDOMAIN:<"IP address"> then OK
      if (waitResponse(GF("+CIPDOMAIN:\"")) != 1) { return false; }
      resolved_ip = stream.readStringUntil('"');
      streamSkipUntil('\n');  // skip the rest of the line
      waitResponse();         // ends with OK
      if (resolved_ip.length() > 0) {
        host = resolved_ip.c_str();
      } else {
        return false;
      }
    }

    // Select TCP/IP transmission mode (normal mode)
    sendAT(GF("+CIPMODE=0"));
    waitResponse();

    // Make the connection
    sendAT(GF("+CIPSTART="), mux, ',', ssl ? GF("\"SSL") : GF("\"TCP"),
           GF("\",\""), host, GF("\","), port
#if defined(TINY_GSM_TCP_KEEP_ALIVE)
           ,
           ',', TINY_GSM_TCP_KEEP_ALIVE
#endif
    );

    String data;
    int8_t rsp = waitResponse(timeout_ms, data, GFP(ModemConfig::GSM_OK),
                              GFP(ModemConfig::GSM_ERROR),
                              GF("ALREADY CONNECT"));
    if (rsp == 1 && data.length() > 8) {
      int16_t coma          = data.indexOf(',');
      int16_t connected_mux = data.substring(0, coma).toInt();
      if (!isExpectedMux(connected_mux, mux)) {
        DBG("WARNING:  Unexpected mux number returned:", connected_mux, "not",
            mux);
      }
    }
    return (1 == rsp || 3 == rsp);  // OK or ALREADY CONNECT
  }

  // Disambiguate modemStopImpl by using the Espressif implementation
  using TinyGsmEspressif<TinyGsmESP8266,
                         TinyGsmESP8266ModemConfig>::modemStopImpl;

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    if (!isValidMux(mux)) { return false; }
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

  bool modemGetConnectedImpl(uint8_t mux) {
    if (!isValidMux(mux)) { return false; }
    sendAT(GF("+CIPSTATE?"));
    // initialize the connection array assuming no connections are active
    bool verified_connections[TcpConfig::kMuxCount] = {0};
    for (uint8_t muxNo = 0; muxNo < TcpConfig::kMuxCount; muxNo++) {
      uint8_t has_status = waitResponse(GF("+CIPSTATE:"),
                                        GFP(ModemConfig::GSM_OK),
                                        GFP(ModemConfig::GSM_ERROR));
      if (has_status == 1) {
        int16_t returned_mux = streamGetIntBefore(',');
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
    for (uint8_t muxNo = 0; muxNo < TcpConfig::kMuxCount; muxNo++) {
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
      int16_t mux          = streamGetIntBefore(',');
      int16_t len_reported = streamGetIntBefore(':');
      if (isValidMux(mux)) {
        // This will handle an invalid len_reported value and will drain the
        // stream if there's a buffer overflow, so we don't need to validate
        // len_reported or check for overflow here.  We just need to make sure
        // the mux is valid before we call it.
        moveCharsFromStreamToFifo(mux, len_reported);
      } else {
        do {  // If the mux is invalid, throw away the stream data
          stream.read();
          TINY_GSM_YIELD();
        } while (stream.available());
      }
      data = "";
      return true;
    } else if (data.endsWith(GF("CLOSED"))) {
      int muxStart =
          TinyGsmMax(0,
                     data.lastIndexOf(String(GFP(ModemConfig::GSM_NL)),
                                      data.length() - 8));
      int16_t coma = data.indexOf(',', muxStart);
      int16_t mux  = data.substring(muxStart, coma).toInt();
      if (isValidMux(mux)) {
        sockets[static_cast<uint8_t>(mux)]->sock_connected = false;
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
  GsmClientESP8266* sockets[TcpConfig::kMuxCount];
};

#endif  // SRC_TINYGSMCLIENTESP8266_H_
