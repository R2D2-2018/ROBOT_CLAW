/**
 * @file
 * @brief     Interface for the uArm Swift Pro claw/gripper
 * @author    Wiebe van Breukelen, Sam Zandee
 * @license   See LICENSE
 */
#ifndef CLAW_HPP
#define CLAW_HPP

#include "claw_state.hpp"
#include "claw_sensing.hpp"
#include "uart_connection.hpp"

class Claw {
  private:
    /**
     * @brief The current position of the claw
     *
     */
    unsigned int position;

    /**
     * @brief UART connection to communicate with the uArm Swift Pro.
     *
     */
    UARTConnection &uartComm;

    /**
     * @brief Used to check if an object has been grabbed/released by the robotic claw.
     *
     */
    ClawSensing clawSensing;

  public:
    explicit Claw(UARTConnection &uart, hwlib::pin_in &touchSensorLeft, hwlib::pin_in &touchSensorRight)
        : position(0), uartComm(uart), clawSensing(touchSensorLeft, touchSensorRight){};

    /**
     * @brief Open the robot claw.
     *
     */
    void open();

    /**
     * @brief Open the robot claw until a object has been gripped.
     *
     */
    void openUntilReleased();

    /**
     * @brief Close the robot claw.
     *
     */
    void close();

    /**
     * @brief Close the robot claw until an object has been gripped.
     *
     */
    void closeUntilGrabbed();

    /**
     * @brief Increment the angle to the robot claw with the smallest precision possible.
     *
     */
    void incrementAngle();

    /**
     * @brief Decrement the angle to the robot claw with the smallest precision possible.
     *
     */
    void decrementAngle();

    /**
     * @brief Check if the robot claw is open.
     *
     * The position of the robot claw must be > 0 if open.
     *
     * @return true Robot arm open
     * @return false Robot arm closed
     */
    bool isOpen();

    /**
     * @brief Check if the robot claw is closed
     *
     * The position of the robot claw must be zero if closed.
     *
     * @return true
     * @return false
     */
    bool isClosed();

    /**
     * @brief Set the position of the robot claw.
     *
     * @param destPos Position of the robotic claw in the scale of 0-100.
     */
    void setPosition(unsigned int destPos);

    /**
     * @brief Get the current claw state.
     *
     * @return CLawState Current state of the claw.
     */
    ClawState getState();

    /**
     * @brief Check if the uArm Swift Pro is connected.
     * 
     * By trying to receive the firmware version, we determine if the uArm Swift Pro is connected.
     * If the arm is not connected, a serial receive timeout will occur.
     * 
     * @return true Device connected.
     * @return false Device is not connected.
     */
    bool isConnected();

    /**
     * @brief Get the version of the firmware currently running on the uArm.
     *
     * @param response Char buffer to write version string to.
     * @return char* Written char buffer.
     */
    void getUarmFirmwareVersion(char response[15]);

    /**
     * @brief Receive Gcode string from the uArm Swift Pro using UART.
     * 
     * We continuely poll the uArm Swift Pro for new serial data. If the read timeout is reached,
     * we will stop the polling and return 0 (no characters received).
     * 
     * @param response Gcode response string.
     * @param responseSize Gcode response string size.
     * @param readTimeout UART receiver timeout in milliseconds.
     * @return int Amount of character read (including \0).
     */
    int receiveGcodeResponse(char *response, size_t responseSize, unsigned int readTimeout = 50);

};

#endif
