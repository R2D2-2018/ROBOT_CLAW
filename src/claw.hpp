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

/**
 * @brief Enum containing all the different possible responses given by the uArm
 *
 */
enum class ClawFeedback { E20 = '0', E21 = '1', E22 = '2', E23 = '3', E24 = '4', E25 = '5', OK };

class Claw {
  private:
    unsigned int position;             ///< The current position of the claw
    UARTLib::UARTConnection &uartComm; ///< UART connection to communicate with the uArm Swift Pro.
    ClawSensing clawSensing;           ///< Used to check if an object has been grabbed/released by the robotic claw.
    int16_t yawAngle = 0;              ///< Stores the current angle of the yaw axis.

    static constexpr char unknownCommand[] = "Unknown command"; ///< Expected response upon unknown command.

  public:
    /**
     * @brief Constructor for claw
     *
     * @param[in] uart Connection to the robot arm's built-in controller
     * @param[in] gripSensor reference to the pin address the gripSensor is connected to
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
     * @param[out] response Char buffer to write version string to.
     * @return char* Written char buffer.
     */
    void getUarmFirmwareVersion(char response[15]);

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

    /**
     * @brief Decode response into claw state
     *
     * Decodes response string into claw state.
     *
     * @param[in] response Gcode reponse string
     * @param[in] vStart start of the response byte
     * @return clawState The state of the claw
     */
    ClawState decodeClawState(char *response, int vStart);

    /**
     * @brief Set rotation of claw yaw
     *
     * @param[in] rotation Rotation in degrees to turn to.
     */
    void setAngle(int16_t rotation);

    /**
     * @brief Get rotation of claw yaw
     *
     * @return Current rotation in degrees.
     */
    int16_t getAngle();

    /**
     * @brief Function checks response for being message about an unknown command
     *
     * A crude pattern matching loop is used to check part of the response against the expected response from an unknown command
     * error.
     *
     * @param[in] input pointer to the reponse.
     * @param[in] inputSize size of the response.
     * @return true if error message is present.
     */
    bool checkUnknownCommand(char *input, size_t inputSize);

  private:
    /**
     * @brief Receive Gcode string from the uArm Swift Pro using UART.
     *
     * We continuely poll the uArm Swift Pro for new serial data. If the read timeout is reached,
     * we will stop the polling and return 0 (no characters received).
     *
     * @param[out] response Gcode response string.
     * @param[in] responseSize Gcode response string size.
     * @param[in] readTimeout UART receiver timeout in milliseconds.
     * @return int Amount of character read (including \0).
     */
    int receiveGcodeResponse(char *response, size_t responseSize, unsigned int readTimeout = 50);
};

#endif
