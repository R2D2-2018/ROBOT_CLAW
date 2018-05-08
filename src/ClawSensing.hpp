#ifndef CLAW_SENSING_HPP
#define CLAW_SENSING_HPP

#include "wrap-hwlib.hpp"

class ClawSensing {
  private:
    hwlib::pin_in &touchSensorLeft, &touchSensorRight;

  public:
    explicit ClawSensing(hwlib::pin_in &touchSensorLeft, hwlib::pin_in &touchSensorRight)
        : touchSensorLeft(touchSensorLeft), touchSensorRight(touchSensorRight){};
};

#endif
