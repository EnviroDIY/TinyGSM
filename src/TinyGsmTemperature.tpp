/**
 * @file       TinyGsmTemperature.tpp
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMTEMPERATURE_H_
#define SRC_TINYGSMTEMPERATURE_H_

#include "TinyGsmCommon.h"

#ifndef TINY_GSM_MODEM_HAS_TEMPERATURE
#define TINY_GSM_MODEM_HAS_TEMPERATURE
#endif

template <class modemType>
class TinyGsmTemperature {
  /* =========================================== */
  /* =========================================== */
  /*
   * Define the interface
   */
 public:
  /*
   * Temperature functions
   */

  /**
   * @brief Get the modem chip temperature in degrees celsius.
   *
   * @return *float* The modem chip temperature in degrees celsius.
   */
  float getTemperature() {
    return thisModem().getTemperatureImpl();
  }

 protected:
  // destructor (protected!)
  ~TinyGsmTemperature() {}

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
   * Temperature functions
   */

  float getTemperatureImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;
};

#endif  // SRC_TINYGSMTEMPERATURE_H_
