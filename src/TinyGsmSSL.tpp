/**
 * @file       TinyGsmSSL.tpp
 * @brief      TLS/SSL connection helper mixin for secure clients.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMSSL_TPP_
#define SRC_TINYGSMSSL_TPP_

#include "TinyGsmCommon.h"

#ifndef TINY_GSM_MODEM_HAS_SSL
/// flag to indicate that the modem has Secure Socket Layer (SSL) functions
#define TINY_GSM_MODEM_HAS_SSL
#endif
#ifndef TINY_GSM_MODEM_CAN_SPECIFY_CERTS
/// flag to indicate that the modem can specify certificates for SSL connections
#define TINY_GSM_MODEM_CAN_SPECIFY_CERTS
#endif
#ifndef TINY_GSM_MODEM_CAN_LOAD_CERTS
/// flag to indicate that the modem can load certificates for SSL connections
#define TINY_GSM_MODEM_CAN_LOAD_CERTS
#endif

#ifndef TINY_GSM_CERT_NAME_LENGTH
/**
 * @brief The maximum length of a certificate file name - 32 characters plus a
 * null terminator.
 *
 * This is the maximum length of a certificate file name used to refer to the
 * certificate in the modems flash memory, not the certificate itself.  The file
 * name length limit varies by modem; some list a limit of 64 characters others
 * don't list one.  Increase this value if you want to use longer file names.
 */
#define TINY_GSM_CERT_NAME_LENGTH 33
#endif

#ifndef TINY_GSM_PSK_IDENTITY_LENGTH
/**
 * @brief The maximum length of the pre-shared key (PSK) identity
 * (identifier/hing) - 32 characters plus a null terminator.
 *
 * This is the maximum length of the identity value itself, not a reference or
 * file name. There is not a standard for the maximum length of a PSK identity,
 * but 32 characters is a common limit.  The null terminator is added to the end
 * of the string to indicate the end of the string.  Increase this value if you
 * need a longer hint value
 */
#define TINY_GSM_PSK_IDENTITY_LENGTH 33
#endif

#ifndef TINY_GSM_PSK_LENGTH
/**
 * @brief The maximum length of the pre-shared key (PSK) - 32 characters plus a
 * null terminator.
 *
 * This is the maximum length of the key itself, not a reference or file name.
 * 32 characters is a common limit for the length of a PSK, though 64-255
 * characters can be used in IPsec PSK (Palo Alto).    Increase this value if
 * you need to use a longer key.
 */

#define TINY_GSM_PSK_LENGTH 33
#endif

#ifndef TINY_GSM_DEFAULT_SSL_CTX
/**
 * @brief The default SSL context to use for SSL connections.
 *
 * This applies to **all** modems using the GsmSecureClient class.  The SSL
 * context is a collection of SSL settings, not the connection identifier.  This
 * library always uses SSL context 0 by default.  At this time, all modems that
 * support SSL via contexts show using context 0 in their documentation.  If you
 * find a modem that uses a different context, please let us know and we will
 * add it to the list of modems that use a different default context.
 */
#define TINY_GSM_DEFAULT_SSL_CTX 0
#endif

/// Enum for different types of SSL/TLS certificates.
enum class CertificateType : int8_t {
  /// The CA certificate is used to verify the server's certificate. This is the
  /// most common type of certificate used in SSL connections.
  CA_CERTIFICATE = 0,
  /// The client certificate is used to authenticate the client to the server.
  /// This is less common and is typically used in mutual authentication
  /// scenarios.
  CLIENT_CERTIFICATE = 1,
  /// The client key is used in conjunction with the client certificate for
  /// mutual authentication. It is a private key that should be kept secure and
  /// not shared.
  CLIENT_KEY = 2,
  /// The client pre-shared key is used for authentication in TLS-PSK
  /// connections. It is a secret key shared between the client and the server.
  CLIENT_PSK = 3,
  /// The client pre-shared key identity is used in conjunction with the client
  /// PSK for authentication in TLS-PSK connections.
  CLIENT_PSK_IDENTITY = 4,
  /// The UNKNOWN type is used to indicate that the certificate type is not
  /// recognized or is not specified. It is a placeholder for error handling or
  /// default cases.
  UNKNOWN = -1
};

