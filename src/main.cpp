/**
 * @file
 * @brief     main.cpp for ROBOT_CLAW module
 * @author    Wiebe van Breukelen, Sam Zandee
 * @license   See LICENSE
 */

#include "wrap-hwlib.hpp"

#include "claw.hpp"
#include "claw_state.hpp"
#include "uart_lib.hpp"

namespace target = hwlib::target;

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    target::pin_in gripSensor(target::pins::d13);

    UARTLib::HardwareUART conn(115200, UARTLib::UARTController::ONE, false);
    Claw claw(conn, gripSensor);

    conn.begin();

    hwlib::wait_ms(1000);

    /// Check if the uArm is connected.
    if (!claw.isConnected()) {
        hwlib::cout << "uArm is not connected!" << hwlib::endlRet;
        while (!claw.isConnected())
            hwlib::wait_ms(500);
    }

    hwlib::wait_ms(1000);

    char response[15];
    hwlib::cout << "Receiving firmware version... -> ";

    /// Print the firmware version running on the claw.
    /// If the claw is not probably connected, this function will hang forever.
    /// In a further sprint, this should be fixed.
    claw.getUarmFirmwareVersion(response);

    hwlib::cout << response << hwlib::endlRet;

    claw.open();

    while (true) {
        ClawState state = claw.getState();
        if (state == ClawState::CLOSED) {
            hwlib::cout << "CLOSED" << hwlib::endlRet << "{--------------------}" << '\r' << "{";
            claw.open();
            for (int i = 0; i < 20; i++) {
                hwlib::wait_ms(250);
                hwlib::cout << "=";
            }
            hwlib::cout << hwlib::endlRet;
            state = claw.getState();
        } else {
            hwlib::cout << "OPEN" << hwlib::endlRet << "{--------------------}" << '\r' << "{";
            claw.close();
            for (int i = 0; i < 20; i++) {
                hwlib::wait_ms(250);
                hwlib::cout << "=";
            }
            hwlib::cout << hwlib::endlRet;
            state = claw.getState();
            if (state != ClawState::CLOSED) {
                hwlib::cout << "OBJECT DETECTED" << hwlib::endlRet << "{--------------------}" << '\r' << "{";
                claw.open();
                for (int i = 0; i < 20; i++) {
                    hwlib::wait_ms(250);
                    hwlib::cout << "=";
                }
                hwlib::cout << hwlib::endlRet;
            }
        }
    }
}
