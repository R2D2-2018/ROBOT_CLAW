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
        while (!claw.isConnected()) hwlib::wait_ms(500);
    }

    hwlib::wait_ms(1000);

    //hwlib::cout << "uArm is connected!" << hwlib::endlRet;

    char response[15];
    hwlib::cout << "Receiving firmware version... -> ";

    /// Print the firmware version running on the claw.
    /// If the claw is not probably connected, this function will hang forever.
    /// In a further sprint, this should be fixed.
    claw.getUarmFirmwareVersion(response);

    hwlib::cout << response << hwlib::endlRet;
    //hwlib::wait_ms(200);

    bool state = false;
    startMsReceive = hwlib::now_us() / 1000;
    startMsSend = hwlib::now_us() / 1000;

    claw.open();

    while (true) {
        if ((hwlib::now_us() / 1000) - startMsSend > 9000) {
            startMsSend = hwlib::now_us() / 1000;

            if (state) {
                hwlib::cout << "Opening claw..." << hwlib::endlRet;

                claw.open();

                hwlib::wait_ms(1500);
                hwlib::cout << "Object detected!" << hwlib::endlRet;
            } else {
                hwlib::cout << "Closing claw..." << hwlib::endlRet;

                claw.close();

                hwlib::wait_ms(1500);
                hwlib::cout << "Object released!" << hwlib::endlRet;
            }

            ClawState curState = claw.getState();

            if (curState == ClawState::STOPPED) {
                hwlib::cout << "Claw stopped!" << hwlib::endlRet;
            } else if (curState == ClawState::MOVING) {
                hwlib::cout << "Claw moving!" << hwlib::endlRet;
            } else if (curState == ClawState::GRIPPED_OBJECT) {
                hwlib::cout << "Claw gripping!" << hwlib::endlRet;
            } else {
                hwlib::cout << "Claw state unknown!" << hwlib::endlRet;
            }

            state = !state;
        }

        //debugUarmRx(conn); /// Debugging purposes. You may remove this if you don't want the serial output of the uArm Swift Pro.
    }
}

/**
 * @brief If the developer would like to view the serial output of the uArm Swift Pro, they can place this function within the
 * endless main loop.
 *
 */
inline void debugUarmRx(UARTConnection &conn) {
    if (conn.available() > 0 && (hwlib::now_us() / 1000) - startMsReceive > 30) {
        for (unsigned int i = 0; i < conn.available(); i++) {
            hwlib::cout << conn.receive();
        }

        startMsReceive = hwlib::now_us() / 1000;
    }
}
