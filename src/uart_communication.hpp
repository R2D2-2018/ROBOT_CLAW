/**
 * @file
 * @brief     UART Communication driver for the Arduino Due.
 * @author    Wiebe van Breukelen
 * @license   See LICENSE
 */

#ifndef UART_COMM_HPP
#define UART_COMM_HPP

#include "wrap-hwlib.hpp"

/**
 * @brief Establishes an serial/UART connection using the dedicated serial controlled located on the Arduino Due.
 *
 */
class UARTCommunication {
  public:
    /**
     * @brief Send a byte of the serial connection.
     *
     * @param byte Byte to send.
     */
    void sendByte(uint8_t byte);
};

#endif