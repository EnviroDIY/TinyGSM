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

enum class CertificateType : int8_t {
  CA_CERTIFICATE      = 0,
  CLIENT_CERTIFICATE  = 1,
  CLIENT_KEY          = 2,
  CLIENT_PSK          = 3,
  CLIENT_PSK_IDENTITY = 4,
};

// <auth_mode>:
//     0: no validation
//        - SRGD Note: Very insecure! You do not need to load any certificates
//        onto your device for this. Not all servers will accept it.
//     1: the client provides the client certificate for the server to
//     verify.
//        - SRGD Note: I do not believe this is commonly used. To use
//        this, you must load a client certificate and a client key onto
//        your device.
//     2: the client loads CA certificate to verify the server’s
//     certificate.
//        - SRGD Note: This is a common authentication type sed by
//        browsers, where the browser verifies the server's certificate.
//        For this to work, you must load either the server's
//        intermediate or parent CA certificate onto your device.
//     3: mutual authentication.
//        - SRGD Note: This is used by AWS IoT Core and other IoT
//        services. In this case you must load 3 certs to your device:
//        The server's CA cert, the client cert, and the client key.
//     4: pre-shared key encryption
enum class SSLAuthMode : int8_t {
  NO_VALIDATION         = 0,
  CLIENT_VALIDATION     = 1,
  CA_VALIDATION         = 2,
  MUTUAL_AUTHENTICATION = 3,
  PRE_SHARED_KEYS       = 4,
};

enum class SSLVersion : int8_t {
  NO_SSL  = -1,
  SSL3_0  = 0,
  TLS1_0  = 1,
  TLS1_1  = 2,
  TLS1_2  = 3,
  ALL_SSL = 4,
  TLS1_3  = 5
};


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
    this->sslAuthMode    = SSLAuthMode::NO_VALIDATION;
    this->sslVersion     = SSLVersion::TLS1_2;
    this->CAcertName     = nullptr;
    this->clientCertName = nullptr;
    this->clientKeyName  = nullptr;
    this->pskIdent       = nullptr;
    this->psKey          = nullptr;
  }
  explicit GsmSecureClient(SSLAuthMode sslAuthMode,
                           const char* CAcertName     = nullptr,
                           const char* clientCertName = nullptr,
                           const char* clientKeyName  = nullptr) {
    this->sslAuthMode    = sslAuthMode;
    this->CAcertName     = CAcertName;
    this->clientCertName = clientCertName;
    this->clientKeyName  = clientKeyName;
    this->pskIdent       = nullptr;
    this->psKey          = nullptr;
  }
  explicit GsmSecureClient(const char* pskIdent, const char* psKey) {
    this->sslAuthMode    = SSLAuthMode::PRE_SHARED_KEYS;
    this->CAcertName     = nullptr;
    this->clientCertName = nullptr;
    this->clientKeyName  = nullptr;
    this->pskIdent       = pskIdent;
    this->psKey          = psKey;
  }

  virtual void setSSLAuthMode(SSLAuthMode mode) {
    this->sslAuthMode = mode;
  }

  virtual void setSSLVersion(SSLVersion version) {
    this->sslVersion = version;
  }

  virtual void setCACertName(const char* CAcertName) {
    this->CAcertName = CAcertName;
  }
  virtual void setCACertName(String CAcertName) {
    setCACertName(CAcertName.c_str());
  }

  virtual void setClientCertName(const char* clientCertName) {
    this->clientCertName = clientCertName;
  }
  virtual void setClientCertName(String clientCertName) {
    setClientCertName(clientCertName.c_str());
  }

  virtual void setPrivateKeyName(const char* clientKeyName) {
    this->clientKeyName = clientKeyName;
  }
  virtual void setPrivateKeyName(String clientKeyName) {
    setPrivateKeyName(clientKeyName.c_str());
  }

  virtual void setPSKTableName(const char* pskTableName) {
    this->pskTableName = pskTableName;
  }
  virtual void setPSKTableName(String pskTableName) {
    setPSKTableName(pskTableName.c_str());
  }
  virtual void setPreSharedKey(const char* pskIdent, const char* psKey) {
    this->pskIdent = pskIdent;
    this->psKey    = psKey;
  }
  virtual void setPreSharedKey(String pskIdent, String psKey) {
    setPreSharedKey(pskIdent.c_str(), psKey.c_str());
  }

  // destructor - need to remove self from the socket pointer array
  virtual ~GsmSecureClient() {}

 protected:
  /// The SSL authorization mode to use for this connection
  SSLAuthMode sslAuthMode;
  /// The SSL version to use for this connection
  SSLVersion sslVersion;
  /// The FILE NAME of the certificate authority certificate loaded onto the
  /// module
  const char* CAcertName;
  /// The FILE NAME of the client certificate loaded onto the module
  const char* clientCertName;
  /// The FILE NAME of the client private key loaded onto the module
  const char* clientKeyName;
  /// The FILE NAME of an identity for PSK cipher suites
  const char* pskTableName;
  /// The identity VALUE for PSK cipher suites
  const char* pskIdent;
  /// The VALUE of the key in hex for PSK cipher suites
  const char* psKey;
};

#endif  // SRC_TINYGSMSSL_H_
