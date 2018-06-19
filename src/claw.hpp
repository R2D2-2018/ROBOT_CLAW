/**
 * @file
 * @brief     Interface for the uArm Swift Pro claw/gripper
 * @author    Wiebe van Breukelen, Sam Zandee
 * @license   See LICENSE
 */
#ifndef CLAW_HPP
#define CLAW_HPP

#include "claw_sensing.hpp"
#include "claw_state.hpp"
#include "uart_connection.hpp"

/**
 * @brief Enum containing all the different possible responses given by the uArm
 *
 */
enum class ClawFeedback { E20 = 0, E21 = 1, E22 = 2, E23 = 3, E24 = 4, E25 = 5, OK };

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
     * @brief Close the robot claw.
     *
     */
    void close();

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

    /**
     * @brief Decode string recieved from the uArm.
     *
     * Returns one of the states from the ClawFeedback enum.
     *
     * @param[in] response Gcode response string.
     * @param[in] responseSize Gcode response string size.
     * @return ClawFeedback The decoded response from the uArm.
     */
    ClawFeedback decodeGcodeResponse(char *response, size_t responseSize);
};

#endif
