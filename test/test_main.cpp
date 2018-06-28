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

    claw.setAngle(90);
    REQUIRE(claw.getAngle() == 90);

    claw.setAngle(200);
    REQUIRE(claw.getAngle() == 90);
}

TEST_CASE("Check unknown command error") {
    UARTLib::MockUART conn(115200, UARTLib::UARTController::THREE, false);
    hwlib::test::pin_in<2> gripSensor{0, 1};
    Claw claw(conn, gripSensor);

    char receivedResponse[32] = "lsis lhgsiUnknown commandLInfli";
    REQUIRE(claw.checkUnknownCommand(receivedResponse, 32) == true);

    char receivedResponse2[32] = "lsis lhgsi888888888888888LInfli";
    REQUIRE(claw.checkUnknownCommand(receivedResponse2, 32) == false);
}

TEST_CASE("ClawState feedback") {
    UARTLib::MockUART conn(115200, UARTLib::UARTController::THREE, false);
    hwlib::test::pin_in<2> gripSensor{0, 1};
    Claw claw(conn, gripSensor);

    char stoppedClaw[] = "O0";
    char movingClaw[] = "O1";
    char grippedClaw[] = "O2";
    char unknownClawState[] = "O4";
    REQUIRE(claw.decodeClawState(stoppedClaw, 0) == ClawState::STOPPED);
    REQUIRE(claw.decodeClawState(movingClaw, 0) == ClawState::MOVING);
    REQUIRE(claw.decodeClawState(grippedClaw, 0) == ClawState::GRIPPED_OBJECT);
    REQUIRE(claw.decodeClawState(unknownClawState, 0) == ClawState::UNKNOWN);
}

TEST_CASE("Decode Gcode response into error") {
    UARTLib::MockUART conn(115200, UARTLib::UARTController::THREE, false);
    hwlib::test::pin_in<2> gripSensor{0, 1};
    Claw claw(conn, gripSensor);

    char E20Response[] = "E20";
    char E21Response[] = "E21";
    char E22Response[] = "E22";
    char E23Response[] = "E23";
    char E24Response[] = "E24";
    char E25Response[] = "E25";
    REQUIRE(claw.decodeGcodeResponse(E20Response, 3) == ClawFeedback::E20);
    REQUIRE(claw.decodeGcodeResponse(E21Response, 3) == ClawFeedback::E21);
    REQUIRE(claw.decodeGcodeResponse(E22Response, 3) == ClawFeedback::E22);
    REQUIRE(claw.decodeGcodeResponse(E23Response, 3) == ClawFeedback::E23);
    REQUIRE(claw.decodeGcodeResponse(E24Response, 3) == ClawFeedback::E24);
    REQUIRE(claw.decodeGcodeResponse(E25Response, 3) == ClawFeedback::E25);
}
