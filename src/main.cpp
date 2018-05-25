/**
 * @file
 * @brief     main.cpp for ROBOT_CLAW module
 * @author    Wiebe van Breukelen, Sam Zandee
 * @license   See LICENSE
 */

#include "wrap-hwlib.hpp"

#include "claw.hpp"
#include "uart_connection.hpp"


int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    namespace target = hwlib::target;

    hwlib::wait_ms(500);

    hwlib::cout << "Hi there!" << hwlib::endlRet;

    target::pin_in touchSensorLeft(target::pins::d4);
    target::pin_in touchSensorRight(target::pins::d5);

    UARTConnection conn(115200, UARTController::ONE);

    conn.begin();

    conn.send('a');
    conn.send("Hello World!");

    hwlib::wait_ms(500);
   

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