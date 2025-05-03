// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "heartbeat.h"

unsigned long lastHeartbeatSent = 0;
bool waitingForReply = false;
int currentJitter = 0;
uint8_t failedSendAttempts = 0;

namespace {
    // Returns a value between -256 and 256
    int getJitter() {
        RNG rng;
        int val = rng.get();
        if (rng.get() >= 128) {
            return val * -1;
        }
        return val;
    }
}

bool checkHeartbeat() {
    unsigned long currTime = millis();
    if (waitingForReply) {
        if (currTime >= lastHeartbeatSent + HEARTBEAT_WAIT) {
            Serial.println("[DEBUG] Wait has been too long for heartbeat!");
        }
        return currTime >= lastHeartbeatSent + HEARTBEAT_WAIT;
    }
    
    if (currTime >= lastHeartbeatSent + HEARTBEAT_INTERVAL + currentJitter) {
        if (!sendMessage(OUTGOING_HEARTBEAT)) {
            Serial.println("[DEBUG] Heartbeat failed to send!");
            failedSendAttempts += 1;
            return failedSendAttempts >= MAX_SEND_ATTEMPTS;
        }
        failedSendAttempts = 0;
        lastHeartbeatSent = currTime;
        waitingForReply = true;
        currentJitter = getJitter();
    }

    return false;
}

void receivedHeartbeat() {
    waitingForReply = false;
    disableAlarm(failedHeartbeat);
    updateLED(none);
}