/**
 * @enum SSLAuthMode
 * @brief Enum for different SSL/TLS authentication modes (auth_mode).
 *
 * @var SSLAuthMode::NO_VALIDATION
 * 0: no validation
 *    - SRGD Note: Very insecure! You do not need to load any certificates
 *    onto your device for this. Not all servers will accept it.
 * @var SSLAuthMode::CLIENT_VALIDATION
 * 1: the client provides the client certificate for the server to
 * verify.
 *    - SRGD Note: I do not believe this is commonly used. To use
 *    this, you must load a client certificate and a client key onto
 *    your device.
 * @var SSLAuthMode::CA_VALIDATION
 * 2: the client loads CA certificate to verify the server’s
 * certificate.
 *    - SRGD Note: This is a common authentication type used by
 *    browsers, where the browser verifies the server's certificate.
 *    For this to work, you must load either the server's intermediate or parent
 *    CA certificate onto your device.
 * @var SSLAuthMode::MUTUAL_AUTHENTICATION
 * 3: mutual authentication.
 *    - SRGD Note: This is used by AWS IoT Core and other IoT
 *    services. In this case you must load 3 certs to your device:
 *    The server's CA cert, the client cert, and the client key.
 * @var SSLAuthMode::PRE_SHARED_KEYS
 * 4: pre-shared key encryption
 */
enum class SSLAuthMode : int8_t {
  NO_VALIDATION         = 0,
  CLIENT_VALIDATION     = 1,
  CA_VALIDATION         = 2,
  MUTUAL_AUTHENTICATION = 3,
  PRE_SHARED_KEYS       = 4,
};

/// Enum for different SSL/TLS versions.
enum class SSLVersion : int8_t {
  /// No SSL/TLS, plain TCP connection
  NO_SSL = -1,
  /// SSL 3.0
  SSL3_0 = 0,
  /// TLS 1.0
  TLS1_0 = 1,
  /// TLS 1.1
  TLS1_1 = 2,
  /// TLS 1.2
  TLS1_2 = 3,
  /// Try all SSL/TLS versions, the modem will negotiate the best version
  ALL_SSL = 4,
  /// TLS 1.3
  TLS1_3 = 5
};

/**
 * @brief The CRTP parent class for Secure Socket Layer (SSL) functions.
 * @tparam modemType The derived modem class
 */
template <class modemType>
class TinyGsmSSL {
 public:
  /// Compile-time capability flag indicating SSL/TLS support
  static constexpr bool hasSSL = true;
  /// Compile-time capability flag indicating certificate specification support
  static constexpr bool canSpecifyCerts = true;
  /// Compile-time capability flag indicating certificate loading support
  static constexpr bool canLoadCerts = true;

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the interface
   */
 public:
  /**
   * @anchor ssl_certificate_management_functions
   * @name Secure socket layer (SSL) certificate management functions
   */
  /**@{*/

  /**
   * @brief Create the SSL capability mixin.
   */
  TinyGsmSSL() {}

  /**
   * @brief Create the SSL capability mixin.
   * @param modem Unused. Accepted for signature compatibility with the derived
   * client constructors.
   * @param mux Unused. Accepted for signature compatibility with the derived
   * client constructors.
   */
  explicit TinyGsmSSL(modemType& /*modem*/, uint8_t /*mux*/ = 0) {}

  /**
   * @brief Load a new certificate onto the module.
   *
   * Load a new certificate onto the module, including the whole text of the
   * certificate. A file will be created (or updated) on the module's filesystem
   * with the content of the certificate.
   *
   * @param certificateName The name of the certificate.
   * @param cert The certificate content.
   * @param len The length of the certificate content.
   * @return True if the certificate was successfully loaded, false otherwise.
   */
  bool loadCertificate(const char* certificateName, const char* cert,
                       const uint16_t len) {
    return thisModem().loadCertificateImpl(certificateName, cert, len);
  }
  /// @copydoc loadCertificate(const char*, const char*, const uint16_t)
  bool loadCertificate(const String& certificateName, const String& cert,
                       const uint16_t len) {
    return loadCertificate(certificateName.c_str(), cert.c_str(), len);
  }

