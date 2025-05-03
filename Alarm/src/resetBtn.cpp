// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "resetBtn.h"

void setupResetBtn() {
    pinMode(resetBtnPin, INPUT_PULLUP);
}

void checkResetBtn() {
    if (digitalRead(resetBtnPin) == LOW) {
        Serial.println("[DEBUG] reset");
        clearWiFiCredentials();
        ESP.restart();
    }
}
