/**
 * @file       TinyGsmClientSIM7000.h
 * @brief      SIM7000 modem client and modem-trait definitions.
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */
/* clang-format off */
/**
 * @defgroup simcom_sim7000 SIMCom SIM7000 Modem Family
 * @brief Manufacturer: SIMCom. Models: SIM7000.
 * @ingroup simcom_sim70xx
 *
 * # Supported Public Functions
 *
 * - Basic functions (TinyGsmModem.tpp)
 *     - @ref TinyGsmModem<modemType, modemConfig>::begin "begin()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::init "init()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::sendAT "sendAT()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::setBaud "setBaud()"
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
 *     - @ref TinyGsmModem<modemType, modemConfig>::radioOff "radioOff()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::sleepEnable "sleepEnable()"
 *     - @ref TinyGsmModem<modemType, modemConfig>::setPhoneFunctionality "setPhoneFunctionality()"
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
 * - SIM card functions (TinyGsmGPRS.tpp)
 *     - @ref TinyGsmGPRS<modemType>::simUnlock "simUnlock()"
 *     - @ref TinyGsmGPRS<modemType>::getSimCCID "getSimCCID()"
 *     - @ref TinyGsmGPRS<modemType>::getIMEI "getIMEI()"
 *     - @ref TinyGsmGPRS<modemType>::getIMSI "getIMSI()"
 *     - @ref TinyGsmGPRS<modemType>::getSimStatus "getSimStatus()"
 * - GPRS functions (TinyGsmGPRS.tpp)
 *     - @ref TinyGsmGPRS<modemType>::gprsConnect "gprsConnect()"
 *     - @ref TinyGsmGPRS<modemType>::gprsDisconnect "gprsDisconnect()"
 *     - @ref TinyGsmGPRS<modemType>::isGprsConnected "isGprsConnected()"
 *     - @ref TinyGsmGPRS<modemType>::getOperator "getOperator()"
 * - GPS (GNSS, GLONASS) functions (TinyGsmGPS.tpp)
 *     - @ref TinyGsmGPS<modemType>::enableGPS "enableGPS()"
 *     - @ref TinyGsmGPS<modemType>::disableGPS "disableGPS()"
 *     - @ref TinyGsmGPS<modemType>::getGPSraw "getGPSraw()"
 *     - @ref TinyGsmGPS<modemType>::getGPS "getGPS()"
 *     - @ref TinyGsmGPS<modemType>::getGPSTime "getGPSTime()"
 * - Generic network functions
 *     - @ref TinyGsmSim70xx::getNetworkModes "getNetworkModes()"
 *     - @ref TinyGsmSim70xx::getNetworkMode "getNetworkMode()"
 *     - @ref TinyGsmSim70xx::setNetworkMode "setNetworkMode()"
 *     - @ref TinyGsmSim70xx::getPreferredModes "getPreferredModes()"
 *     - @ref TinyGsmSim70xx::getPreferredMode "getPreferredMode()"
 *     - @ref TinyGsmSim70xx::setPreferredMode "setPreferredMode()"
 *     - @ref TinyGsmSim70xx::getNetworkSystemMode "getNetworkSystemMode()"
 *     - @ref TinyGsmSim70xx::setNetworkSystemMode "setNetworkSystemMode()"
 * - TCP functions (TinyGsmTCP.tpp)
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::maintain "maintain()"
 * - Text messaging (SMS) functions (TinyGsmSMS.tpp)
 *     - @ref TinyGsmSMS<modemType>::sendUSSD "sendUSSD()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS "sendSMS()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS_UTF16 "sendSMS_UTF16()"
 * - Time functions (TinyGsmTime.tpp)
 *     - @ref TinyGsmTime<modemType>::getGSMDateTime "getGSMDateTime()"
 *     - @ref TinyGsmTime<modemType>::getNetworkTime "getNetworkTime()"
 * - NTP server functions (TinyGsmNTP.tpp)
 *     - @ref TinyGsmNTP<modemType>::NTPServerSync "NTPServerSync()"
 *     - @ref TinyGsmNTP<modemType>::waitForTimeSync "waitForTimeSync()"
 *     - @ref TinyGsmNTP<modemType>::ShowNTPError "ShowNTPError()"
 * - NTP Utilities (TinyGsmNTP.tpp)
 *     - @ref TinyGsmNTP<modemType>::TinyGsmIsValidNumber "TinyGsmIsValidNumber()"
 * - GSM location functions (TinyGsmGSMLocation.tpp)
 *     - @ref TinyGsmGSMLocation<modemType>::getGsmLocationRaw "getGsmLocationRaw()"
 *     - @ref TinyGsmGSMLocation<modemType>::getGsmLocation "getGsmLocation()"
 *     - @ref TinyGsmGSMLocation<modemType>::getGsmLocationTime "getGsmLocationTime()"
 * - Battery functions (TinyGsmBattery.tpp)
 *     - @ref TinyGsmBattery<modemType>::getBattVoltage "getBattVoltage()"
 *     - @ref TinyGsmBattery<modemType>::getBattPercent "getBattPercent()"
 *     - @ref TinyGsmBattery<modemType>::getBattChargeState "getBattChargeState()"
 *     - @ref TinyGsmBattery<modemType>::getBattStats "getBattStats()"
 *
 * # Connection Information
 *
 * - TCP sockets:
 *   - 8
 *   - SSL is not supported
 * - Socket Buffering:
 *   - The modem has an internal buffer for incoming data.
 *   - This gives you leeway to pull data from the buffer as needed with less
 * risk of losing data.
 * - Socket Numbering:
 *   - The modem uses user-specified MUX channel numbers for socket connections.
 *   - If you attempt to create a new client with a channel number that is
 * already in use and other unused channels are available, this library will
 * select the next available one.
 *   - Use the getMux() function to get the assigned multiplexing channel number
 * after a successful connection.
 *
 * @todo In `modemReadImpl()`: validate mux
 * @todo In `modemGetAvailableImpl()`: validate mux
 */
