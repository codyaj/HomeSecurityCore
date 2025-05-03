// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef EEPROMMANAGER_H
#define EEPROMMANAGER_H

#define EEPROM_SIZE (32 + 63)

#include <EEPROM.h>
#include <Arduino.h>

/*
 * @brief Initializes EEPROM ready for reading and writing
 *
 */
void initEEPROM();

/*
 * @brief Saves the given WiFi credentials to EEPROM
 *
 * @param[ssid] Pointer to a char array of the network SSID. Must be 32 bytes (input)
 * @param[pass] Pointer to a char array of the network password. Must be 63 bytes (input)
 * @warning ssid must be 32 bytes and pass must be 63 bytes
 * @warning The ability to write to the EEPROM is finite. This function should not be repeated if not necessary
 */
void saveWiFiCredentials(char* ssid, char* pass);

/*
 * @brief Gets the saved WiFi Credentials from EEPROM
 *
 * @param[ssid] A pointer to an array where the SSID will be stored. Must be 32 bytes (output)
 * @param[pass] A pointer to an array where the Password will be stored. Must be 63 bytes (output)
 * @return True if there are credentials stored. False if there are no credentials stored.
 * @warning ssid must be 32 bytes. pass must be 63 bytes.
 */
bool getWiFiCredentials(char* ssid, char* pass);

/*
 * @brief Clears the first character of the EEPROM so it will return false if it is fetched
 *
 * @warning The ability to write to the EEPROM is finite. This function should not be repeated if not necessary
 */
void clearWiFiCredentials();

#endif
