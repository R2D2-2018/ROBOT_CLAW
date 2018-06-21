/**
 * @file      claw.hpp
 * @brief     Interface for the uArm Swift Pro claw/gripper
 * @author    Wiebe van Breukelen, Sam Zandee, Joost van Lingen
 * @license   See LICENSE
 */

#ifndef CLAW_HPP
#define CLAW_HPP

#include "claw_sensing.hpp"
#include "claw_state.hpp"
#include "uart_lib.hpp"

class Claw {
  private:
    unsigned int position;             ///< The current position of the claw
    UARTLib::UARTConnection &uartComm; ///< UART connection to communicate with the uArm Swift Pro.
    ClawSensing clawSensing;           ///< Used to check if an object has been grabbed/released by the robotic claw.
    int16_t yawAngle = 0;              ///< Stores the current angle of the yaw axis.

  public:
    /**
     * @brief Constructor for claw
     *
     * @param uart Connection to the robot arm's built-in controller
     */
    explicit Claw(UARTLib::UARTConnection &uart, hwlib::pin_in &gripSensor)
        : position(0), uartComm(uart), clawSensing(gripSensor){};

    /**
     * @brief Open the robot claw.
     */
    void open();

    /**
     * @brief Close the robot claw.
     */
    void close();

    /**
     * @brief Get the current claw state.
     *
     * @return ClawState Current state of the claw.
     */
    ClawState getState();

    /**
     * @brief Check if the uArm Swift Pro is connected.
     *
     * By trying to receive the firmware version, we determine if the uArm Swift Pro is connected.
     * If the arm is not connected, a serial receive timeout will occur.
     *
     * @return bool Whether the uArm is connected.
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
     * @brief Set rotation of claw yaw
     *
     * @param rotation Rotation in degrees to turn to.
     */
    void setAngle(int16_t rotation);

    /**
     * @brief Get rotation of claw yaw
     *
     * @return Current rotation in degrees.
     */
    int16_t getAngle();

  private:
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
