/**
 * @file       TinyGsmEnums.h
 * @author     Sara Damiano
 * @license    LGPL-3.0
 */

#ifndef SRC_TINYGSMENUMS_H_
#define SRC_TINYGSMENUMS_H_

#if defined(SPARK) || defined(PARTICLE)
#include "Particle.h"
#elif defined(ARDUINO)
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#endif

/// Enum for different types of SSL/TLS certificates.
enum class CertificateType : int8_t {
  CA_CERTIFICATE = 0,  ///< The CA certificate is used to verify the server's
                       ///< certificate. This is the most common type of
                       ///< certificate used in SSL connections.
  CLIENT_CERTIFICATE =
      1,  ///< The client certificate is used to authenticate the client to the
          ///< server. This is less common and is typically used in mutual
          ///< authentication scenarios.
  CLIENT_KEY = 2,  ///< The client key is used in conjunction with the client
                   ///< certificate for mutual authentication. It is a private
                   ///< key that should be kept secure and not shared.
  CLIENT_PSK = 3,  ///< The client pre-shared key is used for authentication in
                   ///< TLS-PSK connections. It is a secret key shared between
                   ///< the client and the server.
  CLIENT_PSK_IDENTITY =
      4,  ///< The client pre-shared key identity is used in conjunction with
          ///< the client PSK for authentication in TLS-PSK connections.
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
  NO_SSL  = -1,  ///< No SSL/TLS, plain TCP connection
  SSL3_0  = 0,   ///< SSL 3.0
  TLS1_0  = 1,   ///< TLS 1.0
  TLS1_1  = 2,   ///< TLS 1.1
  TLS1_2  = 3,   ///< TLS 1.2
  ALL_SSL = 4,  ///< Try all SSL/TLS versions, the modem will negotiate the best
                ///< version
  TLS1_3 = 5    ///< TLS 1.3
};

/// Enum for different date/time formats.
enum class TinyGSMDateTimeFormat : int8_t {
  DATE_FULL = 0,  ///< The full date and time
  DATE_TIME = 1,  ///< Only the time portion of the date/time
  DATE_DATE = 2   ///< Only the date portion of the date/time
};
/**
 * @brief Set the epoch start value.
 */
enum class TinyGSM_EpochStart : int8_t {
  UNIX = 0,  ///< Use a Unix epoch, starting 1/1/1970 (946684800s behind of Y2K
             ///< epoch, 315878400ss behind of GPS epoch)
  Y2K = 1,   ///< Use an epoch starting 1/1/2000, as some RTC's and Arduinos do
            ///< (946684800s ahead of UNIX epoch, 630806400s ahead of GPS epoch)
  GPS = 2  ///< Use the GPS epoch starting Jan 5, 1980 (315878400s ahead of UNIX
           ///< epoch, 630806400s behind of Y2K epoch)
};

#endif
