/**
 * @file      claw_sensing.hpp
 * @brief     Class for sensing whether the claw is empty when closed
 * @author    Wiebe van Breukelen, Joost van Lingen
 * @license   See LICENSE
 */

#ifndef CLAW_SENSING_HPP
#define CLAW_SENSING_HPP

#include "wrap-hwlib.hpp"

class ClawSensing {
  private:
    hwlib::pin_in &gripSensor;

  public:
    explicit ClawSensing(hwlib::pin_in &gripSensor);
    bool getState();
};

#endif