#include "claw_sensing.hpp"

ClawSensing::ClawSensing(hwlib::pin_in &gripSensor) : gripSensor(gripSensor) {
}

bool ClawSensing::getState() {
    return gripSensor.get();
}