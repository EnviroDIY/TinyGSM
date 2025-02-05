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
#include "TinyGsmTCP.tpp"

#define TINY_GSM_MODEM_HAS_SSL

typedef enum {
  CA_CERTIFICATE      = 0,
  CLIENT_CERTIFICATE  = 1,
  CLIENT_KEY          = 2,
  CLIENT_PSK          = 3,
  CLIENT_PSK_IDENTITY = 4,
} CertificateType;

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
typedef enum {
  NO_VALIDATION         = 0,
  CLIENT_VALIDATION     = 1,
  CA_VALIDATION         = 2,
  MUTUAL_AUTHENTICATION = 3,
  PRE_SHARED_KEYS       = 4,
} SSLAuthMode;

// macro function to help create more functions
#define TINY_GSM_MAKE_SSL_FUNCTIONS(cert_abbrev, cert_type)                   \
  bool add##cert_abbrev(const char* filename) {                               \
    return thisModem().addCertificateImpl(cert_type, filename);               \
  }                                                                           \
  bool add##cert_abbrev(String& filename) {                                   \
    return add##cert_abbrev(cert_type, filename.c_str());                     \
  }                                                                           \
  bool add##cert_abbrev(const char* certificateName, const char* cert,        \
                        const uint16_t len) {                                 \
    return addCertificateImpl(cert_type, certificateName, cert, len);         \
  }                                                                           \
  bool add##cert_abbrev(String& certificateName, String& cert,                \
                        const uint16_t len) {                                 \
    return add##cert_abbrev(cert_type, certificateName.c_str(), cert.c_str(), \
                            len);                                             \
  }                                                                           \
  bool set##cert_abbrev(const char* certificateName, const uint8_t mux = 0) { \
    return set##cert_abbrev(cert_type, certificateName, mux);                 \
  }                                                                           \
  bool set##cert_abbrev(String& certificateName, const uint8_t mux = 0) {     \
    return set##cert_abbrev(cert_type, certificateName, mux);                 \
  }                                                                           \
  bool convert##cert_abbrev(const char* filename) {                           \
    return thisModem().convertCertificateImpl(cert_type, filename);           \
  }                                                                           \
  bool convert##cert_abbrev(String& filename) {                               \
    return thisModem().convertCertificate(cert_type, filename.c_str());       \
  }

#define TINY_GSM_MAKE_CLIENT_SET_FUNCTIONS(cert_abbrev, cert_type)     \
  bool set##cert_abbrev(const char* certificateName) {                 \
    return thisClient().at->setCertificate(cert_type, certificateName, \
                                           thisClient().mux);          \
  }                                                                    \
  bool set##cert_abbrev(String& certificateName) {                     \
    return thisClient().at->setCertificate(cert_type, certificateName, \
                                           thisClient().mux);          \
  }

template <class sslModemType, uint8_t muxCount>
class TinyGsmSSL {  //: public TinyGsmTCP<sslModemType, muxCount> {
                    // friend class TinyGsmTCP<sslModemType, muxCount>;

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the interface
   */
 public:
  /*
   * Secure socket layer (SSL) certificate management functions
   */

  // Assign a certificate by certificate type and the filename of the
  // certificate The filename should be of a file already saved in the module's
  // file system
  bool addCertificate(CertificateType cert_type, const char* filename) {
    return thisModem().addCertificateImpl(cert_type, filename);
  }
  bool addCertificate(CertificateType cert_type, String& filename) {
    return thisModem().addCertificateImpl(cert_type, filename.c_str());
  }

  // Assign a certificate by certificate type, including the whole text of the
  // certificate
  // A file will be created (or updated) on the module's filesystem with the
  // content of the certificate
  bool addCertificate(CertificateType cert_type, const char* certificateName,
                      const char* cert, const uint16_t len) {
    return thisModem().addCertificateImpl(cert_type, certificateName, cert,
                                          len);
  }
  bool addCertificate(CertificateType cert_type, String& certificateName,
                      String& cert, const uint16_t len) {
    return addCertificate(cert_type, certificateName.c_str(), cert.c_str(),
                          len);
  }

