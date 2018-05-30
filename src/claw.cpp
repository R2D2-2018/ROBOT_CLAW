#include "claw.hpp"

void Claw::open() {
    uartComm << "#n M2232 V0\n";
}

void Claw::openUntilReleased() {
    hwlib::cout << "Opening the claw until an item has been detected..." << hwlib::endlRet;

    /// Move the gripper motor until item is detected
    hwlib::wait_ms(1000);
    hwlib::cout << "Item detected!" << hwlib::endlRet;
    /// Stop the gripper motor

    /// Set the current position of the motor (if position, depends on if the uArm Swift Pro has an actual actuator)
    position = 23;

    hwlib::cout << "Stopping gripper motor..." << hwlib::endlRet;
}

void Claw::close() {
    uartComm << "#n M2232 V1\n";
}

void Claw::closeUntilGrabbed() {
    hwlib::cout << "Closing the claw until an item has been detected..." << hwlib::endlRet;

    /// Move the gripper motor until item is detected
    hwlib::wait_ms(1000);
    hwlib::cout << "Item detected!" << hwlib::endlRet;
    /// Stop the gripper motor

    /// Set the current position of the motor (if position, depends on if the uArm Swift Pro has an actual actuator)
    position = 55;

    hwlib::cout << "Stopping gripper motor..." << hwlib::endlRet;
}

void Claw::incrementAngle() {
    /// Increment the angle of the gripper with the finest precision possible.
    if (position >= 100)
        return;

    hwlib::cout << "Incrementing angle..." << hwlib::endlRet;

    /// Increment angle by one step
    position++;
}

void Claw::decrementAngle() {
    /// Decrement the angle of the gripper with the finest precision possible.

    if (position <= 0)
        return;

    hwlib::cout << "Incrementing angle..." << hwlib::endlRet;

    /// Increment angle by one step
    position--;
}

bool Claw::isOpen() {
    return (position > 0) ? true : false;
}

bool Claw::isClosed() {
    return (position == 0) ? true : false;
}

void Claw::setPosition(unsigned int destPos) {
    /// Check if the destination position is in range and not in the current position
    if (position <= 100 && destPos != position) {
        /// Move the motors to the specified position

        /// Set the new position
        position = destPos;
    }
}

bool Claw::isConnected() {
    //char response[15];
    uartComm << "#n P2203\n";

    if (!receiveGcodeResponse(nullptr, 255)) {
        return false;
    }

    return true;
}

void Claw::getUarmFirmwareVersion(char response[15]) {
    uartComm << "#n P2203\n";

    receiveGcodeResponse(response, 15);

    int versionStart = 0;

    for (int i = 0; i < 15; i++) {
        /// If we have the Gcode response `$n ok V3.2.1` we only want to return the stuff behind the V mark (in this example: 3.2.1).
        /// We determine the position of the V mark.
        if (response[i] == 'V') {
            
            versionStart = i;
            break;
        }
    }

    int startIterator = 0;

    /// Move everything after the V mark to the begin of the array.
    for (int versionIterator = versionStart; versionIterator < 15; ++versionIterator) {
        response[startIterator] = response[versionIterator + 1];
        startIterator++;
    }
}

int Claw::receiveGcodeResponse(char *response, size_t responseSize, unsigned int readTimeout) {
    bool receivingData = true;
    unsigned int responseCharCounter = 0;
    char byteRead = 0;

    /// Convert to microseconds
    readTimeout *= 1000;

    /// Decrease the response size as we will include a \0 character by ourselves.
    if (responseSize > 0) {
        responseSize--;
    }

    unsigned int lastRead = hwlib::now_us();

    while (receivingData) {
        if (uartComm.available() > 0) {
            byteRead = uartComm.receive();

            /// Read until we found an endline character.
            /// If the responseCharCounter does equal the size of the response array, we stop to prevent writing out of memory.
            if (byteRead != '\n' && responseCharCounter < responseSize) {
                if (response) {
                    /// Only if the response is not a null pointer, we write to the response array.
                    response[responseCharCounter] = byteRead;
                }
                responseCharCounter += 1;

                lastRead = hwlib::now_us();
            } else if (responseCharCounter > 0) {
                //hwlib::cout << "Endline found!\n";
                receivingData = false;
            }
        }

        /// Check if a timeout occurred. If so, return zero.
        if ((hwlib::now_us() - lastRead) > readTimeout) {
            return 0;
        }
    }

    /// Add a \0 character to the response, if the is a response.
    if (responseCharCounter > 0) {
        response[responseCharCounter++] = '\0';
    }

    /// Return the amount of characters read
    return responseCharCounter;
}

ClawState Claw::getState() {
    uartComm << "#n P2232\n";

    char response[15];
    receiveGcodeResponse(response, 15);

    switch (response[8]) {
        case '0':
            return ClawState::STOPPED;
        case '1':
            return ClawState::MOVING;
        case '2':
            return ClawState::GRIPPED_OBJECT;
        default:
            return ClawState::UNKNOWN;
    }

}