/* clang-format on */

#ifndef SRC_TINYGSMCLIENTSIM7000_H_
#define SRC_TINYGSMCLIENTSIM7000_H_
#pragma message("TinyGSM:  TinyGsmClientSIM7000")

#include "TinyGsmClientSIM70xx.h"
#include "TinyGsmTCP.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmGSMLocation.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"
#include "TinyGsmBattery.tpp"

/// Basic modem configurations for the SIM7000 modem family
/// @ingroup simcom_sim7000
struct TinyGsmSim7000ModemConfig
    : public TinyGsmModemConfigPreset<Sim70xxRegStatus> {
  /// The modem manufacturer
  static constexpr char MODEM_MANUFACTURER[] TINY_GSM_PROGMEM = "SIMCom";
  /// The modem model
  static constexpr char MODEM_MODEL[] TINY_GSM_PROGMEM = "SIM7000";
};

constexpr char TinyGsmSim7000ModemConfig::MODEM_MANUFACTURER[]
    __attribute__((weak));
constexpr char TinyGsmSim7000ModemConfig::MODEM_MODEL[] __attribute__((weak));

/**
 * @brief TCP behavior and limits for the SIM7000 modem family.
 * @ingroup simcom_sim7000
 */
struct TinyGsmSim7000TcpConfig
    : public TinyGsmTcpConfigPreset<
          /*bufferMode*/ TinyGsmTcpBufferMode::BufferReadAndCheckSize,
          /*muxMode*/ TinyGsmTcpMuxMode::Static,
          /*muxCount*/ 8> {};

