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
    hwlib::wait_ms(500);

    long startMsReceive = hwlib::now_us() / 1000;
    long startMsSend = hwlib::now_us() / 1000;

    conn.begin();
    bool state = false;

    while (true) {
        if ((hwlib::now_us() / 1000) - startMsSend > 2500) {
            startMsSend = hwlib::now_us() / 1000;
            conn << "#n P2203\n"; // Getting version

            if (state) {
                conn << "#n M2232 V0\n";
            } else {
                conn << "#n M2232 V1\n";
            }

            state = !state;
            
        }

        if (conn.available() > 0 && (hwlib::now_us() / 1000) - startMsReceive > 50) {
            startMsReceive = hwlib::now_us() / 1000;

            for (unsigned int i = 0; i < conn.available(); i++) {
                hwlib::cout << conn.receive();
            }
        }
    }
}