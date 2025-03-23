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
- Created missing interface for CRTP for the client related modem functions
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
- Added functions `beginWrite` and `endWrite` to directly write into the modem's send buffer to help ensure than an entire request is sent in one chunk.
  - WARNING: These functions will **NOT** warn you or prevent you from writing more to the modem's send buffer than it is possible for the modem to receive
    - Prior to this version of the library, there wasn't any support for limiting requests to the modem's built-in limits on any functions. There now is.
  - These functions are directly analogous to the `beginPublish` and `endPublish` functions within [PubSubClient](https://github.com/knolleary/pubsubclient) and should be usable in combination with them.

```cpp
gsmClient.beginWrite(topic_len + msg_len + MQTT_MAX_HEADER_SIZE);
// NOTE: total length _**MUST**_ be less than the specific module's maximum send size
// You can call TINY_GSM_SEND_MAX_SIZE to verify you haven't exceeded the maximum
mqttClient.beginPublish(topic, msg_len, false);
gsmClient.write((uint8_t*)message, strlen(message));
// or mqttClient.write(...) or stream.write(...)
// print/println functions should also work here
mqttClient.endPublish();
gsmClient.endWrite(strlen(request));
```

- Added an example connecting to AWS IoT Core with mutual authentication.
- Added an example using multiple connections at once
- Added an "is_secure" property to all clients to help differentiate them
- Added a function (`setDefaultBaud(uint32_t baud)`) to set defuault baud rate on Espressif modules
- Added defines for the frequency of checking for lost incoming message notifications (`TINY_GSM_UNREAD_CHECK_MS`)
  - This can be used as an external build flag.
- Added defines for the maximum size each modem accepts in a single send command (`TINY_GSM_SEND_MAX_SIZE`)
  - *This is only to be used internally*
  - Implemented support for break all send calls into chunks less than the specified `TINY_GSM_SEND_MAX_SIZE` for each module.
- Added defines for the number of secure sockets (`TINY_GSM_SECURE_MUX_COUNT`)
  - *This is only for reference to be used internally*
- Added defines `TINY_GSM_MUX_DYNAMIC` and `TINY_GSM_MUX_STATIC` to be used *internally* to help build module support
  - Implemented use of these for all modules.
- Implemented a function (`moveCharsFromStreamToFifo`) for transferring a stream of characters from the modem stream into the fifo.
  - Implemented use of this function for all modules.

### Removed

- Removed certificate arrays from the modem objects
- Only include the SSL template on the modules that support the SSL enhancements and certificate management instead of a bare flag for security.

### Fixed

- Don't force maintain to call modemGetAvailable if the sock_available is already non-zero
- Don't repeatedly call for sock_connected and sock_available for each socket on espressif modules when the response always includes all sockets.
- Fixes to stop logic on Espressif and SIM7080
- Modified HTTP examples to attempt to connect to a site that doesn't require SSL.
  - @vshymanskyy's host of his primary example which displays the TinyGSM logo now requires SSL.

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
