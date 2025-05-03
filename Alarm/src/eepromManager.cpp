// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "eepromManager.h"

void initEEPROM() {
    EEPROM.begin(EEPROM_SIZE);
}

// Save WiFi credentials | SSID 32 Chars, PASS 63 Chars
void saveWiFiCredentials(char* ssid, char* pass) {
    // Write ssid
    for (int i = 0; i < 32; i++) {
        EEPROM.write(i, ssid[i]);
    }

    // Write pass
    for (int i = 32; i < 32 + 63; i++) {
        EEPROM.write(i, pass[i - 32]);
    }

    // Commit changes to EEPROM
    EEPROM.commit();
}

// Get WiFi credentials
bool getWiFiCredentials(char* ssid, char* pass) {
    // Read ssid
    for (int i = 0; i < 32; i++) {
        ssid[i] = EEPROM.read(i);
    }
    //ssid[31] = '\0'; // Ensure null-termination

    // Read pass
    for (int i = 32; i < 32 + 63; i++) {
        pass[i - 32] = EEPROM.read(i);
    }
    //pass[62] = '\0'; // Ensure null-termination

    return ssid[0] != ' ';
}

// Clear stored credentials
void clearWiFiCredentials() {
    EEPROM.write(0, ' ');
    EEPROM.commit();
    Serial.println("[DEBUG] Attempting to write to EEPROM");
}
