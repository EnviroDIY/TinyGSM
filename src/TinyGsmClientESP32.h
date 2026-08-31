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

  /// The set namespace for the certificate authority (CA) certificate
  static constexpr char CA_CERT_NAMESPACE[] TINY_GSM_PROGMEM = "client_ca";
  /// The set namespace for the client certificate
  static constexpr char CLIENT_CERT_NAMESPACE[] TINY_GSM_PROGMEM =
      "client_cert";
  /// The set namespace for the client key
  static constexpr char CLIENT_KEY_NAMESPACE[] TINY_GSM_PROGMEM = "client_key";
};

constexpr char TinyGsmESP32ModemConfig::MODEM_MANUFACTURER[]
    __attribute__((weak));
constexpr char TinyGsmESP32ModemConfig::MODEM_MODEL[] __attribute__((weak));
constexpr char TinyGsmESP32ModemConfig::CA_CERT_NAMESPACE[]
    __attribute__((weak));
constexpr char TinyGsmESP32ModemConfig::CLIENT_CERT_NAMESPACE[]
    __attribute__((weak));
constexpr char TinyGsmESP32ModemConfig::CLIENT_KEY_NAMESPACE[]
    __attribute__((weak));

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
     * @note The ESP32 assigns the multiplexing channel number itself.  The mux
     * argument is only a preliminary placeholder used until the connection
     * completes.  Use the getMux() function to get the assigned multiplexing
     * channel number after a successful connection.
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
      if (sock_connected && assignedMux != mux) {
        // If we successfully connected, and the assigned mux number is
        // different from the requested mux number, we need to move any existing
        // client at the assigned mux number before we can insert this client
        // into the sockets array at the assigned mux.
        // Set the requested mux to -1 to get the next available mux number.
        // If there was no existing client at the assigned mux number, this will
        // do nothing.
        at->moveSocket(assignedMux, static_cast<uint8_t>(-1));
        // If the original mux number was valid, and the pointer to this client
        // is still in the original mux position in the sockets array, set the
        // pointer in that position to null.
        if (mux < TcpConfig::kMuxCount && at->sockets[mux] == this) {
          at->sockets[mux] = nullptr;
        }
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
      uint32_t startMillis = millis();
      if (sock_connected || sock_available) {
        // Update available data first, because if the socket was closed
        // externally, the module may have thrown away the data
        at->modemGetAvailable(mux);
        // Now we throw away any remaining data in the modem buffer
        // We explicitly toss it here because the socket will appear open in
        // response to connected() even after it closes until all data is read
        // to give the user a chance to recover the data if they want it.
        dumpModemBuffer(maxWaitMs);
      }
      uint32_t elapsed = millis() - startMillis;
      // NOTE: It should be safe to only send the close here if sock_connected
      // reads true because the above will have updated sock_connected
      // (dumpModemBuffer calls modemRead until sock_available=0, modemRead
      // calls modemGetAvailable on every read to update sock_available, once
      // sock_available=0 modemGetAvailable calls modemGetConnected, and
      // modemGetConnected updates sock_connected for all sockets.)
      if (sock_connected) { at->modemStop(mux, maxWaitMs - elapsed); }
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
    void setCACertName(const char* in_CAcertName) override {
      if (at == nullptr) { return; }
      // parse the certificate name
      CertificateType parsed_type = CertificateType::UNKNOWN;
      uint8_t         certNumber  = 0;
      at->parseCertificateName(in_CAcertName, parsed_type, certNumber);
      if (parsed_type != CertificateType::CA_CERTIFICATE || certNumber > 1) {
        return;
      }
      // copy the certificate name into owned buffer
      strncpy(this->CAcertName, in_CAcertName, sizeof(this->CAcertName) - 1);
      this->CAcertName[sizeof(this->CAcertName) - 1] = '\0';
      // set the number for the CA certificate
      this->ca_number = certNumber;
    }
    /// @copydoc GsmClientSecureESP32::setCACertName(const char*)
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
    void setClientCertName(const char* in_clientCertName) override {
      if (at == nullptr) { return; }
      // parse the certificate name
      CertificateType parsed_type = CertificateType::UNKNOWN;
      uint8_t         certNumber  = 0;
      at->parseCertificateName(in_clientCertName, parsed_type, certNumber);
      if (parsed_type != CertificateType::CLIENT_CERTIFICATE ||
          certNumber > 1) {
        return;
      }
      // copy the certificate name into owned buffer
      strncpy(this->clientCertName, in_clientCertName,
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
    /// @copydoc GsmClientSecureESP32::setClientCertName(const char*)
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
      if (at == nullptr) { return; }
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
    /// @copydoc GsmClientSecureESP32::setPrivateKeyName(const char*)
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
    for (uint8_t mux = 0; mux < TcpConfig::kMuxCount; mux++) {
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
    if (certNumber > 1) { return false; }
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
    if (certNumber > 1) { return false; }
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
    if (keyNumber > 1) { return false; }
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
    if (certNumber > 1) { return false; }
    // delete any old text in the cert first
    deleteCertificateByNumber(cert_type, certNumber);

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

    switch (cert_type) {
      case CertificateType::CLIENT_PSK:
      case CertificateType::CLIENT_PSK_IDENTITY: {
        return false;
      }
      case CertificateType::CA_CERTIFICATE: {
        sendAT(GF("+SYSMFG=2,\""), ModemConfig::CA_CERT_NAMESPACE, GF("\",\""),
               ModemConfig::CA_CERT_NAMESPACE, '.', certNumber + '0',
               GF("\",8,"), len);
        break;
      }
      case CertificateType::CLIENT_CERTIFICATE: {
        sendAT(GF("+SYSMFG=2,\""), ModemConfig::CLIENT_CERT_NAMESPACE,
               GF("\",\""), ModemConfig::CLIENT_CERT_NAMESPACE, '.',
               certNumber + '0', GF("\",8,"), len);
        break;
      }
      case CertificateType::CLIENT_KEY: {
        sendAT(GF("+SYSMFG=2,\""), ModemConfig::CLIENT_KEY_NAMESPACE,
               GF("\",\""), ModemConfig::CLIENT_KEY_NAMESPACE, '.',
               certNumber + '0', GF("\",8,"), len);
        break;
      }
      default: {
        return false;
      }
    }

    if (waitResponse(GF(">")) != 1) { return false; }
    stream.write(reinterpret_cast<const uint8_t*>(cert), len);
    stream.flush();
    if (waitResponse(10000L) != 1) { return false; }
    return true;
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
    if (certNumber > 1) { return false; }

    // AT+SYSMFG=<operation>,<"namespace">[,<"key">]
    // operation = 0 for erase
    // AT+SYSMFG=0,"client_cert","client_cert.0"

    switch (cert_type) {
      case CertificateType::CLIENT_PSK:
      case CertificateType::CLIENT_PSK_IDENTITY: {
        return false;
      }
      case CertificateType::CA_CERTIFICATE: {
        sendAT(GF("+SYSMFG=0,\""), ModemConfig::CA_CERT_NAMESPACE, GF("\",\""),
               ModemConfig::CA_CERT_NAMESPACE, '.', certNumber + '0');
        break;
      }
      case CertificateType::CLIENT_CERTIFICATE: {
        sendAT(GF("+SYSMFG=0,\""), ModemConfig::CLIENT_CERT_NAMESPACE,
               GF("\",\""), ModemConfig::CLIENT_CERT_NAMESPACE, '.',
               certNumber + '0');
        break;
      }
      case CertificateType::CLIENT_KEY: {
        sendAT(GF("+SYSMFG=0,\""), ModemConfig::CLIENT_KEY_NAMESPACE,
               GF("\",\""), ModemConfig::CLIENT_KEY_NAMESPACE, '.',
               certNumber + '0');
        break;
      }
      default: {
        return false;
      }
    }

    return waitResponse() == 1;
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
    if (certNumber > 1) { return false; }
    // AT+SYSMFG=<operation>,<"namespace">,<"key">,<type>,<value>
    // operation = 1 for read
    // type = 8 for binary (ie, the certificates must be stored in binary,
    // though you can enter them as strings)
    // Read the value of client_cert.0 key from client_cert namespace
    // (That is, read the 0th client certificate)
    // AT+SYSMFG=1,"client_cert","client_cert.0",8

    switch (cert_type) {
      case CertificateType::CLIENT_PSK:
      case CertificateType::CLIENT_PSK_IDENTITY: {
        return false;
      }
      case CertificateType::CA_CERTIFICATE: {
        sendAT(GF("+SYSMFG=1,\""), ModemConfig::CA_CERT_NAMESPACE, GF("\",\""),
               ModemConfig::CA_CERT_NAMESPACE, '.', certNumber + '0');
        break;
      }
      case CertificateType::CLIENT_CERTIFICATE: {
        sendAT(GF("+SYSMFG=1,\""), ModemConfig::CLIENT_CERT_NAMESPACE,
               GF("\",\""), ModemConfig::CLIENT_CERT_NAMESPACE, '.',
               certNumber + '0');
        break;
      }
      case CertificateType::CLIENT_KEY: {
        sendAT(GF("+SYSMFG=1,\""), ModemConfig::CLIENT_KEY_NAMESPACE,
               GF("\",\""), ModemConfig::CLIENT_KEY_NAMESPACE, '.',
               certNumber + '0');
        break;
      }
      default: {
        return false;
      }
    }

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
    // Validate certNumber is 0 or 1
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


  bool loadCertificateImpl(const char* certificateName, const char* cert,
                           const uint16_t len) {
    // parse the certificate name
    CertificateType parsed_type = CertificateType::UNKNOWN;
    uint8_t         certNumber  = 0;
    parseCertificateName(certificateName, parsed_type, certNumber);
    // add the certificate by name
    return loadCertificateByNumber(parsed_type, certNumber, cert, len);
  }

  bool deleteCertificateImpl(const char* certificateName) {
    // parse the certificate name
    CertificateType parsed_type = CertificateType::UNKNOWN;
    uint8_t         certNumber  = 0;
    parseCertificateName(certificateName, parsed_type, certNumber);
    // add the certificate by name
    return deleteCertificateByNumber(parsed_type, certNumber);
  }


  bool printCertificateImpl(const char* filename, Stream& print_stream) {
    // parse the certificate name
    CertificateType parsed_type = CertificateType::UNKNOWN;
    uint8_t         certNumber  = 0;
    parseCertificateName(filename, parsed_type, certNumber);
    // add the certificate by name
    return printCertificateByNumber(parsed_type, certNumber, print_stream);
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
    char     buf[12]    = {0};
    size_t   bytesRead  = stream.readBytesUntil('\n', buf,
                                                static_cast<size_t>(12));
    // if we read 12 or more bytes, it's an overflow
    if (bytesRead && bytesRead < 12) {
      buf[bytesRead] = '\0';
      modem_time     = strtoul(buf, nullptr, 10);
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
    // Validate dynamicMux before accessing sockets array
    if (*dynamicMux >= TcpConfig::kMuxCount || !sockets[*dynamicMux]) {
      return false;
    }
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
      } else {  // pre-shared keys
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
    int8_t connect_rsp =
        waitResponse(timeout_ms, data, GFP(ModemConfig::GSM_OK),
                     GFP(ModemConfig::GSM_ERROR), GF("ALREADY CONNECT"));
    bool success = connect_rsp == 1 ||
        connect_rsp == 3;  // OK or ALREADY CONNECT
    if (success && data.length() > 8) {
      int16_t coma          = data.indexOf(',');
      int16_t connected_mux = data.substring(0, coma).toInt();

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

  size_t modemReadImpl(size_t size, uint8_t mux) {
    if (!isValidMux(mux)) { return 0; }

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
    if (!isValidMux(mux)) { return 0; }
    size_t result = 0;
    sendAT(GF("+CIPRECVLEN?"));
    if (waitResponse(GF("+CIPRECVLEN:")) != 1) { return result; }
    for (uint8_t muxNo = 0; muxNo < TcpConfig::kMuxCount; muxNo++) {
      long mux_avail = stream.parseInt();
      if (sockets[muxNo]) { sockets[muxNo]->sock_available = mux_avail; }
    }
    waitResponse();  // ends with OK
    result = sockets[mux]->sock_available;
    if (!result) { sockets[mux]->sock_connected = modemGetConnected(mux); }
    return result;
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
      int16_t  mux = streamGetIntBefore(',');
      uint16_t len = streamGetIntBefore('\n');
      if (isValidMux(mux)) {
        sockets[static_cast<uint8_t>(mux)]->got_data = true;
        // TODO: I'm not sure if each +IPD URC reports the amount newly received
        // or the total now in the buffer. It appears to be the latter.
        // sockets[mux]->sock_available = sockets[mux]->sock_available + len;
        sockets[static_cast<uint8_t>(mux)]->sock_available = len;
      }
      data = "";
      DBG("### Got Data:", len, "on", mux);
      return true;
    } else if (data.endsWith(GF("CLOSED"))) {
      int16_t muxStart =
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
