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
  CA_VALIDATION         = 1,
  MUTUAL_AUTHENTICATION = 2,
  CLIENT_VALIDATION     = 3,
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

enum class TinyGSMDateTimeFormat : int8_t {
  DATE_FULL = 0,
  DATE_TIME = 1,
  DATE_DATE = 2
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
