/**
 * @file
 * @brief     main.cpp for ROBOT_CLAW module
 * @author    Wiebe van Breukelen, Sam Zandee
 * @license   See LICENSE
 */

#include "wrap-hwlib.hpp"

#include "claw.hpp"
#include "uart_connection.hpp"

//Usart *hardwareUSART = USART0;

//void uartInit();
//void uartSendByte(char b);

// What could be done?
// - A lot of testing
// - Class architecture
// - 

/**void uartInit() {


    // Disable PIO control on PA10, PA11 and set up for peripheral A
    PIOA->PIO_PDR = PIO_PA10;
    PIOA->PIO_ABSR &= ~PIO_PA10;
    PIOA->PIO_PDR = PIO_PA11;
    PIOA->PIO_ABSR &= ~PIO_PA11;

    // Enable the clock to USART0
    PMC->PMC_PCER0 = (0x01 << ID_USART0);

    // Set the control register to reset and disable the receiver and transmitter.
    hardwareUSART->US_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

    // Set the baudrate to 115200. see page 799
    // Calculation: 5241600 / desired baudrate
    hardwareUSART->US_BRGR = 45;

    // No parity, normal channel mode. Use a 8 bit data field.
    hardwareUSART->US_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT; // Might check if this definition is compatable with USART as well.

    // Disable the interrupt controller.
    hardwareUSART->US_IDR = 0xFFFFFFFF;

    // Enable the transmitter and receiver
    hardwareUSART->US_CR = UART_CR_RXEN | UART_CR_TXEN;

    /// USART controller is enabled

}

void uartSendByte(char b) {
    // We use the USART Channel status register to wait until the TXRDY bit is cleared. If so, we are ready to send more data.
    while ((hardwareUSART->US_CSR & 2) == 0);
    hardwareUSART->US_THR = b;
}**/

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    namespace target = hwlib::target;

    //uartInit();

    hwlib::wait_ms(500);

    hwlib::cout << "Hi there!" << hwlib::endlRet;

    target::pin_in touchSensorLeft(target::pins::d4);
    target::pin_in touchSensorRight(target::pins::d5);

    UARTConnection conn;

    conn.begin();

    conn.send("Hello World!");

    /**while (true) {
        uartSendByte(0xA0);

        hwlib::wait_ms(200);
    }**/

    /**UARTCommunication uart;
    Claw claw(uart, touchSensorLeft, touchSensorRight);

    /// Perform some small mock tests.
    claw.open();

    hwlib::wait_ms(1000);

    claw.close();

    hwlib::wait_ms(1000);

    claw.openUntilReleased();

    hwlib::cout << "Actual position: " << claw.getPosition() << hwlib::endlRet;

    return 0;**/
}