#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "../src/wrap-hwlib.hpp"

#include "claw.hpp"
#include "claw_state.hpp"
#include "uart_lib.hpp"

TEST_CASE("Detect closed gripper") {
    UARTLib::MockUART conn(115200, UARTLib::UARTController::THREE, false);
    hwlib::test::pin_in<2> gripSensor{0, 1};
    Claw claw(conn, gripSensor);

    REQUIRE(claw.getState() == ClawState::CLOSED);
    REQUIRE(claw.getState() != ClawState::CLOSED);
}

TEST_CASE("Set and Get yaw axis rotation") {
    UARTLib::MockUART conn(115200, UARTLib::UARTController::THREE, false);
    hwlib::test::pin_in<2> gripSensor{0, 1};
    Claw claw(conn, gripSensor);

    claw.setAngle(45);

    REQUIRE(claw.getAngle() == 45);
}