# ChangeLog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) and its stricter, better defined, brother [Common Changelog](https://common-changelog.org/).

This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

***

## [Unreleased]

### Changed

- **BREAKING** Rewrote SSL support module!
  - All function names have changed!
    - Modem functions have been renamed to "load" instead of "set" to clarify that the certificate data is being loaded into the flash of the module rather than being used somewhere in the library code.
    - Client functions have been renamed to "setXX**Name**" to clarify that what you are selecting is the file name of a certificate that has been pre-loaded onto the module, not the certificate data itself.
    - A new parent class has been created for sockets
    - All enums have been moved into a separate file
- Made date/time enums and SSL enums enum classes, requiring the class name to be included when calling them.
- Separated Espressif into **3** modules:
  - the ESP32 (requiring AT firmware >= 3.2),
  - the ESP8266 using the final release of its AT firmware ([v2.2.1_esp8266](https://github.com/espressif/esp-at/releases/tag/v2.2.1.0_esp8266)),
  - and the ESP8266 using the original "non-OS" firmware that first version of this library was written for.
- Made adjustments and corrections to the A7672x based on similar functionality of the SIM7600.
- Fixed various compiler warnings, where possible
- Increased max baud rate for autobauding.
- Minor changes in notes and comments

### Added

- Added support for selecting SSL version and authentication types on select modules
- Added support for writing/loading certificates on select modules
- Added support for selecting specific certificates for specific connections on select modules
  - This includes both CA and client certs
- Added SSL support for the SIM7600 by @floBik
- Added an example connecting to AWS IoT Core with mutual authentication.
- Added an example using multiple connections at once
- Added an "is_secure" property to all clients to help differentiate them
- Added defines for the number of secure sockets (`TINY_GSM_SECURE_MUX_COUNT`)

### Removed

- Removed certificate arrays from the modem objects
- Only include the SSL template on the modules that support the SSL enhancements and certificate management instead of a bare flag for security.

### Fixed

***

## [v0.12.0]

### Changed

- Moved bulk of waitResponse function to modem template and gave modems handleURCs fxn
- Moved option in waitResponse for verbose outputs.
- setBaud now returns a bool
- Replace new line consts with defines and renamed to AT_NL
- Renamed all RegStatus enums to be unique
- Replaced `NULL` with `nullptr` and replaced c-style casts.
- Moved setCertificates function and the certificate name matrix to the SSL template.
- Changed inputs for (unimplemented) SSL certificate functions.
- All modems will now return the pre-defined manufacturer and model in the name if the function to get the internal name fails.
- Cleaned up code for getting modem names.
- Made battery return types signed.

### Added

- Added support for SSL for the Quentcel BG95 and BG96 from [Aurelien BOUIN](https://github.com/aurelihein) and [George O'Connor](https://github.com/georgeman93)
- Added support for UBLOX SARA-R5 from [Sebastian Bergner](https://github.com/sebastianbergner)
- Added support for SIMCOM A7672X from [Giovanni de Rosso Unruh](https://github.com/giovannirosso)
- Added SIM5320 GPS location from [Bengarman](https://github.com/Bengarman)
- Added functions `getModemSerialNumber`, `getModemModel`, and `getModemRevision`.
- Added deep debugging option
- Added documentation to the FIFO class

### Removed

- Removed non-functional factory reset from SIM70xx series

### Fixed

- Removed extra wait on SIM7000 from [Mikael Fredriksson](https://github.com/Gelemikke)
- Fix status returns on ESP8266/ESP32 AT commands
- Fix length of HEX for Sequans Monarch
- Fix SIM7600 password then user when cid is set from [github0013](https://github.com/github0013)
- Fix cardinal points in location by gps for SIM7600 from [Juxn3](https://github.com/Juxn3)
- Fix NTP server sync for SIM70xx models from [Gonzalo Brusco](https://github.com/gonzabrusco)
- Fixed SIM70xx inheritance

***
