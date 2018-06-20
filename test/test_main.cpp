#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "../src/wrap-hwlib.hpp"

#include "claw.hpp"
#include "claw_state.hpp"
#include "uart_connection.hpp"

TEST_CASE("Virtual controller test demo") {
    hwlib::test::pin_in<6> gripSensor{1, 0, 0, 1, 0, 1};
    UARTConnection conn(115200, UARTController::ONE, false);
    Claw claw(, gripSensor);

    REQUIRE(claw.getState() == ClawState::CLOSED);
}