  /**
   * @brief Delete a certificate by name from the module's filesystem.
   *
   * NOTE: The functions for deleting a certificate rarely depend on the
   * certificate type.
   *
   * @param filename The name of the certificate file to delete.
   * @return True if the certificate was successfully deleted, false otherwise.
   */
  bool deleteCertificate(const char* filename) {
    return thisModem().deleteCertificateImpl(filename);
  }
  /// @copydoc deleteCertificate(const char*)
  bool deleteCertificate(const String& filename) {
    return deleteCertificate(filename.c_str());
  }

  // print the contents of a certificate file to a stream
  /**
   * @brief Print the contents of a certificate file to a stream.
   *
   * @note For security reasons, this function may not be available for all
   * modems.
   *
   * @param filename The name of the certificate file.
   * @param print_stream The stream to print the certificate contents to.
   * @return True if the certificate was successfully printed, false otherwise.
   */
  bool printCertificate(const char* filename, Stream& print_stream) {
    return thisModem().printCertificateImpl(filename, print_stream);
  }
  /// @copydoc printCertificate(const char*, Stream&)
  bool printCertificate(const String& filename, Stream& print_stream) {
    return printCertificate(filename.c_str(), print_stream);
  }

  /**
   * @brief Converts a certificate file into a format usable by the modem for
   * SSL connections.
   *
   * For many (but not all!) modules, the certificate must be "converted" from a
   * file stored somewhere in the flash file system into a certificate ready to
   * be used by the connection.
   *
   * @param cert_type The type of the certificate.
   * @param filename The name of the certificate file.
   * @return True if the certificate was successfully converted, false
   * otherwise.
   */
  bool convertCertificate(CertificateType cert_type, const char* filename) {
    return thisModem().convertCertificateImpl(cert_type, filename);
  }
  /// @copydoc convertCertificate(CertificateType, const char*)
  bool convertCertificate(CertificateType cert_type, const String& filename) {
    return convertCertificate(cert_type, filename.c_str());
  }
  /**
   * @brief Convert a certificate authority (CA) certificate.
   *
   * @param ca_cert_name The name of the CA certificate file.
   * @return True if the CA certificate was successfully converted, false
   * otherwise.
   */
  bool convertCACertificate(const char* ca_cert_name) {
    return thisModem().convertCACertificateImpl(ca_cert_name);
  }
  /// @copydoc convertCACertificate(const char*)
  bool convertCACertificate(const String& ca_cert_name) {
    return convertCACertificate(ca_cert_name.c_str());
  }
  /**
   * @brief Convert a client certificate and its corresponding key.
   *
   * In some cases, the two parts of the client certificate (the certificate
   * itself and the key to it) need to be converted together.
   *
   * @param client_cert_name The name of the client certificate file.
   * @param client_cert_key The name of the client certificate key file.
   * @return True if the client certificate and key were successfully converted,
   * false otherwise.
   */
  bool convertClientCertificates(const char* client_cert_name,
                                 const char* client_cert_key) {
    return thisModem().convertClientCertificatesImpl(client_cert_name,
                                                     client_cert_key);
  }
  /// @copydoc convertClientCertificates(const char*, const char*)
  bool convertClientCertificates(const String& client_cert_name,
                                 const String& client_cert_key) {
    return convertClientCertificates(client_cert_name.c_str(),
                                     client_cert_key.c_str());
  }
  /**
   * @brief Convert a pre-shared key (PSK) and its corresponding identity.
   *
   * In some cases, the two parts of the pre-shared key (the PSK itself and the
   * identity assigned to it) need to be converted together.
   *
   * @param psk The pre-shared key.
   * @param pskIdent The identity assigned to the pre-shared key (also called
   * identifier hint or hint).
   * @return True if the PSK and identity were successfully converted, false
   * otherwise.
   */
  bool convertPSKandID(const char* psk, const char* pskIdent) {
    return thisModem().convertPSKandIDImpl(psk, pskIdent);
  }
  /// @copydoc convertPSKandID(const char*, const char*)
  bool convertPSKandID(const String& psk, const String& pskIdent) {
    return convertPSKandID(psk.c_str(), pskIdent.c_str());
  }
  /**
   * @brief Convert a pre-shared key (PSK) table file.
   *
   * In some cases, a PSK table file needs to be converted into a format usable
   * by the modem for SSL connections.
   *
   * @param psk_table_name The name of the PSK table file.
   * @return True if the PSK table was successfully converted, false otherwise.
   */
  bool convertPSKTable(const char* psk_table_name) {
    return thisModem().convertPSKTableImpl(psk_table_name);
  }
  /// @copydoc convertPSKTable(const char*)
  bool convertPSKTable(const String& psk_table_name) {
    return convertPSKTable(psk_table_name.c_str());
  }
  /**@}*/


