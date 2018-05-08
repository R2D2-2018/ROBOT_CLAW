#ifndef UART_COMM_HPP
#define UART_COMM_HPP

#include "wrap-hwlib.hpp"

class UARTCommunication {
  public:
    void sendByte(uint8_t byte);
};

#endif