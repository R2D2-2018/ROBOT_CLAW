/**
 * @file
 * @brief     UART Communication driver for the Arduino Due.
 * @author    Wiebe van Breukelen
 * @license   See LICENSE
 */

#ifndef UART_COMM_HPP
#define UART_COMM_HPP

#include "wrap-hwlib.hpp"
#include "queue.hpp"

enum class UARTController {ONE, TWO, THREE};

/**
 * @brief Establishes an serial/UART connection using the dedicated serial controlled located on the Arduino Due.
 *
 */
class UARTConnection {
public:

  UARTConnection(bool initializeController = true);

  void begin();
  //bool available();
  unsigned int available();
  inline void enable();
  inline void disable();

  bool send(const char c);
  bool send(const char *str);
  bool send(const char* data, size_t length);

  char receive();
  //const char* receiveString();

  bool isInitialized();

  void operator<<(const char c);
  void operator<<(const char *str);

  ~UARTConnection();

private:
  Usart *hardwareUSART = USART0;
  //Usart *hardwareUSARTTwo = USART1;
  //Usart *hardwareUSARTThree = USART2;

  bool USARTControllerInitialized = false;

  //char rxBuffer[800];
  Queue<char, 800> rxBuffer;
  int rxBufferIndex = 0, rxReadIndex = 0;

  inline bool txReady();

  /**
   * @brief Send a byte of the serial connection.
   *
   * @param char Byte to send.
   */
  void sendByte(const char &b);

  inline char receiveByte();
};

#endif