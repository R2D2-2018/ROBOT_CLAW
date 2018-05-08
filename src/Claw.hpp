#ifndef CLAW_HPP
#define CLAW_HPP

#include "ClawSensing.hpp"
#include "UARTCommunication.hpp"

class Claw {
  private:
    int position;
    UARTCommunication &uartComm;
    ClawSensing clawSensing;

  public:
    explicit Claw(UARTCommunication &uart, hwlib::pin_in &touchSensorLeft, hwlib::pin_in &touchSensorRight)
        : position(0), uartComm(uart), clawSensing(touchSensorLeft, touchSensorRight){};

    void open();
    void openUntilReleased();
    void close();
    void closeUntilGrabbed();
    void setDegrees(int position);
    void incrementAngle();
    void decrementAngle();
    bool isOpen();
    bool isClosed();
};

#endif