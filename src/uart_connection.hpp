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

  UARTConnection(unsigned int baudrate, UARTController controller = UARTController::ONE, bool initializeController = true);

  /**
   * @brief Begin a UART connection.
   * 
   * This method initializes the selected USART (universal synchronous and asynchronous receiver-transmitter) controller located
   * on the Arduino Due. By default, this method is called at object construction, but this can be disabled.
   * 
   */
  void begin();

  /**
   * @brief Check how many bytes are available to read.
   * 
   * By checking the size of the receive buffer, we determine if new data has arrived.
   * 
   * @return unsigned int Amount of bytes available to read.
   */
  unsigned int available();

  /**
   * @brief Enables the internal USART controller.
   * 
   */
  inline void enable();

  /**
   * @brief Disables the internal USART controller.
   * 
   */
  inline void disable();

  /**
   * @brief Send a single byte.
   * 
   * @param c Byte.
   * @return true Byte send.
   * @return false Byte has not been send, USART controller not initialized.
   */
  bool send(const char c);

  /**
   * @brief Send a string.
   * 
   * @param str String.
   * @return true String send.
   * @return false String has not been send, USART controller not initialized.
   */
  bool send(const char *str);

  /**
   * @brief Send a array of bytes with a specified length.
   * 
   * @param data Array of bytes.
   * @param length Length of array.
   * @return true Array of bytes send.
   * @return false Array of bytes has not been send, USART controller not initialized.
   */
  bool send(const char* data, size_t length);

  /**
   * @brief Receive a single byte.
   * 
   * By popping the first element of the receive buffer, we return a received byte in a FIFO manner.
   * 
   * @return char Received byte.
   */
  char receive();

  /**
   * @brief Checks if the internal USART controller has been initialized.
   * 
   * @return true Internal USART controller is initialized.
   * @return false Internal USART has not been initialized. See the begin() method.
   */
  bool isInitialized();

  /**
   * @brief Send a single byte.
   * 
   * @param c Byte.
   */
  void operator<<(const char c);

  /**
   * @brief Send a string.
   * 
   * @param str String (must be null terminated).
   */
  void operator<<(const char *str);

  /**
   * @brief Receive a byte.
   * 
   * @param c Byte.
   */
  void operator>>(char &c);

  ~UARTConnection();

private:
  /**
   * @brief Pointer the access the internal USART controllers.
   * 
   */
  Usart *hardwareUSART = nullptr;

  /**
   * @brief Data baudrate used for sending and receiving.
   * 
   */
  unsigned int baudrate;

  /**
   * @brief Enumerable type used to select on of the three USART controllers located on the Arduino Due. 
   * 
   */
  UARTController controller;

  /**
   * @brief Holds the initialization status of the USART controller.
   * 
   */
  bool USARTControllerInitialized = false;

  /**
   * @brief UART receive buffer.
   * 
   */
  Queue<char, 250> rxBuffer;
  //int rxBufferIndex = 0, rxReadIndex = 0;

  /**
   * @brief Checks if the USART controller reports that the transmitter is ready to send.
   * 
   * @return true Ready to send.
   * @return false Not ready to send.
   */
  inline bool txReady();

  /**
   * @brief Send a byte of the serial connection.
   *
   * @param char Byte to send.
   */
  void sendByte(const char &b);

  /**
   * @brief Receive a single byte by reading the US_RHR register.
   * 
   * @return char 
   */
  inline char receiveByte();
};

#endif