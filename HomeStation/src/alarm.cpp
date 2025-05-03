// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "../include/alarm.h"

Alarm::Alarm(int clientSocket) : Device(clientSocket) {}

bool Alarm::isAlarmActive() {
    return alarmActive;
}

// Example handling block for raw buffer input (not currently in use)
/*
switch (buff[2])
{
    case INCOMMING_HEARTBEAT: {
        std::cout << "[DEBUG] Sent heartbeat back\n";
        std::vector<uint8_t> empty;
        sendMessage(OUTGOING_HEARTBEAT, empty);
        break;
    }
    default:
        std::cout << "[DEBUG] Unknown Message Type: ";
        printf("0x%x ", buff[2]);
        std::cout << std::endl;
        break;
}
*/

void Alarm::handleMessage(uint8_t msgType) {
    switch (msgType) {
        case INCOMMING_JAMMING_DETECTED: {
            // Trigger alarm and log the jamming event
            break;
        }
        case INCOMMING_TAMPER_ALERT: {
            // Trigger alarm and log the tamper alert
            break;
        }
        case INCOMMING_LOW_BATTERY: {
            // Notify home screen of low battery and log the warning
            break;
        }
        default: {
            std::cout << "[DEBUG] Unknown Message Type: ";
            printf("0x%x ", msgType);
            std::cout << std::endl;
            break;
        }
    }
}

void Alarm::handleMessage(uint8_t msgType, std::vector<uint8_t> data) {
    switch (msgType) {
        case INCOMMING_STATUS_REPLY: {
            for (auto x : data) {
                // TODO: Remove this loop; placeholder for data inspection
                if (x != 254) {
                    break;
                }
            }
            // Log status and update any relevant displays
            break;
        }
        default: {
            std::cout << "[DEBUG] Unknown Message Type: ";
            printf("0x%x ", msgType);
            std::cout << std::endl;
            break;
        }
    }
}

Alarm::~Alarm() {}
