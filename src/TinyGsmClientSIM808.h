/**
 * @file     TinyGsmClientSIM808.h
 * @brief      SIM808 modem client and modem-trait definitions.
 * @author   Volodymyr Shymanskyy
 * @license  LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date     Nov 2016
 */
/* clang-format off */
/**
 * @defgroup simcom_sim808 SIMCom SIM808/SIM868 Modem Family
 * @ingroup simcom_sim800
 * @brief Manufacturer: SIMCom. Models: SIM808, SIM868.
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
 *     - @ref TinyGsmModem<modemType, modemConfig>::streamDump "streamDump()"
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
 *     - @ref TinyGsmGPRS<modemType>::getProvider "getProvider()"
 * - TCP functions (TinyGsmTCP.tpp)
 *     - @ref TinyGsmTCP<modemType, tcpConfig>::maintain "maintain()"
 * - Phone call functions (TinyGsmCalling.tpp)
 *     - @ref TinyGsmCalling<modemType>::callAnswer "callAnswer()"
 *     - @ref TinyGsmCalling<modemType>::callNumber "callNumber()"
 *     - @ref TinyGsmCalling<modemType>::callHangup "callHangup()"
 *     - @ref TinyGsmCalling<modemType>::dtmfSend "dtmfSend()"
 * - Text messaging (SMS) functions (TinyGsmSMS.tpp)
 *     - @ref TinyGsmSMS<modemType>::sendUSSD "sendUSSD()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS "sendSMS()"
 *     - @ref TinyGsmSMS<modemType>::sendSMS_UTF16 "sendSMS_UTF16()"
 * - GSM location functions (TinyGsmGSMLocation.tpp)
 *     - @ref TinyGsmGSMLocation<modemType>::getGsmLocationRaw "getGsmLocationRaw()"
 *     - @ref TinyGsmGSMLocation<modemType>::getGsmLocation "getGsmLocation()"
 *     - @ref TinyGsmGSMLocation<modemType>::getGsmLocationTime "getGsmLocationTime()"
 * - Time functions (TinyGsmTime.tpp)
 *     - @ref TinyGsmTime<modemType>::getGSMDateTime "getGSMDateTime()"
 *     - @ref TinyGsmTime<modemType>::getNetworkTime "getNetworkTime()"
 * - NTP server functions (TinyGsmNTP.tpp)
 *     - @ref TinyGsmNTP<modemType>::NTPServerSync "NTPServerSync()"
 *     - @ref TinyGsmNTP<modemType>::waitForTimeSync "waitForTimeSync()"
 *     - @ref TinyGsmNTP<modemType>::ShowNTPError "ShowNTPError()"
 * - NTP Utilities (TinyGsmNTP.tpp)
 *     - @ref TinyGsmNTP<modemType>::TinyGsmIsValidNumber "TinyGsmIsValidNumber()"
 * - Battery functions (TinyGsmBattery.tpp)
 *     - @ref TinyGsmBattery<modemType>::getBattVoltage "getBattVoltage()"
 *     - @ref TinyGsmBattery<modemType>::getBattPercent "getBattPercent()"
 *     - @ref TinyGsmBattery<modemType>::getBattChargeState "getBattChargeState()"
 *     - @ref TinyGsmBattery<modemType>::getBattStats "getBattStats()"
 * - Phone call functions
 *     - @ref TinyGsmSim800::setGsmBusy "setGsmBusy()"
 * - Audio functions
 *     - @ref TinyGsmSim800::setVolume "setVolume()"
 *     - @ref TinyGsmSim800::getVolume "getVolume()"
 *     - @ref TinyGsmSim800::setMicVolume "setMicVolume()"
 *     - @ref TinyGsmSim800::setAudioChannel "setAudioChannel()"
 *     - @ref TinyGsmSim800::playToolkitTone "playToolkitTone()"
 * - GPS (GNSS, GLONASS) functions (TinyGsmGPS.tpp)
 *     - @ref TinyGsmGPS<modemType>::enableGPS "enableGPS()"
 *     - @ref TinyGsmGPS<modemType>::disableGPS "disableGPS()"
 *     - @ref TinyGsmGPS<modemType>::getGPSraw "getGPSraw()"
 *     - @ref TinyGsmGPS<modemType>::getGPS "getGPS()"
 *     - @ref TinyGsmGPS<modemType>::getGPSTime "getGPSTime()"
 * - Bluetooth functions (TinyGsmBluetooth.tpp)
 *     - @ref TinyGsmBluetooth<modemType>::enableBluetooth "enableBluetooth()"
 *     - @ref TinyGsmBluetooth<modemType>::disableBluetooth "disableBluetooth()"
 *     - @ref TinyGsmBluetooth<modemType>::setBluetoothVisibility "setBluetoothVisibility()"
 *     - @ref TinyGsmBluetooth<modemType>::setBluetoothHostName "setBluetoothHostName()"
 *
 * # Connection Information
 *
 * - TCP sockets: 8.
 * - SSL sockets: 5.
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
 * - Note: This module inherits TCP/SSL behavior from TinyGsmClientSIM800.h.
 */
