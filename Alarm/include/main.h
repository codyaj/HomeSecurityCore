// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "messageUtils.h"
#include "setupManager.h"
#include "cryptoUtils.h"
#include "eepromManager.h"
#include "ledControl.h"
#include "resetBtn.h"
#include "config.h"
#include "ecdh.h"

const char serverAddy[] = "10.10.10.10";
const uint16_t serverPort = 19001;

#endif
