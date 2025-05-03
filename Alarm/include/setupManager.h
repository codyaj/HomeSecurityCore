// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef SETUPMANAGER_H
#define SETUPMANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "eepromManager.h"
#include "ledControl.h"

const char setupSSID[] = "changeme";
const char setupPass[] = "changeme";

/*
 * @brief Begins the setup process
 *
 * @note this function starts the ESP in AP mode and starts a captive portal. The function will not exit until the data is received.
 */
void beginSetup();

#endif
