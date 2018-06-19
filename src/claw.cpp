#include "claw.hpp"

void Claw::open() {
    uartComm << "#n M2232 V0\n";
}

void Claw::close() {
    uartComm << "#n M2232 V1\n";
}

bool Claw::isConnected() {
    uartComm << "#n P2203\n";

    /// By giving a null pointer as a method parameter, we save unnecessarily memory space.
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
        /// If we have the Gcode response `$n ok V3.2.1` we only want to return the stuff behind the V mark (in this
        /// example: 3.2.1). We determine the position of the V mark.
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
            } else if (responseCharCounter > 0) { /// We have found a endline. If the response char counter is larger then zero
                                                  /// (there is data), we will stopping polling for new data.
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

    /// Response format: $n ok V1\n
    /// Search for V character
    int vStart = 0;

    for (int i = 0; i < 15; i++) {
        /// If we have the Gcode response `$n ok V1` we only want to return the stuff behind the V mark (in this
        /// example: 1). We determine the position of the V mark.
        if (response[i] == 'V') {
            vStart = i;
            break;
        }
    }

    switch (response[vStart + 1]) {
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

ClawFeedback Claw::decodeGcodeResponse(char *response, size_t responseSize) {
    for (int i = 0; i < 15; ++i) {
        if (response[i] == 'o' && response[i + 1] == 'k') {
            return ClawFeedback::OK;
        } else if (response[i] == 'E') {
            return static_cast<ClawFeedback>(response[i + 1]);
        }
    }
    return ClawFeedback::OK;
}
