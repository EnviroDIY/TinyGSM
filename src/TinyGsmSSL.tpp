/**
 * @file       TinyGsmSSL.tpp
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMSSL_H_
#define SRC_TINYGSMSSL_H_

#include "TinyGsmCommon.h"

#define TINY_GSM_MODEM_HAS_SSL
#define TINY_GSM_MODEM_CAN_SPECIFY_CERTS
#define TINY_GSM_MODEM_CAN_LOAD_CERTS

#ifndef TINY_GSM_DEFAULT_SSL_CTX
#define TINY_GSM_DEFAULT_SSL_CTX 0
#endif


template <class modemType, uint8_t muxCount>
class TinyGsmSSL {
  /* =========================================== */
  /* =========================================== */
  /*
   * Define the interface
   */
 public:
  /*
   * Secure socket layer (SSL) certificate management functions
   */
  TinyGsmSSL() {}

  // Load a new certificate onto the module, including the whole text of the
  // certificate
  // A file will be created (or updated) on the module's filesystem with the
  // content of the certificate
  bool loadCertificate(const char* certificateName, const char* cert,
                       const uint16_t len) {
    return thisModem().loadCertificateImpl(certificateName, cert, len);
  }
  bool loadCertificate(String certificateName, String cert,
                       const uint16_t len) {
    return loadCertificate(certificateName.c_str(), cert.c_str(), len);
  }

  // delete a certificate by name from the module's filesystem
  // NOTE: The functions for deleting a certificate rarely depend on the
  // certificate type
  bool deleteCertificate(const char* filename) {
    return thisModem().deleteCertificateImpl(filename);
  }
  bool deleteCertificate(String filename) {
    return deleteCertificate(filename.c_str());
  }

  // print the contents of a certificate file to a stream
  bool printCertificate(const char* filename, Stream& print_stream) {
    return thisModem().printCertificateImpl(filename, print_stream);
  }
  bool printCertificate(String filename, Stream& print_stream) {
    return printCertificate(filename.c_str(), print_stream);
  }

  // for many (but not all!) modules, the certificate must be "converted" from a
  // file stored somewhere in the flash file system into a certificate ready to
  // be used by the connection
  bool convertCertificate(CertificateType cert_type, const char* filename) {
    return thisModem().convertCertificateImpl(cert_type, filename);
  }
  bool convertCertificate(CertificateType cert_type, String filename) {
    return thisModem().convertCertificate(cert_type, filename.c_str());
  }
  // Convert a certificate authority certificate
  bool convertCACertificate(const char* ca_cert_name) {
    return thisModem().convertCACertificateImpl(ca_cert_name);
  }
  bool convertCACertificate(String ca_cert_name) {
    return thisModem().convertCACertificate(ca_cert_name.c_str());
  }
  // in some cases, the two parts of the client certificate (the certificate
  // itself and the key to it) need to be converted together
  bool convertClientCertificates(const char* client_cert_name,
                                 const char* client_cert_key) {
    return thisModem().convertClientCertificatesImpl(client_cert_name,
                                                     client_cert_key);
  }
  bool convertClientCertificates(String client_cert_name,
                                 String client_cert_key) {
    return thisModem().convertClientCertificates(client_cert_name.c_str(),
                                                 client_cert_key.c_str());
  }
  // in some cases, the two parts of the pre-shared key must be converted
  // together (the PSK and the identity assigned to it)
  bool convertPSKandID(const char* psk, const char* pskIdent) {
    return thisModem().convertPSKandIDImpl(psk, pskIdent);
  }
  bool convertPSKandID(String psk, String pskIdent) {
    return thisModem().convertPSKandID(psk.c_str(), pskIdent.c_str());
  }
  // Convert a single PSK table file
  bool convertPSKTable(const char* psk_table_name) {
    return thisModem().convertPSKTableImpl(psk_table_name);
  }
  bool convertPSKTable(String psk_table_name) {
    return thisModem().convertPSKTable(psk_table_name.c_str());
  }

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

  /*
   * Secure socket layer (SSL) certificate management functions
   */
 protected:
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


/*
 * Secure Client
 * NOTE: This is a base class for secure clients, but it is NOT an inner class
 * of the TinyGsmSSL class.
 */
class GsmSecureClient {
 public:
  GsmSecureClient() {
    sslCtxIndex      = TINY_GSM_DEFAULT_SSL_CTX;
    sslCtxConfigured = false;
    sslAuthMode      = SSLAuthMode::NO_VALIDATION;
    sslVersion       = SSLVersion::TLS1_2;
  }

  virtual void setSSLContextIndex(uint8_t sslCtxIndex) {
    this->sslCtxIndex = sslCtxIndex;
    sslCtxConfigured  = false;
  }

  virtual void setSSLAuthMode(SSLAuthMode mode) {
    this->sslAuthMode = mode;
    sslCtxConfigured  = false;
  }

  virtual void setSSLVersion(SSLVersion version) {
    this->sslVersion = version;
    sslCtxConfigured = false;
  }

  virtual void setCACertName(String CAcertName) {
    this->CAcertName = std::move(CAcertName);    
    sslCtxConfigured = false;
  }
  virtual void setCACertName(const char* CAcertName) {
    setCACertName(String{CAcertName});
  }

  virtual void setClientCertName(String clientCertName) {
    this->clientCertName = std::move(clientCertName);
    sslCtxConfigured     = false;
  }
  virtual void setClientCertName(const char* clientCertName) {
    setClientCertName(String{clientCertName});
  }

  virtual void setPrivateKeyName(String clientKeyName) {
    this->clientKeyName = clientKeyName;
    sslCtxConfigured    = false;
  }
  virtual void setPrivateKeyName(const char*  clientKeyName) {
    setPrivateKeyName(String{clientKeyName});
  }

  virtual void setPSKTableName(String pskTableName) {
    this->pskTableName = pskTableName;
    sslCtxConfigured   = false;
  }
  virtual void setPSKTableName(const char*  pskTableName) {
    setPSKTableName(String{pskTableName});
  }
  virtual void setPreSharedKey(String pskIdent, String psKey) {
    this->pskIdent   = pskIdent;
    this->psKey      = psKey;
    sslCtxConfigured = false;
  }
  virtual void setPreSharedKey(const char*  pskIdent, const char*  psKey) {
    setPreSharedKey(String{pskIdent}, String{psKey});
  }

  // destructor
  virtual ~GsmSecureClient() = default;

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
  String CAcertName;
  /// The FILE NAME of the client certificate loaded onto the module
  String clientCertName;
  /// The FILE NAME of the client private key loaded onto the module
  String clientKeyName;
  /// The FILE NAME of an identity for PSK cipher suites
  String pskTableName;
  /// The identity VALUE for PSK cipher suites
  String pskIdent;
  /// The VALUE of the key in hex for PSK cipher suites
  String psKey;
};


// short-cut macro for constructors
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
    setSSLAuthMode(SSLAuthMode::PRE_SHARED_KEYS);                             \
    setSSLVersion(sslVersion);                                                \
    setPSKTableName(pskTableName);                                            \
  }

#endif  // SRC_TINYGSMSSL_H_