/// Class for the SIMCOM SIM7000 using the TCP-IP toolkit
/// @ingroup simcom_sim7000
class TinyGsmSim7000
    : public TinyGsmSim70xx<TinyGsmSim7000, TinyGsmSim7000ModemConfig>,
      public TinyGsmTCP<TinyGsmSim7000, TinyGsmSim7000TcpConfig>,
      public TinyGsmSMS<TinyGsmSim7000>,
      public TinyGsmTime<TinyGsmSim7000>,
      public TinyGsmNTP<TinyGsmSim7000>,
      public TinyGsmGSMLocation<TinyGsmSim7000>,
      public TinyGsmBattery<TinyGsmSim7000> {
  friend class TinyGsmSim70xx<TinyGsmSim7000, TinyGsmSim7000ModemConfig>;
  friend class TinyGsmModem<TinyGsmSim7000, TinyGsmSim7000ModemConfig>;
  friend class TinyGsmGPRS<TinyGsmSim7000>;
  friend class TinyGsmTCP<TinyGsmSim7000, TinyGsmSim7000TcpConfig>;
  friend class GsmClient<TinyGsmSim7000, TinyGsmSim7000TcpConfig>;
  friend class TinyGsmSMS<TinyGsmSim7000>;
  friend class TinyGsmGSMLocation<TinyGsmSim7000>;
  friend class TinyGsmGPS<TinyGsmSim7000>;
  friend class TinyGsmTime<TinyGsmSim7000>;
  friend class TinyGsmNTP<TinyGsmSim7000>;
  friend class TinyGsmBattery<TinyGsmSim7000>;

 public:
  using ModemConfig = TinyGsmSim7000ModemConfig;
  using TcpConfig   = TinyGsmSim7000TcpConfig;

  /*
   * Inner Client
   */
 public:
  /// Inner client
  /// @ingroup simcom_sim7000
  class GsmClientSim7000
      : public GsmClient<TinyGsmSim7000, TinyGsmSim7000TcpConfig> {
    friend class TinyGsmSim7000;

   public:
    using GsmClient<TinyGsmSim7000, TinyGsmSim7000TcpConfig>::connect;
    using GsmClient<TinyGsmSim7000, TinyGsmSim7000TcpConfig>::stop;
    using TcpConfig = TinyGsmSim7000TcpConfig;

    /**
     * @brief Create a new TCP client and bind it to a modem and optionally a
     * multiplexing channel.
     * @param modem Modem instance used by this client.
     * @param mux Multiplexing channel to use.
     *
     * @note The SIM7000 allows you choose the multiplexing channel number, but
     * if the input mux channel number is already in use and other mux channels
     * are available, this library will select the next available one.  Use the
     * getMux() function to get the assigned multiplexing channel number after a
     * successful connection.
     */
    explicit GsmClientSim7000(TinyGsmSim7000& modem, uint8_t mux = 0)
        : GsmClient<TinyGsmSim7000, TinyGsmSim7000TcpConfig>(modem, mux) {
      is_secure = false;


      sock_available = 0;
      prev_check     = 0;
      sock_connected = false;
      got_data       = false;
      is_mid_send    = false;

      // if it's a valid mux number, and that mux number isn't in use (or it's
      // already this), accept the mux number
      if (mux < TcpConfig::kMuxCount &&
          (at.sockets[mux] == nullptr || at.sockets[mux] == this)) {
        this->mux = mux;
        // If the mux number is in use or out of range, find the next available
        // one
      } else if (at.findFirstUnassignedMux() != static_cast<uint8_t>(-1)) {
        this->mux = at.findFirstUnassignedMux();
      } else {
        // If we can't find anything available, overwrite something, using mod
        // to make sure we're in range
        this->mux = (mux % TcpConfig::kMuxCount);
      }
      at.sockets[this->mux] = this;
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
  // NOTE:  Use modem TinyGsmSim7000SSL for a secure client!

  /*
   * GSM Modem Constructor
   */
 public:
  /**
   * @brief Construct a modem wrapper around a stream transport.
   * @param stream Stream used to communicate with the modem.
   */
  explicit TinyGsmSim7000(Stream& stream)
      : TinyGsmSim70xx<TinyGsmSim7000, TinyGsmSim7000ModemConfig>(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientSIM7000"));

    if (!testAT()) { return false; }

    sendAT(GF("E0"));  // Echo Off
    if (waitResponse() != 1) { return false; }

#ifdef TINY_GSM_DEBUG
    sendAT(GF("+CMEE=2"));  // turn on verbose error codes
#else
    sendAT(GF("+CMEE=0"));  // turn off error codes
#endif
    waitResponse();

    DBG(GF("### Modem:"), getModemName());

    // Enable Local Time Stamp for getting network time
    sendAT(GF("+CLTS=1"));
    if (waitResponse(10000L) != 1) { return false; }

    // Enable battery checks
    sendAT(GF("+CBATCHK=1"));
    if (waitResponse() != 1) { return false; }

    SimStatus ret = getSimStatus();
    // if the sim isn't ready and a pin has been provided, try to unlock the sim
    if (ret != SIM_READY && pin != nullptr && strlen(pin) > 0) {
      simUnlock(pin);
      return (getSimStatus() == SIM_READY);
    } else {
      // if the sim is ready, or it's locked but no pin has been provided,
      // return true
      return (ret == SIM_READY || ret == SIM_LOCKED);
    }
  }

  /*
   * Power functions
   */
  // Follows functions as inherited from TinyGsmClientSIM70xx.h

  /*
   * Generic network functions
   */
 protected:
  String getLocalIPImpl() {
    sendAT(GF("+CIFSR;E0"));
    String res;
    if (waitResponse(10000L, res) != 1) { return ""; }
    cleanResponseString(res);
    return res;
  }

  /*
   * Secure socket layer (SSL) certificate management functions
   */
  // NOTE:  Use modem TinyGsmSim7000SSL for a secure client!

  /*
   * Wifi functions
   */
  // No functions of this type supported

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* user, const char* pwd) {
    gprsDisconnect();

    // Bearer settings for applications based on IP
    // Set the connection type to GPRS
    sendAT(GF("+SAPBR=3,1,\"Contype\",\"GPRS\""));
    waitResponse();

    // Set the APN
    sendAT(GF("+SAPBR=3,1,\"APN\",\""), apn, '"');
    waitResponse();

    // Set the user name
    if (user && strlen(user) > 0) {
      sendAT(GF("+SAPBR=3,1,\"USER\",\""), user, '"');
      waitResponse();
    }
    // Set the password
    if (pwd && strlen(pwd) > 0) {
      sendAT(GF("+SAPBR=3,1,\"PWD\",\""), pwd, '"');
      waitResponse();
    }

    // Define the PDP context
    sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, '"');
    waitResponse();

    // Attach to GPRS
    sendAT(GF("+CGATT=1"));
    if (waitResponse(60000L) != 1) { return false; }

    // Activate the PDP context
    sendAT(GF("+CGACT=1,1"));
    waitResponse(60000L);

    // Open the definied GPRS bearer context
    sendAT(GF("+SAPBR=1,1"));
    waitResponse(85000L);
    // Query the GPRS bearer context status
    sendAT(GF("+SAPBR=2,1"));
    if (waitResponse(30000L) != 1) { return false; }

    // Set the TCP application toolkit to multi-IP
    sendAT(GF("+CIPMUX=1"));
    if (waitResponse() != 1) { return false; }

    // Put the TCP application toolkit in "quick send" mode
    // (thus no extra "Send OK")
    sendAT(GF("+CIPQSEND=1"));
    if (waitResponse() != 1) { return false; }

    // Set the TCP application toolkit to get data manually
    sendAT(GF("+CIPRXGET=1"));
    if (waitResponse() != 1) { return false; }

    // Start the TCP application toolkit task and set APN, USER NAME, PASSWORD
    sendAT(GF("+CSTT=\""), apn, GF("\",\""), user, GF("\",\""), pwd, '"');
    if (waitResponse(60000L) != 1) { return false; }

    // Bring up the TCP application toolkit wireless connection with GPRS or CSD
    sendAT(GF("+CIICR"));
    if (waitResponse(60000L) != 1) { return false; }

    // Get local IP address for the TCP application toolkit
    // only assigned after connection
    sendAT(GF("+CIFSR;E0"));
    if (waitResponse(10000L) != 1) { return false; }

    return true;
  }

  bool gprsDisconnectImpl() {
    // Shut the TCP application toolkit connection
    // CIPSHUT will close *all* open TCP application toolkit connections
    sendAT(GF("+CIPSHUT"));
    if (waitResponse(60000L) != 1) { return false; }

    sendAT(GF("+CGATT=0"));  // Deactivate the bearer context
    if (waitResponse(60000L) != 1) { return false; }

    return true;
  }

  /*
   * SIM card functions
   */
  // Follows functions as inherited from TinyGsmClientSIM70xx.h

  /*
   * Phone call functions
   */
  // No functions of this type supported

  /*
   * Audio functions
   */
  // No functions of this type supported

  /*
   * Text messaging (SMS) functions
   */
  // Follows all text messaging (SMS) functions as inherited from TinyGsmSMS.tpp

  /*
   * GSM location functions
   */
  // Follows all GSM-based location functions as inherited from
  // TinyGsmGSMLocation.tpp

  /*
   * GPS/GNSS/GLONASS location functions
   */
  // Follows functions as inherited from TinyGsmClientSIM70xx.h

  /*
   * Time functions
   */
  // Follows all clock functions as inherited from TinyGsmTime.tpp

  /*
   * NTP server functions
   */
  // Follows all NTP server functions as inherited from TinyGsmNTP.tpp

  /*
   * BLE functions
   */
  // No functions of this type supported

  /*
   * Battery functions
   */
  // Follows all battery functions as inherited from TinyGsmBattery.tpp

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
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;

    // when not using SSL, the TCP application toolkit is more stable
    sendAT(GF("+CIPSTART="), mux, ',', GF("\"TCP"), GF("\",\""), host,
           GF("\","), port);
    return (1 ==
            waitResponse(timeout_ms, GF("CONNECT OK\r\n"),
                         GF("CONNECT FAIL\r\n"), GF("ALREADY CONNECT\r\n"),
                         GFP(ModemConfig::GSM_ERROR), GF("CLOSE OK\r\n")));
  }

  bool modemStopImpl(uint8_t mux, uint32_t maxWaitMs) {
    sendAT(GF("+CIPCLOSE="), mux);
    return waitResponse(min(maxWaitMs, static_cast<uint32_t>(3000))) ==
        1;  // should return within 3s
  }

  bool modemBeginSendImpl(size_t len, uint8_t mux) {
    sendAT(GF("+CIPSEND="), mux, ',', (uint16_t)len);
    return waitResponse(GF(">")) == 1;
  }
  // Between the modemBeginSend and modemEndSend, modemSend calls:
  // stream.write(reinterpret_cast<const uint8_t*>(buff), len);
  // stream.flush();
  size_t modemEndSendImpl(size_t len, uint8_t mux) {
    if (waitResponse(GF("DATA ACCEPT:"), GF("SEND FAIL")) != 1) { return 0; }
    uint8_t  ret_mux = streamGetIntBefore(',');   // check mux
    uint16_t sent    = streamGetIntBefore('\n');  // check send length
    if (sent != len) { DBG("### Sent:", sent, "of", len, "on", mux); }
    if (mux == ret_mux) return sent;
    return 0;
  }

  size_t modemReadImpl(size_t size, uint8_t mux) {
    if (!sockets[mux]) return 0;

#ifdef TINY_GSM_USE_HEX
    sendAT(GF("+CIPRXGET=3,"), mux, ',', (uint16_t)size);
#else
    sendAT(GF("+CIPRXGET=2,"), mux, ',', (uint16_t)size);
#endif
    if (waitResponse(GF("+CIPRXGET:")) != 1) { return 0; }
    streamSkipUntil(',');  // Skip Rx mode 2/normal or 3/HEX
    streamSkipUntil(',');  // Skip mux
    // TODO: validate mux
    int16_t len_reported = streamGetIntBefore(',');
    //  ^^ Requested number of data bytes (1-1460 bytes) to be read
    int16_t len_remaining = streamGetIntBefore('\n');
    // ^^ Confirmed number of data bytes to be read, which may be less than
    // requested. 0 indicates that no data can be read.
    // SRGD NOTE:  Contrary to above (which is copied from AT command manual)
    // this is actually be the number of bytes that will be remaining in the
    // buffer after the read.
    size_t len_read = moveCharsFromStreamToFifo(mux, len_reported);
    // sockets[mux]->sock_available = modemGetAvailable(mux);
    sockets[mux]->sock_available = len_remaining;
    waitResponse();
    return len_read;
  }

  size_t modemGetAvailableImpl(uint8_t mux) {
    if (!sockets[mux]) return 0;

    sendAT(GF("+CIPRXGET=4,"), mux);
    size_t result = 0;
    if (waitResponse(GF("+CIPRXGET:")) == 1) {
      streamSkipUntil(',');  // Skip mode 4
      streamSkipUntil(',');  // Skip mux
      // TODO: validate mux
      result = streamGetIntBefore('\n');
      waitResponse();
    }
    // DBG("### Available:", result, "on", mux);
    if (!result) { sockets[mux]->sock_connected = modemGetConnected(mux); }
    return result;
  }

  bool modemGetConnectedImpl(uint8_t mux) {
    sendAT(GF("+CIPSTATUS="), mux);
    waitResponse(GF("+CIPSTATUS"));
    int8_t res = waitResponse(GF(",\"CONNECTED\""), GF(",\"CLOSED\""),
                              GF(",\"CLOSING\""), GF(",\"REMOTE CLOSING\""),
                              GF(",\"INITIAL\""));
    waitResponse();
    return 1 == res;
  }

  /*
   * Utilities
   */
 protected:
  bool handleURCs(String& data) {
    if (data.endsWith(GF("+CIPRXGET:"))) {
      int8_t mode = streamGetIntBefore(',');
      if (mode == 1) {
        int8_t mux = streamGetIntBefore('\n');
        if (mux >= 0 && mux < TcpConfig::kMuxCount && sockets[mux]) {
          sockets[mux]->got_data = true;
        }
        data = "";
        // DBG("### Got Data:", mux);
        return true;
      } else {
        data += mode;
        return false;
      }
    } else if (data.endsWith(GF("+RECEIVE:"))) {
      int8_t  mux = streamGetIntBefore(',');
      int16_t len = streamGetIntBefore('\n');
      if (mux >= 0 && mux < TcpConfig::kMuxCount && sockets[mux]) {
        sockets[mux]->got_data = true;
        if (len >= 0 && len <= 1024) { sockets[mux]->sock_available = len; }
      }
      data = "";
      // DBG("### Got Data:", len, "on", mux);
      return true;
    } else if (data.endsWith(GF("CLOSED\r\n"))) {
      int nl   = TinyGsmMax(0,
                            data.lastIndexOf(String(GFP(ModemConfig::GSM_NL)),
                                             data.length() - 8));
      int coma = data.indexOf(',', nl + 2);
      int mux  = data.substring(nl + 2, coma).toInt();
      if (mux >= 0 && mux < TcpConfig::kMuxCount && sockets[mux]) {
        sockets[mux]->sock_connected = false;
      }
      data = "";
      DBG("### Closed: ", mux);
      return true;
    } else if (data.endsWith(GF("*PSNWID:"))) {
      streamSkipUntil('\n');  // Refresh network name by network
      data = "";
      DBG("### Network name updated.");
      return true;
    } else if (data.endsWith(GF("*PSUTTZ:"))) {
      streamSkipUntil('\n');  // Refresh time and time zone by network
      data = "";
      DBG("### Network time and time zone updated.");
      return true;
    } else if (data.endsWith(GF("+CTZV:"))) {
      streamSkipUntil('\n');  // Refresh network time zone by network
      data = "";
      DBG("### Network time zone updated.");
      return true;
    } else if (data.endsWith(GF("DST: "))) {
      streamSkipUntil('\n');  // Refresh Network Daylight Saving Time by network
      data = "";
      DBG("### Daylight savings time state updated.");
      return true;
    } else if (data.endsWith(GF("SMS Ready\r\n"))) {
      data = "";
      DBG("### Unexpected module reset!");
      init();
      return true;
    }
    return false;
  }

 protected:
  GsmClientSim7000* sockets[TcpConfig::kMuxCount];
};

#endif  // SRC_TINYGSMCLIENTSIM7000_H_