  // delete a certificate by name from the module's filesystem
  // NOTE: The functions for deleting a certificate rarely depend on the
  // certificate type
  bool deleteCertificate(const char* filename) {
    return thisModem().deleteCertificateImpl(filename);
  }
  bool deleteCertificate(String& filename) {
    return deleteCertificate(filename.c_str());
  }

  // setting a certificate assigns that certificate to be used by a specific
  // socket - ie, it puts the name of the certificate into the correct spot for
  // the mux in the certificate array
  bool setCertificate(CertificateType cert_type, const char* certificateName,
                      const uint8_t mux = 0) {
    if (mux >= muxCount) return false;
    switch (cert_type) {
      case CLIENT_PSK_IDENTITY: {
        thisModem().pskIdents[mux] = certificateName;
        break;
      }
      case CLIENT_PSK: {
        thisModem().psKeys[mux] = certificateName;
        break;
      }
      case CLIENT_KEY: {
        thisModem().clientKeys[mux] = certificateName;
        break;
      }
      case CLIENT_CERTIFICATE: {
        thisModem().clientCerts[mux] = certificateName;
        break;
      }
      case CA_CERTIFICATE:
      default: {
        thisModem().CAcerts[mux] = certificateName;
        break;
      }
    }
    return true;
  }
  bool setCertificate(CertificateType cert_type, String& certificateName,
                      const uint8_t mux = 0) {
    return setCertificate(cert_type, certificateName.c_str(), mux);
  }

  // for many (but not all!) modules, the certificate must be "converted" from a
  // file stored somewhere in the flash file system into a certificate ready to
  // be used by the TCP connection
  bool convertCertificate(CertificateType cert_type, const char* filename) {
    return thisModem().convertCertificateImpl(cert_type, filename);
  }
  bool convertCertificate(CertificateType cert_type, String& filename) {
    return thisModem().convertCertificate(cert_type, filename.c_str());
  }
  // in some cases, the two parts of the client certificate (the certificate
  // itself and the key to it) need to be converted together
  bool convertClientCertificates(const char* client_cert_name,
                                 const char* client_cert_key) {
    return thisModem().convertClientCertificatesImpl(client_cert_name,
                                                     client_cert_key);
  }
  bool convertClientCertificates(String& client_cert_name,
                                 String& client_cert_key) {
    return thisModem().convertClientCertificates(client_cert_name.c_str(),
                                                 client_cert_key.c_str());
  }
  // in some cases, the two parts of the pre-shared key must be converted
  // together (the PSK and the identity assigned to it)
  bool convertPSKandID(const char* psk, const char* pskIdent) {
    return thisModem().convertPSKandIDImpl(psk, pskIdent);
  }
  bool convertPSKandID(String& psk, String& pskIdent) {
    return thisModem().convertPSKandID(psk.c_str(), pskIdent.c_str());
  }

  // NOTE: For backwards compatibility, adding a certificate without a type
  // assumes it's from a certificate authority
  // Rename for CA certificates; ie add/setCertificate(..)
  TINY_GSM_MAKE_SSL_FUNCTIONS(Certificate, CA_CERTIFICATE)
  // functions for client certs; ie add/setCACert(..)
  TINY_GSM_MAKE_SSL_FUNCTIONS(CACert, CA_CERTIFICATE)
  // functions for client certs; ie add/setClientCert(..)
  TINY_GSM_MAKE_SSL_FUNCTIONS(ClientCert, CLIENT_CERTIFICATE)
  // functions for client keys; ie add/setPrivateKey(..)
  TINY_GSM_MAKE_SSL_FUNCTIONS(PrivateKey, CLIENT_KEY)
  // functions for pre-shared keys; ie add/setPSK(..)
  TINY_GSM_MAKE_SSL_FUNCTIONS(PSK, CLIENT_PSK)
  // functions for pre-shared key identities; ie add/setPSKID(..)
  TINY_GSM_MAKE_SSL_FUNCTIONS(PSKID, CLIENT_PSK_IDENTITY)


  // retained for backwards compatibility
  // if a cert is added or set without a type, assume it's a certificate
  // authority certificate