 protected:
  // destructor (protected!)
  ~TinyGsmSSL() {}

  /*
   * CRTP Helper
   */
  inline const modemType& thisModem() const {
    return static_cast<const modemType&>(*this);
  }
  inline modemType& thisModem() {
    return static_cast<modemType&>(*this);
  }

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the default function implementations
   */
 protected:
  /*
   * Secure socket layer (SSL) certificate management functions
   */

  bool loadCertificateImpl(const char* certificateName, const char* cert,
                           const uint16_t len) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool
  deleteCertificateImpl(const char* filename) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool printCertificateImpl(const char* filename,
                            Stream& print_stream) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool convertCertificateImpl(CertificateType cert_type, const char* filename)
      TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool convertCACertificateImpl(const char* ca_cert_name)
      TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool convertClientCertificatesImpl(const char* client_cert_name,
                                     const char* client_cert_key)
      TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool convertPSKandIDImpl(const char* psk,
                           const char* pskIdent) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool
  convertPSKTableImpl(const char* psk_table_name) TINY_GSM_ATTR_NOT_IMPLEMENTED;
};


/**
 * The secure client class
 *
 * @note This is a base class for secure clients, but it is NOT an inner class
 * of the TinyGsmSSL class.
 */
class GsmSecureClient {
 public:
  /// Create a new secure client instance
  GsmSecureClient() {
    sslCtxIndex      = TINY_GSM_DEFAULT_SSL_CTX;
    sslCtxConfigured = false;
    sslAuthMode      = SSLAuthMode::NO_VALIDATION;
    sslVersion       = SSLVersion::TLS1_2;
    memset(CAcertName, '\0', sizeof(CAcertName));
    memset(clientCertName, '\0', sizeof(clientCertName));
    memset(clientKeyName, '\0', sizeof(clientKeyName));
    memset(pskIdent, '\0', sizeof(pskIdent));
    memset(psKey, '\0', sizeof(psKey));
    memset(pskTableName, '\0', sizeof(pskTableName));
  }

  /**
   * @anchor client_ssl_config
   * @name Client SSL configuration functions
   */
  /**@{*/
  /**
   * @brief Set the SSL context index to use for this connection
   * @param sslCtxIndex The SSL context index
   */
  virtual void setSSLContextIndex(uint8_t sslCtxIndex) {
    this->sslCtxIndex = sslCtxIndex;
    sslCtxConfigured  = false;
  }

  /**
   * @brief Set the SSL authorization mode to use for this connection
   * @param mode The SSL authorization mode
   */
  virtual void setSSLAuthMode(SSLAuthMode mode) {
    this->sslAuthMode = mode;
    sslCtxConfigured  = false;
  }

  /**
   * @brief Set the SSL version to use for this connection
   * @param version The SSL version
   */
  virtual void setSSLVersion(SSLVersion version) {
    this->sslVersion = version;
    sslCtxConfigured = false;
  }
  /**@}*/

  /**
   * @anchor client_cert_assignment
   * @name Client certificate assignment functions
   */
  /**@{*/
  /**
   * @brief Set the CA certificate name to use for this connection
   * @param CAcertName The CA certificate name
   */
  virtual void setCACertName(const char* CAcertName) {
    // copy the certificate name into owned buffer
    strncpy(this->CAcertName, CAcertName, sizeof(this->CAcertName) - 1);
    sslCtxConfigured = false;
  }
  /// @copydoc setCACertName(const char*)
  virtual void setCACertName(const String& CAcertName) {
    setCACertName(CAcertName.c_str());
  }

