/**
 * @file      claw_sensing.cpp
 * @brief     Functionality for sensing whether the claw is empty when closed
 * @author    Joost van Lingen
 * @license   See LICENSE
 */

#include "claw_sensing.hpp"

ClawSensing::ClawSensing(hwlib::pin_in &gripSensor) : gripSensor(gripSensor) {
}

bool ClawSensing::getState() {
    return gripSensor.get();
}