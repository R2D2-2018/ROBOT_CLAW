/**
 * @file
 * @brief     UART Communication driver for the Arduino Due.
 * @author    Wiebe van Breukelen
 * @license   See LICENSE
 */

#ifndef UART_COMM_HPP
#define UART_COMM_HPP

#include "wrap-hwlib.hpp"

enum class UARTController {ONE, TWO, THREE};

/**
 * @brief Establishes an serial/UART connection using the dedicated serial controlled located on the Arduino Due.
 *
 */
class UARTConnection {
public:

  void begin();
  void available();
  inline void enable();
  inline void disable();

  bool send(const char c);
  bool send(const char *c);

  char receiveByte();
  const char* receiveString();

private:
  Usart *hardwareUSART = USART0;
  //Usart *hardwareUSARTTwo = USART1;
  //Usart *hardwareUSARTThree = USART2;

  bool USARTControllerInitialized = false;

  char receiveBuffer[800];

  inline bool txReady();

  /**
   * @brief Send a byte of the serial connection.
   *
   * @param char Byte to send.
   */
  void sendByte(const char &b);
  //void sendByte(char b);
};

#endif