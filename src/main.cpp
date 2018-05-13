/**
 * @file
 * @brief     main.cpp for ROBOT_CLAW module
 * @author    Wiebe van Breukelen, Sam Zandee
 * @license   See LICENSE
 */

#include "wrap-hwlib.hpp"

#include "claw.hpp"
#include "uart_communication.hpp"

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    namespace target = hwlib::target;

    target::pin_in touchSensorLeft(target::pins::d4);
    target::pin_in touchSensorRight(target::pins::d5);

    UARTCommunication uart;
    Claw claw(uart, touchSensorLeft, touchSensorRight);

    /// Perform some small mock tests.
    claw.open();

    hwlib::wait_ms(1000);

    claw.close();

    hwlib::wait_ms(1000);

    claw.openUntilReleased();

    hwlib::cout << "Actual position: " << claw.getPosition() << hwlib::endlRet;

    return 0;
}