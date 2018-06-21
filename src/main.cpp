/**
 * @file
 * @brief     main.cpp for ROBOT_CLAW module
 * @author    Wiebe van Breukelen, Sam Zandee
 * @license   See LICENSE
 */

#include "wrap-hwlib.hpp"

#include "claw.hpp"
#include "claw_state.hpp"
#include "uart_connection.hpp"

inline void debugUarmRx(UARTConnection &conn);

namespace target = hwlib::target;

long startMsReceive = 0, startMsSend = 0;

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    target::pin_in touchSensorLeft(target::pins::d4);
    target::pin_in touchSensorRight(target::pins::d5);

    UARTConnection conn(115200, UARTController::ONE, false);
    Claw claw(conn, touchSensorLeft, touchSensorRight);

    conn.begin();

    hwlib::wait_ms(1000);

    // Check if the uArm is connected.
    if (!claw.isConnected()) {
        hwlib::cout << "uArm is not connected!" << hwlib::endlRet;
        while (!claw.isConnected())
            hwlib::wait_ms(500);
    }

    hwlib::wait_ms(1000);

    // hwlib::cout << "uArm is connected!" << hwlib::endlRet;

    char response[15];
    hwlib::cout << "Receiving firmware version... -> ";

    /// Print the firmware version running on the claw.
    /// If the claw is not probably connected, this function will hang forever.
    /// In a further sprint, this should be fixed.
    claw.getUarmFirmwareVersion(response);

    hwlib::cout << response << hwlib::endlRet;
    while (true) {
        hwlib::wait_ms(1000);

        /// Rotation tests
        hwlib::cout << "rotate\n" << hwlib::endlRet;
        // int16_t rotation = 174;
        for (int i = -90; i <= 90; i += 10) {
            claw.setAngle(i);
            hwlib::cout << claw.getAngle() << hwlib::endlRet;
            hwlib::wait_ms(500);
        }
    }
}