  /**
   * @brief Set the client certificate name to use for this connection
   * @param clientCertName The client certificate name
   */
  virtual void setClientCertName(const char* clientCertName) {
    // copy the certificate name into owned buffer
    strncpy(this->clientCertName, clientCertName,
            sizeof(this->clientCertName) - 1);
    sslCtxConfigured = false;
  }
  /// @copydoc setClientCertName(const char*)
  virtual void setClientCertName(const String& clientCertName) {
    setClientCertName(clientCertName.c_str());
  }

  /**
   * @brief Set the client private key name to use for this connection
   * @param clientKeyName The client private key name
   */
  virtual void setPrivateKeyName(const char* clientKeyName) {
    // copy the key name into owned buffer
    strncpy(this->clientKeyName, clientKeyName,
            sizeof(this->clientKeyName) - 1);
    sslCtxConfigured = false;
  }
  /// @copydoc setPrivateKeyName(const char*)
  virtual void setPrivateKeyName(const String& clientKeyName) {
    setPrivateKeyName(clientKeyName.c_str());
  }
  /**@}*/

  /**
   * @anchor client_psk_assignment
   * @name Client PSK assignment functions
   */
  /**@{*/

  /**
   * @brief Set the PSK table name to use for this connection
   * @param pskTableName The PSK table name
   */
  virtual void setPSKTableName(const char* pskTableName) {
    // copy the PSK table name into owned buffer
    strncpy(this->pskTableName, pskTableName, sizeof(this->pskTableName) - 1);
    sslCtxConfigured = false;
  }
  /// @copydoc setPSKTableName(const char*)
  virtual void setPSKTableName(const String& pskTableName) {
    setPSKTableName(pskTableName.c_str());
  }

  /**
   * @brief Set the pre-shared key and identity to use for this connection
   *
   * @param pskIdent The pre-shared key identity (also called the identity hint
   * or just hint)
   * @param psKey The pre-shared key
   */
  virtual void setPreSharedKey(const char* pskIdent, const char* psKey) {
    // copy the PSK identity and key into owned buffers
    strncpy(this->pskIdent, pskIdent, sizeof(this->pskIdent) - 1);
    strncpy(this->psKey, psKey, sizeof(this->psKey) - 1);
    sslCtxConfigured = false;
  }
  /// @copydoc setPreSharedKey(const char*, const char*)
  virtual void setPreSharedKey(const String& pskIdent, const String& psKey) {
    setPreSharedKey(pskIdent.c_str(), psKey.c_str());
  }
  /**@}*/

  /// destructor
  virtual ~GsmSecureClient() {}

 protected:
  /// The SSL context index to use for this connection
  uint8_t sslCtxIndex;
  /// Flag to denote whether the SSL context has been configured
  bool sslCtxConfigured;
  /// The SSL authorization mode to use for this connection
  SSLAuthMode sslAuthMode;
  /// The SSL version to use for this connection
  SSLVersion sslVersion;
  /// The FILE NAME of the certificate authority certificate loaded onto the
  /// module
  char CAcertName[TINY_GSM_CERT_NAME_LENGTH] = "\0";
  /// The FILE NAME of the client certificate loaded onto the module
  char clientCertName[TINY_GSM_CERT_NAME_LENGTH] = "\0";
  /// The FILE NAME of the client private key loaded onto the module
  char clientKeyName[TINY_GSM_CERT_NAME_LENGTH] = "\0";
  /// The FILE NAME of an identity for PSK cipher suites
  char pskTableName[TINY_GSM_CERT_NAME_LENGTH] = "\0";
  /// The pre-shared key identifier VALUE for PSK cipher suites (also called
  /// identity, identifier hint, or hint).
  char pskIdent[TINY_GSM_PSK_IDENTITY_LENGTH] = "\0";
  /// The VALUE of the key in hex for PSK cipher suites
  char psKey[TINY_GSM_PSK_LENGTH] = "\0";
};