  /*
   * CRTP Helper
   */
 protected:
  inline const sslModemType& thisModem() const {
    return static_cast<const sslModemType&>(*this);
  }
  inline sslModemType& thisModem() {
    return static_cast<sslModemType&>(*this);
  }
  ~TinyGsmSSL() {}

  /*
   * Inner Secure Client
   */
 public:
  template <class tcpClientType>
  class GSMSecureClient : public tcpClientType {
    // Make all classes created from the modem template friends
    friend class TinyGsmTCP<sslModemType, muxCount>;
    friend class TinyGsmSSL<sslModemType, muxCount>;

   public:
    GSMSecureClient() {}

    explicit GSMSecureClient(sslModemType& modem, uint8_t mux = -1)
        : tcpClientType(modem, mux) {}

    void setSSLAuthMode(SSLAuthMode mode) {
      thisClient().at->sslAuthModes[thisClient().mux] = mode;
    }

    // set the name of the certificate into the certificate name buffer of the
    // modem
    bool setCertificate(CertificateType cert_type,
                        const char*     certificateName) {
      return thisClient().at->setCertificate(cert_type, certificateName,
                                             thisClient().mux);
    }
    bool setCertificate(CertificateType cert_type, String& certificateName) {
      return thisClient().at->setCertificate(cert_type, certificateName,
                                             thisClient().mux);
    }

    void setPreSharedKey(const char* pskIdent, const char* psKey) {
      thisClient().at->pskIdents[thisClient().mux] = pskIdent;
      thisClient().at->psKeys[thisClient().mux]    = psKey;
    }
    // NOTE: For backwards compatibility, adding a certificate without a type
    // assumes it's from a certificate authority
    // Rename for CA certificates; ie setCertificate(..)
    TINY_GSM_MAKE_CLIENT_SET_FUNCTIONS(Certificate, CA_CERTIFICATE)
    // functions for client certs; ie setCACert(..)
    TINY_GSM_MAKE_CLIENT_SET_FUNCTIONS(CACert, CA_CERTIFICATE)
    // functions for client certs; ie setClientCert(..)
    TINY_GSM_MAKE_CLIENT_SET_FUNCTIONS(ClientCert, CLIENT_CERTIFICATE)
    // functions for client keys; ie setPrivateKey(..)
    TINY_GSM_MAKE_CLIENT_SET_FUNCTIONS(PrivateKey, CLIENT_KEY)
    // functions for pre-shared keys; ie setPSK(..)
    TINY_GSM_MAKE_CLIENT_SET_FUNCTIONS(PSK, CLIENT_PSK)
    // functions for pre-shared key identities; ie setPSKID(..)
    TINY_GSM_MAKE_CLIENT_SET_FUNCTIONS(PSKID, CLIENT_PSK_IDENTITY)


    /*
     * CRTP Helper
     */
   protected:
    inline const tcpClientType& thisClient() const {
      return static_cast<const tcpClientType&>(*this);
    }
    inline tcpClientType& thisClient() {
      return static_cast<tcpClientType&>(*this);
    }
  };

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the default function implementations
   */

  /*
   * Secure socket layer (SSL) certificate management functions
   */
 protected:
  bool addCertificateImpl(CertificateType cert_type,
                          const char* filename) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool addCertificateImpl(CertificateType cert_type,
                          const char* certificateName, const char* cert,
                          const uint16_t len) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool
  deleteCertificateImpl(const char* filename) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool convertCertificateImpl(CertificateType cert_type, const char* filename)
      TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool convertClientCertificatesImpl(const char* client_cert_name,
                                     const char* client_cert_key)
      TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool convertPSKandIDImpl(const char* psk,
                           const char* pskIdent) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  // NOTE: These are all stored within the SSL modem object instead of the
  // secure client object so secure and insecure clients can be mixed in the
  // same socket array
  SSLAuthMode sslAuthModes[muxCount];
  const char* CAcerts[muxCount];
  const char* clientCerts[muxCount];
  const char* clientKeys[muxCount];
  const char* pskIdents[muxCount];  // identity for PSK cipher suites
  const char* psKeys[muxCount];     // key in hex for PSK cipher suites
};

#endif  // SRC_TINYGSMSSL_H_
