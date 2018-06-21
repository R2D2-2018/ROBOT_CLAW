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
    hwlib::pin_in &gripSensor; ///< Reference to the pin address the gripSensor is connected to

  public:
    /**
     * @brief Constructor for ClawSensing
     *
     * @param gripSensor reference to the pin address the gripSensor is connected to
     */
    explicit ClawSensing(hwlib::pin_in &gripSensor);

    /**
     * @brief Get the state of the gripSensor
     *
     * This function returns the state of the gripSensor. When it returns true, the claw is open.
     *
     * When the claw is closed, the ground is connected to the pin so the function returns false.
     * When the claw is open, the pin is internally pulled up to the 5V on the Arduino so the
     * function returns true.
     *
     * @return bool Whether the claw is open or not.
     */
    bool getState();
};

#endif