/// short-cut macro for constructors
#define TINY_GSM_SECURE_CLIENT_CTORS(modemAbbrev)                             \
  GsmClientSecure##modemAbbrev() {                                            \
    is_secure = true;                                                         \
  }                                                                           \
  explicit GsmClientSecure##modemAbbrev(TinyGsm##modemAbbrev& modem,          \
                                        uint8_t               mux = 0)        \
      : GsmClient##modemAbbrev(modem, mux) {                                  \
    is_secure = true;                                                         \
  }                                                                           \
  explicit GsmClientSecure##modemAbbrev(                                      \
      TinyGsm##modemAbbrev& modem, SSLAuthMode sslAuthMode,                   \
      SSLVersion  sslVersion = SSLVersion::TLS1_2,                            \
      const char* CAcertName = nullptr, const char* clientCertName = nullptr, \
      const char* clientKeyName = nullptr)                                    \
      : GsmClient##modemAbbrev(modem) {                                       \
    is_secure = true;                                                         \
    setSSLAuthMode(sslAuthMode);                                              \
    setSSLVersion(sslVersion);                                                \
    setCACertName(CAcertName);                                                \
    setClientCertName(clientCertName);                                        \
    setPrivateKeyName(clientKeyName);                                         \
  }                                                                           \
  explicit GsmClientSecure##modemAbbrev(                                      \
      TinyGsm##modemAbbrev& modem, uint8_t mux, SSLAuthMode sslAuthMode,      \
      SSLVersion  sslVersion = SSLVersion::TLS1_2,                            \
      const char* CAcertName = nullptr, const char* clientCertName = nullptr, \
      const char* clientKeyName = nullptr)                                    \
      : GsmClient##modemAbbrev(modem, mux) {                                  \
    is_secure = true;                                                         \
    setSSLAuthMode(sslAuthMode);                                              \
    setSSLVersion(sslVersion);                                                \
    setCACertName(CAcertName);                                                \
    setClientCertName(clientCertName);                                        \
    setPrivateKeyName(clientKeyName);                                         \
  }                                                                           \
  explicit GsmClientSecure##modemAbbrev(                                      \
      TinyGsm##modemAbbrev& modem, const char* pskIdent, const char* psKey,   \
      SSLVersion sslVersion = SSLVersion::TLS1_2)                             \
      : GsmClient##modemAbbrev(modem) {                                       \
    is_secure = true;                                                         \
    setSSLAuthMode(SSLAuthMode::PRE_SHARED_KEYS);                             \
    setSSLVersion(sslVersion);                                                \
    setPreSharedKey(pskIdent, psKey);                                         \
  }                                                                           \
  explicit GsmClientSecure##modemAbbrev(                                      \
      TinyGsm##modemAbbrev& modem, uint8_t mux, const char* pskIdent,         \
      const char* psKey, SSLVersion sslVersion = SSLVersion::TLS1_2)          \
      : GsmClient##modemAbbrev(modem, mux) {                                  \
    is_secure = true;                                                         \
    setSSLAuthMode(SSLAuthMode::PRE_SHARED_KEYS);                             \
    setSSLVersion(sslVersion);                                                \
    setPreSharedKey(pskIdent, psKey);                                         \
  }                                                                           \
  explicit GsmClientSecure##modemAbbrev(                                      \
      TinyGsm##modemAbbrev& modem, const char* pskTableName,                  \
      SSLVersion sslVersion = SSLVersion::TLS1_2)                             \
      : GsmClient##modemAbbrev(modem) {                                       \
    is_secure = true;                                                         \
    setSSLAuthMode(SSLAuthMode::PRE_SHARED_KEYS);                             \
    setSSLVersion(sslVersion);                                                \
    setPSKTableName(pskTableName);                                            \
  }                                                                           \
  explicit GsmClientSecure##modemAbbrev(                                      \
      TinyGsm##modemAbbrev& modem, uint8_t mux, const char* pskTableName,     \
      SSLVersion sslVersion = SSLVersion::TLS1_2)                             \
      : GsmClient##modemAbbrev(modem, mux) {                                  \
    is_secure = true;                                                         \
    setSSLAuthMode(SSLAuthMode::PRE_SHARED_KEYS);                             \
    setSSLVersion(sslVersion);                                                \
    setPSKTableName(pskTableName);                                            \
  }

// cSpell:words Palo

#endif  // SRC_TINYGSMSSL_TPP_