/* clang-format on */

#ifndef SRC_TINYGSMCLIENTSIM808_H_
#define SRC_TINYGSMCLIENTSIM808_H_
#pragma message("TinyGSM:  TinyGsmClientSIM808")

#include "TinyGsmClientSIM800.h"
#include "TinyGsmGPS.tpp"
#include "TinyGsmBluetooth.tpp"

/**
 * @brief Class for the SIMCOM SIM808 and SIM868, which extends the SIM800 with
 * GPS and Bluetooth support
 * @ingroup simcom_sim808
 */
class TinyGsmSim808 : public TinyGsmSim800,
                      public TinyGsmGPS<TinyGsmSim808>,
                      public TinyGsmBluetooth<TinyGsmSim808> {
  friend class TinyGsmGPS<TinyGsmSim808>;
  friend class TinyGsmBluetooth<TinyGsmSim808>;

 public:
  /**
   * @brief Construct a modem wrapper around a stream transport.
   * @param stream Stream used to communicate with the modem.
   */
  explicit TinyGsmSim808(Stream& stream) : TinyGsmSim800(stream) {}


  /*
   * GPS/GNSS/GLONASS location functions
   */
 protected:
  // enable GPS
  bool enableGPSImpl() {
    sendAT(GF("+CGNSPWR=1"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  bool disableGPSImpl() {
    sendAT(GF("+CGNSPWR=0"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  // get the RAW GPS output
  // works only with ans SIM808 V2
  String getGPSrawImpl() {
    sendAT(GF("+CGNSINF"));
    if (waitResponse(10000L, GF("+CGNSINF:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  // get GPS informations
  // works only with ans SIM808 V2
  bool getGPSImpl(float* lat, float* lon, float* speed, float* alt, int* vsat,
                  int* usat, float* accuracy, int* year, int* month, int* day,
                  int* hour, int* minute, int* second) {
    sendAT(GF("+CGNSINF"));
    if (waitResponse(10000L, GF("+CGNSINF:")) != 1) { return false; }

#if defined(TINY_GSM_DEBUG) && 0
    int16_t run_status = streamGetIntBefore(',');
#else
    streamSkipUntil(',');  // GNSS run status
#endif
    int16_t fix_status = streamGetIntBefore(',');  // fix status

    // init variables
    float   ilat         = 0;
    float   ilon         = 0;
    float   ispeed       = 0;
    float   ialt         = 0;
    int16_t ivsat        = 0;
    int16_t iusat        = 0;
    float   iaccuracy    = 0;
    int16_t iyear        = 0;
    int16_t imonth       = 0;
    int16_t iday         = 0;
    int16_t ihour        = 0;
    int16_t imin         = 0;
    float   secondWithSS = 0;

    // UTC date & Time
    // If the modem hasn't gotten a fix, the date and time might be missing.
    // Before we parse the date and time by the expecte d number of characters,
    // check if the date and time is present by checking if the next character
    // is a comma.  If it is a comma, then the date and time is missing and we
    // will skip parsing it.
    bool hasDateTime = stream.peek() != ',';
    if (hasDateTime) {
      iyear  = streamGetIntLength(4);  // Four digit year
      imonth = streamGetIntLength(2);  // Two digit month
      iday   = streamGetIntLength(2);  // Two digit day
      ihour  = streamGetIntLength(2);  // Two digit hour
      imin   = streamGetIntLength(2);  // Two digit minute
      secondWithSS =
          streamGetFloatBefore(',');  // 6 digit second with subseconds
    } else {
      stream.read();  // Throw away the comma
    }

    ilat   = streamGetFloatBefore(',');     // Latitude
    ilon   = streamGetFloatBefore(',');     // Longitude
    ialt   = streamGetFloatBefore(',');     // MSL Altitude. Unit is meters
    ispeed = streamGetFloatBefore(',');     // Speed Over Ground. Unit is knots.
    streamSkipUntil(',');                   // Course Over Ground. Degrees.
    streamSkipUntil(',');                   // Fix Mode
    streamSkipUntil(',');                   // Reserved1
    iaccuracy = streamGetFloatBefore(',');  // Horizontal Dilution Of Precision
    streamSkipUntil(',');                   // Position Dilution Of Precision
    streamSkipUntil(',');                   // Vertical Dilution Of Precision
    streamSkipUntil(',');                   // Reserved2
    ivsat = streamGetIntBefore(',');        // GNSS Satellites in View
    iusat = streamGetIntBefore(',');        // GNSS Satellites Used
    streamSkipUntil(',');                   // GLONASS Satellites Used
    streamSkipUntil(',');                   // Reserved3
    streamSkipUntil(',');                   // C/N0 max
    streamSkipUntil(',');                   // HPA
    streamSkipUntil('\n');                  // VPA

    // Set pointers
    if (lat != nullptr) *lat = ilat;
    if (lon != nullptr) *lon = ilon;
    if (speed != nullptr) *speed = ispeed;
    if (alt != nullptr) *alt = ialt;
    if (vsat != nullptr) *vsat = ivsat;
    if (usat != nullptr) *usat = iusat;
    if (accuracy != nullptr) *accuracy = iaccuracy;
    if (iyear < 2000) iyear += 2000;
    if (year != nullptr) *year = iyear;
    if (month != nullptr) *month = imonth;
    if (day != nullptr) *day = iday;
    if (hour != nullptr) *hour = ihour;
    if (minute != nullptr) *minute = imin;
    if (second != nullptr) *second = static_cast<int>(secondWithSS);

    waitResponse();

#if 0
    DBG("GNSS run status:", run_status);
    DBG("Fix status:", fix_status);
    DBG(GF("Latitude:"), String(ilat, 8), GF("\tLongitude:"), String(ilon, 8),
        GF("\tAltitude:"), String(ialt, 4));
    DBG(GF("VSAT:"), ivsat, GF("\tUSAT:"), iusat, GF("\tAccuracy:"), iaccuracy);
    DBG(GF("Year:"), iyear, GF("\tMonth:"), imonth, GF("\tDay:"), iday);
    DBG(GF("Hour:"), ihour, GF("\tMinute:"), imin, GF("\tSecond:"),
        String(secondWithSS, 3));
#endif

    return fix_status == 1;  // return true if we have a fix
  }

  /*
   * Bluetooth functions
   */

  bool enableBluetoothImpl() {
    sendAT(GF("+BTPOWER=1"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  bool disableBluetoothImpl() {
    sendAT(GF("+BTPOWER=0"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  bool setBluetoothVisibilityImpl(bool visible) {
    sendAT(GF("+BTVIS="), visible);
    if (waitResponse() != 1) { return false; }

    return true;
  }

  bool setBluetoothHostNameImpl(const char* name) {
    sendAT(GF("+BTHOST="), name);
    if (waitResponse() != 1) { return false; }

    return true;
  }
};

#endif  // SRC_TINYGSMCLIENTSIM808_H_
